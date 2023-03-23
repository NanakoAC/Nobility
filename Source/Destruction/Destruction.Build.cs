using UnrealBuildTool; 

public class Destruction: ModuleRules 

{ 

        public Destruction(ReadOnlyTargetRules Target) : base(Target) 
        { 
			PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"}); 

		} 

}
