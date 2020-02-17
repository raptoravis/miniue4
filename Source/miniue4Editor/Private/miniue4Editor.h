// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "miniue4Editor.generated.h"

class UBlueprint;
class UEdGraph;

UCLASS()
class MINIUE4EDITOR_API UBPNode_SayHello : public UK2Node
{
	GENERATED_BODY()

public:
	// UEdGraphNode interface
	virtual FText GetTooltipText() const override
	{
		return FText::FromString(TEXT("a hello world node"));
	}
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(TEXT("Say Hello World"));
	}
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual FText GetMenuCategory() const
	{
		return FText::FromString(TEXT("MyBlueprintNodes"));
	}
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	// End of UK2Node interface
};
