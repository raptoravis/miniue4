// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Templates/Greater.h"
#include "HAL/IConsoleManager.h"
#include "Misc/App.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectIterator.h"
#include "Engine/Blueprint.h"
#include "EdGraph/EdGraph.h"
#include "Iminiue4EditorModule.h"
#include "miniue4Editor.h"

DEFINE_LOG_CATEGORY_STATIC(Logminiue4Editor, Verbose, All);

class Fminiue4EditorModule : public Iminiue4EditorModule
{
public:
	Fminiue4EditorModule()
		: Dumpminiue4EditorCmd(NULL)
	{
	}

	// IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End of IModuleInterface interface

private:
	IConsoleObject* Dumpminiue4EditorCmd;

};

IMPLEMENT_MODULE(Fminiue4EditorModule, miniue4Editor)


void Fminiue4EditorModule::StartupModule()
{
	if (!IsRunningCommandlet())
	{
		//Dumpminiue4EditorCmd = IConsoleManager::Get().RegisterConsoleCommand(
		//	TEXT("Dumpminiue4Editor"),
		//	TEXT("Dumps statistics about blueprint node usage to the log."),
		//	FConsoleCommandDelegate::CreateStatic(Dumpminiue4Editor),
		//	ECVF_Default
		//	);
	}
}

void Fminiue4EditorModule::ShutdownModule()
{
	if (Dumpminiue4EditorCmd != NULL)
	{
		IConsoleManager::Get().UnregisterConsoleObject(Dumpminiue4EditorCmd);
	}
}


