#pragma once

#include "CoreMinimal.h"
#include "SimpleHttpServerHeader.generated.h"

USTRUCT(BlueprintType)
struct SIMPLEHTTPSERVER_API FSimpleHttpServerHeader
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	TArray<FString> Values;
};
