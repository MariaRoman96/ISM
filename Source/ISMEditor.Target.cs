
using UnrealBuildTool;
using System.Collections.Generic;

public class ISMEditorTarget : TargetRules
{
	public ISMEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("ISM");
	}
}
