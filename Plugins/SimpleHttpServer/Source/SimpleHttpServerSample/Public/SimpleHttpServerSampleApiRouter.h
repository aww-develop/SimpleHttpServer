#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleHttpServerApiRouterInterface.h"

#include "SimpleHttpServerSampleApiRouter.generated.h"

UCLASS(Blueprintable)
class USimpleHttpServerSampleApiRouter : public UObject, public ISimpleServerApiRouterInterface
{
	GENERATED_BODY()
public:

	virtual TArray<FSimpleHttpServerEndpoint> GetRoutes_Implementation() override
	{
		return  Routes;
	}

	virtual TArray<FSimpleHttpServerHeader> GetCorsHeaders_Implementation() override
	{
		return CorsHeaders;
	}

	virtual FSimpleHttpServerResponse HandleGetRequest_Implementation(const FSimpleHttpServerRequest& Request) override;
	virtual FSimpleHttpServerResponse HandlePostRequest_Implementation(const FSimpleHttpServerRequest& Request) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServerSampleApiRouter")
	FString Version = TEXT("1.0.0");

private:

	USimpleHttpServerSampleApiRouter();
	
	UPROPERTY()
	TArray<FSimpleHttpServerEndpoint> Routes;

	UPROPERTY()
	TArray<FSimpleHttpServerHeader> CorsHeaders;

	const FString VersionEndpointPath = TEXT("/version");
	const FString EchoEndpointPath = TEXT("/echo");

	// Note: Dynamic routes have a bug. Please check the progress.
	// https://github.com/EpicGames/UnrealEngine/pull/14168
	const FString UserProfileEndpointPath = TEXT("/user/:id/profile");

};
