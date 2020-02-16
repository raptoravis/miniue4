// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBlueprintFunctionLibrary.h"

int UTestBlueprintFunctionLibrary::Sum(int a, int b)
{
	return a + b;
}

void UTestBlueprintFunctionLibrary::Generic_ShowStructFields(const void* StructAddr, const UStructProperty* StructProperty)
{
	UScriptStruct* Struct = StructProperty->Struct;
	for (TFieldIterator<UProperty> iter(Struct); iter; ++iter)
	{
		FScreenMessageString NewMessage;
		NewMessage.CurrentTimeDisplayed = 0.0f;
		NewMessage.Key = INDEX_NONE;
		NewMessage.DisplayColor = FColor::Blue;
		NewMessage.TimeToDisplay = 5;
		UProperty* p = *iter;

		auto pValuePtr = p->ContainerPtrToValuePtr<void*>(StructAddr);

		FString outPropertyValueString;
		p->ExportTextItem(outPropertyValueString, pValuePtr, nullptr, (UObject*) StructAddr, PPF_None);

		NewMessage.ScreenMessage =
			FString::Printf(TEXT("Property: [%s].[%s] %s"), *(Struct->GetName()), *(p->GetName()), *outPropertyValueString);

		NewMessage.TextScale = FVector2D::UnitVector;
		GEngine->PriorityScreenMessages.Insert(NewMessage, 0);
	}
}

float UTestBlueprintFunctionLibrary::GenericArray_NumericPropertyAverage(
	const void* TargetArray, const UArrayProperty* ArrayProperty, FName PropertyName)
{
	UStructProperty* InnerProperty = Cast<UStructProperty>(ArrayProperty->Inner);
	if (!InnerProperty)
	{
		UE_LOG(LogTemp, Error, TEXT("Array inner property is NOT a UStruct!"));
		return 0.f;
	}

	UScriptStruct* Struct = InnerProperty->Struct;
	FString PropertyNameStr = PropertyName.ToString();
	UNumericProperty* NumProperty = nullptr;
	for (TFieldIterator<UNumericProperty> iter(Struct); iter; ++iter)
	{
		if (Struct->PropertyNameToDisplayName(iter->GetFName()) == PropertyNameStr)
		{
			NumProperty = *iter;
			break;
		}
	}
	if (!NumProperty)
	{
		UE_LOG(LogTemp, Log, TEXT("Struct property NOT numeric = [%s]"), *(PropertyName.ToString()));
	}

	FScriptArrayHelper ArrayHelper(ArrayProperty, TargetArray);
	int Count = ArrayHelper.Num();
	float Sum = 0.f;

	if (Count <= 0)
		return 0.f;

	if (NumProperty->IsFloatingPoint())
		for (int i = 0; i < Count; i++)
		{
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetFloatingPointPropertyValue(ValuePtr);
		}
	else if (NumProperty->IsInteger())
	{
		for (int i = 0; i < Count; i++)
		{
			void* ElemPtr = ArrayHelper.GetRawPtr(i);
			const uint8* ValuePtr = NumProperty->ContainerPtrToValuePtr<uint8>(ElemPtr);
			Sum += NumProperty->GetSignedIntPropertyValue(ValuePtr);
		}
	}
	// TODO: else if(enum¿‡–Õ)

	return Sum / Count;
}
