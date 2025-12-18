#include "Utilities/SimpleHttpServerResponseUtility.h"

#include "SimpleHttpServerModule.h"


TUniquePtr<FHttpServerResponse> FSimpleHttpServerResponseUtility::Success()
{
	return Generate(EHttpServerResponseCodes::Ok, TEXT("success"));
}

TUniquePtr<FHttpServerResponse> FSimpleHttpServerResponseUtility::Generate(const EHttpServerResponseCodes ResponseCode,
                                                                           const FString& Value,
                                                                           const FString& ContentType)
{
	return Generate(ResponseCode, TEXT("message"), Value, ContentType);
}

TUniquePtr<FHttpServerResponse> FSimpleHttpServerResponseUtility::Generate(const EHttpServerResponseCodes ResponseCode,
                                                                           const FString& Field,
                                                                           const FString& Value,
                                                                           const FString& ContentType)
{
	FSimpleHttpServerJsonVariant Variant = FSimpleHttpServerJsonVariant();
	Variant.Set<FString>(Value);
	return Generate(ResponseCode, Field, Variant, ContentType);
}

TUniquePtr<FHttpServerResponse> FSimpleHttpServerResponseUtility::Generate(const EHttpServerResponseCodes ResponseCode,
                                                                           const FString& Field,
                                                                           const FSimpleHttpServerJsonVariant& Value,
                                                                           const FString& ContentType)
{
	if (Field.IsEmpty())
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Field is empty. Cannot generate response."));
		return nullptr;
	}

	const TSharedPtr<FJsonObject> MessageJsonObject = FSimpleHttpServerJsonUtility::CreateJsonObject(Field, Value);
	return Generate(ResponseCode, MessageJsonObject, ContentType);
}

TUniquePtr<FHttpServerResponse> FSimpleHttpServerResponseUtility::Generate(const EHttpServerResponseCodes ResponseCode,
                                                                           const TSharedPtr<FJsonObject>&
                                                                           MessageJsonObject,
                                                                           const FString& ContentType)
{
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (!FJsonSerializer::Serialize(MessageJsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Failed to serialize JSON response: %s "), *JsonString);
		JsonString = TEXT("{\"message\":\"Internal Serialization error\"}");
	}

	Writer->Close();

	TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(JsonString, ContentType);
	Response->Code = ResponseCode;

	return Response;
}
