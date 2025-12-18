#pragma once

#include "CoreMinimal.h"
#include "SimpleHttpServerRequestVerbs.generated.h"

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESimpleHttpServerRequestVerbs : uint8
{
	None = 0 UMETA(Hidden),
	Get = 1 << 0,
	Post = 1 << 1,
	Put = 1 << 2,
	Patch = 1 << 3,
	Delete = 1 << 4,
	Options = 1 << 5
};

ENUM_CLASS_FLAGS(ESimpleHttpServerRequestVerbs);

static FString LexToString(const ESimpleHttpServerRequestVerbs In)
{
	if (In == ESimpleHttpServerRequestVerbs::None)
	{
		return TEXT("NONE");
	}

	TArray<FString> Parts;

	if (EnumHasAnyFlags(In, ESimpleHttpServerRequestVerbs::Get))
	{
		Parts.Add(TEXT("GET"));
	}
	if (EnumHasAnyFlags(In, ESimpleHttpServerRequestVerbs::Post))
	{
		Parts.Add(TEXT("POST"));
	}
	if (EnumHasAnyFlags(In, ESimpleHttpServerRequestVerbs::Put))
	{
		Parts.Add(TEXT("PUT"));
	}
	if (EnumHasAnyFlags(In, ESimpleHttpServerRequestVerbs::Patch))
	{
		Parts.Add(TEXT("PATCH"));
	}
	if (EnumHasAnyFlags(In, ESimpleHttpServerRequestVerbs::Delete))
	{
		Parts.Add(TEXT("DELETE"));
	}
	if (EnumHasAnyFlags(In, ESimpleHttpServerRequestVerbs::Options))
	{
		Parts.Add(TEXT("OPTIONS"));
	}

	if (Parts.Num() == 0)
	{
		return TEXT("UNKNOWN");
	}

	return FString::Join(Parts, TEXT("|"));
}
