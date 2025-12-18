#pragma once

#include "CoreMinimal.h"
#include "HttpRouteHandle.h"
#include "UObject/Object.h"
#include "SimpleHttpServerApiRouterInterface.h"
#include "SimpleHttpServer.generated.h"

class IHttpRouter;

UCLASS(BlueprintType, Blueprintable)
class SIMPLEHTTPSERVER_API USimpleHttpServer : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SimpleHttpServer")
	void Initialize(const TScriptInterface<ISimpleServerApiRouterInterface>& InApiRouter,
	                const int InPort = 9120);

	UFUNCTION(BlueprintCallable, Category = "SimpleHttpServer")
	void StartServer() const;

	UFUNCTION(BlueprintCallable, Category = "SimpleHttpServer")
	void StopServer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	bool bForceIncludeOptionsVerb = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	bool bApplyCorsHeaders = true;

	TMap<FString, FHttpRouteHandle> GetBoundRouteHandles() const
	{
		return RouteHandles;
	}

	virtual void BeginDestroy() override
	{
		StopServer();
		Super::BeginDestroy();
	}

private:
	int Port;

	static const TArray<FSimpleHttpServerHeader> DefaultCorsHeaders;

	void ApplyCorsHeaders(FHttpServerResponse* InOutResponse) const;

	UPROPERTY()
	TScriptInterface<ISimpleServerApiRouterInterface> ApiRouter;

	TSharedPtr<IHttpRouter> HttpRouter;

	// Path : RouteHandle
	TMap<FString, FHttpRouteHandle> RouteHandles = {};

	void RegisterEndpoints(const TArray<FSimpleHttpServerEndpoint>& Endpoints);
	void RegisterEndpoint(const FSimpleHttpServerEndpoint& Endpoint);

	bool HandleRequest(const FHttpServerRequest& Request,
	                   const FHttpResultCallback& OnComplete,
	                   const FSimpleHttpServerEndpoint Endpoint) const;

	using FSimpleHttpServerRequestHandler
		= TFunctionRef<FSimpleHttpServerResponse(UObject*, const FSimpleHttpServerRequest&)>;
	
	static FSimpleHttpServerResponse HandleRequest(
		const TObjectPtr<UObject>& ApiRouterObject,
		const FSimpleHttpServerRequest& Request,
		const FSimpleHttpServerRequestHandler& Exec
	);
	
	static FSimpleHttpServerResponse HandleOptionsRequest();

	void Reset();
};
