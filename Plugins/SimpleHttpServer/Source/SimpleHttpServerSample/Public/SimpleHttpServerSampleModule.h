// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

SIMPLEHTTPSERVERSAMPLE_API DECLARE_LOG_CATEGORY_EXTERN(LogSimpleHttpServerSample, Log, All);


class FSimpleHttpServerSampleModule final : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
