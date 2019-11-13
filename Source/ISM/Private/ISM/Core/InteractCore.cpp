#include "InteractCore.h"
#include "InteractComponent.h"
#include <Paths.h>
#include <FileManager.h>
#include <Engine/Texture2D.h>
#include <Regex.h>
#include "InteractComponent.h"
#include <SlateApplication.h>
#include <AssetRegistryModule.h>
#include <ModuleManager.h>

// Blueprint merging
#if WITH_EDITOR
#include "Widgets/Input/SHyperlink.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Dialogs/Dialogs.h"
#include <Internationalization.h>
#include <BlueprintEditorUtils.h>
#include <KismetEditorUtilities.h>
#include <BlueprintEditor.h>
#include <BlueprintEditorSettings.h>
#include "Engine/SCS_Node.h"
#include "EdGraph/EdGraphSchema.h"
#include <Engine/Selection.h>
#include <Editor/EditorEngine.h>
#include <UnrealEd.h>
#include "Layers/ILayers.h"
#endif
#include "InteractConfig.h"
#include "AssetToolsModule.h"

DEFINE_LOG_CATEGORY(ISM);

TArray<FString> UInteractCore::logArrayString;

TArray<FString> UInteractCore::logArrayFile;

#if WITH_EDITOR
TArray<ELogHelpers> UInteractCore::logArrayType;
#endif

TArray<FDateTime> UInteractCore::dateTime;

bool UInteractCore::showDebugMessages = true;

bool UInteractCore::showLogMessages = false;

void UInteractCore::PrintLog(FString text, float time, FColor color)
{
	ISM_PRINTLOG_EDIT(text, time, color)
}

bool UInteractCore::JumpStageGlobal(AActor* actor, FString stageString /*= ""*/, float delay /*= 0.1f*/, int stage /*= -1*/, bool cancelTimers /*= true*/)
{
	return UInteractComponent::JumpStageGlobal(actor, stageString, delay, stage, cancelTimers);
}

void UInteractCore::BezierCurve(const FVector CPInicio, const FVector CP1, const FVector CP2, const FVector CPFin, int32 NumPoints, TArray <FVector> & OutPoints)
{
	FVector ControlPoints[] = { CPInicio, CP1, CP2, CPFin };

	FVector::EvaluateBezier(ControlPoints, NumPoints, OutPoints);
}

AActor* UInteractCore::MakeInteractActor(AActor* actor)
{
	AActor* acto = actor;
	if (acto && GEditor->CanSelectActor(acto, true, true, true)) {
		FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

		bool hasBPBefore = false;

		FString pathS2 = "Blueprint'" + acto->GetClass()->GetPathName().LeftChop(2) + "'";
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		FAssetData asstData2 = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*pathS2));
		UBlueprint* bp2 = Cast<UBlueprint>(asstData2.GetAsset());
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
			if (Actors.Num() > 0 && ISM_ISVALID(Actors[0])) {
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

			//monitor->ChangeActor(data.index, NewActor);

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

			//FString procedence = tablRow->m_procedence;

			if (hasBPBefore) {
				AActor* oldActor = UInteractCore::FindActorByName(actor->GetName());
				FString newActNamm = "REINST_" + actor->GetName();
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
						return NewActor2;
					}
				}
				else {
					return NewActor;
				}
			}
			else {
				return NewActor;
			}
		}
	}
	return nullptr;
}

void UInteractCore::ISM_Destroy(UObject* ToDestroy)
{
	if (!ISM_ISVALID(ToDestroy)) return;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	UWorld* World = ToDestroy->GetWorld();

	//Actor?
	AActor * AnActor = Cast<AActor>(ToDestroy);
	//GC
	if (World) {
		World->RemoveActor(AnActor, true);
	}

	if (AnActor)
	{
		AnActor->K2_DestroyActor();
		ToDestroy = NULL;
	}
	else
	{
		//Begin Destroy
		ToDestroy->ConditionalBeginDestroy();
		ToDestroy = NULL;
	}

	if (World) {
		GEngine->ForceGarbageCollection(true);
	}

}

float UInteractCore::GetValue(FString value, float max)
{
	float res = NaN;
	FRegexPattern myPattern(TEXT("^(stop)$"));
	FRegexMatcher myMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		res = -1.f;
		return res;
	}
	myPattern = FRegexPattern(TEXT("^(pause)$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		res = -3.f;
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		res = FCString::Atoi(*value);
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+[,.]\\d+$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		res = atof(TCHAR_TO_ANSI(*newval));
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+(\\s*to\\s*)((-?\\d+)|(last))$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString left = "";
		FString right = "";
		newval.Split("to", &left, &right);
		if (right == "last") {
			if (max < NaN) {
				right = FString::FromInt(max);
			}
			else {
				return res;
			}
		}
		int leftint = FCString::Atoi(*left);
		int rightint = FCString::Atoi(*right);
		if (leftint <= rightint) {
			res = FMath::RandRange(leftint, rightint);
		}
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?(\\s*to\\s*)((-?\\d+([,.]\\d+)?)|(last))$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString left = "";
		FString right = "";
		newval.Split("to", &left, &right);
		if (right == "last") {
			if (max < NaN) {
				right = FString::SanitizeFloat(max);
			}
			else {
				return res;
			}
		}
		float leftfloat = atof(TCHAR_TO_ANSI(*left));
		float rightfloat = atof(TCHAR_TO_ANSI(*right));
		if (leftfloat <= rightfloat) {
			res = FMath::RandRange(leftfloat, rightfloat);
		}
		return res;
	}
	return res;
}

