#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SimpleHttpServerRequest.h"
#include "SimpleHttpServerResponse.h"
#include "Utilities/SimpleHttpServerResponseUtility.h"

#include "SimpleHttpServerApiRouterInterface.generated.h"

struct FHttpServerRequest;
struct FHttpServerResponse;
struct FSimpleHttpServerEndpoint;

UINTERFACE(Blueprintable, BlueprintType)
class USimpleServerApiRouterInterface : public UInterface
{
	GENERATED_BODY()
};


class SIMPLEHTTPSERVER_API ISimpleServerApiRouterInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleHttpServer")
	TArray<FSimpleHttpServerEndpoint> GetRoutes();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleHttpServer")
	TArray<FSimpleHttpServerHeader> GetCorsHeaders();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleHttpServer")
	FSimpleHttpServerResponse HandleGetRequest(const FSimpleHttpServerRequest& Request);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleHttpServer")
	FSimpleHttpServerResponse HandlePostRequest(const FSimpleHttpServerRequest& Request);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleHttpServer")
	FSimpleHttpServerResponse HandlePutRequest(const FSimpleHttpServerRequest& Request);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleHttpServer")
	FSimpleHttpServerResponse HandlePatchRequest(const FSimpleHttpServerRequest& Request);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimpleHttpServer")
	FSimpleHttpServerResponse HandleDeleteRequest(const FSimpleHttpServerRequest& Request);
};
