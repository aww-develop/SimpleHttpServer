// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public sealed class HttpServerSampleEditorTarget : TargetRules
{
	public HttpServerSampleEditorTarget( TargetInfo tTarget) : base(tTarget)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("HttpServerSample");
	}
}
