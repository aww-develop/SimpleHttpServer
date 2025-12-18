// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public sealed class HttpServerSampleTarget : TargetRules
{
	public HttpServerSampleTarget(TargetInfo target) : base(target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("HttpServerSample");
	}
}
