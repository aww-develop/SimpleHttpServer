#include "SimpleHttpServer.h"

#include "HttpServerModule.h"
#include "IHttpRouter.h"

const TArray<FSimpleHttpServerHeader> USimpleHttpServer::DefaultCorsHeaders =
{
	FSimpleHttpServerHeader{
		TEXT("Access-Control-Allow-Origin"),
		{TEXT("*")}
	},
	FSimpleHttpServerHeader{
		TEXT("Access-Control-Allow-Methods"),
		{TEXT("GET"), TEXT("POST"), TEXT("PUT"), TEXT("PATCH"), TEXT("DELETE"), TEXT("OPTIONS")}
	},
	FSimpleHttpServerHeader{
		TEXT("Access-Control-Allow-Headers"),
		{TEXT("Content-Type"), TEXT("Authorization")}
	}
};

void USimpleHttpServer::Initialize(const TScriptInterface<ISimpleServerApiRouterInterface>& InApiRouter,
                                   const int32 InPort)
{
	ApiRouter = InApiRouter;
	Port = InPort;

	UObject* RouterObj = ApiRouter.GetObject();
	if (!IsValid(RouterObj))
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("ApiRouter is not set."));
		return;
	}

	if (HttpRouter.IsValid())
	{
		UE_LOG(LogSimpleHttpServer, Warning,
		       TEXT("HttpRouter is already initialized. Resetting before reinitialization."));
		Reset();
	}

	HttpRouter = FHttpServerModule::Get().GetHttpRouter(Port, /*bFailOnBindFailure=*/true);
	if (!HttpRouter.IsValid())
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Failed to create HttpRouter on port %d"), Port);
		return;
	}

	RegisterEndpoints(ISimpleServerApiRouterInterface::Execute_GetRoutes(RouterObj));

	UE_LOG(LogSimpleHttpServer, Log, TEXT("SimpleHttpServer initialized on port %d"), Port);
}

void USimpleHttpServer::Reset()
{
	if (!HttpRouter)
	{
		return;
	}

	for (const auto& RouteHandle : RouteHandles)
	{
		HttpRouter->UnbindRoute(RouteHandle.Value);
	}

	RouteHandles.Reset();
	HttpRouter.Reset();

	UE_LOG(LogSimpleHttpServer, Log, TEXT("Reset complete."));
}

void USimpleHttpServer::RegisterEndpoints(const TArray<FSimpleHttpServerEndpoint>& Endpoints)
{
	TMap<FString, FSimpleHttpServerEndpoint> CombinedEndpoints;
	for (const FSimpleHttpServerEndpoint& Endpoint : Endpoints)
	{
		if (CombinedEndpoints.Contains(Endpoint.Path))
		{
			FSimpleHttpServerEndpoint& ExistingEndpoint = CombinedEndpoints[Endpoint.Path];
			ExistingEndpoint.RequestVerbs |= Endpoint.RequestVerbs;
		}
		else
		{
			CombinedEndpoints.Add(Endpoint.Path, Endpoint);
		}
	}

	for (const TTuple<FString, FSimpleHttpServerEndpoint>& Pair : CombinedEndpoints)
	{
		RegisterEndpoint(Pair.Value);
	}
}

void USimpleHttpServer::RegisterEndpoint(const FSimpleHttpServerEndpoint& Endpoint)
{
	if (Endpoint.Path.IsEmpty())
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Endpoint is empty. Cannot register."));
		return;
	}

	const FString Path = Endpoint.GetSafePath();
	if (RouteHandles.Contains(Path))
	{
		UE_LOG(LogSimpleHttpServer, Warning, TEXT("Endpoint %s is already registered."), *Endpoint.Path);
		return;
	}

	const FHttpRequestHandler Handler = FHttpRequestHandler::CreateUObject(
		this,
		&USimpleHttpServer::HandleRequest,
		Endpoint
	);

	const FHttpPath HttpPath = FHttpPath(Path);

	EHttpServerRequestVerbs RequestVerbs = static_cast<EHttpServerRequestVerbs>(Endpoint.RequestVerbs);

	if (bForceIncludeOptionsVerb)
	{
		UE_LOG(LogSimpleHttpServer, Log, TEXT("Force binding OPTIONS request for endpoint %s"), *Path);
		RequestVerbs |= EHttpServerRequestVerbs::VERB_OPTIONS;
	}

	const FHttpRouteHandle RouteHandle = HttpRouter->BindRoute(HttpPath, RequestVerbs, Handler);

	if (!RouteHandle.IsValid())
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Failed to register endpoint %s"), *Path);
		return;
	}

	RouteHandles.Add(Path, RouteHandle);
	UE_LOG(LogSimpleHttpServer, Log, TEXT("Registered endpoint %s"), *Path);
}

