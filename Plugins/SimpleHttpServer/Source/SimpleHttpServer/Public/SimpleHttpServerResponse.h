#pragma once

#include "CoreMinimal.h"
#include "SimpleHttpServerResponseCodes.h"
#include "Utilities/SimpleHttpServerResponseUtility.h"
#include "SimpleHttpServerResponse.generated.h"

USTRUCT(BlueprintType)
struct FSimpleHttpServerResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	ESimpleHttpServerResponseCodes ResponseCode = ESimpleHttpServerResponseCodes::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	FString Body = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	FString ContentType = TEXT("application/json");

	bool TryCreateHttpServerResponse(TUniquePtr<FHttpServerResponse>& OutResponse) const
	{
		OutResponse = FHttpServerResponse::Create(Body, ContentType);
		if (!OutResponse)
		{
			UE_LOG(LogSimpleHttpServer, Error,
			       TEXT("Failed to create HttpServerResponse in FSimpleHttpServerResponse::To()"));
			return false;
		}

		const EHttpServerResponseCodes Code = ToHttpServerResponseCode(ResponseCode);
		OutResponse->Code = Code;

		return true;
	}
};
