// Copyright 2020 Gamemakin LLC. All Rights Reserved.

#include "LinterCommandlet.h"
#include "Editor.h"
#include "AssetRegistryModule.h"
#include "AssetData.h"
#include "Engine/ObjectLibrary.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Linter.h"
#include "LintRule.h"
#include "LinterSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LinterCommandlet, All, All);

ULinterCommandlet::ULinterCommandlet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsClient = false;
	IsServer = false;
}

static void PrintUsage()
{
	UE_LOG(LinterCommandlet, Display, TEXT("Linter Usage: {Editor}.exe Project.uproject -run=Linter \"/Game/\""));
	UE_LOG(LinterCommandlet, Display, TEXT(""));
	UE_LOG(LinterCommandlet, Display, TEXT("This will run the Linter on the provided project and will scan the supplied directory, example being the project's full Content/Game tree. Can add multiple paths as additional arguments."));
}

int32 ULinterCommandlet::Main(const FString& InParams)
{
	FString Params = InParams;
	// Parse command line.
	TArray<FString> Paths;
	TArray<FString> Switches;
	TMap<FString, FString> ParamsMap;
	UCommandlet::ParseCommandLine(*Params, Paths, Switches, ParamsMap);

	UE_LOG(LinterCommandlet, Display, TEXT("Linter is indeed running!"));
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	UE_LOG(LinterCommandlet, Display, TEXT("Loading the asset registry..."));
	AssetRegistryModule.Get().SearchAllAssets(/*bSynchronousSearch =*/true);
	UE_LOG(LinterCommandlet, Display, TEXT("Finished loading the asset registry. Determining Rule Set..."));

	ULintRuleSet* RuleSet = GetDefault<ULinterSettings>()->DefaultLintRuleSet.LoadSynchronous();
	if (ParamsMap.Contains(TEXT("RuleSet")))
	{
		RuleSetName = *ParamsMap.FindChecked(TEXT("RuleSet"));
		UE_LOG(LinterCommandlet, Display, TEXT("Trying to find Rule Set with Commandlet Name: %s"), *RuleSetName);

		FLinterModule::TryToLoadAllLintRuleSets();

		TArray<FAssetData> FoundRuleSets;
		AssetRegistryModule.Get().GetAssetsByClass(ULintRuleSet::StaticClass()->GetFName(), FoundRuleSets, true);

		for (const FAssetData& RuleSetData : FoundRuleSets)
		{
			ULintRuleSet* LoadedRuleSet = Cast<ULintRuleSet>(RuleSetData.GetAsset());
			if (LoadedRuleSet != nullptr && LoadedRuleSet->NameForCommandlet == RuleSetName)
			{
				RuleSet = LoadedRuleSet;
				UE_LOG(LinterCommandlet, Display, TEXT("Found Rule Set for name %s: %s"), *RuleSetName, *RuleSet->GetFullName());
			}
		}
	}
	else
	{
		UE_LOG(LinterCommandlet, Display, TEXT("Using default rule set..."));
	}

	if (RuleSet == nullptr)
	{
		UE_LOG(LinterCommandlet, Error, TEXT("Failed to load a rule set. Aborting. Returning error code 1."));
		return 2;
	}

	UE_LOG(LinterCommandlet, Display, TEXT("Using rule set: %s"), *RuleSet->GetFullName());

	UE_LOG(LinterCommandlet, Display, TEXT("Try to Parser ChangeList File"));
	bool bSkipPath = false;
	TArray<FLintRuleViolation> RuleViolations;
	if (ParamsMap.Contains(TEXT("CLFile")))
	{
		bSkipPath = true;

		const FString ChangeListFile = *ParamsMap.FindChecked(TEXT("CLFile"));
		TArray<FString> PackageNames;
		EPackagesNeedCheckCode code = GetPackagesNeededCheck(ChangeListFile, RuleViolations, PackageNames);
		switch (code)
		{
		case ULinterCommandlet::EPNCC_ERROR:
			return 2;
		case ULinterCommandlet::EPNCC_CHECKALL:
			if (IsCheckBlueprint())
			{
				TArray<FAssetData> BlueprintList;
				FARFilter Filter;
				Filter.bRecursivePaths = true;
				Filter.bRecursiveClasses = true;
				Filter.PackagePaths.Push(FName(TEXT("/Game")));
				Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
				AssetRegistryModule.Get().GetAssets(Filter, BlueprintList);
				for (const FAssetData& Blueprint : BlueprintList)
					PackageNames.Add(Blueprint.PackageName.ToString());
			}
			else
			{
				bSkipPath = false;
			}
			break;
		default:
			break;
		}

		if (bSkipPath)
		{
			UE_LOG(LinterCommandlet, Display, TEXT("Attempting to Lint Objects..."));
			RuleViolations = RuleSet->LintObjects(PackageNames);
		}
	}
	
	if (!bSkipPath)
	{
		if (Paths.Num() == 0)
		{
			Paths.Add(TEXT("/Game"));
		}

		UE_LOG(LinterCommandlet, Display, TEXT("Attempting to Lint paths: %s"), *FString::Join(Paths, TEXT(", ")));
		RuleViolations = RuleSet->LintPath(Paths);
	}

	int32 NumErrors = 0;
	int32 NumWarnings = 0;

	for (const FLintRuleViolation& Violation : RuleViolations)
	{
		if (Violation.Violator == nullptr)
		{
			NumErrors++;
		}
		else if (Violation.ViolatedRule->GetDefaultObject<ULintRule>()->RuleSeverity <= ELintRuleSeverity::Error)
		{
			NumErrors++;
		}
		else
		{
			NumWarnings++;
		}
	}

	FString ResultsString = FText::FormatNamed(FText::FromString("Lint completed with {NumWarnings} {NumWarnings}|plural(one=warning,other=warnings), {NumErrors} {NumErrors}|plural(one=error,other=errors)."), TEXT("NumWarnings"), FText::FromString(FString::FromInt(NumWarnings)), TEXT("NumErrors"), FText::FromString(FString::FromInt(NumErrors))).ToString();
	UE_LOG(LinterCommandlet, Display, TEXT("Lint completed with %s."), *ResultsString);

	bool bWriteReport = Switches.Contains(TEXT("json")) || ParamsMap.Contains(TEXT("json")) || Switches.Contains(TEXT("html")) || ParamsMap.Contains(TEXT("html"));
	if (bWriteReport)
	{
		UE_LOG(LinterCommandlet, Display, TEXT("Generating output report..."));

		bool DoNull = false;
		TSharedPtr<FJsonObject> RootJsonObject = MakeShareable(new FJsonObject);
		TArray<TSharedPtr<FJsonValue>> ViolatorJsonObjects;

		TArray<UObject*> UniqueViolators = FLintRuleViolation::AllRuleViolationViolators(RuleViolations);
		for (const UObject* Violator : UniqueViolators)
		{
			if (DoNull)
				break;
			else if (!Violator)
				DoNull = true;

			TSharedPtr<FJsonObject> AssetJsonObject = MakeShareable(new FJsonObject);
			const TArray<FLintRuleViolation> UniqueViolatorViolations = FLintRuleViolation::AllRuleViolationsWithViolator(RuleViolations, Violator);

			FAssetData AssetData;
			if (UniqueViolatorViolations.Num() > 0)
			{
				AssetData = UniqueViolatorViolations[0].ViolatorAssetData;
				AssetJsonObject->SetStringField(TEXT("ViolatorAssetName"), AssetData.AssetName.ToString());
				AssetJsonObject->SetStringField(TEXT("ViolatorAssetPath"), AssetData.ObjectPath.ToString());
				AssetJsonObject->SetStringField(TEXT("ViolatorFullName"), AssetData.GetFullName());
#if 1
				//AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*AssetPath));
				AssetJsonObject->SetStringField(TEXT("ViolatorAssetName"), TEXT(""));
				AssetJsonObject->SetStringField(TEXT("ViolatorAssetPath"), TEXT(""));
				if (Violator)
					AssetJsonObject->SetStringField(TEXT("ViolatorFullName"), Violator->GetFullName());
				else
					AssetJsonObject->SetStringField(TEXT("ViolatorFullName"),TEXT("Load Object Failed"));
#endif
				//@TODO: Thumbnail export?

				TArray<TSharedPtr<FJsonValue>> RuleViolationJsonObjects;

				for (const FLintRuleViolation& Violation : UniqueViolatorViolations)
				{
					TSharedPtr<FJsonObject> RuleJsonObject = MakeShareable(new FJsonObject);
					if (Violator)
					{
						ULintRule* LintRule = Violation.ViolatedRule->GetDefaultObject<ULintRule>();
						check(LintRule != nullptr);

						FString RuleSeverity;
						switch (LintRule->RuleSeverity)
						{
						case ELintRuleSeverity::Error:
							RuleSeverity = TEXT("Error: ");
							break;
						case ELintRuleSeverity::Warning:
							RuleSeverity = TEXT("Warning: ");
							break;
						case ELintRuleSeverity::Info:
							RuleSeverity = TEXT("Info: ");
							break;
						default:
							RuleSeverity = TEXT("Unknow: ");
							break;
						}

						RuleJsonObject->SetStringField(TEXT("RuleGroup"), LintRule->RuleGroup.ToString());
						RuleJsonObject->SetStringField(TEXT("RuleTitle"), RuleSeverity + LintRule->RuleTitle.ToString());
						RuleJsonObject->SetStringField(TEXT("RuleDesc"), LintRule->RuleDescription.ToString());
						RuleJsonObject->SetStringField(TEXT("RuleURL"), LintRule->RuleURL);
						RuleJsonObject->SetNumberField(TEXT("RuleSeverity"), (int32)LintRule->RuleSeverity);
						RuleJsonObject->SetStringField(TEXT("RuleRecommendedAction"), Violation.RecommendedAction.ToString());
					}
					else
					{
						RuleJsonObject->SetStringField(TEXT("RuleRecommendedAction"), TEXT("Error: ") + Violation.RecommendedAction.ToString());
					}
					RuleViolationJsonObjects.Push(MakeShareable(new FJsonValueObject(RuleJsonObject)));
				}

				AssetJsonObject->SetArrayField(TEXT("Violations"), RuleViolationJsonObjects);
			}

			ViolatorJsonObjects.Add(MakeShareable(new FJsonValueObject(AssetJsonObject)));
		}

		// Save off our JSON to a string
		RootJsonObject->SetArrayField(TEXT("Violators"), ViolatorJsonObjects);
		FString JsonReport;
		TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&JsonReport);
		FJsonSerializer::Serialize(RootJsonObject.ToSharedRef(), Writer);

		// write json file if requested
		if (Switches.Contains(TEXT("json")) || ParamsMap.Contains(FString(TEXT("json"))))
		{
			FDateTime Now = FDateTime::Now();
			FString JsonOutputName = TEXT("lint-report-") + FDateTime::Now().ToString() + TEXT(".json");

			const FString LintReportPath = FPaths::ProjectSavedDir() / TEXT("LintReports");
			FString FullOutputPath = LintReportPath / JsonOutputName;

			if (ParamsMap.Contains(FString(TEXT("json"))))
			{
				const FString JsonOutputOverride = *ParamsMap.FindChecked(FString(TEXT("json")));
				if (FPaths::IsRelative(JsonOutputOverride))
				{
					JsonOutputName = JsonOutputOverride;
					FullOutputPath = LintReportPath / JsonOutputName;
				}
				else
				{
					FullOutputPath = JsonOutputOverride;
				}
			}

			FullOutputPath = FPaths::ConvertRelativePathToFull(FullOutputPath);
			IFileManager::Get().MakeDirectory(*FPaths::GetPath(FullOutputPath), true);

			UE_LOG(LinterCommandlet, Display, TEXT("Exporting JSON report to %s"), *FullOutputPath);
			if (FFileHelper::SaveStringToFile(JsonReport, *FullOutputPath))
			{
				UE_LOG(LinterCommandlet, Display, TEXT("Exported JSON report successfully."));
			}
			else
			{
				UE_LOG(LinterCommandlet, Error, TEXT("Failed to export JSON report. Aborting. Returning error code 1."));
				return 2;
			}
		}

		// write HTML report if requested
		if (Switches.Contains(TEXT("html")) || ParamsMap.Contains(FString(TEXT("html"))))
		{
			FDateTime Now = FDateTime::Now();
			FString HtmlOutputName = TEXT("lint-report-") + RuleSetName + FDateTime::Now().ToString() + TEXT(".html");

			const FString LintReportPath = FPaths::ProjectSavedDir() / TEXT("LintReports");
			FString FullOutputPath = LintReportPath / HtmlOutputName;

			if (ParamsMap.Contains(FString(TEXT("html"))))
			{
				const FString HtmlOutputOverride = *ParamsMap.FindChecked(TEXT("html"));
				if (FPaths::IsRelative(HtmlOutputName))
				{
					HtmlOutputName = HtmlOutputOverride;
					FullOutputPath = LintReportPath / HtmlOutputName;
				}
				else
				{
					FullOutputPath = HtmlOutputOverride;
				}
			}

			FullOutputPath = FPaths::ConvertRelativePathToFull(FullOutputPath);
			IFileManager::Get().MakeDirectory(*FPaths::GetPath(FullOutputPath), true);
			UE_LOG(LinterCommandlet, Display, TEXT("Exporting HTML report to %s"), *FullOutputPath);

			FString TemplatePath = FPaths::Combine(*IPluginManager::Get().FindPlugin(TEXT("Linter"))->GetBaseDir(), TEXT("Resources"), TEXT("LintReportTemplate.html"));
			UE_LOG(LinterCommandlet, Display, TEXT("Loading HTML report template from %s"), *TemplatePath);

			FString HTMLReport;
			if (FFileHelper::LoadFileToString(HTMLReport, *TemplatePath))
			{
				UE_LOG(LinterCommandlet, Display, TEXT("Loading HTML report template successfully."));

				HTMLReport.ReplaceInline(TEXT("{% TITLE %}"), *FPaths::GetBaseFilename(FPaths::GetProjectFilePath()));
				HTMLReport.ReplaceInline(TEXT("{% RESULTS %}"), *ResultsString);
				HTMLReport.ReplaceInline(TEXT("{% LINT_REPORT %}"), *JsonReport);
			}
			else
			{
				UE_LOG(LinterCommandlet, Error, TEXT("Failed to load HTML report template."));
				return 2;
			}

			if (FFileHelper::SaveStringToFile(HTMLReport, *FullOutputPath))
			{
				UE_LOG(LinterCommandlet, Display, TEXT("Exported HTML report successfully."));
			}
			else
			{
				UE_LOG(LinterCommandlet, Error, TEXT("Failed to export HTML report. Aborting. Returning error code 1."));
				return 2;
			}

			//if (ParamsMap.Contains(TEXT("ReportBaseURL")))
			{
				//const FString ReportBaseURL = *ParamsMap.FindChecked(TEXT("ReportBaseURL"));
				FString URLInfoOutPath = FString::Printf(TEXT("%s/URLInfo%s.txt"), *LintReportPath, *RuleSetName);
				//FString URLInfo = FString::Printf(TEXT("%s"), *(ReportBaseURL + HtmlOutputName));
				FString URLInfo = FString::Printf(TEXT("%s"), *HtmlOutputName);
				if (FFileHelper::SaveStringToFile(URLInfo, *URLInfoOutPath))
				{
					UE_LOG(LinterCommandlet, Display, TEXT("%s"), *URLInfo);
				}
				else
				{
					UE_LOG(LinterCommandlet, Error, TEXT("Failed to save url info. Aborting. Returning error code 1."));
					return 2;
				}
			}
		}
	}

	if (NumErrors > 0 || Switches.Contains(TEXT("TreatWarningsAsErrors")) && NumWarnings > 0)
	{
		UE_LOG(LinterCommandlet, Display, TEXT("Lint completed with errors. Returning error code 2."));
		return 2;
	}

	return 0;
}