void USimpleHttpServer::StartServer() const
{
	UE_LOG(LogSimpleHttpServer, Log, TEXT("Starting HttpServer..."));

	UObject* ApiRouterObject = ApiRouter.GetObject();
	if (!IsValid(ApiRouterObject))
	{
		UE_LOG(LogSimpleHttpServer, Warning, TEXT("ApiRouter object is not valid. Cannot start HttpServer."));
		return;
	}

	if (!HttpRouter.IsValid())
	{
		UE_LOG(LogSimpleHttpServer, Warning,
		       TEXT(
			       "HttpRouter or ApiRouter is not initialized. Cannot start HttpServer. Please call Initialize first."
		       ));
		return;
	}

	if (!FHttpServerModule::IsAvailable())
	{
		UE_LOG(LogSimpleHttpServer, Warning, TEXT("HttpServerModule is not available. Cannot start HttpServer."));
		return;
	}

	FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
	HttpServerModule.StartAllListeners();

	UE_LOG(LogSimpleHttpServer, Log, TEXT("HttpServer started on port %d"), Port);
}

bool USimpleHttpServer::HandleRequest(const FHttpServerRequest& Request,
                                      const FHttpResultCallback& OnComplete,
                                      const FSimpleHttpServerEndpoint Endpoint) const
{
	UObject* ApiRouterObject = ApiRouter.GetObject();
	if (!IsValid(ApiRouterObject))
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("ApiRouter object is null. Cannot handle request."));
		TUniquePtr<FHttpServerResponse> ErrorResponse = FSimpleHttpServerResponseUtility::Generate(
			EHttpServerResponseCodes::ServerError,
			TEXT("Internal Server Error: ApiRouter object is null.")
		);
		OnComplete(MoveTemp(ErrorResponse));
		return true;
	}

	const FSimpleHttpServerRequest SimpleRequest(Request, Endpoint);
	FSimpleHttpServerResponse SimpleResponse;

	if ((SimpleRequest.Verb & ESimpleHttpServerRequestVerbs::Options) == ESimpleHttpServerRequestVerbs::Options)
	{
		// Handle CORS preflight request
		SimpleResponse = HandleOptionsRequest();
	}

	else if ((SimpleRequest.Verb & ESimpleHttpServerRequestVerbs::Get) == ESimpleHttpServerRequestVerbs::Get)
	{
		SimpleResponse = HandleRequest(
			ApiRouterObject,
			SimpleRequest,
			ISimpleServerApiRouterInterface::Execute_HandleGetRequest
		);
	}

	else if ((SimpleRequest.Verb & ESimpleHttpServerRequestVerbs::Post) == ESimpleHttpServerRequestVerbs::Post)
	{
		SimpleResponse = HandleRequest(
			ApiRouterObject,
			SimpleRequest,
			ISimpleServerApiRouterInterface::Execute_HandlePostRequest
		);
	}

	else if ((SimpleRequest.Verb & ESimpleHttpServerRequestVerbs::Put) == ESimpleHttpServerRequestVerbs::Put)
	{
		SimpleResponse = HandleRequest(
			ApiRouterObject,
			SimpleRequest,
			ISimpleServerApiRouterInterface::Execute_HandlePutRequest
		);
	}

	else if ((SimpleRequest.Verb & ESimpleHttpServerRequestVerbs::Patch) == ESimpleHttpServerRequestVerbs::Patch)
	{
		SimpleResponse = HandleRequest(
			ApiRouterObject,
			SimpleRequest,
			ISimpleServerApiRouterInterface::Execute_HandlePatchRequest
		);
	}

	else if ((SimpleRequest.Verb & ESimpleHttpServerRequestVerbs::Delete) == ESimpleHttpServerRequestVerbs::Delete)
	{
		SimpleResponse = HandleRequest(
			ApiRouterObject,
			SimpleRequest,
			ISimpleServerApiRouterInterface::Execute_HandleDeleteRequest
		);
	}

	else
	{
		UE_LOG(LogSimpleHttpServer, Warning, TEXT("Unhandled request verb."));
		TUniquePtr<FHttpServerResponse> ErrorResponse = FSimpleHttpServerResponseUtility::Generate(
			EHttpServerResponseCodes::BadRequest,
			TEXT("Bad Request: Unhandled request verb.")
		);
		OnComplete(MoveTemp(ErrorResponse));
		return true;
	}

	TUniquePtr<FHttpServerResponse> Response;
	
	if (!SimpleResponse.TryCreateHttpServerResponse(Response))
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Failed to convert FSimpleHttpServerResponse to FHttpServerResponse."));
		TUniquePtr<FHttpServerResponse> ErrorResponse = FSimpleHttpServerResponseUtility::Generate(
			EHttpServerResponseCodes::ServerError,
			TEXT("Internal Server Error: Failed to generate response.")
		);
		OnComplete(MoveTemp(ErrorResponse));
		return true;
	}

	ApplyCorsHeaders(Response.Get());

	OnComplete(MoveTemp(Response));

	return true;
}

