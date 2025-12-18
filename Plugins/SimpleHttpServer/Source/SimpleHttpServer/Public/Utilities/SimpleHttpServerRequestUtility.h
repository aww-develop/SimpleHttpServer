#pragma once

#include "HttpServerRequest.h"
#include "SimpleHttpServerConcepts.h"
#include "Utilities/SimpleHttpServerJsonUtility.h"

class FSimpleHttpServerRequestUtility
{
public:
	static FString Deserialize(const FHttpServerRequest& Request)
	{
		const UTF8CHAR* Data = reinterpret_cast<const UTF8CHAR*>(Request.Body.GetData());
		const FUTF8ToTCHAR Conv(Data, Request.Body.Num());
		FString Body(Conv.Length(), Conv.Get());

		// Remove BOM if present
		if (Body.Len() && Body[0] == 0xFEFF)
		{
			Body.RemoveAt(0);
		}
		Body.TrimStartAndEndInline();

		return Body;
	}

	template <struct_type T>
	static TUniquePtr<T> From(const FHttpServerRequest& Request)
	{
		const FString Body = Deserialize(Request);

		if (Body.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("Request body is empty."));
			return nullptr;
		}

		return FSimpleHttpServerJsonUtility::From<T>(Body);
	}
};
