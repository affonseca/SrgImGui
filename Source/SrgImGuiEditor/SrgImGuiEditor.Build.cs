// © Surgent Studios

using UnrealBuildTool;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SrgImGuiEditor : ModuleRules
	{
		public SrgImGuiEditor(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"Slate",
					"SlateCore",
					"UnrealEd",
					"BlueprintGraph",
					"Kismet",
					"KismetCompiler",
			});

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"ImGui",
					"SrgImGui",
			});
		}
	}
}
