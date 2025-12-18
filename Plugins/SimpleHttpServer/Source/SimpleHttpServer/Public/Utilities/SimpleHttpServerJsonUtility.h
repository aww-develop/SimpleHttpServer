#pragma once
#include "JsonObjectConverter.h"
#include "SimpleHttpServerConcepts.h"

using FSimpleHttpServerJsonVariant = TVariant<float, int, bool, FString, TArray<TSharedPtr<FJsonValue>>>;

class SIMPLEHTTPSERVER_API FSimpleHttpServerJsonUtility
{
public:
	template <struct_type T>
	static TUniquePtr<T> From(const FString& Json)
	{
		TUniquePtr<T> Result = MakeUnique<T>();

		if (FJsonObjectConverter::JsonObjectStringToUStruct(Json, Result.Get(), 0, CPF_SkipSerialization, true))
		{
			return MoveTemp(Result);
		}

		return nullptr;
	}

	template <struct_type T>
	static TSharedRef<FJsonObject> To(const T& StructInstance)
	{
		TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		FJsonObjectConverter::UStructToJsonObject(
			T::StaticStruct(),
			&StructInstance,
			JsonObject,
			0,
			CPF_Deprecated | CPF_SkipSerialization,
			nullptr,
			EJsonObjectConversionFlags::SkipStandardizeCase
		);

		return MoveTemp(JsonObject);
	}

	static TSharedPtr<FJsonObject> CreateJsonObject(const FString& Field, const FString& Value);
	static TSharedPtr<FJsonObject> CreateJsonObject(const FString& Field, const FSimpleHttpServerJsonVariant& Value);

	static FString ToJsonString(const TSharedPtr<FJsonObject>& JsonObject);

	static FString CreateJsonString(const FString& Field, const FString& Value);
	static FString CreateJsonString(const FString& Field, const FSimpleHttpServerJsonVariant& Value);

private:
	static void SetJsonField(const TSharedPtr<FJsonObject>& InOutJsonObject,
	                         const FString& Field,
	                         const FSimpleHttpServerJsonVariant& Value);
};
