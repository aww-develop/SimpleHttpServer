#pragma once

#include "CoreMinimal.h"
#include "HttpPath.h"
#include "IPAddress.h"
#include "HttpServerHttpVersion.h"
#include "SimpleHttpServerEndpoint.h"
#include "SimpleHttpServerHeader.h"
#include "SimpleHttpServerModule.h"
#include "Utilities/SimpleHttpServerRequestUtility.h"

#include "SimpleHttpServerRequest.generated.h"

USTRUCT(Blueprintable)
struct SIMPLEHTTPSERVER_API FSimpleHttpServerRequest
{
	FSimpleHttpServerRequest()
	{
	}

	explicit FSimpleHttpServerRequest(const FHttpServerRequest& Request, const FSimpleHttpServerEndpoint& Endpoint)
		: PeerAddress(Request.PeerAddress.Get()->ToString(false))
		  , PeerPort(Request.PeerAddress.Get()->GetPort())
		  , EndpointPath(Endpoint.GetSafePath())
		  , RelativePath(Request.RelativePath.GetPath())
		  , Verb(static_cast<ESimpleHttpServerRequestVerbs>(Request.Verb))
		  , HttpVersion(Request.HttpVersion)
		  , Body(FSimpleHttpServerRequestUtility::Deserialize(Request))
	{
		Headers.Reserve(Request.Headers.Num());

		for (const TPair<FString, TArray<FString>>& Header : Request.Headers)
		{
			FSimpleHttpServerHeader NewHeader;
			NewHeader.Key = Header.Key;
			NewHeader.Values = Header.Value;
			Headers.Add(MoveTemp(NewHeader));
		}

		QueryParams = Request.QueryParams;
		PathParams = Request.PathParams;
	}

	GENERATED_BODY()

	/** The IP address of the peer that initiated the request. */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	FString PeerAddress;

	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	int PeerPort = 0;

	/** Endpoint path template from the server root (starts with '/'), e.g. '/version' or '/user/:id/profile'. */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	FString EndpointPath;

	/** The handler-route-relative HTTP path */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	FString RelativePath;

	/** The HTTP-compliant verb  */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	ESimpleHttpServerRequestVerbs Verb = ESimpleHttpServerRequestVerbs::None;

	/** The request HTTP protocol version */
	HttpVersion::EHttpServerHttpVersion HttpVersion = HttpVersion::EHttpServerHttpVersion::HTTP_VERSION_1_1;

	/** The HTTP headers */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	TArray<FSimpleHttpServerHeader> Headers;

	/** The query parameters */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	TMap<FString, FString> QueryParams;

	/** The path parameters */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	TMap<FString, FString> PathParams;

	/** The raw body contents */
	UPROPERTY(BlueprintReadOnly, Category = "SimpleHttpServer")
	FString Body;
};