bool ULinterCommandlet::GetFilesInChangeList(TArray<ULinterCommandlet::FFileInChangList>& FilesInChangList, FString ChangeListFile)
{
	UE_LOG(LinterCommandlet, Display, TEXT("ChangeList file: %s"), *ChangeListFile);
	TArray<FString> Lines;
	if (FFileHelper::LoadFileToStringArray(Lines, *ChangeListFile))
	{
		for (int32 i = 0; i < Lines.Num(); ++i)
		{
			FString& Line = Lines[i];
			int32 index = 0;
			if (Line.FindLastChar(TEXT('|'), index))
			{
				UE_LOG(LinterCommandlet, Display, TEXT("%s"), *Line);
				FFileInChangList FileInChangList;
				FileInChangList.FilePath = Line.Left(index);
#if 0
				if (!NeedCheckedFileExt(FileInChangList.FilePath))
					continue;

				FString OP = Line.Right(Line.Len() - index - 1);
#else
				FString OP = Line.Right(Line.Len() - index - 1);
				FString Ext = FPaths::GetExtension(FileInChangList.FilePath);
				bool bDel = OP.Equals(TEXT("delete"), ESearchCase::IgnoreCase);
				if (bDel && Ext.IsEmpty() && !FPaths::DirectoryExists(FileInChangList.FilePath))
				{
					FileInChangList.ChangeListOP = EChangeListOPType::ECLO_DELETE;
					FilesInChangList.Add(FileInChangList);
					continue;
				}
				else if (!NeedCheckedFileExt(FileInChangList.FilePath))
					continue;
#endif

				if (OP.Equals(TEXT("add"), ESearchCase::IgnoreCase))
				{
					FileInChangList.ChangeListOP = EChangeListOPType::ECLO_ADD;
					FilesInChangList.Add(FileInChangList);
				}
				else if (OP.Equals(TEXT("delete"), ESearchCase::IgnoreCase))
				{
					FileInChangList.ChangeListOP = EChangeListOPType::ECLO_DELETE;
					FilesInChangList.Add(FileInChangList);
				}
				else if (OP.Equals(TEXT("move/add"), ESearchCase::IgnoreCase))
				{
					FileInChangList.ChangeListOP = EChangeListOPType::ECLO_MOVE;
					FilesInChangList.Add(FileInChangList);
				}
				else if (OP.Equals(TEXT("edit"), ESearchCase::IgnoreCase))
				{
					FileInChangList.ChangeListOP = EChangeListOPType::ECLO_CHANGE;
					FilesInChangList.Add(FileInChangList);
				}
				else if (OP.Equals(TEXT("branch"), ESearchCase::IgnoreCase))
				{
					FileInChangList.ChangeListOP = EChangeListOPType::ECLO_BRANCH;
					FilesInChangList.Add(FileInChangList);
				}
				else
				{
					FileInChangList.ChangeListOP = EChangeListOPType::ECLO_ADD;
					FilesInChangList.Add(FileInChangList);
				}
			}
		}

		return true;
	}

	UE_LOG(LinterCommandlet, Error, TEXT("Load ChangeList file failed!"));
	return false;
}

