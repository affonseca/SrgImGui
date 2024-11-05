// © Surgent Studios

using UnrealBuildTool;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SrgImGui : ModuleRules
	{
		public SrgImGui(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"Slate",
					"DeveloperSettings",
					"InputCore",
					"GameplayTags",
			});

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"ImGui",
			});
		}
	}
}
