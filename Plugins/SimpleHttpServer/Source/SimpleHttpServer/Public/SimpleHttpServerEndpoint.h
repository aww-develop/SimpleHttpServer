#pragma once

#include "CoreMinimal.h"
#include "SimpleHttpServerRequestVerbs.h"
#include "SimpleHttpServerEndpoint.generated.h"

USTRUCT(BlueprintType)
struct SIMPLEHTTPSERVER_API FSimpleHttpServerEndpoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer",
		Meta = (Bitmask, BitmaskEnum = "/Script/SimpleHttpServer.ESimpleHttpServerRequestVerbs"))
	int32 RequestVerbs = static_cast<int32>(ESimpleHttpServerRequestVerbs::Get);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleHttpServer")
	FString Path = TEXT("/");

	FString GetSafePath() const
	{
		if (Path.IsEmpty())
		{
			return Path;
		}

		if (Path.Len() == 1 && Path.Equals(TEXT("/")))
		{
			return Path;
		}

		FString Result = Path.TrimStartAndEnd();

		if (!Result.StartsWith(TEXT("/")))
		{
			Result = TEXT("/") + Result;
		}

		if (Result.EndsWith(TEXT("/")))
		{
			Result = Result.LeftChop(1);
		}

		return Result;
	}
};
