
using UnrealBuildTool;

public class ISM : ModuleRules
{
	public ISM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","LevelSequence","MovieScene", "AIModule", "GameplayTasks", "Slate", "SlateCore" });
    }

}
