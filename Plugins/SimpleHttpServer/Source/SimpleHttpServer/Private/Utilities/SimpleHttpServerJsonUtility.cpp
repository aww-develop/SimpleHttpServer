#include "Utilities/SimpleHttpServerJsonUtility.h"

#include "SimpleHttpServerModule.h"

TSharedPtr<FJsonObject> FSimpleHttpServerJsonUtility::CreateJsonObject(const FString& Field,
                                                                       const FString& Value)
{
	FSimpleHttpServerJsonVariant Variant = FSimpleHttpServerJsonVariant();
	Variant.Set<FString>(Value);
	return CreateJsonObject(Field, Variant);
}


TSharedPtr<FJsonObject> FSimpleHttpServerJsonUtility::CreateJsonObject(const FString& Field,
                                                                       const FSimpleHttpServerJsonVariant& Value)
{
	const TSharedPtr<FJsonObject> MessageJsonObject = MakeShareable(new FJsonObject());
	SetJsonField(MessageJsonObject, Field, Value);
	return MessageJsonObject;
}

void FSimpleHttpServerJsonUtility::SetJsonField(const TSharedPtr<FJsonObject>& InOutJsonObject,
                                                const FString& Field,
                                                const FSimpleHttpServerJsonVariant& Value)
{
	if (!InOutJsonObject.IsValid())
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("SetJsonField: JsonObject is null."));
		return;
	}

	if (const float* FloatValue = Value.TryGet<float>())
	{
		InOutJsonObject->SetNumberField(Field, *FloatValue);
	}
	else if (const int* IntValue = Value.TryGet<int>())
	{
		InOutJsonObject->SetNumberField(Field, *IntValue);
	}
	else if (const bool* BoolValue = Value.TryGet<bool>())
	{
		InOutJsonObject->SetBoolField(Field, *BoolValue);
	}
	else if (const FString* StringValue = Value.TryGet<FString>())
	{
		InOutJsonObject->SetStringField(Field, *StringValue);
	}
	else if (const TArray<TSharedPtr<FJsonValue>>* ArrayValue = Value.TryGet<TArray<TSharedPtr<FJsonValue>>>())
	{
		InOutJsonObject->SetArrayField(Field, *ArrayValue);
	}
	else
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Unsupported value type for field %s."), *Field);
	}
}

FString FSimpleHttpServerJsonUtility::ToJsonString(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!JsonObject.IsValid())
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("ToJsonString: JsonObject is null."));
		return TEXT("");
	}

	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogSimpleHttpServer, Error, TEXT("Failed to serialize JSON object to string."));
		return TEXT("");
	}

	Writer->Close();
	return JsonString;
}

FString FSimpleHttpServerJsonUtility::CreateJsonString(const FString& Field, const FString& Value)
{
	FSimpleHttpServerJsonVariant Variant = FSimpleHttpServerJsonVariant();
	Variant.Set<FString>(Value);
	return CreateJsonString(Field, Variant);
}

FString FSimpleHttpServerJsonUtility::CreateJsonString(const FString& Field,
                                                       const FSimpleHttpServerJsonVariant& Value)
{
	const TSharedPtr<FJsonObject> JsonObject = CreateJsonObject(Field, Value);
	return ToJsonString(JsonObject);
}
