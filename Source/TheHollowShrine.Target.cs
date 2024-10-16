// Copyright Ola Magnusson

using UnrealBuildTool;
using System.Collections.Generic;

public class TheHollowShrineTarget : TargetRules
{
	public TheHollowShrineTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "TheHollowShrine" } );
	}
}
