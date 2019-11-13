#include "ICE_Button.h"
#include "ICE_Stage.h"
#include "ICE_Wrapper.h"
#include "ICE_BasicFunctionArray.h"
#include "ICE_Array.h"
#include "ICE_ArrayElement.h"
#include "ICE_TriggerCapsule.h"
#include "ICE_BasicFunctionArray.h"
#include "ICE_Tooltip.h"
#include "ICE_BasicFunction.h"
#include "ICE_Enum.h"
#include "ICE_InteractMonitor.h"
#include "ICE_InteractLog.h"
#include "ICE_TableRow.h"
#if WITH_EDITOR
#include <Editor.h>
#include <UnrealEd.h>
#include <LevelUtils.h>
#include <AssetToolsModule.h>
#include <PackageTools.h>
#include <ObjectTools.h>
#include <KismetCompilerModule.h>
#include <KismetEditorUtilities.h>
#include <AssetEditorManager.h>
#endif
#include <BlueprintEditorUtils.h>


bool UICE_Button::isCopiedFunctionData = false;
FFunctionData UICE_Button::copiedFunctionData;
UICE_BasicFunction* UICE_Button::copiedFunctionDataWidget = nullptr;

bool UICE_Button::isCopiedArrayFunctionData = false;
TArray<FFunctionData> UICE_Button::copiedArrayFunctionData;
UICE_BasicFunctionArray* UICE_Button::copiedArrayFunctionDataWidget = nullptr;

bool UICE_Button::isCopiedInteractData = false;
TArray<FInteractData> UICE_Button::copiedInteractData;

TArray<FString> UICE_Button::undoInteractData;

int UICE_Button::undoInteractDataIndex = 0;

bool UICE_Button::isCopiedStageData = false;
FInteractData UICE_Button::copiedStageData;
UICE_Stage* UICE_Button::copiedStageDataWidget = nullptr;

bool UICE_Button::isCopiedTooltipData = false;
FTooltipData UICE_Button::copiedTooltipData;
UICE_Tooltip* UICE_Button::copiedTooltipDataWidget = nullptr;

void UICE_Button::CreateArrayElement(void * dir2, UArrayProperty* arrProp)
{
#if WITH_EDITOR

	UProperty* It2 = arrProp->Inner;
	if (It2->GetCPPType() == "float") {
		TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
		ArrayOfValues->Add(0.f);
	}
	else if (It2->GetCPPType() == "int32") {
		TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
		ArrayOfValues->Add(0);
	}
	else if (It2->GetCPPType() == "FString") {
		TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
		ArrayOfValues->Add("");
	}
	else if (It2->GetCPPType() == "bool") {
		TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
		ArrayOfValues->Add(false);
	}
	else if (It2->GetCPPType() == "FName") {
		TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
		ArrayOfValues->Add(NAME_None);
	}
	else if (It2->GetCPPType() == "FVector") {
		TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
		ArrayOfValues->Add(FVector(0.f, 0.f, 0.f));
	}
	else if (It2->GetCPPType() == "FRotator") {
		TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
		ArrayOfValues->Add(FRotator(0.f, 0.f, 0.f));
	}
	else if (It2->GetCPPType() == "FColor") {
		TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
		ArrayOfValues->Add(FColor(0, 0, 0, 0));
	}
	else if (It2->GetCPPType() == "FFunctionActorData") {
		TArray<FFunctionActorData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionActorData>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Add(InitFunctionActorData());
	}
	else if (It2->GetCPPType() == "FFunctionCheckData") {
		TArray<FFunctionCheckData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckData>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Add(InitFunctionCheckData());
	}
	else if (It2->GetCPPType() == "FFunctionCheckDataBase") {
		TArray<FFunctionCheckDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckDataBase>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Add(InitFunctionCheckDataBase());
	}
	else if (It2->GetCPPType() == "FFunctionDataWrapper") {
		TArray<FFunctionDataWrapper>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataWrapper>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Add(InitFunctionDataWrapper());
	}
	else if (It2->GetCPPType() == "FFunctionDataBase") {
		TArray<FFunctionDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataBase>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Add(InitFunctionDataBase());
	}
	else if (It2->GetCPPType() == "FBaseCreateEditVariable") {
		TArray<FBaseCreateEditVariable>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FBaseCreateEditVariable>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Add(InitBaseCreateEditVariable());
	}
	else if (It2->GetCPPType().StartsWith("U") || It2->GetCPPType().StartsWith("A") || It2->GetCPPType().StartsWith("TSubclassOf")) {
		if (It2->GetCPPType() == "UClass*" || It2->GetCPPType().Contains("TSubclassOf<AActor>")) {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "UCurveFloat*") {
			TArray<UCurveFloat*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UCurveFloat*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "UAnimMontage*") {
			TArray<UAnimMontage*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UAnimMontage*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "UParticleSystem*") {
			TArray<UParticleSystem*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UParticleSystem*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "ULevelSequence*") {
			TArray<ULevelSequence*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<ULevelSequence*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "AActor*") {
			TArray<AActor*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<AActor*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "USoundBase*") {
			TArray<USoundBase*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<USoundBase*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "UMaterialInterface*") {
			TArray<UMaterial*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UMaterial*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<USceneComponent>")) {
			TArray<TSubclassOf<USceneComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<USceneComponent>>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UActorComponent>")) {
			TArray<TSubclassOf<UActorComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UActorComponent>>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType() == "UClass*") {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UUserWidget>")) {
			TArray<TSubclassOf<UUserWidget>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UUserWidget>>>(dir2);
			ArrayOfValues->Add(nullptr);
		}
	}
	else if (It2->GetCPPType() == "TArray") {
		//TODO ARRAY DE ARRAY
	}
#endif
}

void UICE_Button::DeleteAllArrayElement(void * dir2, UArrayProperty* arrProp)
{
#if WITH_EDITOR

	UProperty* It2 = arrProp->Inner;
	if (It2->GetCPPType() == "float") {
		TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "int32") {
		TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FString") {
		TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "bool") {
		TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FName") {
		TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FVector") {
		TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FRotator") {
		TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FColor") {
		TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FFunctionActorData") {
		TArray<FFunctionActorData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionActorData>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FFunctionCheckData") {
		TArray<FFunctionCheckData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckData>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FFunctionCheckDataBase") {
		TArray<FFunctionCheckDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckDataBase>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FFunctionDataWrapper") {
		TArray<FFunctionDataWrapper>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataWrapper>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FFunctionDataBase") {
		TArray<FFunctionDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataBase>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType() == "FBaseCreateEditVariable") {
		TArray<FBaseCreateEditVariable>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FBaseCreateEditVariable>>(dir2);
		ArrayOfValues->Empty();
	}
	else if (It2->GetCPPType().StartsWith("U") || It2->GetCPPType().StartsWith("A") || It2->GetCPPType().StartsWith("TSubclassOf")) {
		if (It2->GetCPPType() == "UClass*" || It2->GetCPPType().Contains("TSubclassOf<AActor>")) {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "UCurveFloat*") {
			TArray<UCurveFloat*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UCurveFloat*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "UAnimMontage*") {
			TArray<UAnimMontage*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UAnimMontage*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "UParticleSystem*") {
			TArray<UParticleSystem*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UParticleSystem*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "ULevelSequence*") {
			TArray<ULevelSequence*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<ULevelSequence*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "AActor*") {
			TArray<AActor*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<AActor*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "USoundBase*") {
			TArray<USoundBase*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<USoundBase*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "UMaterialInterface*") {
			TArray<UMaterial*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UMaterial*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<USceneComponent>")) {
			TArray<TSubclassOf<USceneComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<USceneComponent>>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UActorComponent>")) {
			TArray<TSubclassOf<UActorComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UActorComponent>>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType() == "UClass*") {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->Empty();
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UUserWidget>")) {
			TArray<TSubclassOf<UUserWidget>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UUserWidget>>>(dir2);
			ArrayOfValues->Empty();
		}
	}
	else if (It2->GetCPPType() == "TArray") {
		//TODO ARRAY DE ARRAY
	}
#endif
}

void UICE_Button::InsertArrayElement(void * dir2, UArrayProperty* arrProp, int index)
{
#if WITH_EDITOR

	UProperty* It2 = arrProp->Inner;
	if (It2->GetCPPType() == "float") {
		TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
		ArrayOfValues->Insert(0.f, index);
	}
	else if (It2->GetCPPType() == "int32") {
		TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
		ArrayOfValues->Insert(0, index);
	}
	else if (It2->GetCPPType() == "FString") {
		TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
		ArrayOfValues->Insert("", index);
	}
	else if (It2->GetCPPType() == "bool") {
		TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
		ArrayOfValues->Insert(false, index);
	}
	else if (It2->GetCPPType() == "FName") {
		TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
		ArrayOfValues->Insert(NAME_None, index);
	}
	else if (It2->GetCPPType() == "FVector") {
		TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
		ArrayOfValues->Insert(FVector(0.f, 0.f, 0.f), index);
	}
	else if (It2->GetCPPType() == "FRotator") {
		TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
		ArrayOfValues->Insert(FRotator(0.f, 0.f, 0.f), index);
	}
	else if (It2->GetCPPType() == "FColor") {
		TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
		ArrayOfValues->Insert(FColor(0, 0, 0, 0), index);
	}
	else if (It2->GetCPPType() == "FFunctionActorData") {
		TArray<FFunctionActorData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionActorData>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Insert(InitFunctionActorData(), index);
	}
	else if (It2->GetCPPType() == "FFunctionCheckData") {
		TArray<FFunctionCheckData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckData>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Insert(InitFunctionCheckData(), index);
	}
	else if (It2->GetCPPType() == "FFunctionCheckDataBase") {
		TArray<FFunctionCheckDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckDataBase>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Insert(InitFunctionCheckDataBase(), index);
	}
	else if (It2->GetCPPType() == "FFunctionDataWrapper") {
		TArray<FFunctionDataWrapper>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataWrapper>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Insert(InitFunctionDataWrapper(), index);
	}
	else if (It2->GetCPPType() == "FFunctionDataBase") {
		TArray<FFunctionDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataBase>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Insert(InitFunctionDataBase(), index);
	}
	else if (It2->GetCPPType() == "FBaseCreateEditVariable") {
		TArray<FBaseCreateEditVariable>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FBaseCreateEditVariable>>(dir2);
		ArrayOfValues->Reserve(1);
		ArrayOfValues->Insert(InitBaseCreateEditVariable(), index);
	}
	else if (It2->GetCPPType().StartsWith("U") || It2->GetCPPType().StartsWith("A") || It2->GetCPPType().StartsWith("TSubclassOf")) {
		if (It2->GetCPPType() == "UClass*" || It2->GetCPPType().Contains("TSubclassOf<AActor>")) {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "UCurveFloat*") {
			TArray<UCurveFloat*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UCurveFloat*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "UAnimMontage*") {
			TArray<UAnimMontage*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UAnimMontage*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "UParticleSystem*") {
			TArray<UParticleSystem*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UParticleSystem*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "ULevelSequence*") {
			TArray<ULevelSequence*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<ULevelSequence*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "AActor*") {
			TArray<AActor*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<AActor*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "USoundBase*") {
			TArray<USoundBase*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<USoundBase*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "UMaterialInterface*") {
			TArray<UMaterial*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UMaterial*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<USceneComponent>")) {
			TArray<TSubclassOf<USceneComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<USceneComponent>>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UActorComponent>")) {
			TArray<TSubclassOf<UActorComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UActorComponent>>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType() == "UClass*") {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UUserWidget>")) {
			TArray<TSubclassOf<UUserWidget>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UUserWidget>>>(dir2);
			ArrayOfValues->Reserve(1);
			ArrayOfValues->Insert(nullptr, index);
		}
	}
	else if (It2->GetCPPType() == "TArray") {
		//TODO ARRAY DE ARRAY
	}
#endif
}

