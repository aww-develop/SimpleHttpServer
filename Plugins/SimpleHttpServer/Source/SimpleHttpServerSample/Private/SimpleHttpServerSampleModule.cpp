// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleHttpServerSampleModule.h"

DEFINE_LOG_CATEGORY(LogSimpleHttpServerSample);

#define LOCTEXT_NAMESPACE "FSimpleHttpServerSampleModule"

void FSimpleHttpServerSampleModule::StartupModule()
{
}

void FSimpleHttpServerSampleModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSimpleHttpServerSampleModule, SimpleHttpServerSample)
