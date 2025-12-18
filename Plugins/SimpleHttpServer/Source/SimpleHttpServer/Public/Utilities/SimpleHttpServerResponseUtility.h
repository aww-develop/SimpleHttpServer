#pragma once

#include "CoreMinimal.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "SimpleHttpServerConcepts.h"
#include "SimpleHttpServerModule.h"
#include "Utilities/SimpleHttpServerRequestUtility.h"
#include "StructUtils/InstancedStruct.h"
#include "Utilities/SimpleHttpServerJsonUtility.h"


class SIMPLEHTTPSERVER_API FSimpleHttpServerResponseUtility
{
public:
	static TUniquePtr<FHttpServerResponse> Success();

	static TUniquePtr<FHttpServerResponse> Generate(const EHttpServerResponseCodes ResponseCode,
	                                                const FString& Value,
	                                                const FString& ContentType = TEXT("application/json"));

	static TUniquePtr<FHttpServerResponse> Generate(const EHttpServerResponseCodes ResponseCode,
	                                                const FString& Field,
	                                                const FString& Value,
	                                                const FString& ContentType = TEXT("application/json"));

	static TUniquePtr<FHttpServerResponse> Generate(const EHttpServerResponseCodes ResponseCode,
	                                                const FString& Field,
	                                                const FSimpleHttpServerJsonVariant& Value,
	                                                const FString& ContentType = TEXT("application/json"));

	static TUniquePtr<FHttpServerResponse> Generate(const EHttpServerResponseCodes ResponseCode,
	                                                const TSharedPtr<FJsonObject>& MessageJsonObject,
	                                                const FString& ContentType = TEXT("application/json"));
};