ULinterCommandlet::EPackagesNeedCheckCode ULinterCommandlet::GetPackagesNeededCheck(const FString ChangeListFile, TArray<FLintRuleViolation> &RuleViolations, TArray<FString>& PackageNames)
{
	TArray<FFileInChangList> FilesInChangList;
	if (!GetFilesInChangeList(FilesInChangList, ChangeListFile))
	{
		UE_LOG(LinterCommandlet, Error, TEXT("Aborting. Returning error code 1."));
		return EPackagesNeedCheckCode::EPNCC_ERROR;
	}

	for (const FFileInChangList& FileInChangList : FilesInChangList)
	{
		if (IsCheckName() && FileInChangList.ChangeListOP == EChangeListOPType::ECLO_DELETE)
			continue;

		if (IsCheckBlueprint() || IsCheckDependencies())
		{
			FString Ext = FPaths::GetExtension(FileInChangList.FilePath);
			bool bDel = FileInChangList.ChangeListOP == EChangeListOPType::ECLO_DELETE;
			if (bDel && Ext.IsEmpty() && !FPaths::DirectoryExists(FileInChangList.FilePath))
			{
				UE_LOG(LinterCommandlet, Display, TEXT("Detected delete dirs, check all!"));
				return EPackagesNeedCheckCode::EPNCC_CHECKALL;
			}
		}

		if (IsCheckBlueprint())
		{
			if (FileInChangList.FilePath.EndsWith(TEXT(".cpp")) || FileInChangList.FilePath.EndsWith(TEXT(".h")))
			{
				UE_LOG(LinterCommandlet, Display, TEXT("Detected c++ code changed, check all"));
				return EPackagesNeedCheckCode::EPNCC_CHECKALL;
			}
		}

		if (FPackageName::IsPackageFilename(FileInChangList.FilePath))
		{
			UE_LOG(LinterCommandlet, Display, TEXT("File: %s"), *FileInChangList.FilePath);
			UE_LOG(LinterCommandlet, Display, TEXT("Status: %d"), FileInChangList.ChangeListOP);
			
			if (FileInChangList.ChangeListOP == EChangeListOPType::ECLO_BRANCH)
			{
				UE_LOG(LinterCommandlet, Display, TEXT("Detected merge branch, check all"));
				return EPackagesNeedCheckCode::EPNCC_CHECKALL;
			}

			if (IsCheckDependencies())
			{
				if (FileInChangList.ChangeListOP == EChangeListOPType::ECLO_DELETE
					|| FileInChangList.ChangeListOP == EChangeListOPType::ECLO_MOVE)
				{
					UE_LOG(LinterCommandlet, Display, TEXT("Detected asset file be deleted or moved, check all"));
					return EPackagesNeedCheckCode::EPNCC_CHECKALL;
				}
			}

			if (IsCheckBlueprint())
			{
				if (FileInChangList.ChangeListOP == EChangeListOPType::ECLO_DELETE)
				{
					UE_LOG(LinterCommandlet, Display, TEXT("Detected asset file be deleted, check all"));
					return EPackagesNeedCheckCode::EPNCC_CHECKALL;
				}
			}

			FString PackageName;
			FString FailureReason;
			if (FPackageName::TryConvertFilenameToLongPackageName(FileInChangList.FilePath, PackageName, &FailureReason))
			{
				UE_LOG(LinterCommandlet, Display, TEXT("Package: %s"), *PackageName);
				if (FileInChangList.ChangeListOP == EChangeListOPType::ECLO_CHANGE || FileInChangList.ChangeListOP == EChangeListOPType::ECLO_MOVE)
				{
					if (IsCheckBlueprint())
					{
						if (LoadObject<UBlueprint>(nullptr, *PackageName, nullptr, LOAD_None, nullptr))
						{
							UE_LOG(LinterCommandlet, Display, TEXT("Detected blueprint or map changed, check all"));
							return EPackagesNeedCheckCode::EPNCC_CHECKALL;
						}
						else if (!LoadObject<UWorld>(nullptr, *PackageName, nullptr, LOAD_None, nullptr))
							continue;
					}
				}

				if (!PackageName.StartsWith(TEXT("/Linter/")))
					PackageNames.Add(PackageName);
			}
			else
			{
				FLintRuleViolation RuleViolation;
				RuleViolation.RecommendedAction = FText::FromString(FString::Printf(TEXT("Can not convert filename to long package, Reson: %s"), *FailureReason));
				RuleViolations.Push(RuleViolation);
			}
		}
	}

	return EPackagesNeedCheckCode::EPNCC_NORMAL;
}

bool ULinterCommandlet::NeedCheckedFileExt(const FString& FileName)
{
	if (FPackageName::IsPackageFilename(FileName) || FileName.EndsWith(TEXT(".cpp")) || FileName.EndsWith(TEXT(".h")))
		return true;
	return false;
}