void UICE_Button::DeleteArrayElement(void * dir2, UArrayProperty* arrProp, int index)
{
#if WITH_EDITOR

	UProperty* It2 = arrProp->Inner;
	if (It2->GetCPPType() == "float") {
		TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "int32") {
		TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FString") {
		TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "bool") {
		TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FName") {
		TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FVector") {
		TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FRotator") {
		TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FColor") {
		TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FFunctionActorData") {
		TArray<FFunctionActorData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionActorData>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FFunctionCheckData") {
		TArray<FFunctionCheckData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckData>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FFunctionCheckDataBase") {
		TArray<FFunctionCheckDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckDataBase>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FFunctionDataWrapper") {
		TArray<FFunctionDataWrapper>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataWrapper>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FFunctionDataBase") {
		TArray<FFunctionDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataBase>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType() == "FBaseCreateEditVariable") {
		TArray<FBaseCreateEditVariable>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FBaseCreateEditVariable>>(dir2);
		ArrayOfValues->RemoveAt(index);
	}
	else if (It2->GetCPPType().StartsWith("U") || It2->GetCPPType().StartsWith("A") || It2->GetCPPType().StartsWith("TSubclassOf")) {
		if (It2->GetCPPType() == "UClass*" || It2->GetCPPType().Contains("TSubclassOf<AActor>")) {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "UCurveFloat*") {
			TArray<UCurveFloat*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UCurveFloat*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "UAnimMontage*") {
			TArray<UAnimMontage*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UAnimMontage*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "UParticleSystem*") {
			TArray<UParticleSystem*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UParticleSystem*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "ULevelSequence*") {
			TArray<ULevelSequence*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<ULevelSequence*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "AActor*") {
			TArray<AActor*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<AActor*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "USoundBase*") {
			TArray<USoundBase*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<USoundBase*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "UMaterialInterface*") {
			TArray<UMaterial*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UMaterial*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<USceneComponent>")) {
			TArray<TSubclassOf<USceneComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<USceneComponent>>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UActorComponent>")) {
			TArray<TSubclassOf<UActorComponent>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UActorComponent>>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType() == "UClass*") {
			TArray<UClass*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UClass*>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UUserWidget>")) {
			TArray<TSubclassOf<UUserWidget>>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<TSubclassOf<UUserWidget>>>(dir2);
			ArrayOfValues->RemoveAt(index);
		}
	}
	else if (It2->GetCPPType() == "TArray") {
		//TODO ARRAY DE ARRAY
	}
#endif
}

void UICE_Button::NativeConstruct()
{
#if WITH_EDITOR

	GetButton()->OnClicked.AddDynamic(this, &UICE_Button::onClick);
	delegate.AddDynamic(this, &UICE_Button::ButtonAction);
#endif
// 	isCopiedFunctionData = false;
// 	isCopiedArrayFunctionData = false;
}

UICE_BasicFunction* UICE_Button::AddBasicFunctElement(UICE_Stage* stage, UICE_BasicFunctionArray* basicFuncArray, EActionStageType locationStage, int index, int interactIndex, bool addEnd)
{
	UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
	basicFunc->currentIndex = index;
	basicFunc->stage = stage;
	basicFunc->parentArr = basicFuncArray;
	basicFunc->currentStage = stage->stage;
#if WITH_EDITOR

	if (locationStage == EActionStageType::Pre) {
		basicFunc->currentState = { EStateFunctionList::PreStage, interactIndex };
	}
	else if (locationStage == EActionStageType::Init) {
		basicFunc->currentState = { EStateFunctionList::Init, interactIndex };
	}
	else if (locationStage == EActionStageType::Exit) {
		basicFunc->currentState = { EStateFunctionList::Exit, interactIndex };
	}
	else if (locationStage == EActionStageType::Interact) {
		basicFunc->currentState = { EStateFunctionList::Action, interactIndex };
	}
	basicFunc->SetEnum(0);
	basicFunc->GetData();
	basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

	basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
	basicFunc->GetICESearchBar()->basicFun = basicFunc;
	basicFunc->GetICESearchBar()->UpdateValues();

	UICE_Button* functInsButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functInsButton->GetButtonName()->SetText(FText::FromString(""));
	functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
	functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
	functInsButton->m_data = { ETargetInfo::E_Insert, index, basicFunc, "preStage", locationStage, interactIndex };
	basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

	UICE_Button* functCopyButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functCopyButton->GetButtonName()->SetText(FText::FromString(""));
	functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
	functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
	functCopyButton->m_data = { ETargetInfo::E_Copy, index, basicFunc, "preStage", locationStage, interactIndex };
	basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

	UICE_Button* functPasteButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functPasteButton->GetButtonName()->SetText(FText::FromString(""));
	functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
	functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
	functPasteButton->m_data = { ETargetInfo::E_Paste, index, basicFunc, "preStage", locationStage, interactIndex };
	basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

	UICE_Button* functDelButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functDelButton->GetButtonName()->SetText(FText::FromString(""));
	functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
	functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	functDelButton->m_data = { ETargetInfo::E_Delete, index, basicFunc, "preStage", locationStage, interactIndex };
	basicFunc->GetICEButtonOptions()->AddChild(functDelButton);

	if (addEnd) {
		basicFuncArray->basicFunctions.Add(basicFunc);
		basicFuncArray->GetICEContent()->AddChild(basicFunc);
	}
	else {
		basicFuncArray->basicFunctions.Insert(basicFunc, index);
		basicFuncArray->GetICEContent()->InsertChildAt(index, basicFunc);
	}
#endif
	return basicFunc;
}

UICE_Tooltip* UICE_Button::AddTooltipElement(UICE_Stage* stage, int index, bool addEnd /*= true*/, EAction action, UICE_Button* buttonInsert)
{

	FText name = GETENUMDISPLAYNAME(TEXT("EAction"), action);

	UICE_Button* buttonTooltip = nullptr;
	UICE_Tooltip* tooltip = nullptr;

#if WITH_EDITOR
	if (addEnd) {
		buttonTooltip = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		buttonTooltip->GetButtonName()->SetText(name);
		buttonTooltip->targetContent = stage->GetTooltipContent();
		buttonTooltip->m_data = { ETargetInfo::E_Select, index, stage, "tooltip" };
		stage->GetTooltipNavBar()->AddChild(buttonTooltip);
		stage->tooltipNavBarButtons.Add(buttonTooltip);
	}
	else {
		buttonTooltip = buttonInsert;
	}

	tooltip = CreateWidget<UICE_Tooltip>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Tooltip_BP.ICE_tooltip_BP_C'")));
	tooltip->button = buttonTooltip;
	tooltip->stageData = stage;
	tooltip->stage = stage->stage;
	tooltip->tooltipIndex = index;

	UICE_Button* tooltipCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	tooltipCopyButton->GetButtonName()->SetText(FText::FromString(""));
	tooltipCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy Tooltip"))));
	tooltipCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	tooltipCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
	tooltipCopyButton->m_data = { ETargetInfo::E_Copy, index, stage, "tooltip" };
	tooltip->GetTooltipOptions()->AddChild(tooltipCopyButton);

	UICE_Button* tooltipPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	tooltipPasteButton->GetButtonName()->SetText(FText::FromString(""));
	tooltipPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste Tooltip"))));
	tooltipPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	tooltipPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
	tooltipPasteButton->m_data = { ETargetInfo::E_Paste, index, stage, "tooltip" };
	tooltip->GetTooltipOptions()->AddChild(tooltipPasteButton);

	UICE_Button* tooltipDelButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	tooltipDelButton->GetButtonName()->SetText(FText::FromString(""));
	tooltipDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete Tooltip"))));
	tooltipDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	tooltipDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	tooltipDelButton->m_data = { ETargetInfo::E_Delete, index, stage, "tooltip" };
	tooltip->GetTooltipOptions()->AddChild(tooltipDelButton);

	UICE_BasicFunctionArray* stageFuncArray = CreateWidget<UICE_BasicFunctionArray>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunctionArray_BP.ICE_BasicFunctionArray_BP_C'")));
	stageFuncArray->stage = stage;
	FString nameStage = "Functions - Stage";
	stageFuncArray->GetICEName()->SetText(FText::FromString(nameStage));

	UICE_Button* stageFunctButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stageFunctButton->GetButtonName()->SetText(FText::FromString(""));
	stageFunctButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create function"))));
	stageFunctButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stageFunctButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
	stageFunctButton->m_data = { ETargetInfo::E_Create, index, stageFuncArray, "stageInteract", EActionStageType::Interact, index };
	stageFuncArray->GetICEButtonOptions()->AddChild(stageFunctButton);
	tooltip->stageButton = stageFunctButton;

	UICE_Button* stageArrFunctCopyButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stageArrFunctCopyButton->GetButtonName()->SetText(FText::FromString(""));
	stageArrFunctCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy function"))));
	stageArrFunctCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stageArrFunctCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
	stageArrFunctCopyButton->m_data = { ETargetInfo::E_Copy, index, stageFuncArray, "stageInteract", EActionStageType::Interact, index };
	stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctCopyButton);

	UICE_Button* stageArrFunctPasteButton = CreateWidget<UICE_Button>(stage, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stageArrFunctPasteButton->GetButtonName()->SetText(FText::FromString(""));
	stageArrFunctPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste function"))));
	stageArrFunctPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stageArrFunctPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
	stageArrFunctPasteButton->m_data = { ETargetInfo::E_Paste, index, stageFuncArray, "stageInteract", EActionStageType::Interact, index };
	stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctPasteButton);

	UICE_Button* stageArrFunctDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stageArrFunctDelButton->GetButtonName()->SetText(FText::FromString(""));
	stageArrFunctDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete function"))));
	stageArrFunctDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stageArrFunctDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	stageArrFunctDelButton->m_data = { ETargetInfo::E_Delete, index, stageFuncArray, "stageInteract", EActionStageType::Interact, index };
	stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctDelButton);

	tooltip->GetStageFunction()->AddChild(stageFuncArray);
	tooltip->stageFunc = stageFuncArray;

	if (addEnd) {
		stage->GetTooltipContent()->AddChild(tooltip);
	}
	else {
		stage->GetTooltipContent()->InsertChildAt(index, tooltip);
	}
	tooltip->UpdateImageSelector();
	tooltip->UpdateBackgroundSelector();
#endif
	return tooltip;
}