void UInteractCore::ReparentBlueprint(UBlueprint* BlueprintObj, UClass* ChosenClass, bool compile)
{
#if WITH_EDITOR

	ISM_ULOG_EDIT("Reparenting blueprint %s from %s to %s...", Warning, *BlueprintObj->GetFullName(), BlueprintObj->ParentClass ? *BlueprintObj->ParentClass->GetName() : TEXT("[None]"), *ChosenClass->GetName());

	UClass* OldParentClass = BlueprintObj->ParentClass;
	BlueprintObj->ParentClass = ChosenClass;

	if (compile) {
		UInteractCore::CompileBlueprint(BlueprintObj);
	}
	FBlueprintEditorUtils::RefreshAllNodes(BlueprintObj);
	FBlueprintEditorUtils::MarkBlueprintAsModified(BlueprintObj);

	if (BlueprintObj->NativizationFlag != EBlueprintNativizationFlag::Disabled)
	{
		UBlueprint* ParentBlueprint = UBlueprint::GetBlueprintFromClass(ChosenClass);
		if (ParentBlueprint && ParentBlueprint->NativizationFlag == EBlueprintNativizationFlag::Disabled)
		{
			ParentBlueprint->NativizationFlag = EBlueprintNativizationFlag::Dependency;

			FNotificationInfo Warning(FText::Format(
				FText::FromString("flagged for nativization (as a required dependency)."),
				FText::FromName(ParentBlueprint->GetFName())
			)
			);
			Warning.ExpireDuration = 5.0f;
			Warning.bFireAndForget = true;
			Warning.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));
			FSlateNotificationManager::Get().AddNotification(Warning);
		}
	}

	FSlateApplication::Get().DismissAllMenus();
#endif

}



AActor* UInteractCore::FindActorByName(FString name, bool containsName) {
	for (TObjectIterator<AActor> actor; actor; ++actor)
	{
		if (containsName) {
			if (actor->GetName().Contains(name)) {
				return *actor;
			}
		}
		else {
			if (actor->GetName() == name) {
				return *actor;
			}
		}
	}
	return nullptr;
}



void UInteractCore::CompileBlueprint(UBlueprint* Blueprint) {
#if WITH_EDITOR

	FCompilerResultsLog LogResults;
	LogResults.SetSourcePath(Blueprint->GetPathName());
	LogResults.BeginEvent(TEXT("Compile"));
	FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, &LogResults);

	LogResults.EndEvent();
#endif
}

AActor* UInteractCore::SpawnActorFromBlueprint(UBlueprint* Blueprint, AActor* actor, FVector location, FRotator rotation, UWorld* world)
{
	AActor* NewActor = nullptr;
#if WITH_EDITOR

	UWorld* World = world;

	GEditor->GetSelectedActors()->Modify();

	NewActor = World->SpawnActor(Blueprint->GeneratedClass, &location, &rotation);
	GEditor->Layers->InitializeNewActorLayers(NewActor);
	FActorLabelUtilities::SetActorLabelUnique(NewActor, Blueprint->GetName());
	USelection* ComponentSelection = GEditor->GetSelectedComponents();
	ComponentSelection->BeginBatchSelectOperation();
	ComponentSelection->DeselectAll();
	ComponentSelection->EndBatchSelectOperation(false);

	// Update selection to new actor
	GEditor->SelectActor(NewActor, /*bSelected=*/ true, /*bNotify=*/ true);
#endif
	return NewActor;
}

void UInteractCore::AddComponentToBlueprint(UBlueprint* Blueprint, USceneComponent* Component)
{
#if WITH_EDITOR

	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;

	USCS_Node* NewSCSNode = SCS->CreateNode(Component->GetClass(), Component->GetFName());
	UEditorEngine::FCopyPropertiesForUnrelatedObjectsParams Params;
	Params.bDoDelta = false; // We need a deep copy of parameters here so the CDO values get copied as well
	UEditorEngine::CopyPropertiesForUnrelatedObjects(Component, NewSCSNode->ComponentTemplate, Params);

	// Clear the instance component flag
	NewSCSNode->ComponentTemplate->CreationMethod = EComponentCreationMethod::Native;

	SCS->AddNode(NewSCSNode);

	FBlueprintEditorUtils::RefreshAllNodes(Blueprint);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
#endif
}