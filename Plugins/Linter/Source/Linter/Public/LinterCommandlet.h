// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#pragma once
#include "Commandlets/Commandlet.h"
#include "LintRule.h"
#include "LinterCommandlet.generated.h"

UCLASS()
class ULinterCommandlet : public UCommandlet
{
	GENERATED_UCLASS_BODY()
	//~ Begin UCommandlet Interface
	virtual int32 Main(const FString& Params) override;
	//~ End UCommandlet Interface

	//////////////////////////////////////////////////////////////////////////
	// change list
	enum EChangeListOPType
	{
		ECLO_ADD,
		ECLO_DELETE,
		ECLO_MOVE,
		ECLO_CHANGE,
		ECLO_BRANCH,
	};

	enum EPackagesNeedCheckCode
	{
		EPNCC_NORMAL,
		EPNCC_ERROR,
		EPNCC_CHECKALL,
	};

	struct FFileInChangList
	{
		FString FilePath;
		EChangeListOPType ChangeListOP;
	};

	bool GetFilesInChangeList(TArray<FFileInChangList>& FilesInChangList, FString ChangeListFile);

	EPackagesNeedCheckCode GetPackagesNeededCheck(const FString ChangeListFile, TArray<FLintRuleViolation> &RuleViolations, TArray<FString>& PackageNames);

	FORCEINLINE bool IsCheckName() { return !RuleSetName.Compare(TEXT("Name"), ESearchCase::IgnoreCase); }
	FORCEINLINE bool IsCheckBlueprint() { return !RuleSetName.Compare(TEXT("Blueprint"), ESearchCase::IgnoreCase); }
	FORCEINLINE bool IsCheckDependencies() { return !RuleSetName.Compare(TEXT("Dependencies"), ESearchCase::IgnoreCase); }
	bool NeedCheckedFileExt(const FString& FileName);

protected:
	FString RuleSetName;
	//////////////////////////////////////////////////////////////////////////
};
