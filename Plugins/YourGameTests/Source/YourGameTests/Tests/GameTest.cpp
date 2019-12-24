#include "YourGameTests.h"

#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine.h"
#include "EngineUtils.h"

//#include "YourGameModeBase.h"
//#include "MyEssentialActor.h"

// Copy of the hidden method GetAnyGameWorld() in AutomationCommon.cpp.
// Marked as temporary there, hence, this one is temporary, too.
UWorld* GetTestWorld() {
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts) {
        if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game))
            && (Context.World() != nullptr)) {
            return Context.World();
        }
    }

    return nullptr;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameTest, "YourGame.Game",
    EAutomationTestFlags::EditorContext |
    EAutomationTestFlags::ClientContext |
    EAutomationTestFlags::ProductFilter)

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FMyCommand, FGameTest*, Test);

bool FMyCommand::Update() {
	//if(!SomethingHasHappened) return false; // Try again later

	//Test->TestTrue("SomethingWasCaused", SomethingWasCaused);
	return true; // Command completed
}

bool FGameTest::RunTest(const FString& Parameters) {
    AutomationOpenMap(TEXT("/Game/Levels/StartupLevel"));

    UWorld* world = GetTestWorld();

    //TestTrue("GameMode class is set correctly",
    //    world->GetAuthGameMode()->IsA<YourGameModeBase>());
    //TestTrue("Essential actor is spawned",
    //    TActorIterator<AMyEssentialActor>(world));
	ADD_LATENT_AUTOMATION_COMMAND(FMyCommand(this));

    return true;
}




