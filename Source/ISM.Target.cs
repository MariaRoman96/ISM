
using UnrealBuildTool;
using System.Collections.Generic;

public class ISMTarget : TargetRules
{
	public ISMTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Game;
        ExtraModuleNames.Add("ISM");
    }
}
