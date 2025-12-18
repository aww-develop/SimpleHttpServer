// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

SIMPLEHTTPSERVER_API DECLARE_LOG_CATEGORY_EXTERN(LogSimpleHttpServer, Log, All);


class FSimpleHttpServerModule final : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
