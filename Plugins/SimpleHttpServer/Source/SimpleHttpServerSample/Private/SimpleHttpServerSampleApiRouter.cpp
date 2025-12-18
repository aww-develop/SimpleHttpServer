#include "SimpleHttpServerSampleApiRouter.h"

USimpleHttpServerSampleApiRouter::USimpleHttpServerSampleApiRouter()
{
	FSimpleHttpServerEndpoint VersionEndpoint;
	VersionEndpoint.Path = VersionEndpointPath;
	VersionEndpoint.RequestVerbs = static_cast<int32>(ESimpleHttpServerRequestVerbs::Get);

	FSimpleHttpServerEndpoint EchoEndpoint;
	EchoEndpoint.Path = EchoEndpointPath;
	EchoEndpoint.RequestVerbs = static_cast<int32>(ESimpleHttpServerRequestVerbs::Post);

	FSimpleHttpServerEndpoint UserProfileEndpoint;
	UserProfileEndpoint.Path = UserProfileEndpointPath;
	UserProfileEndpoint.RequestVerbs = static_cast<int32>(ESimpleHttpServerRequestVerbs::Get);
	
	Routes = { VersionEndpoint, EchoEndpoint, UserProfileEndpoint };
	
	FSimpleHttpServerHeader AllowOriginHeader;
	AllowOriginHeader.Key = TEXT("Access-Control-Allow-Origin");
	AllowOriginHeader.Values = { TEXT("*") };

	FSimpleHttpServerHeader AllowMethodsHeader;
	AllowMethodsHeader.Key = TEXT("Access-Control-Allow-Methods");
	AllowMethodsHeader.Values = { TEXT("GET"), TEXT("POST"), TEXT("OPTIONS") };

	FSimpleHttpServerHeader AllowHeadersHeader;
	AllowHeadersHeader.Key = TEXT("Access-Control-Allow-Headers");
	AllowHeadersHeader.Values = { TEXT("Content-Type"), TEXT("Authorization"), TEXT("HX-Request"), TEXT("HX-Target"), TEXT("HX-Current-URL") };

	CorsHeaders = { AllowOriginHeader, AllowMethodsHeader, AllowHeadersHeader };
}

FSimpleHttpServerResponse USimpleHttpServerSampleApiRouter::HandleGetRequest_Implementation(const FSimpleHttpServerRequest& Request)
{
	if (Request.EndpointPath == VersionEndpointPath)
	{
		const FString JsonString = FSimpleHttpServerJsonUtility::CreateJsonString(TEXT("version"), Version);
		return FSimpleHttpServerResponse(
			ESimpleHttpServerResponseCodes::Ok,
			JsonString,
			TEXT("application/json")
		);
	}

	// Note: Dynamic routes have a bug. Please check the progress.
	// https://github.com/EpicGames/UnrealEngine/pull/14168
	if(Request.EndpointPath == UserProfileEndpointPath)
	{
		const FString UserId = Request.PathParams.FindRef(TEXT("id"));
		const FString JsonString = FSimpleHttpServerJsonUtility::CreateJsonString(
			TEXT("profile"),
			UserId
			);

		return FSimpleHttpServerResponse(
			ESimpleHttpServerResponseCodes::Ok,
			JsonString,
			TEXT("application/json")
		);
	}

	UE_LOG(LogTemp, Warning, TEXT("Unhandled GET request for endpoint: %s"), *Request.EndpointPath);
	return FSimpleHttpServerResponse(
		ESimpleHttpServerResponseCodes::NotFound,
		FSimpleHttpServerJsonUtility::CreateJsonString(
			TEXT("message"),
			TEXT("Endpoint not found or not implemented")),
		TEXT("application/json")
	);
}

FSimpleHttpServerResponse USimpleHttpServerSampleApiRouter::HandlePostRequest_Implementation(const FSimpleHttpServerRequest& Request)
{
	if (Request.EndpointPath == EchoEndpointPath)
	{
		// Echo back the received body
		return FSimpleHttpServerResponse(
			ESimpleHttpServerResponseCodes::Ok,
			Request.Body,
			TEXT("application/json")
		);
	}

	UE_LOG(LogTemp, Warning, TEXT("Unhandled POST request for endpoint: %s"), *Request.EndpointPath);
	return FSimpleHttpServerResponse(
		ESimpleHttpServerResponseCodes::NotFound,
		FSimpleHttpServerJsonUtility::CreateJsonString(
			TEXT("message"),
			TEXT("Endpoint not found or not implemented")),
		TEXT("application/json")
	);
}