UICE_Stage* UICE_Button::AddStageElement(UICE_Wrapper* wrapper, int index, bool addEnd /*= true*/, UICE_Button* buttonInsert /*= nullptr*/)
{
	UICE_Stage* stage = nullptr;
#if WITH_EDITOR

	UICE_Button* button = nullptr;
	if (addEnd) {
		button = CreateWidget<UICE_Button>(wrapper, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		button->GetButtonName()->SetText(FText::FromString(wrapper->interactComponent->interactDataArray[index].stageTitle + " (" + FString::FromInt(index) + ")"));
		button->GetButtonName()->SetToolTipText(FText::FromString(wrapper->interactComponent->interactDataArray[index].stageDescription));
		button->targetContent = wrapper->GetContent();
		button->m_data = { ETargetInfo::E_Select, index, wrapper, "stage" };
		wrapper->GetNavBar()->AddChild(button);
		wrapper->navBarButtons.Add(button);
	}
	else {
		button = buttonInsert;
	}

	 stage = CreateWidget<UICE_Stage>(wrapper, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Stage_BP.ICE_Stage_BP_C'")));
	stage->stage = index;
	stage->wrapper = wrapper;
	stage->button = button;
	FString namePrio = "Priority";
	stage->GetPriority()->GetICENameContainer()->SetText(FText::FromString(namePrio));
	if (stage->GetPriority()->GetICEEnum()->GetOptionCount() == 0) {
		int maxE = (int)GETENUMMAX(TEXT("EPriority"));
		for (int j = 0; j < maxE; j++) {
			stage->GetPriority()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EPriority"), (EPriority)j));
		}
	}
	stage->GetPriority()->GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EPriority"), wrapper->interactComponent->interactDataArray[index].priorityStage));
	//stage->GetPriority()->searchBar = CreateWidget<UICE_Search>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Search_BP.ICE_Search_BP_C'")));
	stage->GetPriority()->GetICESearchBar()->selector = stage->GetPriority()->GetICEEnum();
	stage->GetPriority()->GetICESearchBar()->UpdateValues();
	stage->GetPriorityRepeat()->SetIsChecked(wrapper->interactComponent->interactDataArray[index].doOtherInteract);

	if (addEnd) {
		wrapper->stages.Push(stage);
		wrapper->GetContent()->AddChild(stage);
	}
	else {
		wrapper->stages.Insert(stage, index);
		wrapper->GetContent()->InsertChildAt(index, stage);
	}
#endif
	return stage;
}

void UICE_Button::onClick()
{
#if WITH_EDITOR

	delegate.Broadcast(this->m_data);
#endif
}

void UICE_Button::ButtonAction(FTargetData data)
{
#if WITH_EDITOR

	UICE_Wrapper* wrapper = Cast<UICE_Wrapper>(data.wrapper);
	UICE_Stage* stage = Cast<UICE_Stage>(data.wrapper);
	UICE_BasicFunctionArray* funcArr = Cast<UICE_BasicFunctionArray>(data.wrapper);
	UICE_BasicFunction* funct = Cast<UICE_BasicFunction>(data.wrapper);
	UICE_Array* arr = Cast<UICE_Array>(data.wrapper);
	UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(data.wrapper);
	UICE_InteractMonitor* monitor = Cast<UICE_InteractMonitor>(data.wrapper);
	UICE_InteractLog* intLog = Cast<UICE_InteractLog>(data.wrapper);

	if (data.info == ETargetInfo::E_Undo) {
		if (wrapper) {
			if (wrapper->interactComponent && undoInteractData.Num() > 0) {
				if (undoInteractData.IsValidIndex(undoInteractDataIndex-1)) {
					undoInteractDataIndex = undoInteractDataIndex - 1;
					wrapper->interactComponent->ImportInteractData(undoInteractData[undoInteractDataIndex]);
					wrapper->OnLoadStart();
					FTimerDelegate loadingDel;
					loadingDel.BindUFunction(wrapper, FName("GetAllData"), false, 0);
					GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_Redo) {
		if (wrapper) {
			if (wrapper->interactComponent && undoInteractData.Num() > 0) {
				if (undoInteractData.IsValidIndex(undoInteractDataIndex + 1)) {
					undoInteractDataIndex = undoInteractDataIndex + 1;
					wrapper->interactComponent->ImportInteractData(undoInteractData[undoInteractDataIndex]);
					wrapper->OnLoadStart();
					FTimerDelegate loadingDel;
					loadingDel.BindUFunction(wrapper, FName("GetAllData"), false, 0);
					GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);
				}
			}
		}
	}


	if (data.info == ETargetInfo::E_Select && targetContent) {
		targetContent->SetActiveWidgetIndex(data.index);

		if (wrapper) {
			wrapper->UpdateNavBar(data.index);
		}
		else if (stage)
		{
			if (data.navBarName == "capsule")
			{
				stage->UpdateCapsuleNavBar(data.index);
			}
			else if (data.navBarName == "tooltip")
			{
				stage->UpdateTooltipNavBar(data.index);
			}
		}
	}
	else if (data.info == ETargetInfo::E_Create) {

		if (wrapper) {
			wrapper->UpdateData();
			if (wrapper->interactComponent) {
				wrapper->interactComponent->m_currentStage = wrapper->interactComponent->interactDataArray.Num();
				data.index = wrapper->interactComponent->interactDataArray.Num();
				wrapper->interactComponent->interactDataArray.Add(InitInteractData());
				AddStageElement(wrapper, data.index)->GetData();
			}

			//wrapper->UpdateData();
			//wrapper->ResetWrapper(false);
			//wrapper->BuildEditor();

			wrapper->UpdateNavBar(data.index);
			wrapper->GetContent()->SetActiveWidgetIndex(data.index);

		}
		else if (stage)
		{
			if (data.navBarName == "capsule")
			{
				//POSIBLE OPTION; MULTIPLE TRIGGER CAPSULE CREATION
			}
			else if (data.navBarName == "tooltip")
			{
				int currentStage = 0;
				stage->wrapper->UpdateData();
				if (stage->wrapper->interactComponent) {
					currentStage = stage->wrapper->interactComponent->m_currentStage;
					data.index = stage->wrapper->interactComponent->interactDataArray[currentStage].actions.Num();
					stage->wrapper->interactComponent->interactDataArray[currentStage].actions.Add(InitTooltipData());
					stage->wrapper->interactComponent->interactDataArray[currentStage].actions[stage->wrapper->interactComponent->interactDataArray[currentStage].actions.Num() - 1].m_quickTimeEvent = false;

					UICE_Tooltip* toolCreat = AddTooltipElement(stage, data.index);
					toolCreat->GetData();
					//stage->GetTooltipContent()->AddChild(toolCreat);
				}

				//stage->wrapper->UpdateData();
				//stage->wrapper->ResetWrapper(false);
				stage->UpdateTooltipNavBar(data.index);
				stage->GetTooltipContent()->SetActiveWidgetIndex(data.index);
				//stage->wrapper->BuildEditor();

				stage->wrapper->UpdateNavBar(currentStage);
				stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);
			}
		}
		else if (funcArr) {
			int currentStage = 0;
			funcArr->stage->wrapper->UpdateData();
			if (funcArr->stage->wrapper->interactComponent) {
				currentStage = funcArr->stage->wrapper->interactComponent->m_currentStage;
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction.Add(InitFunctionData());
					if (funcArr->stage->GetPreStageFunctions()->GetAllChildren().Num() > 0) {
						basicFuncArray = Cast<UICE_BasicFunctionArray>(funcArr->stage->GetPreStageFunctions()->GetAllChildren()[0]);
						if (basicFuncArray) {
							AddBasicFunctElement(funcArr->stage, basicFuncArray, data.functionState, funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction.Num()-1);
						}
					}
				}
				else if (data.functionState == EActionStageType::Init) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction.Add(InitFunctionData());
					if (funcArr->stage->GetCapsuleContent()->GetAllChildren().Num() > 0) {
						UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(funcArr->stage->GetCapsuleContent()->GetAllChildren()[0]);
						if (capsule2) {
							if (capsule2->GetInitContent()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetInitContent()->GetAllChildren()[0]);
								if (basicFuncArray) {
									AddBasicFunctElement(funcArr->stage, basicFuncArray, data.functionState, funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction.Num()-1);
								}
							}
						}
					}
				}
				else if (data.functionState == EActionStageType::Exit) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction.Add(InitFunctionData());
					if (funcArr->stage->GetCapsuleContent()->GetAllChildren().Num() > 0) {
						UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(funcArr->stage->GetCapsuleContent()->GetAllChildren()[0]);
						if (capsule2) {
							if (capsule2->GetExitContent()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetExitContent()->GetAllChildren()[0]);
								if (basicFuncArray) {
									AddBasicFunctElement(funcArr->stage, basicFuncArray, data.functionState, funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction.Num()-1);
								}
							}
						}
					}
				}
				else if (data.functionState == EActionStageType::Interact) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction.Add(InitFunctionData());
					if (funcArr->stage->GetTooltipContent()->GetAllChildren().IsValidIndex(data.functionIndex)) {
						UICE_Tooltip* capsule2 = Cast<UICE_Tooltip>(funcArr->stage->GetTooltipContent()->GetAllChildren()[data.functionIndex]);
						if (capsule2) {
							if (capsule2->GetStageFunction()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetStageFunction()->GetAllChildren()[0]);
								if (basicFuncArray) {
									AddBasicFunctElement(funcArr->stage, basicFuncArray, data.functionState, funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction.Num()-1, data.functionIndex);
								}
							}
						}
					}
				}
			}

			//funcArr->stage->wrapper->UpdateData();