FSimpleHttpServerResponse USimpleHttpServer::HandleOptionsRequest()
{
	const FString JsonString = FSimpleHttpServerJsonUtility::CreateJsonString(TEXT("message"),TEXT("success"));

	return FSimpleHttpServerResponse(
		ESimpleHttpServerResponseCodes::Ok,
		JsonString,
		TEXT("application/json")
	);
}

FSimpleHttpServerResponse USimpleHttpServer::HandleRequest(
	const TObjectPtr<UObject>& ApiRouterObject,
	const FSimpleHttpServerRequest& Request,
	const FSimpleHttpServerRequestHandler& Exec
)
{
	const FSimpleHttpServerResponse Response = Exec(ApiRouterObject.Get(), Request);

	if (Response.ResponseCode == ESimpleHttpServerResponseCodes::Unknown)
	{
		const FString VerbString = LexToString(Request.Verb);
		const FString Message = FString::Printf(
			TEXT("Failed to handle %s request"),
			*VerbString
		);

		const FString JsonString = FSimpleHttpServerJsonUtility::CreateJsonString(
			TEXT("message"),
			Message
		);

		return FSimpleHttpServerResponse(
			ESimpleHttpServerResponseCodes::BadRequest,
			JsonString,
			TEXT("application/json")
		);
	}

	return Response;
}

void USimpleHttpServer::ApplyCorsHeaders(FHttpServerResponse* InOutResponse) const
{
	if (!bApplyCorsHeaders)
	{
		UE_LOG(LogSimpleHttpServer, Verbose, TEXT("CORS headers application is disabled."));
		return;
	}

	if (!InOutResponse)
	{
		UE_LOG(LogSimpleHttpServer, Warning, TEXT("ApplyCorsHeaders called with null response."));
		return;
	}

	TArray<FSimpleHttpServerHeader> CorsHeaders = DefaultCorsHeaders;
	
	UObject* ApiRouterObject = ApiRouter.GetObject();
	if (IsValid(ApiRouterObject))
	{
		TArray<FSimpleHttpServerHeader> CustomCorsHeaders
			= ISimpleServerApiRouterInterface::Execute_GetCorsHeaders(ApiRouterObject);

		if (CustomCorsHeaders.Num() > 0)
		{
			UE_LOG(LogSimpleHttpServer, Verbose, TEXT("Using custom CORS headers from ApiRouter."));
			CorsHeaders = MoveTemp(CustomCorsHeaders);
		}
	}

	for (const FSimpleHttpServerHeader& Header : CorsHeaders)
	{
		const FString Key = Header.Key.ToLower();
		TArray<FString>& Values = InOutResponse->Headers.FindOrAdd(Key);
		for (const FString& Value : Header.Values)
		{
			Values.AddUnique(Value);
		}
	}
}

void USimpleHttpServer::StopServer()
{
	UE_LOG(LogSimpleHttpServer, Log, TEXT("Stopping HttpServer..."));

	if (FHttpServerModule::IsAvailable())
	{
		FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
		HttpServerModule.StopAllListeners();
	}

	Reset();

	UE_LOG(LogSimpleHttpServer, Log, TEXT("HttpServer stopped."));
}
