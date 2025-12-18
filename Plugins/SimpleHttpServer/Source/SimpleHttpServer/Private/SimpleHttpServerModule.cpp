// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleHttpServerModule.h"

DEFINE_LOG_CATEGORY(LogSimpleHttpServer);

#define LOCTEXT_NAMESPACE "FSimpleHttpServerModule"

void FSimpleHttpServerModule::StartupModule()
{
}

void FSimpleHttpServerModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSimpleHttpServerModule, SimpleHttpServer)