// 			funcArr->stage->wrapper->ResetWrapper(false);
// 			funcArr->stage->wrapper->BuildEditor();

			funcArr->stage->wrapper->UpdateNavBar(currentStage);
			funcArr->stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);
			if (funcArr->stage->wrapper->stages.IsValidIndex(currentStage) && data.functionState == EActionStageType::Interact) {
				funcArr->stage->wrapper->stages[currentStage]->GetTooltipContent()->SetActiveWidgetIndex(data.functionIndex);
				funcArr->stage->wrapper->stages[currentStage]->UpdateTooltipNavBar(data.functionIndex);
			}

			funcArr->GetICEExpandable()->SetIsExpanded(true);
		}
		else if (arr) {
			int currentStage = 0;
			if (arr->stage->wrapper->interactComponent) {
				currentStage = arr->stage->wrapper->interactComponent->m_currentStage;
			}
			arr->basicFunc->UpdateData();
			CreateArrayElement(arr->parent, arr->It);

			// 			arr->basicFunc->UpdateData();
			arr->basicFunc->GetData();
			for (int i = 0; i < arr->basicFunc->actorArr.Num(); i++) {
				if (arr->basicFunc->actorArr[i]) {
					if (arr->basicFunc->actorArr[i]->array->typeArray == arr->typeArray) {
						arr->basicFunc->actorArr[i]->array->GetICEExpandable()->SetIsExpanded(true);
					}
				}
			}
			// 			arr->stage->wrapper->UpdateData();
			// 			arr->stage->wrapper->ResetWrapper(false);
			// 			arr->stage->wrapper->BuildEditor();
			// 
			// 			arr->stage->wrapper->UpdateNavBar(currentStage);
			// 			arr->stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);
			// 
			// 			arr->stage->UpdateTooltipNavBar(data.index);
			// 			arr->stage->GetTooltipContent()->SetActiveWidgetIndex(data.index);
		}
		else if (monitor && data.navBarName == "addBP") {
			if (monitor->GetTableContent() && monitor->GetTableContent()->GetAllChildren().IsValidIndex(data.index)) {
				UICE_TableRow* tablRow = Cast<UICE_TableRow>(monitor->GetTableContent()->GetAllChildren()[data.index]);
				if (tablRow) {
					//FString procedence = tablRow->m_procedence;
					//AActor* acto = tablRow->m_actor;
// 					AActor* acto = nullptr;
// 					if (ISM_ISVALID(tablRow->m_actor)) {
// 						acto = UInteractCore::FindActorByName(tablRow->m_rowActor, tablRow->m_actor->GetWorld(), tablRow->m_actor->GetClass());
// 					}
// 					else {
// 						acto = UInteractCore::FindActorByName(tablRow->m_rowActor);
// 					}
					AActor* acto = UInteractCore::FindActorByName(tablRow->m_rowActor);
// 					if (acto) {
// 						ISM_PRINTLOG(acto->GetName())
// 					}
					if (acto && GEditor->CanSelectActor(acto, true, true, true)) {
						//ULevel* levelAct = acto->GetLevel();
						FString name = acto->GetName();

						FString pathS = "Blueprint'/Game/" + name + "." + name + "'";
						FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
						FAssetData asstData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*pathS));
						UBlueprint* bp = Cast<UBlueprint>(asstData.GetAsset());
						FString otherName = name;
						int valNu = 1;
						while (bp) {
							otherName = name + "(" + FString::FromInt(valNu) + ")";
							pathS = "Blueprint'/Game/" + otherName + "." + otherName + "'";
							asstData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*pathS));
							bp = Cast<UBlueprint>(asstData.GetAsset());
							valNu++;
						}
						name = otherName;

						FString packagePath = "/Game/" + name;
						UPackage* OuterForAsset = CreatePackage(nullptr, *packagePath);


						UBlueprint* newBlueprint = FKismetEditorUtilities::CreateBlueprintFromActor(packagePath, acto, false, true);
						//FAssetRegistryModule::AssetCreated(newBlueprint);
						//FAssetEditorManager::Get().OpenEditorForAsset(newBlueprint);
						TArray<AActor*> Actors;
						Actors.Add(acto);

						FVector Location = acto->GetActorLocation();
						FRotator Rotator = acto->GetActorRotation();

						AActor* NewActor = FKismetEditorUtilities::CreateBlueprintInstanceFromSelection(newBlueprint, Actors, Location, Rotator);
						if (NewActor)
						{
							NewActor->SetActorScale3D(acto->GetActorScale3D());
							FString newName = acto->GetName();
							UWorld* worldA = acto->GetWorld();
							ULevel* lvlA = acto->GetLevel();
							FString tempName = newName + "_tempInvisibleInteract";
							acto->Rename(*tempName, lvlA);
							//worldA->RemoveActor(acto, true);
							//worldA->ForceGarbageCollection(true);
							UInteractCore::ISM_Destroy(acto);
							NewActor->Rename(*newName, lvlA);
							lvlA->Actors.Add(NewActor);

						}

						monitor->ChangeActor(data.index, NewActor);

						OuterForAsset->SetDirtyFlag(true);
						TArray<UPackage*> packagesToSave;
						packagesToSave.Add(OuterForAsset);
						FEditorFileUtils::PromptForCheckoutAndSave(packagesToSave, false, false);


						AActor* remAct1 = UInteractCore::FindActorByName("REINST_", true);
						if (remAct1) {
							UInteractCore::ISM_Destroy(remAct1);
						}


						FString procedence = tablRow->m_procedence;
						//ULevel* levl = tablRow->m_level;

						FTargetData dat2 = { ETargetInfo::E_Search, 0, monitor, procedence };
						ButtonAction(dat2);

					}
				}
			}
		}
		else if (monitor && data.navBarName == "addInteract") {
			if (monitor->GetTableContent() && monitor->GetTableContent()->GetAllChildren().IsValidIndex(data.index)) {
				UICE_TableRow* tablRow = Cast<UICE_TableRow>(monitor->GetTableContent()->GetAllChildren()[data.index]);
				if (tablRow) {
					AActor* acto = UInteractCore::FindActorByName(tablRow->m_rowActor);
					if (acto && GEditor->CanSelectActor(acto, true, true, true)) {
						FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

						bool hasBPBefore = false;

						FString pathS2 = "Blueprint'" + acto->GetClass()->GetPathName().LeftChop(2) + "'";
						FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
						FAssetData asstData2 = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*pathS2));
						UBlueprint* bp2= Cast<UBlueprint>(asstData2.GetAsset());
						UBlueprint* newBlueprint = bp2;
						FString name = acto->GetName();
						FString packagePath = "/Game/" + name;
						UPackage* OuterForAsset = nullptr;
						if (!bp2) {

							FString pathS = "Blueprint'/Game/" + name + "." + name + "'";
							FAssetRegistryModule& AssetRegistryModule2 = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
							FAssetData asstData = AssetRegistryModule2.Get().GetAssetByObjectPath(FName(*pathS));
							UBlueprint* bp = Cast<UBlueprint>(asstData.GetAsset());
							FString otherName = name;
							int valNu = 1;
							while (bp) {
								otherName = name + "(" + FString::FromInt(valNu) + ")";
								pathS = "Blueprint'/Game/" + otherName + "." + otherName + "'";
								asstData = AssetRegistryModule2.Get().GetAssetByObjectPath(FName(*pathS));
								bp = Cast<UBlueprint>(asstData.GetAsset());
								valNu++;
							}
							name = otherName;

							OuterForAsset = CreatePackage(nullptr, *packagePath);


							newBlueprint = FKismetEditorUtilities::CreateBlueprintFromActor(packagePath, acto, false, true);
						}
						else {
							hasBPBefore = true;
						}
						
						if (newBlueprint) {
							UStaticMeshComponent* statMesh = Cast<UStaticMeshComponent>(acto->GetComponentByClass(UStaticMeshComponent::StaticClass()));
							FBlueprintEditorUtils::ImplementNewInterface(newBlueprint, "InteractInterface");
							//UInteractCore::ReparentBlueprint(newBlueprint, AItem::StaticClass(), false);

							FString strInt = "Interact";
							FName nameInt = FName(*strInt);
							UInteractComponent* newInteract = NewObject<UInteractComponent>(newBlueprint, UInteractComponent::StaticClass(), nameInt);
							if (newInteract) {
								newInteract->SetFlags(EObjectFlags::RF_Public);
							}
							UInteractCore::AddComponentToBlueprint(newBlueprint, newInteract);

							UInteractCore::CompileBlueprint(newBlueprint);

							TArray<AActor*> Actors;
							Actors.Add(acto);

							FVector Location = acto->GetActorLocation();
							FRotator Rotator = acto->GetActorRotation();

							if (!OuterForAsset) {
								OuterForAsset = FindPackage(nullptr, *acto->GetClass()->GetPathName().LeftChop(2));
							}

							AActor* NewActor = nullptr;
							if (Actors.Num() > 0 && ISM_ISVALID(Actors[0])){
								NewActor = FKismetEditorUtilities::CreateBlueprintInstanceFromSelection(newBlueprint, Actors, Location, Rotator);
							}
							if (NewActor)
							{
								UStaticMeshComponent* otherStatMesh = nullptr;
								AActor* outAct = NewActor->GetClass()->GetDefaultObject<AActor>();
								TArray<UActorComponent*> usc2 = outAct->GetComponentsByClass(UStaticMeshComponent::StaticClass());
								for (int h = 0; h < usc2.Num(); h++) {
									otherStatMesh = Cast<UStaticMeshComponent>(usc2[h]);
									if (otherStatMesh) {
										break;
									}
								}

								if (ISM_ISVALID(statMesh) && ISM_ISVALID(otherStatMesh)) {
									otherStatMesh->SetStaticMesh(statMesh->GetStaticMesh());
									for (int f = 0; f < statMesh->GetMaterials().Num(); f++) {
										if (ISM_ISVALID(statMesh->GetMaterials()[f])) {
											otherStatMesh->SetMaterial(f, statMesh->GetMaterials()[f]);
										}
									}
									UStaticMeshComponent* innStatMesh = Cast<UStaticMeshComponent>(NewActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
									if (innStatMesh) {
										innStatMesh->SetStaticMesh(statMesh->GetStaticMesh());
										for (int f = 0; f < statMesh->GetMaterials().Num(); f++) {
											innStatMesh->SetMaterial(f, statMesh->GetMaterials()[f]);
										}
									}
								}
								if (outAct) {
									outAct->MarkPackageDirty();
								}



								NewActor->SetActorScale3D(acto->GetActorScale3D());
								FString newName = acto->GetName();
								UWorld* worldA = acto->GetWorld();
								ULevel* lvlA = acto->GetLevel();
								FString tempName = newName + "_tempInvisibleInteract";
								acto->Rename(*tempName, lvlA);
								UInteractCore::ISM_Destroy(acto);
								NewActor->Rename(*newName, lvlA);
								lvlA->Actors.Add(NewActor);

							}

							monitor->ChangeActor(data.index, NewActor);

							if (OuterForAsset) {
								OuterForAsset->SetDirtyFlag(true);
								TArray<UPackage*> packagesToSave;
								packagesToSave.Add(OuterForAsset);
								FEditorFileUtils::PromptForCheckoutAndSave(packagesToSave, false, false);
							}

							AActor* remAct1 = UInteractCore::FindActorByName("REINST_", true);
							if (remAct1) {
								UInteractCore::ISM_Destroy(remAct1);
							}

							FString procedence = tablRow->m_procedence;

							if (hasBPBefore) {
								AActor* oldActor = UInteractCore::FindActorByName(tablRow->m_rowActor);
								FString newActNamm = "REINST_" + tablRow->m_rowActor;
								AActor* newActor = UInteractCore::FindActorByName(newActNamm, true);

								if (oldActor && newActor) {
									UWorld* worldB = oldActor->GetWorld();
									ULevel* lvlB = oldActor->GetLevel();
									TArray<AActor*> Actors4;
									Actors4.Add(oldActor);
									FString name4A = oldActor->GetName();
									FVector Location4 = oldActor->GetActorLocation();
									FRotator Rotator4 = oldActor->GetActorRotation();
									FVector Scale4 = oldActor->GetActorScale3D();
									FString deleteName1 = "_InteractDelete1" + NewActor->GetName();
									NewActor->Rename(*deleteName1, lvlB);
									UInteractCore::ISM_Destroy(NewActor);
									FString deleteName2 = "_InteractDelete2" + oldActor->GetName();
									oldActor->Rename(*deleteName2, lvlB);
									UInteractCore::ISM_Destroy(oldActor);
									FString deleteName3 = "_InteractDelete3" + newActor->GetName();
									newActor->Rename(*deleteName3, lvlB);
									UInteractCore::ISM_Destroy(newActor);


									AActor* NewActor2 = FKismetEditorUtilities::CreateBlueprintInstanceFromSelection(newBlueprint, Actors4, Location4, Rotator4);
									if (NewActor2)
									{
										NewActor2->SetActorScale3D(Scale4);
										NewActor2->Rename(*name4A, lvlB);
										lvlB->Actors.Add(NewActor2);
									}
								}
							}

							FTargetData dat2 = { ETargetInfo::E_Search, 0, monitor, procedence };
							ButtonAction(dat2);


						}

					}
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_Delete) {
		if (funct) {
			int currentStage = 0;
			funct->stage->wrapper->UpdateData();
			if (funct->stage->wrapper->interactComponent) {
				currentStage = funct->stage->wrapper->interactComponent->m_currentStage;
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction.RemoveAt(data.index);
					funct->stage->GetPreStageData();
				}
				else if (data.functionState == EActionStageType::Init) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction.RemoveAt(data.index);
					funct->stage->GetInitData();
				}
				else if (data.functionState == EActionStageType::Exit) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction.RemoveAt(data.index);
					funct->stage->GetExitData();
				}
				else if (data.functionState == EActionStageType::Interact) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction.RemoveAt(data.index);
					funct->stage->GetStageData(data.functionIndex);
				}
			}

			funct->stage->wrapper->UpdateNavBar(currentStage);
			funct->stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);
			if (funct->stage->wrapper->stages.IsValidIndex(currentStage) && data.functionState == EActionStageType::Interact) {
				funct->stage->wrapper->stages[currentStage]->GetTooltipContent()->SetActiveWidgetIndex(data.functionIndex);
				funct->stage->wrapper->stages[currentStage]->UpdateTooltipNavBar(data.functionIndex);
			}

			funct->parentArr->GetICEExpandable()->SetIsExpanded(true);
		}
		else if (funcArr) {
			int currentStage = 0;
			funcArr->stage->wrapper->UpdateData();
			if (funcArr->stage->wrapper->interactComponent) {
				currentStage = funcArr->stage->wrapper->interactComponent->m_currentStage;
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction.Empty();
					funcArr->stage->GetPreStageData();
				}
				else if (data.functionState == EActionStageType::Init) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction.Empty();
					funcArr->stage->GetInitData();
				}
				else if (data.functionState == EActionStageType::Exit) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction.Empty();
					funcArr->stage->GetExitData();
				}
				else if (data.functionState == EActionStageType::Interact) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction.Empty();
					funcArr->stage->GetStageData(data.functionIndex);
				}
			}

			funcArr->stage->wrapper->UpdateNavBar(currentStage);
			funcArr->stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);
			if (funcArr->stage->wrapper->stages.IsValidIndex(currentStage) && data.functionState == EActionStageType::Interact) {
				funcArr->stage->wrapper->stages[currentStage]->GetTooltipContent()->SetActiveWidgetIndex(data.functionIndex);
				funcArr->stage->wrapper->stages[currentStage]->UpdateTooltipNavBar(data.functionIndex);
			}

		}
		else if (arr) {
			int currentStage = 0;
			if (arr->stage->wrapper->interactComponent) {
				currentStage = arr->stage->wrapper->interactComponent->m_currentStage;
			}
			arr->basicFunc->UpdateData();
			DeleteAllArrayElement(arr->parent, arr->It);

			arr->basicFunc->GetData();
			for (int i = 0; i < arr->basicFunc->actorArr.Num(); i++) {
				if (arr->basicFunc->actorArr[i]) {
					if (arr->basicFunc->actorArr[i]->array->typeArray == arr->typeArray) {
						arr->basicFunc->actorArr[i]->array->GetICEExpandable()->SetIsExpanded(true);
					}
				}
			}
		}
		else if (arrElem) {
			int currentStage = 0;
			if (arrElem->stage->wrapper->interactComponent) {
				currentStage = arrElem->stage->wrapper->interactComponent->m_currentStage;
			}
			arrElem->basicFunc->UpdateData();
			DeleteArrayElement(arrElem->parent, arrElem->It, data.index);

			// 			arrElem->basicFunc->UpdateData();
			arrElem->basicFunc->GetData();
			for (int i = 0; i < arrElem->basicFunc->actorArr.Num(); i++) {
				if (arrElem->basicFunc->actorArr[i]) {
					if (arrElem->basicFunc->actorArr[i]->array->typeArray == arrElem->array->typeArray) {
						arrElem->basicFunc->actorArr[i]->array->GetICEExpandable()->SetIsExpanded(true);
					}
				}
			}
		}
		else if (stage)
		{
			if (data.navBarName == "stage")
			{
				bool bResetBP = true;
				const FText Title = FText::FromString("Delete interact stage data");
				const FText Message = FText::FromString("Do you want to delete interact stage data?");

				FSuppressableWarningDialog::FSetupInfo Info(Message, Title, "Warning_DeleteInteractStageData");
				Info.ConfirmText = FText::FromString("Delete");
				Info.CancelText = FText::FromString("Cancel");

				FSuppressableWarningDialog ReparentBlueprintDlg(Info);
				if (ReparentBlueprintDlg.ShowModal() == FSuppressableWarningDialog::Cancel)
				{
					bResetBP = false;
				}

				if (bResetBP) {
					stage->wrapper->UpdateData();
					if (stage->wrapper->interactComponent) {
						int numElem = stage->wrapper->interactComponent->interactDataArray.Num();
						stage->wrapper->interactComponent->interactDataArray.RemoveAt(data.index);

						if (stage->wrapper->GetNavBar()->GetAllChildren().IsValidIndex(data.index)) {
							stage->wrapper->GetNavBar()->RemoveChildAt(data.index);
						}
						if (stage->wrapper->GetContent()->GetAllChildren().IsValidIndex(data.index)) {
							stage->wrapper->GetContent()->RemoveChildAt(data.index);
						}
						if (numElem == 1) {
							stage->wrapper->interactComponent->interactDataArrayString = "";
						}
					}

					stage->wrapper->OnLoadStart();
					FTimerDelegate loadingDel;
					loadingDel.BindUFunction(stage->wrapper, FName("GetAllData"), false, 0);
					GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);

				}
			}
			else if (data.navBarName == "tooltip")
			{
				int currentStage = 0;
				stage->wrapper->UpdateData();
				if (stage->wrapper->interactComponent) {
					currentStage = stage->wrapper->interactComponent->m_currentStage;
					stage->wrapper->interactComponent->interactDataArray[currentStage].actions.RemoveAt(data.index);

					stage->GetTooltipContent()->RemoveChildAt(data.index);
					stage->GetTooltipData();
				}

				stage->wrapper->UpdateNavBar(currentStage);
				stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);

				stage->UpdateTooltipNavBar(0);
				stage->GetTooltipContent()->SetActiveWidgetIndex(0);
			}
		}
		else if (wrapper) {
			bool bResetBP = true;
			const FText Title = FText::FromString("Delete all interact data");
			const FText Message = FText::FromString("Do you want to delete all interact data?");

			FSuppressableWarningDialog::FSetupInfo Info(Message, Title, "Warning_DeleteInteractData");
			Info.ConfirmText = FText::FromString("Delete");
			Info.CancelText = FText::FromString("Cancel");

			FSuppressableWarningDialog ReparentBlueprintDlg(Info);
			if (ReparentBlueprintDlg.ShowModal() == FSuppressableWarningDialog::Cancel)
			{
				bResetBP = false;
			}

			if (bResetBP) {

				if (wrapper->interactComponent) {
					wrapper->interactComponent->interactDataArray.Empty();
					wrapper->interactComponent->interactDataArrayString = "";
				}

				wrapper->OnLoadStart();
				FTimerDelegate loadingDel;
				loadingDel.BindUFunction(wrapper, FName("GetAllData"), false, -1);
				GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);

			}
		}
	}
	else if (data.info == ETargetInfo::E_Insert) {
		if (funct) {
			int currentStage = 0;
			funct->stage->wrapper->UpdateData();
			if (funct->stage->wrapper->interactComponent) {
				currentStage = funct->stage->wrapper->interactComponent->m_currentStage;
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction.Insert(InitFunctionData(), data.index);
					funct->stage->GetPreStageData();
				}
				else if (data.functionState == EActionStageType::Init) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction.Insert(InitFunctionData(), data.index);
					funct->stage->GetInitData();
				}
				else if (data.functionState == EActionStageType::Exit) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction.Insert(InitFunctionData(), data.index);
					funct->stage->GetExitData();
				}
				else if (data.functionState == EActionStageType::Interact) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction.Insert(InitFunctionData(), data.index);
					funct->stage->GetStageData(data.functionIndex);
				}
			}

			funct->stage->wrapper->UpdateNavBar(currentStage);
			funct->stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);

			if (funct->stage->wrapper->stages.IsValidIndex(currentStage) && data.functionState == EActionStageType::Interact) {
				funct->stage->wrapper->stages[currentStage]->GetTooltipContent()->SetActiveWidgetIndex(data.functionIndex);
				funct->stage->wrapper->stages[currentStage]->UpdateTooltipNavBar(data.functionIndex);
			}
			funct->parentArr->GetICEExpandable()->SetIsExpanded(true);
		}
		else if (arrElem) {
			int currentStage = 0;
			arrElem->basicFunc->UpdateData();
			if (arrElem->stage->wrapper->interactComponent) {
				currentStage = arrElem->stage->wrapper->interactComponent->m_currentStage;
			}
			InsertArrayElement(arrElem->parent, arrElem->It, data.index);

			arrElem->basicFunc->GetData();
			for (int i = 0; i < arrElem->basicFunc->actorArr.Num(); i++) {
				if (arrElem->basicFunc->actorArr[i]) {
					if (arrElem->basicFunc->actorArr[i]->array->typeArray == arrElem->array->typeArray) {
						arrElem->basicFunc->actorArr[i]->array->GetICEExpandable()->SetIsExpanded(true);
					}
				}
			}
		}
		else if (stage) {
			stage->wrapper->UpdateData();
			if (stage->wrapper->interactComponent) {
				stage->wrapper->interactComponent->interactDataArray.Insert(InitInteractData(), data.index);
			}

			stage->wrapper->OnLoadStart();
			FTimerDelegate loadingDel;
			loadingDel.BindUFunction(stage->wrapper, FName("GetAllData"), false, data.index);
			GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);
		}
	}
	else if (data.info == ETargetInfo::E_Copy) {
		if (funct) {
			int currentStage = 0;
			if (funct->stage->wrapper->interactComponent) {
				currentStage = funct->stage->wrapper->interactComponent->m_currentStage;
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					copiedFunctionData = funct->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[data.index];
					isCopiedFunctionData = true;
					if (funct->stage->GetPreStageFunctions()->GetAllChildren().Num() > 0) {
						basicFuncArray = Cast<UICE_BasicFunctionArray>(funct->stage->GetPreStageFunctions()->GetAllChildren()[0]);
						if (basicFuncArray) {
							if (basicFuncArray->GetICEContent()->GetAllChildren().IsValidIndex(data.index)) {
								UICE_BasicFunction* basicF = Cast<UICE_BasicFunction>(basicFuncArray->GetICEContent()->GetAllChildren()[data.index]);
								if (basicF) {
									copiedFunctionDataWidget = basicF;
								}
							}
						}
					}
				}
				else if (data.functionState == EActionStageType::Init) {
					copiedFunctionData = funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[data.index];
					isCopiedFunctionData = true;
					if (funct->stage->GetCapsuleContent()->GetAllChildren().Num() > 0) {
						UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(funct->stage->GetCapsuleContent()->GetAllChildren()[0]);
						if (capsule2) {
							if (capsule2->GetInitContent()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetInitContent()->GetAllChildren()[0]);
								if (basicFuncArray) {
									if (basicFuncArray->GetICEContent()->GetAllChildren().IsValidIndex(data.index)) {
										UICE_BasicFunction* basicF = Cast<UICE_BasicFunction>(basicFuncArray->GetICEContent()->GetAllChildren()[data.index]);
										if (basicF) {
											copiedFunctionDataWidget = basicF;
										}
									}
								}
							}
						}
					}
				}
				else if (data.functionState == EActionStageType::Exit) {
					copiedFunctionData = funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[data.index];
					isCopiedFunctionData = true;
					if (funct->stage->GetCapsuleContent()->GetAllChildren().Num() > 0) {
						UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(funct->stage->GetCapsuleContent()->GetAllChildren()[0]);
						if (capsule2) {
							if (capsule2->GetExitContent()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetExitContent()->GetAllChildren()[0]);
								if (basicFuncArray) {
									if (basicFuncArray->GetICEContent()->GetAllChildren().IsValidIndex(data.index)) {
										UICE_BasicFunction* basicF = Cast<UICE_BasicFunction>(basicFuncArray->GetICEContent()->GetAllChildren()[data.index]);
										if (basicF) {
											copiedFunctionDataWidget = basicF;
										}
									}
								}
							}
						}
					}
				}
				else if (data.functionState == EActionStageType::Interact) {
					copiedFunctionData = funct->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction[data.index];
					isCopiedFunctionData = true;
					if (funct->stage->GetTooltipContent()->GetAllChildren().IsValidIndex(data.functionIndex)) {
						UICE_Tooltip* capsule2 = Cast<UICE_Tooltip>(funct->stage->GetTooltipContent()->GetAllChildren()[data.functionIndex]);
						if (capsule2) {
							if (capsule2->GetStageFunction()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetStageFunction()->GetAllChildren()[0]);
								if (basicFuncArray) {
									if (basicFuncArray->GetICEContent()->GetAllChildren().IsValidIndex(data.index)) {
										UICE_BasicFunction* basicF = Cast<UICE_BasicFunction>(basicFuncArray->GetICEContent()->GetAllChildren()[data.index]);
										if (basicF) {
											copiedFunctionDataWidget = basicF;
										}
									}
								}
							}
						}
					}
				}
			}


		}
		else if (funcArr) {
			int currentStage = 0;
			if (funcArr->stage->wrapper->interactComponent) {
				currentStage = funcArr->stage->wrapper->interactComponent->m_currentStage;
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					copiedArrayFunctionData = funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction;
					isCopiedArrayFunctionData = true;
					if (funcArr->stage->GetPreStageFunctions()->GetAllChildren().Num() > 0) {
						basicFuncArray = Cast<UICE_BasicFunctionArray>(funcArr->stage->GetPreStageFunctions()->GetAllChildren()[0]);
						if (basicFuncArray) {
							copiedArrayFunctionDataWidget = basicFuncArray;
						}
					}
				}
				else if (data.functionState == EActionStageType::Init) {
					copiedArrayFunctionData = funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction;
					isCopiedArrayFunctionData = true;
					if (funcArr->stage->GetCapsuleContent()->GetAllChildren().Num() > 0) {
						UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(funcArr->stage->GetCapsuleContent()->GetAllChildren()[0]);
						if (capsule2) {
							if (capsule2->GetInitContent()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetInitContent()->GetAllChildren()[0]);
								if (basicFuncArray) {
									copiedArrayFunctionDataWidget = basicFuncArray;
								}
							}
						}
					}
				}
				else if (data.functionState == EActionStageType::Exit) {
					copiedArrayFunctionData = funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction;
					isCopiedArrayFunctionData = true;
					if (funcArr->stage->GetCapsuleContent()->GetAllChildren().Num() > 0) {
						UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(funcArr->stage->GetCapsuleContent()->GetAllChildren()[0]);
						if (capsule2) {
							if (capsule2->GetExitContent()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetExitContent()->GetAllChildren()[0]);
								if (basicFuncArray) {
									copiedArrayFunctionDataWidget = basicFuncArray;
								}
							}
						}
					}
				}
				else if (data.functionState == EActionStageType::Interact) {
					copiedArrayFunctionData = funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction;
					isCopiedArrayFunctionData = true;
					if (funcArr->stage->GetTooltipContent()->GetAllChildren().IsValidIndex(data.functionIndex)) {
						UICE_Tooltip* capsule2 = Cast<UICE_Tooltip>(funcArr->stage->GetTooltipContent()->GetAllChildren()[data.functionIndex]);
						if (capsule2) {
							if (capsule2->GetStageFunction()->GetAllChildren().Num() > 0) {
								basicFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetStageFunction()->GetAllChildren()[0]);
								if (basicFuncArray) {
									copiedArrayFunctionDataWidget = basicFuncArray;
								}
							}
						}
					}
				}
			}
		}
		else if (wrapper) {
			if (wrapper->interactComponent) {
				copiedInteractData = wrapper->interactComponent->interactDataArray;
				isCopiedInteractData = true;
			}
		}
		else if (stage) {
			if (data.navBarName == "stage")
			{
				if (stage->wrapper->interactComponent) {
					copiedStageData = stage->wrapper->interactComponent->interactDataArray[data.index];
					isCopiedStageData = true;
					if (stage->wrapper->GetContent()->GetAllChildren().IsValidIndex(data.index)) {
						UICE_Stage* basicF = Cast<UICE_Stage>(stage->wrapper->GetContent()->GetAllChildren()[data.index]);
						if (basicF) {
							copiedStageDataWidget = basicF;
						}
					}
				}
			}
			else if (data.navBarName == "tooltip")
			{
				if (stage->wrapper->interactComponent) {
					int currentStage = stage->wrapper->interactComponent->m_currentStage;
					copiedTooltipData = stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.index];
					isCopiedTooltipData = true;
					if (stage->wrapper->GetContent()->GetAllChildren().IsValidIndex(currentStage)) {
						UICE_Stage* basicF = Cast<UICE_Stage>(stage->wrapper->GetContent()->GetAllChildren()[currentStage]);
						if (basicF) {
							if (basicF->GetTooltipContent()->GetAllChildren().IsValidIndex(data.index)) {
								UICE_Tooltip* toolF = Cast<UICE_Tooltip>(basicF->GetTooltipContent()->GetAllChildren()[data.index]);
								copiedTooltipDataWidget = toolF;
							}
						}
					}
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_Paste) {
		if (funct && isCopiedFunctionData) {
			int currentStage = 0;
			if (funct->stage->wrapper->interactComponent) {
				currentStage = funct->stage->wrapper->interactComponent->m_currentStage;
				funct->stage->wrapper->UpdateData();
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[data.index] = copiedFunctionData;
					funct->stage->GetPreStageData();
				}
				else if (data.functionState == EActionStageType::Init) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[data.index] = copiedFunctionData;
					funct->stage->GetInitData();
				}
				else if (data.functionState == EActionStageType::Exit) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[data.index] = copiedFunctionData;
					funct->stage->GetExitData();
				}
				else if (data.functionState == EActionStageType::Interact) {
					funct->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction[data.index] = copiedFunctionData;
					funct->stage->GetStageData(data.functionIndex);
				}
			}

			funct->stage->wrapper->UpdateNavBar(currentStage);
			funct->stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);

			if (funct->stage->wrapper->stages.IsValidIndex(currentStage) && data.functionState == EActionStageType::Interact) {
				funct->stage->wrapper->stages[currentStage]->GetTooltipContent()->SetActiveWidgetIndex(data.functionIndex);
				funct->stage->wrapper->stages[currentStage]->UpdateTooltipNavBar(data.functionIndex);
			}
			funct->parentArr->GetICEExpandable()->SetIsExpanded(true);
			funct->GetICEExpandable()->SetIsExpanded(true);
		}
		else if (funcArr && isCopiedArrayFunctionData) {
			int currentStage = 0;
			if (funcArr->stage->wrapper->interactComponent) {
				currentStage = funcArr->stage->wrapper->interactComponent->m_currentStage;
				funcArr->stage->wrapper->UpdateData();
				UICE_BasicFunctionArray* basicFuncArray = nullptr;
				if (data.functionState == EActionStageType::Pre) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction = copiedArrayFunctionData;
					funcArr->stage->GetPreStageData();
				}
				else if (data.functionState == EActionStageType::Init) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction = copiedArrayFunctionData;
					funcArr->stage->GetInitData();
				}
				else if (data.functionState == EActionStageType::Exit) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction = copiedArrayFunctionData;
					funcArr->stage->GetExitData();
				}
				else if (data.functionState == EActionStageType::Interact) {
					funcArr->stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.functionIndex].stageFunction = copiedArrayFunctionData;
					funcArr->stage->GetStageData(data.functionIndex);
				}
			}

			funcArr->stage->wrapper->UpdateNavBar(currentStage);
			funcArr->stage->wrapper->GetContent()->SetActiveWidgetIndex(currentStage);
			funcArr->GetICEExpandable()->SetIsExpanded(true);
			if (funcArr->stage->wrapper->stages.IsValidIndex(currentStage) && data.functionState == EActionStageType::Interact) {
				funcArr->stage->wrapper->stages[currentStage]->GetTooltipContent()->SetActiveWidgetIndex(data.functionIndex);
				funcArr->stage->wrapper->stages[currentStage]->UpdateTooltipNavBar(data.functionIndex);
			}

		}
		else if (wrapper && isCopiedInteractData) {
			if (wrapper->interactComponent) {
				wrapper->interactComponent->interactDataArray = copiedInteractData;
			}

			wrapper->OnLoadStart();
			FTimerDelegate loadingDel;
			loadingDel.BindUFunction(wrapper, FName("GetAllData"), false, -1);
			GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);
		}
		else if (stage && isCopiedStageData && data.navBarName == "stage") {
			if (stage->wrapper->interactComponent) {
				stage->wrapper->interactComponent->interactDataArray[data.index] = copiedStageData;
			}

			stage->wrapper->OnLoadStart();
			FTimerDelegate loadingDel;
			loadingDel.BindUFunction(stage->wrapper, FName("GetAllData"), false, -1);
			GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);

		}
		else if (stage && isCopiedTooltipData && data.navBarName == "tooltip")
		{
			if (stage->wrapper->interactComponent) {
				int currentStage = stage->wrapper->interactComponent->m_currentStage;
				stage->wrapper->interactComponent->interactDataArray[currentStage].actions[data.index] = copiedTooltipData;
			}

			stage->GetTooltipData();

			stage->UpdateTooltipNavBar(data.index);
			stage->GetTooltipContent()->SetActiveWidgetIndex(data.index);
		}
	}
	else if (data.info == ETargetInfo::E_ResetToBP) {
		if (wrapper) {
			bool bResetBP = true;
			const FText Title = FText::FromString("Reset Data From Blueprint");
			const FText Message = FText::FromString("Do you want to reset data from blueprint?");

			FSuppressableWarningDialog::FSetupInfo Info(Message, Title, "Warning_ResetDataFromBlueprintInteractEditor");
			Info.ConfirmText = FText::FromString("Reset");
			Info.CancelText = FText::FromString("Cancel");

			FSuppressableWarningDialog ReparentBlueprintDlg(Info);
			if (ReparentBlueprintDlg.ShowModal() == FSuppressableWarningDialog::Cancel)
			{
				bResetBP = false;
			}

			if (bResetBP) {
				if (wrapper->interactComponentBP) {
					if (wrapper->interactComponent) {
						wrapper->interactComponentBP->ImportInteractDataInternal();
						wrapper->interactComponent->interactDataArray = wrapper->interactComponentBP->interactDataArray;
 						wrapper->interactComponentBP->ExportInteractDataInternal();
						wrapper->interactComponent->m_initStage = wrapper->interactComponentBP->m_initStage;
						wrapper->interactComponent->m_ticksNum = wrapper->interactComponentBP->m_ticksNum;
						wrapper->interactComponent->m_ticksTime = wrapper->interactComponentBP->m_ticksTime;
						wrapper->interactComponent->GetOwner()->MarkPackageDirty();
					}
				}
				else {
					if (wrapper->interactComponent) {
						wrapper->interactComponent->interactDataArray.Empty();
					}
				}
				wrapper->OnLoadStart();
				FTimerDelegate loadingDel;
				loadingDel.BindUFunction(wrapper, FName("GetAllData"), false, -1);
				GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 1.f, false);
			}
		}
		else if (monitor && data.navBarName == "reset") {
			bool bResetBP = true;
			const FText Title = FText::FromString("Reset data from blueprint");
			const FText Message = FText::FromString("Do you want to reset interact data from blueprint?");

			FSuppressableWarningDialog::FSetupInfo Info(Message, Title, "Warning_ResetDataFromBlueprintInteractMonitor");
			Info.ConfirmText = FText::FromString("Reset");
			Info.CancelText = FText::FromString("Cancel");

			FSuppressableWarningDialog ReparentBlueprintDlg(Info);
			if (ReparentBlueprintDlg.ShowModal() == FSuppressableWarningDialog::Cancel)
			{
				bResetBP = false;
			}

			if (bResetBP) {
				if (monitor->GetTableContent() && monitor->GetTableContent()->GetAllChildren().IsValidIndex(data.index)) {
					UICE_TableRow* tablRow = Cast<UICE_TableRow>(monitor->GetTableContent()->GetAllChildren()[data.index]);
					if (tablRow) {
						FString procedence = tablRow->m_procedence;
						AActor* acto = tablRow->m_actor;
						if (acto) {

							UInteractComponent* intComponent = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
							UInteractComponent* interactComponentBP = nullptr;
							if (intComponent) {
								AActor* actorInteractBP = acto->GetClass()->GetDefaultObject<AActor>();
								if (actorInteractBP) {
									TArray<UObject*> usc;
									acto->GetClass()->CollectDefaultSubobjects(usc, true);
									for (int h = 0; h < usc.Num(); h++) {
										interactComponentBP = Cast<UInteractComponent>(usc[h]->GetOuter());
										if (interactComponentBP) {
											break;
										}
									}
								}

								if (interactComponentBP) {
									intComponent->ImportInteractDataInternal();
									interactComponentBP->ImportInteractDataInternal();
									if (intComponent) {
										intComponent->interactDataArray = interactComponentBP->interactDataArray;
										intComponent->ExportInteractDataInternal();
										interactComponentBP->ExportInteractDataInternal();
										intComponent->m_initStage = interactComponentBP->m_initStage;
										intComponent->m_ticksNum = interactComponentBP->m_ticksNum;
										intComponent->m_ticksTime = interactComponentBP->m_ticksTime;
										intComponent->GetOwner()->MarkPackageDirty();
									}
								}
								else {
									if (intComponent) {
										intComponent->interactDataArray.Empty();
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_SetBP) {
		bool bResetBP = true;
		const FText Title = FText::FromString("Save data to blueprint");
		const FText Message = FText::FromString("Do you want to save interact data to blueprint?");

		FSuppressableWarningDialog::FSetupInfo Info(Message, Title, "Warning_SaveDataToBlueprintInteract");
		Info.ConfirmText = FText::FromString("Save");
		Info.CancelText = FText::FromString("Cancel");

		FSuppressableWarningDialog ReparentBlueprintDlg(Info);
		if (ReparentBlueprintDlg.ShowModal() == FSuppressableWarningDialog::Cancel)
		{
			bResetBP = false;
		}

		if (bResetBP) {

			if (wrapper) {
				if (wrapper->interactComponentBP) {
					if (wrapper->interactComponent) {
						wrapper->interactComponentBP->ImportInteractDataInternal();
						wrapper->interactComponentBP->interactDataArray = wrapper->interactComponent->interactDataArray;
 						wrapper->interactComponentBP->ExportInteractDataInternal();
						wrapper->interactComponentBP->m_initStage = wrapper->interactComponent->m_initStage;
						wrapper->interactComponentBP->m_ticksNum = wrapper->interactComponent->m_ticksNum;
						wrapper->interactComponentBP->m_ticksTime = wrapper->interactComponent->m_ticksTime;
					}
					if (wrapper->actorInteractBP) {
						wrapper->actorInteractBP->MarkPackageDirty();
					}
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_Open) {
		if (wrapper) {
			if (wrapper->interactComponent) {
				FString pathS = "Blueprint'" + wrapper->interactComponent->GetOwner()->GetClass()->GetPathName().LeftChop(2) + "'";
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
				FAssetData asstData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*pathS));
				UBlueprint* bp = Cast<UBlueprint>(asstData.GetAsset());
				if (bp) {
					GEditor->EditObject(bp);
				}
			}

		}
		else if (monitor && data.navBarName == "openActor") {
			if (monitor->GetTableContent() && monitor->GetTableContent()->GetAllChildren().IsValidIndex(data.index)) {
				UICE_TableRow* tablRow = Cast<UICE_TableRow>(monitor->GetTableContent()->GetAllChildren()[data.index]);
				if (tablRow) {
					FString procedence = tablRow->m_procedence;
					AActor* acto = tablRow->m_actor;
					if (acto) {
						FString pathS = "Blueprint'" + acto->GetClass()->GetPathName().LeftChop(2) + "'";
						FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
						FAssetData asstData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*pathS));
						UBlueprint* bp = Cast<UBlueprint>(asstData.GetAsset());
						if (bp) {
							GEditor->EditObject(bp);
						}
					}
				}
			}
		}
		else if (monitor && data.navBarName == "level") {
			if (monitor->GetTableContent() && monitor->GetTableContent()->GetAllChildren().IsValidIndex(data.index)) {
				UICE_TableRow* tablRow = Cast<UICE_TableRow>(monitor->GetTableContent()->GetAllChildren()[data.index]);
				if (tablRow) {
					FString procedence = tablRow->m_procedence;
					ULevel* levl = tablRow->m_level;
					if (levl) {
						FLevelUtils::ToggleLevelLock(levl);
					}
					FTargetData dat2 = { ETargetInfo::E_Search, 0, monitor, procedence };
					ButtonAction(dat2);
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_Close) {
		if (monitor && data.navBarName == "level") {
			if (monitor->GetTableContent() && monitor->GetTableContent()->GetAllChildren().IsValidIndex(data.index)) {
				UICE_TableRow* tablRow = Cast<UICE_TableRow>(monitor->GetTableContent()->GetAllChildren()[data.index]);
				if (tablRow) {
					FString procedence = tablRow->m_procedence;
					ULevel* levl = tablRow->m_level;
					if (levl) {
						FLevelUtils::ToggleLevelLock(levl);
					}
					FTargetData dat2 = { ETargetInfo::E_Search, 0, monitor, procedence };
					ButtonAction(dat2);
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_Search) {
		if (monitor && data.navBarName == "search") {
			int maxE2Action = (int)GETENUMMAX(TEXT("EActionListCore"));
			int actionIndex = monitor->GetEnum()->GetICEEnum()->GetSelectedIndex();
			FString actionNameCore = "EActionList";
			if (actionIndex >= maxE2Action) {
				actionIndex = actionIndex - maxE2Action + 1;
			}
			else {
				actionNameCore = actionNameCore + "Core";
			}
			FString action = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), actionIndex);
			int maxE2Check = (int)GETENUMMAX(TEXT("ECheckListCore"));
			int checkIndex = monitor->GetEnumCheck()->GetICEEnum()->GetSelectedIndex();
			FString checkNameCore = "ECheckList";
			if (checkIndex >= maxE2Check) {
				checkIndex = checkIndex - maxE2Check + 1;
			}
			else {
				checkNameCore = checkNameCore + "Core";
			}
			FString checkaction = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), checkIndex);
			FString nameAct = monitor->GetActorName()->Text.ToString();
			FString levelNam = monitor->GetLevelName()->Text.ToString();
			FString tagAct = monitor->GetActorTag()->Text.ToString();
			FString levelName = "";
			UObject* objClass = monitor->GetActorValue();
			int numAct = 0;
			monitor->ClearSearch();
			for (TObjectIterator<AActor> actor; actor; ++actor)
			{
				AActor* acto2 = *actor;
				if (ISM_ISVALID(acto2)) {
					UInteractComponent* intComp = Cast<UInteractComponent>(actor->GetComponentByClass(UInteractComponent::StaticClass()));
					if ((!monitor->GetCheckedValue() || (actionIndex == 0 && checkIndex == 0 && intComp)) && nameAct == "" && tagAct == "" && !objClass && levelNam == "") {
						FString worldName = "";
						if (actor->GetWorld() && !(*actor)->GetWorld()->GetName().Contains("World_") && !(*actor)->GetName().Contains("_tempInvisibleInteract") && !(*actor)->GetName().Contains("_InteractDelete")) {
							worldName = actor->GetWorld()->GetName();
							ULevel* level2 = actor->GetLevel();
							if (level2) {
								levelName = actor->GetLevel()->GetOuter() ? actor->GetLevel()->GetOuter()->GetName() : actor->GetLevel()->GetName();
								if (levelName == actor->GetWorld()->GetName()) {
									levelName = actor->GetLevel()->GetName();
								}
							}
							numAct++;
							monitor->AddSearchData(actor->GetName(), worldName, levelName, *actor, data.navBarName, level2);
						}
					}
					else {
						bool bAction = false;
						bool bActionCheck = false;
						bool bActorName = false;
						bool bActorTag = false;
						bool bActorClass = false;
						bool bLevelName = false;

						if (intComp && monitor->GetCheckedValue()) {
							intComp->ImportInteractDataInternal();
							if (monitor->GetEnum()->GetICEEnum()->GetSelectedIndex() > 0) {
								for (int g = 0; g < intComp->interactDataArray.Num(); g++) {
									for (int f = 0; f < intComp->interactDataArray[g].preStageFunction.Num(); f++) {
										if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].preStageFunction[f].enumName) == action) {
											bAction = true;
											break;
										}
										else if (intComp->interactDataArray[g].preStageFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].preStageFunction[f].ifFunction.function.enumName) == action
												|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].preStageFunction[f].ifFunction.elseFunction.enumName) == action) {
												bAction = true;
												break;
											}
										}
									}
									if (bAction) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.initFunction.Num(); f++) {
										if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].enumName) == action) {
											bAction = true;
											break;
										}
										else if (intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].ifFunction.function.enumName) == action
												|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].ifFunction.elseFunction.enumName) == action) {
												bAction = true;
												break;
											}
										}
									}
									if (bAction) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction.Num(); f++) {
										if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].enumName) == action) {
											bAction = true;
											break;
										}
										else if (intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].ifFunction.function.enumName) == action
												|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].ifFunction.elseFunction.enumName) == action) {
												bAction = true;
												break;
											}
										}
									}
									if (bAction) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].actions.Num(); f++) {
										for (int z = 0; z < intComp->interactDataArray[g].actions[f].stageFunction.Num(); z++) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].enumName) == action) {
												bAction = true;
												break;
											}
											else if (intComp->interactDataArray[g].actions[f].stageFunction[z].enumNameCore == EActionListCore::ifFunction) {
												if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].ifFunction.function.enumName) == action
													|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].ifFunction.elseFunction.enumName) == action) {
													bAction = true;
													break;
												}
											}
										}
										if (bAction) {
											break;
										}
									}
									if (bAction) {
										break;
									}
								}
							}
							else if (monitor->GetEnum()->GetICEEnum()->GetSelectedIndex() == 0) {
								bAction = true;
							}

							if (monitor->GetEnumCheck()->GetICEEnum()->GetSelectedIndex() > 0) {
								for (int g = 0; g < intComp->interactDataArray.Num(); g++) {
									for (int f = 0; f < intComp->interactDataArray[g].preStageFunction.Num(); f++) {
										if (intComp->interactDataArray[g].preStageFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].preStageFunction[f].ifFunction.condition.enumName) == checkaction) {
												bActionCheck = true;
												break;
											}
										}
									}
									if (bActionCheck) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.initFunction.Num(); f++) {
										if (intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].ifFunction.condition.enumName) == checkaction) {
												bActionCheck = true;
												break;
											}
										}
									}
									if (bActionCheck) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction.Num(); f++) {
										if (intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].ifFunction.condition.enumName) == checkaction) {
												bActionCheck = true;
												break;
											}
										}
									}
									if (bActionCheck) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].actions.Num(); f++) {
										for (int z = 0; z < intComp->interactDataArray[g].actions[f].stageFunction.Num(); z++) {
											if (intComp->interactDataArray[g].actions[f].stageFunction[z].enumNameCore == EActionListCore::ifFunction) {
												if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].ifFunction.condition.enumName) == checkaction) {
													bActionCheck = true;
													break;
												}
											}
										}
										if (bActionCheck) {
											break;
										}
									}
									if (bActionCheck) {
										break;
									}
								}
							}
							else if (monitor->GetEnumCheck()->GetICEEnum()->GetSelectedIndex() == 0) {
								bActionCheck = true;
							}
							intComp->interactDataArray.Empty();
						}

						if (actor->GetName().Contains(nameAct) || nameAct == "") {
							bActorName = true;
						}

						if (tagAct == "") {
							bActorTag = true;
						}
						else {
							for (int t = 0; t < actor->Tags.Num(); t++) {
								if (actor->Tags[t].ToString().Contains(tagAct)) {
									bActorTag = true;
									break;
								}
							}
						}

						if (!objClass || actor->GetClass()->GetName() == objClass->GetName()) {
							bActorClass = true;
						}

						if ((!monitor->GetCheckedValue() ||(bAction && bActionCheck)) && bActorName && bActorTag && bActorClass) {
							FString worldName = "";
							if (actor->GetWorld() && !(*actor)->GetWorld()->GetName().Contains("World_") && !(*actor)->GetName().Contains("_tempInvisibleInteract") && !(*actor)->GetName().Contains("_InteractDelete")) {
								worldName = actor->GetWorld()->GetName();
								ULevel* level2 = actor->GetLevel();
								if (level2) {
									levelName = actor->GetLevel()->GetOuter() ? actor->GetLevel()->GetOuter()->GetName() : actor->GetLevel()->GetName();
									if (levelName == actor->GetWorld()->GetName()) {
										levelName = actor->GetLevel()->GetName();
									}
								}
								if (levelName.Contains(levelNam) || levelNam == "") {
									bLevelName = true;
								}
								if (bLevelName) {
									numAct++;
									monitor->AddSearchData(actor->GetName(), worldName, levelName, *actor, data.navBarName, level2);
								}
							}
						}
					}
				}
			}
			monitor->GetActorsFound()->SetText(FText::FromString(FString::FromInt(numAct) + " Actors found"));
		}
		else if (monitor && data.navBarName == "tableRow") {
			if (monitor->GetTableContent() && monitor->GetTableContent()->GetAllChildren().IsValidIndex(data.index)) {
				UICE_TableRow* tableRow = Cast<UICE_TableRow>(monitor->GetTableContent()->GetAllChildren()[data.index]);
				if (tableRow) {
					AActor* acto = tableRow->m_actor;
					if (acto) {
						if (GEditor->CanSelectActor(acto, true, true, true)) {
							GEditor->SelectNone(true, true);
							GEditor->SelectActor(acto, true, true, true);
							TArray<AActor*> arrAct;
							arrAct.Add(acto);
							GEditor->MoveViewportCamerasToActor(arrAct, true);
						}
						else {
							GEditor->SelectNone(true, true);
							FString blockText1 = "Level bloqueado";
							ISM_ULOG_EDIT("%s", Log, *blockText1)
						}
					}
				}
			}
		}
	}
	else if (data.info == ETargetInfo::E_Clear) {
		if (intLog && data.navBarName == "logs") {
			intLog->ClearData();
		}
	}
	else if (data.info == ETargetInfo::E_Filter) {
		if (intLog && data.navBarName == "Errors") {
			intLog->ClearData(true);
			intLog->showErrors = !intLog->showErrors;
			intLog->BuildMonitor();
			intLog->GetData(intLog->GetSearch()->Text);
		}
		else if (intLog && data.navBarName == "Warnings") {
			intLog->ClearData(true);
			intLog->showWarnings = !intLog->showWarnings;
			intLog->BuildMonitor();
			intLog->GetData(intLog->GetSearch()->Text);
		}
		else if (intLog && data.navBarName == "Info") {
			intLog->ClearData(true);
			intLog->showInfo = !intLog->showInfo;
			intLog->BuildMonitor();
			intLog->GetData(intLog->GetSearch()->Text);
		}
		else if (intLog && data.navBarName == "Spam") {
			intLog->ClearData(true);
			intLog->showSpam = !intLog->showSpam;
			intLog->BuildMonitor();
			intLog->GetData(intLog->GetSearch()->Text);
		}
		else if (intLog && data.navBarName == "Permanent") {
			if (intLog->showSpam) {
				intLog->GetPermanentHolder()->SetVisibility(ESlateVisibility::Collapsed);
			}
			else {
				intLog->GetPermanentHolder()->SetVisibility(ESlateVisibility::Visible);
			}
			intLog->showSpam = !intLog->showSpam;
		}
	}
	else if (data.info == ETargetInfo::E_Execute) {
		if (intLog && data.navBarName == "Command") {
			FString command = intLog->GetCommandText()->Text.ToString();
			if (!GEngine->Exec(GetWorld(), *command)) {
				GEditor->Exec(GetWorld(), *command);
			}
		}
	}
	else if (data.info == ETargetInfo::E_SelectAll) {
		if (monitor) {
			GEditor->SelectNone(true, true);
			TArray<AActor*> selectActors;
			int maxE2Action = (int)GETENUMMAX(TEXT("EActionListCore"));
			int actionIndex = monitor->GetEnum()->GetICEEnum()->GetSelectedIndex();
			FString actionNameCore = "EActionList";
			if (actionIndex >= maxE2Action) {
				actionIndex = actionIndex - maxE2Action + 1;
			}
			else {
				actionNameCore = actionNameCore + "Core";
			}
			FString action = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), actionIndex);
			int maxE2Check = (int)GETENUMMAX(TEXT("ECheckListCore"));
			int checkIndex = monitor->GetEnumCheck()->GetICEEnum()->GetSelectedIndex();
			FString checkNameCore = "ECheckList";
			if (checkIndex >= maxE2Check) {
				checkIndex = checkIndex - maxE2Check + 1;
			}
			else {
				checkNameCore = checkNameCore + "Core";
			}
			FString checkaction = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), checkIndex);
			FString nameAct = monitor->GetActorName()->Text.ToString();
			FString tagAct = monitor->GetActorTag()->Text.ToString();
			FString levelNam = monitor->GetLevelName()->Text.ToString();
			FString levelName = "";
			UObject* objClass = monitor->GetActorValue();
			int numAct = 0;
			monitor->ClearSearch();
			for (TObjectIterator<AActor> actor; actor; ++actor)
			{
				AActor* acto2 = *actor;
				if (ISM_ISVALID(acto2)) {
					UInteractComponent* intComp = Cast<UInteractComponent>(actor->GetComponentByClass(UInteractComponent::StaticClass()));
					if ((!monitor->GetCheckedValue() || (actionIndex == 0 && checkIndex == 0 && intComp)) && nameAct == "" && tagAct == "" && !objClass && levelNam == "") {
						FString worldName = "";
						if (actor->GetWorld() && !(*actor)->GetWorld()->GetName().Contains("World_") && !(*actor)->GetName().Contains("_tempInvisibleInteract") && !(*actor)->GetName().Contains("_InteractDelete")) {
							worldName = actor->GetWorld()->GetName();
							ULevel* level2 = actor->GetLevel();
							if (level2) {
								levelName = actor->GetLevel()->GetOuter() ? actor->GetLevel()->GetOuter()->GetName() : actor->GetLevel()->GetName();
								if (levelName == actor->GetWorld()->GetName()) {
									levelName = actor->GetLevel()->GetName();
								}
							}
							numAct++;
							monitor->AddSearchData(actor->GetName(), worldName, levelName, *actor, data.navBarName, level2);
							selectActors.Add(*actor);
						}
					}
					else {
						bool bAction = false;
						bool bActionCheck = false;
						bool bActorName = false;
						bool bActorTag = false;
						bool bActorClass = false;
						bool bLevelName = false;

						if (intComp && monitor->GetCheckedValue()) {
							intComp->ImportInteractDataInternal();
							if (monitor->GetEnum()->GetICEEnum()->GetSelectedIndex() > 0) {
								for (int g = 0; g < intComp->interactDataArray.Num(); g++) {
									for (int f = 0; f < intComp->interactDataArray[g].preStageFunction.Num(); f++) {
										if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].preStageFunction[f].enumName) == action) {
											bAction = true;
											break;
										}
										else if (intComp->interactDataArray[g].preStageFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].preStageFunction[f].ifFunction.function.enumName) == action
												|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].preStageFunction[f].ifFunction.elseFunction.enumName) == action) {
												bAction = true;
												break;
											}
										}
									}
									if (bAction) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.initFunction.Num(); f++) {
										if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].enumName) == action) {
											bAction = true;
											break;
										}
										else if (intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].ifFunction.function.enumName) == action
												|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].ifFunction.elseFunction.enumName) == action) {
												bAction = true;
												break;
											}
										}
									}
									if (bAction) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction.Num(); f++) {
										if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].enumName) == action) {
											bAction = true;
											break;
										}
										else if (intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].ifFunction.function.enumName) == action
												|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].ifFunction.elseFunction.enumName) == action) {
												bAction = true;
												break;
											}
										}
									}
									if (bAction) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].actions.Num(); f++) {
										for (int z = 0; z < intComp->interactDataArray[g].actions[f].stageFunction.Num(); z++) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].enumName) == action) {
												bAction = true;
												break;
											}
											else if (intComp->interactDataArray[g].actions[f].stageFunction[z].enumNameCore == EActionListCore::ifFunction) {
												if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].ifFunction.function.enumName) == action
													|| GETENUMSTRING(reinterpret_cast<const TCHAR*>(*actionNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].ifFunction.elseFunction.enumName) == action) {
													bAction = true;
													break;
												}
											}
										}
										if (bAction) {
											break;
										}
									}
									if (bAction) {
										break;
									}
								}
							}
							else if (monitor->GetEnum()->GetICEEnum()->GetSelectedIndex() == 0) {
								bAction = true;
							}

							if (monitor->GetEnumCheck()->GetICEEnum()->GetSelectedIndex() > 0) {
								for (int g = 0; g < intComp->interactDataArray.Num(); g++) {
									for (int f = 0; f < intComp->interactDataArray[g].preStageFunction.Num(); f++) {
										if (intComp->interactDataArray[g].preStageFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].preStageFunction[f].ifFunction.condition.enumName) == checkaction) {
												bActionCheck = true;
												break;
											}
										}
									}
									if (bActionCheck) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.initFunction.Num(); f++) {
										if (intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.initFunction[f].ifFunction.condition.enumName) == checkaction) {
												bActionCheck = true;
												break;
											}
										}
									}
									if (bActionCheck) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction.Num(); f++) {
										if (intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].enumNameCore == EActionListCore::ifFunction) {
											if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].capsuleTriggerStruct.exitFunction[f].ifFunction.condition.enumName) == checkaction) {
												bActionCheck = true;
												break;
											}
										}
									}
									if (bActionCheck) {
										break;
									}
									for (int f = 0; f < intComp->interactDataArray[g].actions.Num(); f++) {
										for (int z = 0; z < intComp->interactDataArray[g].actions[f].stageFunction.Num(); z++) {
											if (intComp->interactDataArray[g].actions[f].stageFunction[z].enumNameCore == EActionListCore::ifFunction) {
												if (GETENUMSTRING(reinterpret_cast<const TCHAR*>(*checkNameCore), intComp->interactDataArray[g].actions[f].stageFunction[z].ifFunction.condition.enumName) == checkaction) {
													bActionCheck = true;
													break;
												}
											}
										}
										if (bActionCheck) {
											break;
										}
									}
									if (bActionCheck) {
										break;
									}
								}
							}
							else if (monitor->GetEnumCheck()->GetICEEnum()->GetSelectedIndex() == 0) {
								bActionCheck = true;
							}
							intComp->interactDataArray.Empty();
						}

						if (actor->GetName().Contains(nameAct) || nameAct == "") {
							bActorName = true;
						}

						if (tagAct == "") {
							bActorTag = true;
						}
						else {
							for (int t = 0; t < actor->Tags.Num(); t++) {
								if (actor->Tags[t].ToString().Contains(tagAct)) {
									bActorTag = true;
									break;
								}
							}
						}

						if (!objClass || actor->GetClass()->GetName() == objClass->GetName()) {
							bActorClass = true;
						}

						if ((!monitor->GetCheckedValue() || (bAction && bActionCheck)) && bActorName && bActorTag && bActorClass) {
							FString worldName = "";
							if (actor->GetWorld() && !(*actor)->GetWorld()->GetName().Contains("World_") && !(*actor)->GetName().Contains("_tempInvisibleInteract") && !(*actor)->GetName().Contains("_InteractDelete")) {
								worldName = actor->GetWorld()->GetName();
								ULevel* level2 = actor->GetLevel();
								if (level2) {
									levelName = actor->GetLevel()->GetOuter() ? actor->GetLevel()->GetOuter()->GetName() : actor->GetLevel()->GetName();
									if (levelName == actor->GetWorld()->GetName()) {
										levelName = actor->GetLevel()->GetName();
									}
								}
								if (levelName.Contains(levelNam) || levelNam == "") {
									bLevelName = true;
								}
								if (bLevelName) {
									numAct++;
									monitor->AddSearchData(actor->GetName(), worldName, levelName, *actor, data.navBarName, level2);
									selectActors.Add(*actor);
								}
							}
						}
					}
				}
			}
			monitor->GetActorsFound()->SetText(FText::FromString(FString::FromInt(numAct) + " Actors found"));
			for (int f = 0; f < selectActors.Num(); f++) {
				if (GEditor->CanSelectActor(selectActors[f], true, true, true)) {
					GEditor->SelectActor(selectActors[f], true, true, true);
				}
			}
			GEditor->MoveViewportCamerasToActor(selectActors, true);
		}
	}
	else if (data.info == ETargetInfo::E_AddInteract) {
		if (wrapper && wrapper->savedAct) {
			FString realName = wrapper->savedAct->GetName();
			wrapper->savedAct = UInteractCore::MakeInteractActor(wrapper->savedAct);
			if (wrapper->savedAct && wrapper->savedAct->GetName().StartsWith("REINST_")) {
				UInteractCore::ISM_Destroy(wrapper->savedAct);
				GEditor->SelectNone(true, true);
				wrapper->savedAct = UInteractCore::FindActorByName(realName, false);
				GEditor->SelectActor(wrapper->savedAct, true, true, true);
			}
			wrapper->ResetWrapper(false);
			wrapper->BuildEditor();
			TArray<AActor*> actors1;
			actors1.Add(wrapper->savedAct);
			wrapper->CheckIfAreChanges(actors1);
		}
	}

	if (data.info != ETargetInfo::E_Undo && data.info != ETargetInfo::E_Redo && data.info != ETargetInfo::E_Open && data.info != ETargetInfo::E_SetBP && data.info != ETargetInfo::E_Copy && data.info != ETargetInfo::E_Select && data.info != ETargetInfo::E_AddInteract) {
		if (wrapper) {
			if (wrapper->interactComponent) {
				if (undoInteractDataIndex < undoInteractData.Num() - 1) {
					undoInteractData.RemoveAt(undoInteractDataIndex + 1, (undoInteractData.Num() - 1) - undoInteractDataIndex);
				}
				undoInteractData.Add(wrapper->interactComponent->ExportInteractData());
				undoInteractDataIndex++;
			}
		}
		else if (stage) {
			if (stage->wrapper->interactComponent) {
				if (undoInteractDataIndex < undoInteractData.Num() - 1) {
					undoInteractData.RemoveAt(undoInteractDataIndex + 1, (undoInteractData.Num() - 1) - undoInteractDataIndex);
				}
				undoInteractData.Add(stage->wrapper->interactComponent->ExportInteractData());
				undoInteractDataIndex++;
			}
		}
		else if (funcArr) {
			if (funcArr->stage->wrapper->interactComponent) {
				if (undoInteractDataIndex < undoInteractData.Num() - 1) {
					undoInteractData.RemoveAt(undoInteractDataIndex + 1, (undoInteractData.Num() - 1) - undoInteractDataIndex);
				}
				undoInteractData.Add(funcArr->stage->wrapper->interactComponent->ExportInteractData());
				undoInteractDataIndex++;
			}
		}
		else if (funct) {
			if (funct->parentArr->stage->wrapper->interactComponent) {
				if (undoInteractDataIndex < undoInteractData.Num() - 1) {
					undoInteractData.RemoveAt(undoInteractDataIndex + 1, (undoInteractData.Num() - 1) - undoInteractDataIndex);
				}
				undoInteractData.Add(funct->parentArr->stage->wrapper->interactComponent->ExportInteractData());
				undoInteractDataIndex++;
			}
		}
		else if (arr) {
			if (arr->stage->wrapper->interactComponent) {
				if (undoInteractDataIndex < undoInteractData.Num() - 1) {
					undoInteractData.RemoveAt(undoInteractDataIndex + 1, (undoInteractData.Num() - 1) - undoInteractDataIndex);
				}
				undoInteractData.Add(arr->stage->wrapper->interactComponent->ExportInteractData());
				undoInteractDataIndex++;
			}
		}
		else if (arrElem) {
			if (arrElem->stage->wrapper->interactComponent) {
				if (undoInteractDataIndex < undoInteractData.Num() - 1) {
					undoInteractData.RemoveAt(undoInteractDataIndex + 1, (undoInteractData.Num() - 1) - undoInteractDataIndex);
				}
				undoInteractData.Add(arrElem->stage->wrapper->interactComponent->ExportInteractData());
				undoInteractDataIndex++;
			}
		}
		else if (arrElem) {
			if (arrElem->stage->wrapper->interactComponent) {
				if (undoInteractDataIndex < undoInteractData.Num() - 1) {
					undoInteractData.RemoveAt(undoInteractDataIndex + 1, (undoInteractData.Num() - 1) - undoInteractDataIndex);
				}
				undoInteractData.Add(arrElem->stage->wrapper->interactComponent->ExportInteractData());
				undoInteractDataIndex++;
			}
		}
	}

#endif
}