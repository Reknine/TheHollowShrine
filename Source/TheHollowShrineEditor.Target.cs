// Copyright Ola Magnusson

using UnrealBuildTool;
using System.Collections.Generic;

public class TheHollowShrineEditorTarget : TargetRules
{
	public TheHollowShrineEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "TheHollowShrine" } );
	}
}
