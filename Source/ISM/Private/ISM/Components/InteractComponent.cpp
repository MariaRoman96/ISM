#include "InteractComponent.h"
#include "ChPlayer.h"
#include <GameFramework/Character.h>
#include "TooltipWidget.h"
#include <Kismet/GameplayStatics.h>
#include <Components/ActorComponent.h>
#if WITH_EDITOR
#include <ConstructorHelpers.h>
#include <PropertyEditorModule.h>
#include <ModuleManager.h>
#endif
#include <Regex.h>
#include <stdlib.h>
#include <string>
#include <random>
#include <stdarg.h>
#include <vector>
#include <Engine/EngineTypes.h>
#include <Engine/World.h>
#include <TimerManager.h>
#include <DelegateSignatureImpl.inl>
#include "Item.h"
#include <SlateApplicationBase.h>
#include <SlateApplication.h>
#include "Ch.h"
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Animation/AnimNode_StateMachine.h>
#include <Animation/AnimBlueprintGeneratedClass.h>
#include "LevelManager.h"
#include <UnrealType.h>
#include <NoExportTypes.h>
#include <Components/AudioComponent.h>
#include <WidgetBlueprintLibrary.h>
#include "InteractCore.h"
#include <GameFramework/PlayerStart.h>
#include <Materials/MaterialInstanceDynamic.h>
#include "InteractCoreClass.h"
#include <chrono>
#include "Engine/LevelStreaming.h"
#include <Components/ArrowComponent.h>
#include "InteractConfig.h"
#include "UObjectIterator.h"

using namespace std;
using namespace std::placeholders;

DEFINE_LOG_CATEGORY(InteractLog)

TArray<FString> UInteractComponent::functionType;
TArray<FString> UInteractComponent::propertyBaseFunctionName;
TArray<FString> UInteractComponent::propertyWrapperFunctionName;
TArray<FString> UInteractComponent::propertyFunctionName;
TArray<FString> UInteractComponent::checkFunctionName;
TArray<FString> UInteractComponent::checkFunctionNameAnd;
TArray<FString> UInteractComponent::actorFunctionName;

TArray<TArray<FInteractData>> UInteractComponent::datasStreamLoad;
TArray<FString> UInteractComponent::actorsStreamLoad;

TArray<FDelegateInteractData> UInteractComponent::delegateAuxFunctions;

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	if (functionType.Num() <= 0) {
		functionType.Add("initFunction");
		functionType.Add("exitFunction");
	}

	if (propertyFunctionName.Num() <= 0) {
		for (TFieldIterator<UProperty> It(FFunctionData::StaticStruct()); It; ++It) {
			if (It->GetFName().ToString() != "enumName") {
				propertyFunctionName.Add(It->GetFName().ToString());
			}
		}
	}

	if (propertyBaseFunctionName.Num() <= 0) {
		for (TFieldIterator<UProperty> It(FFunctionDataBase::StaticStruct()); It; ++It) {
			if (It->GetFName().ToString() != "enumName") {
				propertyBaseFunctionName.Add(It->GetFName().ToString());
			}
		}
	}

	if (propertyWrapperFunctionName.Num() <= 0) {
		for (TFieldIterator<UProperty> It(FFunctionDataWrapper::StaticStruct()); It; ++It) {
			if (It->GetFName().ToString() != "enumName") {
				propertyWrapperFunctionName.Add(It->GetFName().ToString());
			}
		}
	}

	if (checkFunctionName.Num() <= 0) {
		for (TFieldIterator<UProperty> It(FFunctionCheckData::StaticStruct()); It; ++It) {
			if (It->GetFName().ToString() != "enumName") {
				checkFunctionName.Add(It->GetFName().ToString());
			}
		}
	}

	if (checkFunctionNameAnd.Num() <= 0) {
		for (TFieldIterator<UProperty> It(FFunctionCheckDataBase::StaticStruct()); It; ++It) {
			if (It->GetFName().ToString() != "enumName") {
				checkFunctionNameAnd.Add(It->GetFName().ToString());
			}
		}
	}

	if (actorFunctionName.Num() <= 0) {
		for (TFieldIterator<UProperty> It(FFunctionActorData::StaticStruct()); It; ++It) {
			if (It->GetFName().ToString() != "enumName") {
				actorFunctionName.Add(It->GetFName().ToString());
			}
		}
	}

	circleTexture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/circle.circle'"), NULL, LOAD_None, NULL);

	if (!m_innerCapsule) {
		m_innerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InnerCapsule"));
		if (m_innerCapsule) {
			m_innerCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			m_innerCapsule->ComponentTags.Add("Interact");
			m_innerCapsule->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::InsideCapsuleOverlapBegin);
			m_innerCapsule->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::InsideCapsuleOverlapEnd);
			m_innerCapsule->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			m_innerCapsule->SetFlags(EObjectFlags::RF_Public);
		}
	}

	if (!m_outsideCapsule) {
		m_outsideCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("OutsideCapsule"));
		if (m_outsideCapsule) {
			m_outsideCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			m_outsideCapsule->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::OutsideCapsuleOverlapBegin);
			m_outsideCapsule->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::OutsideCapsuleOverlapEnd);
			m_outsideCapsule->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			m_outsideCapsule->SetFlags(EObjectFlags::RF_Public);
		}
	}

	if (!m_triggerCapsule) {
		m_triggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerCapsule"));
		if (m_triggerCapsule) {
			m_triggerCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			m_triggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &UInteractComponent::TriggerCapsuleOverlapBegin);
			m_triggerCapsule->OnComponentEndOverlap.AddDynamic(this, &UInteractComponent::TriggerCapsuleOverlapEnd);
			m_triggerCapsule->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
			m_triggerCapsule->SetFlags(EObjectFlags::RF_Public);
		}
	}

//#if WITH_EDITOR
	FWorldDelegates::OnPreWorldInitialization.AddUObject(this, &UInteractComponent::OnPreWorldInitializationInteract);
//#endif

}

bool UInteractComponent::CheckActorLocation(AActor* actor, FCheckActorLocation& data, UInteractComponent* interactComponent)
{
	AActor* intActor = UInteractComponent::GetCurrentActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	USceneComponent* compRot = nullptr;
	if (IsValid(data.specificChild)) {
		TSet<UActorComponent*> comps;
		comps = intActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
			if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
				if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
					compRot = scene;
				}
			}
			else {
				if (scene && scene->GetClass() == data.specificChild) {
					compRot = scene;
				}
			}
		}
	}
	else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
		TSet<UActorComponent*> comps;
		comps = intActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
			if (scene && scene->ComponentHasTag(data.componentTag)) {
				compRot = scene;
			}
		}
	}

	if (intActor) {
		if (data.actor.enumName != EGetActorList::None) {
			int val = (int)data.actor.enumName - 1;
			AActor* acto = nullptr;
			if (val >= 0) {
				acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(false, 0));
			}
			if (acto) {
				USceneComponent* otherCompRot = nullptr;
				if (IsValid(data.targetComponentClass)) {
					TSet<UActorComponent*> comps;
					comps = acto->GetComponents();
					for (int i = 0; i < comps.Array().Num(); i++) {
						USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
						if (data.targetComponentTag.ToString() != "" && data.targetComponentTag != NAME_None) {
							if (scene && scene->GetClass() == data.targetComponentClass && scene->ComponentHasTag(data.targetComponentTag)) {
								otherCompRot = scene;
							}
						}
						else {
							if (scene && scene->GetClass() == data.targetComponentClass) {
								otherCompRot = scene;
							}
						}
					}
				}
				else if (data.targetComponentTag.ToString() != "" && data.targetComponentTag != NAME_None) {
					TSet<UActorComponent*> comps;
					comps = acto->GetComponents();
					for (int i = 0; i < comps.Array().Num(); i++) {
						USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
						if (scene && scene->ComponentHasTag(data.targetComponentTag)) {
							otherCompRot = scene;
						}
					}
				}

				float distance = 0.f;
				FVector vect = { 0.f, 0.f, 0.f };
				FVector location = { 0.f, 0.f, 0.f };
				if (data.direction == EVectorDirection::Forward) {
					if (otherCompRot) {
						vect = otherCompRot->GetForwardVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
					else if (acto) {
						vect = acto->GetActorForwardVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
				}
				else if (data.direction == EVectorDirection::Backward) {
					if (otherCompRot) {
						vect = -otherCompRot->GetForwardVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
					else if (acto) {
						vect = -acto->GetActorForwardVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
				}
				else if (data.direction == EVectorDirection::Up) {
					if (otherCompRot) {
						vect = otherCompRot->GetUpVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
					else if (acto) {
						vect = acto->GetActorUpVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
				}
				else if (data.direction == EVectorDirection::Down) {
					if (otherCompRot) {
						vect = -otherCompRot->GetUpVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
					else if (acto) {
						vect = -acto->GetActorUpVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
				}
				else if (data.direction == EVectorDirection::Right) {
					if (otherCompRot) {
						vect = otherCompRot->GetRightVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
					else if (acto) {
						vect = acto->GetActorRightVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
				}
				else if (data.direction == EVectorDirection::Left) {
					if (otherCompRot) {
						vect = -otherCompRot->GetRightVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
					else if (acto) {
						vect = -acto->GetActorRightVector();
						vect.Normalize();
						location = vect * data.distanceDirection;
					}
				}
				location += data.location;
				if (otherCompRot) {
					location += otherCompRot->GetComponentLocation();
				}
				else if (acto) {
					location += acto->GetActorLocation();
				}

				if (compRot) {
					if (data.selectAxis == ESpecificAxis::None) {
						distance = FVector::Dist(location, compRot->GetComponentLocation());
					}
					else if (data.selectAxis == ESpecificAxis::X) {
						distance = FMath::Abs(location.X - compRot->GetComponentLocation().X);
					}
					else if (data.selectAxis == ESpecificAxis::Y) {
						distance = FMath::Abs(location.Y - compRot->GetComponentLocation().Y);
					}
					else if (data.selectAxis == ESpecificAxis::Z) {
						distance = FMath::Abs(location.Z - compRot->GetComponentLocation().Z);
					}
					interactComponent->ReturnValue(&data.customReturn, compRot->GetComponentLocation());
				}
				else if (intActor) {
					if (data.selectAxis == ESpecificAxis::None) {
						distance = FVector::Dist(location, intActor->GetActorLocation());
					}
					else if (data.selectAxis == ESpecificAxis::X) {
						distance = FMath::Abs(location.X - intActor->GetActorLocation().X);
					}
					else if (data.selectAxis == ESpecificAxis::Y) {
						distance = FMath::Abs(location.Y - intActor->GetActorLocation().Y);
					}
					else if (data.selectAxis == ESpecificAxis::Z) {
						distance = FMath::Abs(location.Z - intActor->GetActorLocation().Z);
					}
					interactComponent->ReturnValue(&data.customReturn, intActor->GetActorLocation());
				}
				CONFIRM_RETURN(interactComponent, data, FCheckActorLocation)
					return distance <= data.toleranceError;
			}
		}
		else {
			float distance = 0.f;
			FVector location = data.location;
			if (compRot) {
				if (data.selectAxis == ESpecificAxis::None) {
					distance = FVector::Dist(location, compRot->GetComponentLocation());
				}
				else if (data.selectAxis == ESpecificAxis::X) {
					distance = FMath::Abs(location.X - compRot->GetComponentLocation().X);
				}
				else if (data.selectAxis == ESpecificAxis::Y) {
					distance = FMath::Abs(location.Y - compRot->GetComponentLocation().Y);
				}
				else if (data.selectAxis == ESpecificAxis::Z) {
					distance = FMath::Abs(location.Z - compRot->GetComponentLocation().Z);
				}
				interactComponent->ReturnValue(&data.customReturn, compRot->GetComponentLocation());
			}
			else if (intActor) {
				if (data.selectAxis == ESpecificAxis::None) {
					distance = FVector::Dist(location, intActor->GetActorLocation());
				}
				else if (data.selectAxis == ESpecificAxis::X) {
					distance = FMath::Abs(location.X - intActor->GetActorLocation().X);
				}
				else if (data.selectAxis == ESpecificAxis::Y) {
					distance = FMath::Abs(location.Y - intActor->GetActorLocation().Y);
				}
				else if (data.selectAxis == ESpecificAxis::Z) {
					distance = FMath::Abs(location.Z - intActor->GetActorLocation().Z);
				}
				interactComponent->ReturnValue(&data.customReturn, intActor->GetActorLocation());
			}
			CONFIRM_RETURN(interactComponent, data, FCheckActorLocation)
				return distance <= data.toleranceError;
		}
	}
	return false;
}

bool UInteractComponent::CheckActorRotation(AActor* actor, FCheckActorRotation& data, UInteractComponent* interactComponent)
{
	AActor* intActor = UInteractComponent::GetCurrentActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);

	USceneComponent* compRot = nullptr;
	if (IsValid(data.specificChild)) {
		TSet<UActorComponent*> comps;
		comps = intActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
			if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
				if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
					compRot = scene;
				}
			}
			else {
				if (scene && scene->GetClass() == data.specificChild) {
					compRot = scene;
				}
			}
		}
	}
	else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
		TSet<UActorComponent*> comps;
		comps = intActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
			if (scene && scene->ComponentHasTag(data.componentTag)) {
				compRot = scene;
			}
		}
	}

	if (intActor) {
		if (data.actor.enumName != EGetActorList::None) {
			int val = (int)data.actor.enumName - 1;
			AActor* acto = nullptr;
			if (val >= 0) {
				acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(false, 0));
			}
			if (acto) {
				USceneComponent* otherCompRot = nullptr;
				if (IsValid(data.targetComponentClass)) {
					TSet<UActorComponent*> comps;
					comps = acto->GetComponents();
					for (int i = 0; i < comps.Array().Num(); i++) {
						USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
						if (data.targetComponentTag.ToString() != "" && data.targetComponentTag != NAME_None) {
							if (scene && scene->GetClass() == data.targetComponentClass && scene->ComponentHasTag(data.targetComponentTag)) {
								otherCompRot = scene;
							}
						}
						else {
							if (scene && scene->GetClass() == data.targetComponentClass) {
								otherCompRot = scene;
							}
						}
					}
				}
				else if (data.targetComponentTag.ToString() != "" && data.targetComponentTag != NAME_None) {
					TSet<UActorComponent*> comps;
					comps = acto->GetComponents();
					for (int i = 0; i < comps.Array().Num(); i++) {
						USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
						if (scene && scene->ComponentHasTag(data.targetComponentTag)) {
							otherCompRot = scene;
						}
					}
				}
				FRotator firstR = { 0.f, 0.f, 0.f };
				FVector firstV = { 0.f, 0.f, 0.f };
				FVector secondV = { 0.f, 0.f, 0.f };
				if (compRot) {
					firstV = compRot->GetComponentLocation();
					firstR = compRot->GetComponentRotation();
				}
				else if (intActor) {
					firstV = intActor->GetActorLocation();
					firstR = intActor->GetActorRotation();
				}
				interactComponent->ReturnValue(&data.customReturn, firstR);
				if (otherCompRot) {
					secondV = otherCompRot->GetComponentLocation();
				}
				else if (acto) {
					secondV = acto->GetActorLocation();
				}
				FRotator rot = UKismetMathLibrary::FindLookAtRotation(firstV, secondV);
				rot.Add(data.rotator.Pitch, data.rotator.Yaw, data.rotator.Roll);
				bool res = false;
				if (data.selectRotAxis == ESpecificRotAxis::None) {
					res = rot.Equals(firstR, data.toleranceError);
				}
				else if (data.selectRotAxis == ESpecificRotAxis::Pitch) {
					res = FMath::Abs(rot.Pitch - firstR.Pitch) <= data.toleranceError;
				}
				else if (data.selectRotAxis == ESpecificRotAxis::Yaw) {
					res = FMath::Abs(rot.Yaw - firstR.Yaw) <= data.toleranceError;
				}
				else if (data.selectRotAxis == ESpecificRotAxis::Roll) {
					res = FMath::Abs(rot.Roll - firstR.Roll) <= data.toleranceError;
				}
				CONFIRM_RETURN(interactComponent, data, FCheckActorRotation)
					return res;
			}
		}
		else {
			FRotator firstR = { 0.f, 0.f, 0.f };
			if (compRot) {
				firstR = compRot->GetComponentRotation();
			}
			else if (intActor) {
				firstR = intActor->GetActorRotation();
			}
			interactComponent->ReturnValue(&data.customReturn, firstR);
			bool res = false;
			if (data.selectRotAxis == ESpecificRotAxis::None) {
				res = data.rotator.Equals(firstR, data.toleranceError);
			}
			else if (data.selectRotAxis == ESpecificRotAxis::Pitch) {
				res = FMath::Abs(data.rotator.Pitch - firstR.Pitch) <= data.toleranceError;
			}
			else if (data.selectRotAxis == ESpecificRotAxis::Yaw) {
				res = FMath::Abs(data.rotator.Yaw - firstR.Yaw) <= data.toleranceError;
			}
			else if (data.selectRotAxis == ESpecificRotAxis::Roll) {
				res = FMath::Abs(data.rotator.Roll - firstR.Roll) <= data.toleranceError;
			}
			CONFIRM_RETURN(interactComponent, data, FCheckActorRotation)
				return res;
		}
	}
	return false;
}



bool UInteractComponent::AndFunction(AActor* actor, FAndFunction& data, UInteractComponent* interactComponent)
{
	if (data.conditions.Num() > 0) {
		bool res = true;
		for (int i = 0; i < data.conditions.Num(); i++) {

			int maxE = (int)GETENUMMAX(TEXT("ECheckListCore"));
			int valueFunc = (int)data.conditions[i].enumName - 1;
			int valueFuncCore = (int)data.conditions[i].enumNameCore - 1;
			if (valueFuncCore >= 0) {
				if (data.conditions[i].negate) {
					res = res && !interactComponent->checkFunctionMap[valueFuncCore].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType);
				}
				else {
					res = res && interactComponent->checkFunctionMap[valueFuncCore].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType);
				}
			}
			else if (valueFunc >= 0) {
				if (data.conditions[i].negate) {
					res = res && !interactComponent->checkFunctionMap[valueFunc + maxE - 1].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType);
				}
				else {
					res = res && interactComponent->checkFunctionMap[valueFunc + maxE - 1].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType);
				}
			}

			if (!res) {
				break;
			}
			data.conditionIndex++;
		}
		data.conditionIndex = 0;
		return res;
	}
	data.conditionIndex = 0;
	return false;
}

bool UInteractComponent::CheckVariableValue(AActor* actor, FCheckVariableValue& data, UInteractComponent* interactComponent)
{
	bool result = false;
	FString value = interactComponent->FindVariable(data.variableName.TrimStartAndEnd());
	FString res = "";
	float val1 = interactComponent->CalculateNumber(value.TrimStartAndEnd());
	float val2 = interactComponent->CalculateNumber(data.variableValue.TrimStartAndEnd());
	if (val1 != NaN && val2 != NaN) {
		if (data.numericComparison == ENumericComparison::Equals) {
			result = val1 == val2;
		}
		else if (data.numericComparison == ENumericComparison::Diferent) {
			result = val1 != val2;
		}
		else if (data.numericComparison == ENumericComparison::LessThan) {
			result = val1 < val2;
		}
		else if (data.numericComparison == ENumericComparison::GreaterThan) {
			result = val1 > val2;
		}
		else if (data.numericComparison == ENumericComparison::LessEqualsThan) {
			result = val1 <= val2;
		}
		else if (data.numericComparison == ENumericComparison::GreaterEqualsThan) {
			result = val1 >= val2;
		}
	}
	else {
		FString v4 = "";
		bool rVector = interactComponent->CalculateVector(value.TrimStartAndEnd(), &v4, EVectorType::Vector);
		if (rVector) {
			FString v5 = "";
			bool rVector2 = interactComponent->CalculateVector(data.variableValue.TrimStartAndEnd(), &v5, EVectorType::Vector);
			if (rVector2) {
				FVector vector1 = { 0.f, 0.f, 0.f };
				vector1.InitFromString(v4);
				FVector vector2 = { 0.f, 0.f, 0.f };
				vector2.InitFromString(v5);
				if (data.numericComparison == ENumericComparison::Equals) {
					result = vector1.Equals(vector2);
				}
				else if (data.numericComparison == ENumericComparison::Diferent) {
					result = !vector1.Equals(vector2);
				}
				else if (data.numericComparison == ENumericComparison::LessThan) {
					FVector diffVector = vector1 - vector2;
					float diff = diffVector.X + diffVector.Y + diffVector.Z;
					result = ((diff / abs(diff))*diffVector.Size()) < 0.f;
				}
				else if (data.numericComparison == ENumericComparison::GreaterThan) {
					FVector diffVector = vector1 - vector2;
					float diff = diffVector.X + diffVector.Y + diffVector.Z;
					result = ((diff / abs(diff))*diffVector.Size()) > 0.f;
				}
				else if (data.numericComparison == ENumericComparison::LessEqualsThan) {
					FVector diffVector = vector1 - vector2;
					float diff = diffVector.X + diffVector.Y + diffVector.Z;
					result = ((diff / abs(diff))*diffVector.Size()) <= 0.f;
				}
				else if (data.numericComparison == ENumericComparison::GreaterEqualsThan) {
					FVector diffVector = vector1 - vector2;
					float diff = diffVector.X + diffVector.Y + diffVector.Z;
					result = ((diff / abs(diff))*diffVector.Size()) >= 0.f;
				}
			}
		}
		else {
			FString v2 = "";
			bool rRotator = interactComponent->CalculateVector(value.TrimStartAndEnd(), &v2, EVectorType::Rotator);
			if (rRotator) {
				FString v3 = "";
				bool rRotator2 = interactComponent->CalculateVector(data.variableValue.TrimStartAndEnd(), &v3, EVectorType::Rotator);
				if (rRotator2) {
					FRotator vector3 = { 0.f, 0.f, 0.f };
					vector3.InitFromString(v2);
					FRotator vector4 = { 0.f, 0.f, 0.f };
					vector4.InitFromString(v3);
					if (data.numericComparison == ENumericComparison::Equals) {
						result = vector3.Equals(vector4);
					}
					else if (data.numericComparison == ENumericComparison::Diferent) {
						result = !vector3.Equals(vector4);
					}
					else if (data.numericComparison == ENumericComparison::LessThan) {
						FRotator diffVector = vector3 - vector4;
						float diff = diffVector.Pitch + diffVector.Yaw + diffVector.Roll;
						result = diff < 0.f;
					}
					else if (data.numericComparison == ENumericComparison::GreaterThan) {
						FRotator diffVector = vector3 - vector4;
						float diff = diffVector.Pitch + diffVector.Yaw + diffVector.Roll;
						result = diff > 0.f;
					}
					else if (data.numericComparison == ENumericComparison::LessEqualsThan) {
						FRotator diffVector = vector3 - vector4;
						float diff = diffVector.Pitch + diffVector.Yaw + diffVector.Roll;
						result = diff <= 0.f;
					}
					else if (data.numericComparison == ENumericComparison::GreaterEqualsThan) {
						FRotator diffVector = vector3 - vector4;
						float diff = diffVector.Pitch + diffVector.Yaw + diffVector.Roll;
						result = diff >= 0.f;
					}
				}
			}
			else {
				FString v7 = "";
				bool rColor = interactComponent->CalculateVector(value.TrimStartAndEnd(), &v2, EVectorType::Color);
				if (rColor) {
					FString v3 = "";
					bool rColor2 = interactComponent->CalculateVector(data.variableValue.TrimStartAndEnd(), &v3, EVectorType::Color);
					if (rColor2) {
						FColor vector3 = { 0, 0, 0, 0 };
						vector3.InitFromString(v7);
						FColor vector4 = { 0, 0, 0, 0 };
						vector4.InitFromString(v3);
						if (data.numericComparison == ENumericComparison::Equals) {
							result = vector3.R == vector4.R && vector3.G == vector4.G && vector3.B == vector4.B && vector3.A == vector4.A;
						}
						else if (data.numericComparison == ENumericComparison::Diferent) {
							result = !(vector3.R == vector4.R && vector3.G == vector4.G && vector3.B == vector4.B && vector3.A == vector4.A);
						}
						else if (data.numericComparison == ENumericComparison::LessThan) {
							float diff = (vector3.R - vector4.R) + (vector3.G - vector4.G) + (vector3.B - vector4.B) + (vector3.A - vector4.A);
							result = diff < 0.f;
						}
						else if (data.numericComparison == ENumericComparison::GreaterThan) {
							float diff = (vector3.R - vector4.R) + (vector3.G - vector4.G) + (vector3.B - vector4.B) + (vector3.A - vector4.A);
							result = diff > 0.f;
						}
						else if (data.numericComparison == ENumericComparison::LessEqualsThan) {
							float diff = (vector3.R - vector4.R) + (vector3.G - vector4.G) + (vector3.B - vector4.B) + (vector3.A - vector4.A);
							result = diff <= 0.f;
						}
						else if (data.numericComparison == ENumericComparison::GreaterEqualsThan) {
							float diff = (vector3.R - vector4.R) + (vector3.G - vector4.G) + (vector3.B - vector4.B) + (vector3.A - vector4.A);
							result = diff >= 0.f;
						}
					}
				}
				else {
					result = value.TrimStartAndEnd() == data.variableValue.TrimStartAndEnd();
				}
			}
		}
	}
	return result;
}

void UInteractComponent::UpdateSharedVariable(FString name, FString value)
{
	FString res = "";
	FString noneS = "";
	FString nonsS = "";
	FString valS = "";
	res = name.TrimStartAndEnd();
	if (res.StartsWith("_")) {
		res.Split("_", &noneS, &res);
	}
	for (int i = 0; i < m_sharedVariablesArray.Num(); i++) {
		if (m_sharedVariablesArray[i].Contains(res)) {
			m_sharedVariablesArray[i].Split("=", &noneS, &valS);
			if (valS.TrimStartAndEnd().StartsWith("%%")) {
				valS.Split("%%", &noneS, &valS);
				for (int x = 0; x < m_variablesObject.Num(); x++) {
					if (valS.TrimStartAndEnd() == m_variablesObject[x]->GetName()) {
						m_variablesObject.RemoveAt(x);
						break;
					}
				}
			}
			else if (valS.TrimStartAndEnd().StartsWith("[")) {
				valS.Replace(TEXT("["), TEXT(""));
				valS.Replace(TEXT("]"), TEXT(""));
				while (valS.Split(",", &noneS, &valS)) {
					if (noneS.TrimStartAndEnd().StartsWith("%%")) {
						noneS.Split("%%", &nonsS, &noneS);
						for (int x = 0; x < m_variablesObject.Num(); x++) {
							if (noneS.TrimStartAndEnd() == m_variablesObject[x]->GetName()) {
								m_variablesObject.RemoveAt(x);
								break;
							}
						}
					}
				}
				if (valS.TrimStartAndEnd().StartsWith("%%")) {
					valS.Split("%%", &nonsS, &valS);
					for (int x = 0; x < m_variablesObject.Num(); x++) {
						if (valS.TrimStartAndEnd() == m_variablesObject[x]->GetName()) {
							m_variablesObject.RemoveAt(x);
							break;
						}
					}
				}
			}
			m_sharedVariablesArray[i] = res.TrimStartAndEnd() + "=" + value.TrimStartAndEnd();
			break;
		}
	}
}

FString UInteractComponent::CalculateValue(FString value, bool init)
{
	FString res = "";
	float v = CalculateNumber(value);
	if (v != NaN) {
		res = FString::SanitizeFloat(v);
	}
	else {
		FString v3 = "";
		bool rBool = CalculateBoolean(value, &v3);
		if (rBool) {
			res = v3;
		}
		else {
			FString v2 = "";
			bool rString = CalculateString(value, &v2, init);
			if (rString) {
				res = v2;
			}
			else {
				FString v7 = "";
				bool rObject = CalculateObject(value, &v7, init);
				if (rObject) {
					res = v7;
				}
				else {
					FString v4 = "";
					bool rVector = CalculateVector(value, &v4, EVectorType::Vector);
					if (rVector) {
						res = v4;
					}
					else {
						FString v5 = "";
						bool rRotator = CalculateVector(value, &v5, EVectorType::Rotator);
						if (rRotator) {
							res = v5;
						}
						else {
							FString v6 = "";
							bool rArray = CalculateArray(value, &v6, init);
							if (rArray) {
								res = v6;
							}
							else {
								res = value;
							}
						}
					}
				}
			}
		}
	}
	return res;
}

AActor* UInteractComponent::GetTargetActor(UInteractComponent* interactComponent, FFunctionActorData dataActor, AActor* actor, EAction action, int tooltipIndex, EActionStageType stageType, bool inElse, int wrapperIndex /*= -1*/, int functIndex /*= -1*/, int stageIndex /*= -1*/)
{
	int val = (int)dataActor.enumName - 1;
	AActor* acto = nullptr;
	if (val >= 0) {
		acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, action, tooltipIndex, stageType, wrapperIndex, functIndex, stageIndex, FExtraActorData(inElse, 0));
	}
	if (!acto) {
		acto = actor;
	}
	return acto;
}

AActor* UInteractComponent::GetCurrentActor(UInteractComponent* interactComponent, FFunctionActorData dataActor, AActor* actor, EAction action, int tooltipIndex, EActionStageType stageType, bool inElse, int wrapperIndex /*= -1*/, int functIndex /*= -1*/, int stageIndex /*= -1*/)
{
	int val = (int)dataActor.enumName - 1;
	AActor* acto = nullptr;
	if (val >= 0) {
		acto = interactComponent->actorFunctionMap[val].Execute(interactComponent, actor, action, tooltipIndex, stageType, wrapperIndex, functIndex, stageIndex, FExtraActorData(inElse, 1));
	}
	if (!acto) {
		acto = actor;
	}
	return acto;
}

USceneComponent* UInteractComponent::GetSceneComponent(UInteractComponent* interactComponent, AActor* dataActor, TSubclassOf<USceneComponent> specificChild, FName componentTag)
{
	USceneComponent* compRot = nullptr;
	if (IsValid(specificChild)) {
		TSet<UActorComponent*> comps;
		comps = dataActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
			if (componentTag.ToString() != "" && componentTag != NAME_None) {
				if (scene && scene->GetClass() == specificChild && scene->ComponentHasTag(componentTag)) {
					compRot = scene;
					//scene->AddLocalRotation(data.rotation);
				}
			}
			else {
				if (scene && scene->GetClass() == specificChild) {
					compRot = scene;
					//scene->AddLocalRotation(data.rotation);
				}
			}
		}
	}
	else if (componentTag.ToString() != "" && componentTag != NAME_None) {
		TSet<UActorComponent*> comps;
		comps = dataActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
			if (scene && scene->ComponentHasTag(componentTag)) {
				compRot = scene;
				//scene->AddLocalRotation(data.rotation);
			}
		}
	}
	return compRot;
}

UActorComponent* UInteractComponent::GetActorComponent(UInteractComponent* interactComponent, AActor* dataActor, TSubclassOf<UActorComponent> specificChild, FName componentTag)
{
	UActorComponent* compRot = nullptr;
	if (IsValid(specificChild)) {
		TSet<UActorComponent*> comps;
		comps = dataActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			UActorComponent* scene = comps.Array()[i];
			if (componentTag.ToString() != "" && componentTag != NAME_None) {
				if (scene && scene->GetClass() == specificChild && scene->ComponentHasTag(componentTag)) {
					compRot = scene;
					//scene->AddLocalRotation(data.rotation);
				}
			}
			else {
				if (scene && scene->GetClass() == specificChild) {
					compRot = scene;
					//scene->AddLocalRotation(data.rotation);
				}
			}
		}
	}
	else if (componentTag.ToString() != "" && componentTag != NAME_None) {
		TSet<UActorComponent*> comps;
		comps = dataActor->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			UActorComponent* scene = comps.Array()[i];
			if (scene && scene->ComponentHasTag(componentTag)) {
				compRot = scene;
				//scene->AddLocalRotation(data.rotation);
			}
		}
	}
	return compRot;
}

FVector UInteractComponent::GetDirectionInteract(EVectorDirection direction, float distance, FVector offsetLocation, AActor* dataActor /*= nullptr*/, USceneComponent* dataComponent /*= nullptr*/)
{
	FVector vect = { 0.f, 0.f, 0.f };
	FVector location = { 0.f, 0.f, 0.f };
	if (direction == EVectorDirection::Forward) {
		if (dataComponent) {
			vect = dataComponent->GetForwardVector();
			vect.Normalize();
			location = vect * distance;
		}
		else if (dataActor) {
			vect = dataActor->GetActorForwardVector();
			vect.Normalize();
			location = vect * distance;
		}
	}
	else if (direction == EVectorDirection::Backward) {
		if (dataComponent) {
			vect = -dataComponent->GetForwardVector();
			vect.Normalize();
			location = vect * distance;
		}
		else if (dataActor) {
			vect = -dataActor->GetActorForwardVector();
			vect.Normalize();
			location = vect * distance;
		}
	}
	else if (direction == EVectorDirection::Up) {
		if (dataComponent) {
			vect = dataComponent->GetUpVector();
			vect.Normalize();
			location = vect * distance;
		}
		else if (dataActor) {
			vect = dataActor->GetActorUpVector();
			vect.Normalize();
			location = vect * distance;
		}
	}
	else if (direction == EVectorDirection::Down) {
		if (dataComponent) {
			vect = -dataComponent->GetUpVector();
			vect.Normalize();
			location = vect * distance;
		}
		else if (dataActor) {
			vect = -dataActor->GetActorUpVector();
			vect.Normalize();
			location = vect * distance;
		}
	}
	else if (direction == EVectorDirection::Right) {
		if (dataComponent) {
			vect = dataComponent->GetRightVector();
			vect.Normalize();
			location = vect * distance;
		}
		else if (dataActor) {
			vect = dataActor->GetActorRightVector();
			vect.Normalize();
			location = vect * distance;
		}
	}
	else if (direction == EVectorDirection::Left) {
		if (dataComponent) {
			vect = -dataComponent->GetRightVector();
			vect.Normalize();
			location = vect * distance;
		}
		else if (dataActor) {
			vect = -dataActor->GetActorRightVector();
			vect.Normalize();
			location = vect * distance;
		}
	}
	location += offsetLocation;
	if (dataComponent) {
		location += dataComponent->GetComponentLocation();
	}
	else if (dataActor) {
		location += dataActor->GetActorLocation();
	}
	return location;
}

void UInteractComponent::RegisterTimer(UInteractComponent* interactComponent, ETimerList enumName, int currentStage, int currentFunction, EActionStageType currentStageType, EAction currentAction, ETimerListCore enumNameCore)
{
	if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerList"), enumName) + "-" + FString::FromInt(currentStage) + "-" + FString::FromInt(currentFunction)+ "-"+ FString::FromInt((int)currentStageType) + "-" + FString::FromInt((int)currentAction))) {
		interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerList"), enumName) + "-" + FString::FromInt(currentStage) + "-" + FString::FromInt(currentFunction) + "-" + FString::FromInt((int)currentStageType) + "-" + FString::FromInt((int)currentAction));
	}
	if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), enumNameCore) + "-" + FString::FromInt(currentStage) + "-" + FString::FromInt(currentFunction) + "-" + FString::FromInt((int)currentStageType) + "-" + FString::FromInt((int)currentAction))) {
		interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), enumNameCore) + "-" + FString::FromInt(currentStage) + "-" + FString::FromInt(currentFunction) + "-" + FString::FromInt((int)currentStageType) + "-" + FString::FromInt((int)currentAction));
	}
}

bool UInteractComponent::IsTimerRegister(UInteractComponent* interactComponent, ETimerList enumName, int currentStage, int currentFunction, EActionStageType currentStageType, EAction currentAction, ETimerListCore enumNameCore)
{
	return interactComponent->GetWorld() && interactComponent->GetOwner() && (interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerList"), enumName) + "-" + FString::FromInt(currentStage) + "-" + FString::FromInt(currentFunction) + "-"+ FString::FromInt((int)currentStageType) + "-" + FString::FromInt((int)currentAction)) || interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), enumNameCore) + "-" + FString::FromInt(currentStage) + "-" + FString::FromInt(currentFunction) + "-" + FString::FromInt((int)currentStageType) + "-" + FString::FromInt((int)currentAction)));
}

void UInteractComponent::ReturnValueFloat(UPARAM(ref) TArray<FString>& customReturn, float returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueInteger(UPARAM(ref) TArray<FString>& customReturn, int returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueString(UPARAM(ref) TArray<FString>& customReturn, FString returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueBool(UPARAM(ref) TArray<FString>& customReturn, bool returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueName(UPARAM(ref) TArray<FString>& customReturn, FName returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueVector(UPARAM(ref) TArray<FString>& customReturn, FVector returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueColor(UPARAM(ref) TArray<FString>& customReturn, FColor returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueRotator(UPARAM(ref) TArray<FString>& customReturn, FRotator returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueObject(UPARAM(ref) TArray<FString>& customReturn, UObject* returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValueActor(UPARAM(ref) TArray<FString>& customReturn, AActor* returnValue)
{
	ReturnValue(&customReturn, returnValue);
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, UObject* returnValue)
{
	FString resVa = "";
	resVa = "%%" + returnValue->GetName();
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
		if (!m_variablesObject.Contains(returnValue)) {
			m_variablesObject.Add(returnValue);
		}
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, int returnValue)
{
	FString resVa = "";
	resVa = FString::FromInt(returnValue);
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, float returnValue)
{
	FString resVa = "";
	resVa = FString::SanitizeFloat(returnValue);
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, FString returnValue, bool isString)
{
	FString resVa = "";
	if (isString) {
		resVa = "\"" + returnValue + "\"";
	}
	else {
		resVa = returnValue;
	}
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, bool returnValue)
{
	FString resVa = "";
	resVa = returnValue ? "true" : "false";
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, FName returnValue)
{
	FString resVa = "";
	resVa = "\"" + returnValue.ToString() + "\"";
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, FVector returnValue)
{
	FString resVa = "";
	resVa = "{" + returnValue.ToString() + "}";
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, FRotator returnValue)
{
	FString resVa = "";
	resVa = "{" + returnValue.ToString() + "}";
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, AActor* returnValue)
{
	FString resVa = "null";
	if (returnValue) {	
		resVa = "%%" + returnValue->GetName();
		if (!customReturn->Contains(resVa)) {
			customReturn->Add(resVa);
			if (!m_variablesObject.Contains(returnValue)) {
				m_variablesObject.Add(returnValue);
			}
		}
	}
	else {
		if (!customReturn->Contains(resVa)) {
			customReturn->Add(resVa);
		}
	}
}

void UInteractComponent::ReturnValue(TArray<FString>* customReturn, FColor returnValue)
{
	FString resVa = "";
	resVa = "{" + returnValue.ToString() + "}";
	if (!customReturn->Contains(resVa)) {
		customReturn->Add(resVa);
	}
}

float UInteractComponent::CalculateNumber(FString value)
{
	float res = NaN;
	FRegexPattern myPattern(TEXT("^-?\\d+[,.]\\d+$"));
	FRegexMatcher myMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		res = atof(TCHAR_TO_ANSI(*newval));
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		res = FCString::Atoi(*value);
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?(\\s*\\+\\s*)(-?\\d+([,.]\\d+)?)$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString val1 = "";
		FString val2 = "";
		newval.Split("+", &val1, &val2);
		float leftfloat = atof(TCHAR_TO_ANSI(*val1));
		float rightfloat = atof(TCHAR_TO_ANSI(*val2));
		res = leftfloat + rightfloat;
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?(\\s*\\-\\s*)(-?\\d+([,.]\\d+)?)$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString val1 = "";
		FString val2 = "";
		newval.Split("-", &val1, &val2);
		float leftfloat = atof(TCHAR_TO_ANSI(*val1));
		float rightfloat = atof(TCHAR_TO_ANSI(*val2));
		res = leftfloat - rightfloat;
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?(\\s*\\*\\s*)(-?\\d+([,.]\\d+)?)$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString val1 = "";
		FString val2 = "";
		newval.Split("*", &val1, &val2);
		float leftfloat = atof(TCHAR_TO_ANSI(*val1));
		float rightfloat = atof(TCHAR_TO_ANSI(*val2));
		res = leftfloat * rightfloat;
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?(\\s*\\/\\s*)(-?\\d+([,.]\\d+)?)$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString val1 = "";
		FString val2 = "";
		newval.Split("/", &val1, &val2);
		float leftfloat = atof(TCHAR_TO_ANSI(*val1));
		float rightfloat = atof(TCHAR_TO_ANSI(*val2));
		res = leftfloat / rightfloat;
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?(\\s*\\%\\s*)(-?\\d+([,.]\\d+)?)$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString val1 = "";
		FString val2 = "";
		newval.Split("%", &val1, &val2);
		float leftfloat = atof(TCHAR_TO_ANSI(*val1));
		float rightfloat = atof(TCHAR_TO_ANSI(*val2));
		res = FMath::Fmod(leftfloat, rightfloat);
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?(\\s*\\^\\s*)(-?\\d+([,.]\\d+)?)$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString val1 = "";
		FString val2 = "";
		newval.Split("^", &val1, &val2);
		float leftfloat = atof(TCHAR_TO_ANSI(*val1));
		float rightfloat = atof(TCHAR_TO_ANSI(*val2));
		res = FMath::Pow(leftfloat, rightfloat);
		return res;
	}
	myPattern = FRegexPattern(TEXT("^-?\\d+([,.]\\d+)?->sqrt$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT(","), TEXT("."));
		newval = newval.Replace(TEXT(" "), TEXT(""));
		FString val1 = "";
		FString val2 = "";
		newval.Split("->sqrt", &val1, &val2);
		float leftfloat = atof(TCHAR_TO_ANSI(*val1));
		//float rightfloat = atof(TCHAR_TO_ANSI(*val2));
		res = FMath::Sqrt(leftfloat);
		return res;
	}
	res = UInteractCore::GetValue(value);
	return res;
}

bool UInteractComponent::CalculateString(FString value, FString* res, bool init)
{
	FRegexPattern myPattern(TEXT("^\"[^\"]*\"$"));
	FRegexMatcher myMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString newval = value.Replace(TEXT("\""), TEXT(""));
		*res = newval;
		if (init) {
			*res = "\"" + *res + "\"";
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\"[^\"]*\"\\+\\s*-?\\d+([\\.]\\d+)?$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString part1 = "";
		FString part2 = "";
		FString noneS = "";
		value.Split("+", &part1, &part2);
		part2 = part2.TrimStartAndEnd();
		part2.Split(".", &part2, &noneS);
		if (UInteractCore::GetValue(noneS) != 0.f) {
			part2 += noneS;
		}
		part1 = part1.Replace(TEXT("\""), TEXT(""));
		*res = part1 + part2;
		if (init) {
			*res = "\"" + *res + "\"";
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\"[^\"]*\"\\+[^\"]+$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString part1 = "";
		FString part2 = "";
		value.Split("+", &part1, &part2);
		part1 = part1.Replace(TEXT("\""), TEXT(""));
		*res = part1 + part2;
		if (init) {
			*res = "\"" + *res + "\"";
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^[^\"]+\\+\"[^\"]*\"$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString part1 = "";
		FString part2 = "";
		value.Split("+", &part1, &part2);
		part2 = part2.Replace(TEXT("\""), TEXT(""));
		*res = part1 + part2;
		if (init) {
			*res = "\"" + *res + "\"";
		}
		return true;
	}
	return false;
}

bool UInteractComponent::CalculateBoolean(FString value, FString* res)
{
	FRegexPattern myPattern(TEXT("^((true)|(false))$"));
	FRegexMatcher myMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		*res = value;
		return true;
	}
	myPattern = FRegexPattern(TEXT("^!((true)|(false))$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		if (value == "!true") {
			*res = "false";
		}
		else if (value == "!false") {
			*res = "true";
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^!?((true)|(false))&&!?((true)|(false))$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString part1 = "";
		FString part2 = "";
		value.Split("&&", &part1, &part2);
		if (part1 == "!true") {
			part1 = "false";
		}
		else if (part1 == "!false") {
			part1 = "true";
		}
		if (part2 == "!true") {
			part2 = "false";
		}
		else if (part2 == "!false") {
			part2 = "true";
		}
		bool bPart1 = part1 == "true" ? true : false;
		bool bPart2 = part2 == "true" ? true : false;
		bool bRes = bPart1 && bPart2;
		*res = bRes ? "true" : "false";
		return true;
	}
	myPattern = FRegexPattern(TEXT("^!?((true)|(false))\\|\\|!?((true)|(false))$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString part1 = "";
		FString part2 = "";
		value.Split("||", &part1, &part2);
		if (part1 == "!true") {
			part1 = "false";
		}
		else if (part1 == "!false") {
			part1 = "true";
		}
		if (part2 == "!true") {
			part2 = "false";
		}
		else if (part2 == "!false") {
			part2 = "true";
		}
		bool bPart1 = part1 == "true" ? true : false;
		bool bPart2 = part2 == "true" ? true : false;
		bool bRes = bPart1 || bPart2;
		*res = bRes ? "true" : "false";
		return true;
	}
	return false;
}

bool UInteractComponent::CalculateVector(FString value, FString* res, EVectorType type)
{
	FString newval = value.Replace(TEXT(" "), TEXT(""));
	FString val1 = "";
	FString val2 = "";
	newval = newval.ToUpper();
	if (type == EVectorType::Color) {
		FString myNone = "";
		FString myValV = "";
		FString rValV = "";
		FString gValV = "";
		FString bValV = "";
		FString aValV = "";
		FRegexPattern myPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}$"));
		FRegexMatcher myMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.ToUpper().TrimStartAndEnd().Split("R=", &myNone, &myValV);
			myValV.Split(",G=", &rValV, &myValV);
			myValV.Split(",B=", &gValV, &myValV);
			myValV.Split(",A=", &bValV, &myValV);
			myValV.Split("}", &aValV, &myValV);
			newval = "{R=" + FString::SanitizeFloat(UInteractCore::GetValue(rValV.ToLower())) + ",G=" + FString::SanitizeFloat(UInteractCore::GetValue(gValV.ToLower())) + ",B=" + FString::SanitizeFloat(UInteractCore::GetValue(bValV.ToLower())) + ",A=" + FString::SanitizeFloat(UInteractCore::GetValue(aValV.ToLower())) + "}";
			FColor vectorVal = { 0, 0, 0, 0 };
			bool bVal = vectorVal.InitFromString(newval.ToUpper());
			if (bVal) {
				*res = newval;
				return true;
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}\\+\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("+", &val1, &val2);
			val1.ToUpper().TrimStartAndEnd().Split("R=", &myNone, &myValV);
			myValV.Split(",G=", &rValV, &myValV);
			myValV.Split(",B=", &gValV, &myValV);
			myValV.Split(",A=", &bValV, &myValV);
			myValV.Split("}", &aValV, &myValV);
			val1 = "{R=" + FString::SanitizeFloat(UInteractCore::GetValue(rValV.ToLower())) + ",G=" + FString::SanitizeFloat(UInteractCore::GetValue(gValV.ToLower())) + ",B=" + FString::SanitizeFloat(UInteractCore::GetValue(bValV.ToLower())) + ",A=" + FString::SanitizeFloat(UInteractCore::GetValue(aValV.ToLower())) + "}";
			FColor vectorVal1 = { 0, 0, 0, 0 };
			bool bVal = vectorVal1.InitFromString(val1.ToUpper());
			if (bVal) {
				val2.ToUpper().TrimStartAndEnd().Split("R=", &myNone, &myValV);
				myValV.Split(",G=", &rValV, &myValV);
				myValV.Split(",B=", &gValV, &myValV);
				myValV.Split(",A=", &bValV, &myValV);
				myValV.Split("}", &aValV, &myValV);
				val2 = "{R=" + FString::SanitizeFloat(UInteractCore::GetValue(rValV.ToLower())) + ",G=" + FString::SanitizeFloat(UInteractCore::GetValue(gValV.ToLower())) + ",B=" + FString::SanitizeFloat(UInteractCore::GetValue(bValV.ToLower())) + ",A=" + FString::SanitizeFloat(UInteractCore::GetValue(aValV.ToLower())) + "}";
				FColor vectorVal2 = { 0, 0, 0, 0 };
				bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
				if (bVal1) {
					vectorVal1 = { (uint8)(vectorVal1.R + vectorVal2.R), (uint8)(vectorVal1.G + vectorVal2.G), (uint8)(vectorVal1.B + vectorVal2.B), (uint8)(vectorVal1.A + vectorVal2.A) };
					*res = "{" + vectorVal1.ToString() + "}";
					*res = res->Replace(TEXT("G"), TEXT(",G"));
					*res = res->Replace(TEXT("B"), TEXT(",B"));
					*res = res->Replace(TEXT("A"), TEXT(",A"));
					return true;
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}-\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("-{", &val1, &val2);
			val1.ToUpper().TrimStartAndEnd().Split("R=", &myNone, &myValV);
			myValV.Split(",G=", &rValV, &myValV);
			myValV.Split(",B=", &gValV, &myValV);
			myValV.Split(",A=", &bValV, &myValV);
			myValV.Split("}", &aValV, &myValV);
			val1 = "{R=" + FString::SanitizeFloat(UInteractCore::GetValue(rValV.ToLower())) + ",G=" + FString::SanitizeFloat(UInteractCore::GetValue(gValV.ToLower())) + ",B=" + FString::SanitizeFloat(UInteractCore::GetValue(bValV.ToLower())) + ",A=" + FString::SanitizeFloat(UInteractCore::GetValue(aValV.ToLower())) + "}";
			FColor vectorVal1 = { 0, 0, 0, 0 };
			bool bVal = vectorVal1.InitFromString(val1.ToUpper());
			if (bVal) {
				val2.ToUpper().TrimStartAndEnd().Split("R=", &myNone, &myValV);
				myValV.Split(",G=", &rValV, &myValV);
				myValV.Split(",B=", &gValV, &myValV);
				myValV.Split(",A=", &bValV, &myValV);
				myValV.Split("}", &aValV, &myValV);
				val2 = "{R=" + FString::SanitizeFloat(UInteractCore::GetValue(rValV.ToLower())) + ",G=" + FString::SanitizeFloat(UInteractCore::GetValue(gValV.ToLower())) + ",B=" + FString::SanitizeFloat(UInteractCore::GetValue(bValV.ToLower())) + ",A=" + FString::SanitizeFloat(UInteractCore::GetValue(aValV.ToLower())) + "}";
				FColor vectorVal2 = { 0, 0, 0, 0 };
				bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
				if (bVal1) {
					vectorVal1 = { (uint8)(vectorVal1.R - vectorVal2.R), (uint8)(vectorVal1.G - vectorVal2.G), (uint8)(vectorVal1.B - vectorVal2.B), (uint8)(vectorVal1.A - vectorVal2.A) };
					*res = "{" + vectorVal1.ToString() + "}";
					*res = res->Replace(TEXT("G"), TEXT(",G"));
					*res = res->Replace(TEXT("B"), TEXT(",B"));
					*res = res->Replace(TEXT("A"), TEXT(",A"));
					return true;
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}\\*\\d+([.,]\\d+)$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("*", &val1, &val2);
			val1.ToUpper().TrimStartAndEnd().Split("R=", &myNone, &myValV);
			myValV.Split(",G=", &rValV, &myValV);
			myValV.Split(",B=", &gValV, &myValV);
			myValV.Split(",A=", &bValV, &myValV);
			myValV.Split("}", &aValV, &myValV);
			val1 = "{R=" + FString::SanitizeFloat(UInteractCore::GetValue(rValV.ToLower())) + ",G=" + FString::SanitizeFloat(UInteractCore::GetValue(gValV.ToLower())) + ",B=" + FString::SanitizeFloat(UInteractCore::GetValue(bValV.ToLower())) + ",A=" + FString::SanitizeFloat(UInteractCore::GetValue(aValV.ToLower())) + "}";
			FColor vectorVal1 = { 0, 0, 0, 0 };
			bool bVal = vectorVal1.InitFromString(val1.ToUpper());
			if (bVal) {
				float vectorVal2 = UInteractCore::GetValue(val2);
				if (vectorVal2 != NaN) {
					vectorVal1 = { (uint8)(vectorVal1.R * vectorVal2), (uint8)(vectorVal1.G * vectorVal2), (uint8)(vectorVal1.B * vectorVal2), (uint8)(vectorVal1.A * vectorVal2) };
					*res = "{" + vectorVal1.ToString() + "}";
					*res = res->Replace(TEXT("G"), TEXT(",G"));
					*res = res->Replace(TEXT("B"), TEXT(",B"));
					*res = res->Replace(TEXT("A"), TEXT(",A"));
					return true;
				}
			}
		}
	}
	else {
		FString myNone = "";
		FString myValV = "";
		FString xValV = "";
		FString yValV = "";
		FString zValV = "";
		FRegexPattern myPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}$"));
		FRegexMatcher myMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			if (type == EVectorType::Rotator) {
				newval.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",R=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				newval = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FRotator vectorVal = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal.InitFromString(newval.ToUpper());
				if (bVal) {
					*res = newval;
					return true;
				}
			}
			else if (type == EVectorType::Vector) {
				newval.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",Z=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				newval = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FVector vectorVal = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal.InitFromString(newval.ToUpper());
				if (bVal) {
					*res = newval;
					return true;
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}\\+\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("+", &val1, &val2);
			if (type == EVectorType::Rotator) {
				val1.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",R=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FRotator vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					val2.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
					myValV.Split(",Y=", &xValV, &myValV);
					myValV.Split(",R=", &yValV, &myValV);
					myValV.Split("}", &zValV, &myValV);
					val2 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
					FRotator vectorVal2 = { 0.f, 0.f, 0.f };
					bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
					if (bVal1) {
						vectorVal1 += vectorVal2;
						*res = "{" + vectorVal1.ToString() + "}";
						*res = res->Replace(TEXT("Y"), TEXT(",Y"));
						*res = res->Replace(TEXT("R"), TEXT(",R"));
						return true;
					}
				}
			}
			else if (type == EVectorType::Vector) {
				val1.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",Z=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FVector vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					val2.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
					myValV.Split(",Y=", &xValV, &myValV);
					myValV.Split(",Z=", &yValV, &myValV);
					myValV.Split("}", &zValV, &myValV);
					val2 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
					FVector vectorVal2 = { 0.f, 0.f, 0.f };
					bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
					if (bVal1) {
						vectorVal1 += vectorVal2;
						*res = "{" + vectorVal1.ToString() + "}";
						*res = res->Replace(TEXT("Y"), TEXT(",Y"));
						*res = res->Replace(TEXT("Z"), TEXT(",Z"));
						return true;
					}
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}-\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("-{", &val1, &val2);
			if (type == EVectorType::Rotator) {
				val1.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",R=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FRotator vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					val2.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
					myValV.Split(",Y=", &xValV, &myValV);
					myValV.Split(",R=", &yValV, &myValV);
					myValV.Split("}", &zValV, &myValV);
					val2 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
					FRotator vectorVal2 = { 0.f, 0.f, 0.f };
					bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
					if (bVal1) {
						vectorVal1 -= vectorVal2;
						*res = "{" + vectorVal1.ToString() + "}";
						*res = res->Replace(TEXT("Y"), TEXT(",Y"));
						*res = res->Replace(TEXT("R"), TEXT(",R"));
						return true;
					}
				}
			}
			else if (type == EVectorType::Vector) {
				val1.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",Z=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FVector vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					val2.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
					myValV.Split(",Y=", &xValV, &myValV);
					myValV.Split(",Z=", &yValV, &myValV);
					myValV.Split("}", &zValV, &myValV);
					val2 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
					FVector vectorVal2 = { 0.f, 0.f, 0.f };
					bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
					if (bVal1) {
						vectorVal1 -= vectorVal2;
						*res = "{" + vectorVal1.ToString() + "}";
						*res = res->Replace(TEXT("Y"), TEXT(",Y"));
						*res = res->Replace(TEXT("Z"), TEXT(",Z"));
						return true;
					}
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}\\*\\d+([.,]\\d+)$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("*", &val1, &val2);
			if (type == EVectorType::Rotator) {
				val1.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",R=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FRotator vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					float vectorVal2 = UInteractCore::GetValue(val2);
					if (vectorVal2 != NaN) {
						vectorVal1 = { vectorVal1.Pitch*vectorVal2,vectorVal1.Yaw*vectorVal2,vectorVal1.Roll*vectorVal2 };
						*res = "{" + vectorVal1.ToString() + "}";
						*res = res->Replace(TEXT("Y"), TEXT(",Y"));
						*res = res->Replace(TEXT("R"), TEXT(",R"));
						return true;
					}
				}
			}
			else if (type == EVectorType::Vector) {
				val1.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",Z=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FVector vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					float vectorVal2 = UInteractCore::GetValue(val2);
					if (vectorVal2 != NaN) {
						vectorVal1 = { vectorVal1.X *vectorVal2, vectorVal1.Y *vectorVal2, vectorVal1.Z *vectorVal2 };
						*res = "{" + vectorVal1.ToString() + "}";
						*res = res->Replace(TEXT("Y"), TEXT(",Y"));
						*res = res->Replace(TEXT("Z"), TEXT(",Z"));
						return true;
					}
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}->mod$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("->mod", &val1, &val2);
			if (type == EVectorType::Rotator) {
				val1.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",R=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FRotator vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					float vf = vectorVal1.Pitch + vectorVal1.Yaw + vectorVal1.Roll;
					*res = FString::SanitizeFloat(vf);
					return true;
				}
			}
			else if (type == EVectorType::Vector) {
				val1.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",Z=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FVector vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					float vectorVal2 = vectorVal1.Size();
					*res = FString::SanitizeFloat(vectorVal2);
					return true;
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}->dist->\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("->dist->", &val1, &val2);
			if (type == EVectorType::Rotator) {
				val1.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",R=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FRotator vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					val2.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
					myValV.Split(",Y=", &xValV, &myValV);
					myValV.Split(",R=", &yValV, &myValV);
					myValV.Split("}", &zValV, &myValV);
					val2 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
					FRotator vectorVal2 = { 0.f, 0.f, 0.f };
					bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
					if (bVal1) {
						float vf = vectorVal1.Pitch - vectorVal2.Pitch + vectorVal1.Yaw - vectorVal2.Yaw + vectorVal1.Roll - vectorVal2.Roll;
						*res = FString::SanitizeFloat(vf);
						return true;
					}
				}
			}
			else if (type == EVectorType::Vector) {
				val1.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",Z=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FVector vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					val2.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
					myValV.Split(",Y=", &xValV, &myValV);
					myValV.Split(",Z=", &yValV, &myValV);
					myValV.Split("}", &zValV, &myValV);
					val2 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
					FVector vectorVal2 = { 0.f, 0.f, 0.f };
					bool bVal1 = vectorVal2.InitFromString(val2.ToUpper());
					if (bVal1) {
						float vf = FVector::Dist(vectorVal1, vectorVal2);
						*res = FString::SanitizeFloat(vf);
						return true;
					}
				}
			}
		}
		myPattern = FRegexPattern(TEXT("^\\{\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?,\\w=-?\\d+([,.]\\d+)?((\\s*TO\\s*)((-?\\d+([,.]\\d+)?)))?\\}->norm$"));
		myMatcher = FRegexMatcher(myPattern, newval);
		if (myMatcher.FindNext()) {
			newval.Split("->norm", &val1, &val2);
			if (type == EVectorType::Rotator) {
				val1.ToUpper().TrimStartAndEnd().Split("P=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",R=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{P=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",R=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FRotator vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					vectorVal1.Normalize();
					*res = "{" + vectorVal1.ToString() + "}";
					*res = res->Replace(TEXT("Y"), TEXT(",Y"));
					*res = res->Replace(TEXT("R"), TEXT(",R"));
					return true;
				}
			}
			else if (type == EVectorType::Vector) {
				val1.ToUpper().TrimStartAndEnd().Split("X=", &myNone, &myValV);
				myValV.Split(",Y=", &xValV, &myValV);
				myValV.Split(",Z=", &yValV, &myValV);
				myValV.Split("}", &zValV, &myValV);
				val1 = "{X=" + FString::SanitizeFloat(UInteractCore::GetValue(xValV.ToLower())) + ",Y=" + FString::SanitizeFloat(UInteractCore::GetValue(yValV.ToLower())) + ",Z=" + FString::SanitizeFloat(UInteractCore::GetValue(zValV.ToLower())) + "}";
				FVector vectorVal1 = { 0.f, 0.f, 0.f };
				bool bVal = vectorVal1.InitFromString(val1.ToUpper());
				if (bVal) {
					vectorVal1.Normalize();
					*res = "{" + vectorVal1.ToString() + "}";
					*res = res->Replace(TEXT("Y"), TEXT(",Y"));
					*res = res->Replace(TEXT("Z"), TEXT(",Z"));
					return true;
				}
			}
		}
	}
	return false;
}

bool UInteractComponent::CalculateArray(FString value, FString* res, bool init)
{
	//FString newval = value.Replace(TEXT(" "), TEXT(""));
	FString val1 = "";
	FString val2 = "";
	FRegexPattern myPattern(TEXT("^\\[.*\\]$"));
	FRegexMatcher myMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		*res = value;
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\\[.*\\]->size$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		int elemNum = 0;
		val2 = value;
		val2.Split("[", &val1, &val2);
		if (val2.Contains("[") && val2.Contains("]")) {
			while (val2.Split("],", &val1, &val2)) {
				elemNum++;
			}
			elemNum++;
		}
		else if (val2.Contains("{") && val2.Contains("}")) {
			while (val2.Split("},", &val1, &val2)) {
				elemNum++;
			}
			elemNum++;
		}
		else if (val2.Contains("\"")) {
			while (val2.Split("\",", &val1, &val2)) {
				elemNum++;
			}
			elemNum++;
		}
		else {
			while (val2.Split(",", &val1, &val2)) {
				elemNum++;
			}
			elemNum++;
		}
		*res = FString::FromInt(elemNum);
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\\[.*\\]->push->.+$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		value.Split("]->push->", &val1, &val2);
		if (val2.StartsWith("[") && val2.EndsWith("]")) {
			val2.Replace(TEXT("["), TEXT(""));
			val2.Replace(TEXT("]"), TEXT(""));
		}
		FString val4 = "";
		FString none4 = "";
		while (!init && val2.Contains("\"")) {
			val2.Split("\"", &none4, &val2);
			val4 += none4;
		}
		if (val4 != "") {
			val2 = val4;
		}
		*res = val1;
		if (val1.TrimStartAndEnd() != "[") {
			*res += ",";
		}
		*res += val2.TrimStartAndEnd() + "]";
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\\[.+\\]->remove->\\d+([,.]\\d+)?$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		FString retValue = "";
		int elemNum = 0;
		value.Split("]->remove->", &val1, &val2);
		int index = (int)UInteractCore::GetValue(val2);
		//val2 = value;
		FString val3 = val1;
		val3.Split("[", &val1, &val2);
		retValue = "[";
		if (val2.Contains("[") && val2.Contains("]")) {
			while (val2.Split("],", &val1, &val2)) {
				if (index != elemNum) {
					retValue += val1 + "],";
				}
				elemNum++;
			}
			if (index != elemNum) {
				retValue += val2 + "]";
			}
			elemNum++;
		}
		else if (val2.Contains("{") && val2.Contains("}")) {
			while (val2.Split("},", &val1, &val2)) {
				if (index != elemNum) {
					retValue += val1 + "},";
				}
				elemNum++;
			}
			if (index != elemNum) {
				retValue += val2 + "]";
			}
			elemNum++;
		}
		else if (val2.Contains("\"")) {
			while (val2.Split("\",", &val1, &val2)) {
				if (index != elemNum) {
					retValue += val1 + "\",";
				}
				elemNum++;
			}
			if (index != elemNum) {
				retValue += val2 + "]";
			}
			elemNum++;
		}
		else {
			while (val2.Split(",", &val1, &val2)) {
				if (index != elemNum) {
					retValue += val1 + ",";
				}
				elemNum++;
			}
			if (index != elemNum) {
				retValue += val2 + "]";
			}
			elemNum++;
		}
		*res = retValue;
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\\[.+\\]->\\d+([,.]\\d+)?$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		int elemNum = 0;
		value.Split("]->", &val1, &val2);
		int index = (int)UInteractCore::GetValue(val2);
		val2 = value;
		val2.Split("[", &val1, &val2);
		if (val2.Contains("[") && val2.Contains("]")) {
			while (val2.Split("],", &val1, &val2)) {
				if (index == elemNum) {
					val1 = val1 + "]";
					*res = CalculateValue(val1.TrimStartAndEnd());
					return true;
				}
				elemNum++;
			}
			if (index == elemNum) {
				val2.Split("]->", &val1, &val2);
				*res = CalculateValue(val1.TrimStartAndEnd());
				return true;
			}
			elemNum++;
		}
		else if (val2.Contains("{") && val2.Contains("}")) {
			while (val2.Split("},", &val1, &val2)) {
				if (index == elemNum) {
					val1 = val1 + "}";
					*res = CalculateValue(val1.TrimStartAndEnd());
					return true;
				}
				elemNum++;
			}
			if (index == elemNum) {
				val2.Split("]->", &val1, &val2);
				*res = CalculateValue(val1.TrimStartAndEnd());
				return true;
			}
			elemNum++;
		}
		else if (val2.Contains("\"")) {
			while (val2.Split("\",", &val1, &val2)) {
				if (index == elemNum) {
					val1 = val1 + "\"";
					*res = CalculateValue(val1.TrimStartAndEnd());
					return true;
				}
				elemNum++;
			}
			if (index == elemNum) {
				val2.Split("]->", &val1, &val2);
				*res = CalculateValue(val1.TrimStartAndEnd());
				return true;
			}
			elemNum++;
		}
		else {
			while (val2.Split(",", &val1, &val2)) {
				if (index == elemNum) {
					*res = CalculateValue(val1.TrimStartAndEnd());
					return true;
				}
				elemNum++;
			}
			if (index == elemNum) {
				val2.Split("]->", &val1, &val2);
				*res = CalculateValue(val1.TrimStartAndEnd());
				return true;
			}
			elemNum++;
		}
	}
	return false;
}

bool UInteractComponent::CalculateObject(FString value, FString* res, bool init)
{
	FString val1 = "";
	FString val2 = "";
	FRegexPattern myPattern(TEXT("^null$"));
	FRegexMatcher myMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		*res = value;
		return true;
	}
	myPattern = FRegexPattern(TEXT("^this$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		*res = "%%" + GetName();
		bool hav = false;
		for (int h = 0; h < m_variablesObject.Num(); h++) {
			if (m_variablesObject[h]->GetName() == GetName()) {
				hav = true;
				break;
			}
		}
		if (!hav) {
			m_variablesObject.Add(this);
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^owner$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		if (GetOwner()) {
			*res = "%%" + GetOwner()->GetName();
			bool hav = false;
			for (int h = 0; h < m_variablesObject.Num(); h++) {
				if (m_variablesObject[h]->GetName() == GetOwner()->GetName()) {
					hav = true;
					break;
				}
			}
			if (!hav) {
				m_variablesObject.Add(GetOwner());
			}
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^otherActor$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		if (tempActor) {
			*res = "%%" + tempActor->GetName();
			bool hav = false;
			for (int h = 0; h < m_variablesObject.Num(); h++) {
				if (m_variablesObject[h]->GetName() == tempActor->GetName()) {
					hav = true;
					break;
				}
			}
			if (!hav) {
				m_variablesObject.Add(tempActor);
			}
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\\%\\%\\w+$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		*res = value;
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\\%\\%\\w+->class$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		value.Split("->class", &val1, &val2);
		val1.Split("%%", &val2, &val1);
		for (int h = 0; h < m_variablesObject.Num(); h++) {
			if (m_variablesObject[h]->GetName() == val1) {
				*res = "%%" + m_variablesObject[h]->GetClass()->GetName();
				m_variablesObject.Add(m_variablesObject[h]->GetClass());
				return true;
			}
		}
	}
	myPattern = FRegexPattern(TEXT("^\\%\\%\\w+->name$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		value.Split("->name", &val1, &val2);
		val1.Split("%%", &val2, &val1);
		if (init) {
			*res = "\"" + val1 + "\"";
		}
		else {
			*res = val1;
		}
		return true;
	}
	myPattern = FRegexPattern(TEXT("^\\%\\%\\w+->tags$"));
	myMatcher = FRegexMatcher(myPattern, value);
	if (myMatcher.FindNext()) {
		value.Split("->tags", &val1, &val2);
		val1.Split("%%", &val2, &val1);
		AActor* act2 = nullptr;
		UActorComponent* actComp3 = nullptr;
		for (int h = 0; h < m_variablesObject.Num(); h++) {
			if (m_variablesObject[h]->GetName() == val1) {
				act2 = Cast<AActor>(m_variablesObject[h]);
				actComp3 = Cast<UActorComponent>(m_variablesObject[h]);
			}
		}
		if (act2) {
			if (init) {
				*res = "";
				for (int d = 0; d < act2->Tags.Num() - 1; d++) {
					*res += "\"" + act2->Tags[d].ToString() + "\",";
				}
				if (act2->Tags.Num() > 0) {
					*res = "[" + *res + "\"" + act2->Tags[act2->Tags.Num() - 1].ToString() + "\"]";
				}
				else {
					*res = "[]";
				}
				return true;
			}
			else {
				*res = "";
				for (int d = 0; d < act2->Tags.Num() - 1; d++) {
					*res += act2->Tags[d].ToString() + ",";
				}
				if (act2->Tags.Num() > 0) {
					*res = "[" + *res + act2->Tags[act2->Tags.Num() - 1].ToString() + "]";
				}
				else {
					*res = "[]";
				}
				return true;
			}
		}
		else if (actComp3) {
			if (init) {
				*res = "";
				for (int d = 0; d < actComp3->ComponentTags.Num() - 1; d++) {
					*res += "\"" + actComp3->ComponentTags[d].ToString() + "\",";
				}
				if (actComp3->ComponentTags.Num() > 0) {
					*res = "[" + *res + "\"" + actComp3->ComponentTags[actComp3->ComponentTags.Num() - 1].ToString() + "\"]";
				}
				else {
					*res = "[]";
				}
				return true;
			}
			else {
				*res = "";
				for (int d = 0; d < actComp3->ComponentTags.Num() - 1; d++) {
					*res += actComp3->ComponentTags[d].ToString() + ",";
				}
				if (actComp3->ComponentTags.Num() > 0) {
					*res = "[" + *res + actComp3->ComponentTags[actComp3->ComponentTags.Num() - 1].ToString() + "]";
				}
				else {
					*res = "[]";
				}
				return true;
			}
		}
	}
	return false;
}


FString UInteractComponent::FindVariable(FString name)
{
	FString res = "";
	FString noneS = "";
	res = name.TrimStartAndEnd();
	if (res.StartsWith("_")) {
		res.Split("_", &noneS, &res);
	}
	for (int i = 0; i < m_sharedVariablesArray.Num(); i++) {
		if (m_sharedVariablesArray[i].Contains(res + "=")) {
			m_sharedVariablesArray[i].Split("=", &noneS, &res);
			return res.TrimStartAndEnd();
		}
	}
	return "";
}

FString UInteractComponent::FindLocalVariable(void* parent, UStruct* structClass, FString var)
{
	FString res = "";
	FString noneS = "";
	bool foundV = false;
	res = var.TrimStartAndEnd();
	if (res.StartsWith("$")) {
		res.Split("$", &noneS, &res);
	}
	if (res != "") {
		FString name = res;
		FString secondary = "";
		if (res.Contains("::")) {
			name.Split("::", &res, &secondary);
		}

		UProperty* It = FindField<UProperty>(structClass, *res);

		if (It) {
			foundV = true;
			if (It->GetCPPType() == "float") {
				UFloatProperty* It4 = Cast<UFloatProperty>(It);
				float v = It4->GetPropertyValue_InContainer(parent, 0);
				res = FString::SanitizeFloat(v);
			}
			else if (It->GetCPPType() == "int32") {
				UIntProperty* It4 = Cast<UIntProperty>(It);
				int v = It4->GetPropertyValue_InContainer(parent, 0);
				res = FString::FromInt(v);
			}
			else if (It->GetCPPType() == "FString") {
				UStrProperty* It4 = Cast<UStrProperty>(It);
				FString v = It4->GetPropertyValue_InContainer(parent, 0);
				res = v;
			}
			else if (It->GetCPPType() == "bool") {
				UBoolProperty* It4 = Cast<UBoolProperty>(It);
				bool b = It4->GetPropertyValue_InContainer(parent, 0);
				res = b ? "true" : "false";
			}
			else if (It->GetCPPType() == "FName") {
				UNameProperty* It4 = Cast<UNameProperty>(It);
				FName n = It4->GetPropertyValue_InContainer(parent, 0);
				res = n.ToString();
			}
			else if (It->GetCPPType() == "FVector") {
				UStructProperty* It4 = Cast<UStructProperty>(It);
				FVector vectorVal = { 0.f, 0.f, 0.f };
				void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
				It4->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir2, 1);
				res = "{" + vectorVal.ToString() + "}";
				res = res.Replace(TEXT("Y"), TEXT(",Y"));
				res = res.Replace(TEXT("Z"), TEXT(",Z"));
			}
			else if (It->GetCPPType() == "FRotator") {
				UStructProperty* It4 = Cast<UStructProperty>(It);
				FRotator vectorVal = { 0.f, 0.f, 0.f };
				void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
				It4->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir2, 1);
				res = "{" + vectorVal.ToString() + "}";
				res = res.Replace(TEXT("Y"), TEXT(",Y"));
				res = res.Replace(TEXT("R"), TEXT(",R"));
			}
			else if (It->GetCPPType() == "FColor") {
				UStructProperty* It4 = Cast<UStructProperty>(It);
				FColor vectorVal = { 0, 0, 0, 0 };
				void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
				It4->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir2, 1);
				res = "{" + vectorVal.ToString() + "}";
				res = res.Replace(TEXT("G"), TEXT(",G"));
				res = res.Replace(TEXT("B"), TEXT(",B"));
				res = res.Replace(TEXT("A"), TEXT(",A"));
			}
			else if (It->GetCPPType().StartsWith("E")) {
				UEnumProperty* It5 = Cast<UEnumProperty>(It);
				uint8* enumVal = It5->ContainerPtrToValuePtr<uint8>(parent);
				//value.Split("##", &nonsS, &value);
				res = FString::FromInt((int)(*enumVal));
			}
			else if (It->GetCPPType().StartsWith("A") || It->GetCPPType().StartsWith("U") || It->GetCPPType().StartsWith("TSubclassOf")) {
				UObjectProperty* It4 = Cast<UObjectProperty>(It);
				UObject* vectorVal = It4->GetPropertyValue_InContainer(parent, 0);
				if (vectorVal) {
					m_variablesObject.Add(vectorVal);
					res = "%%" + vectorVal->GetName();
				}
				else {
					res = "null";
				}
			}
			else if (It->GetCPPType() == "TArray") {
				UArrayProperty* arrProp = Cast<UArrayProperty>(It);
				UProperty* innerProp = arrProp->Inner;
				if (innerProp->GetCPPType() == "float")
				{
					TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(parent);
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						res += FString::SanitizeFloat((*ArrayOfValues)[g]) + ",";
					}
					if (ArrayOfValues->Num() > 0) {
						res = "[" + res + FString::SanitizeFloat((*ArrayOfValues)[ArrayOfValues->Num() - 1]) + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType() == "int32")
				{
					TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(parent);
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						res += FString::FromInt((*ArrayOfValues)[g]) + ",";
					}
					if (ArrayOfValues->Num() > 0) {
						res = "[" + res + FString::FromInt((*ArrayOfValues)[ArrayOfValues->Num() - 1]) + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType() == "FString")
				{
					TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(parent);
					res = "\"";
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						res += (*ArrayOfValues)[g] + "\",\"";
					}
					if (ArrayOfValues->Num() > 0) {
						res = "[" + res + (*ArrayOfValues)[ArrayOfValues->Num() - 1] + "\"" + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType() == "bool")
				{
					TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(parent);
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						res += (*ArrayOfValues)[g] ? "true" : "false";
						res += ",";
					}
					if (ArrayOfValues->Num() > 0) {
						res = "[" + res + FString::FromInt((*ArrayOfValues)[ArrayOfValues->Num() - 1]) + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType() == "FName")
				{
					TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(parent);
					res = "\"";
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						res += (*ArrayOfValues)[g].ToString() + "\",\"";
					}
					if (ArrayOfValues->Num() > 0) {
						res = "[" + res + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "\"" + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType() == "FVector")
				{
					TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(parent);
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						FString vectorVal = "{" + (*ArrayOfValues)[g].ToString() + "}";
						vectorVal = vectorVal.Replace(TEXT("Y"), TEXT(",Y"));
						vectorVal = vectorVal.Replace(TEXT("Z"), TEXT(",Z"));
						res += vectorVal + ",";
					}
					if (ArrayOfValues->Num() > 0) {
						FString vectorVal2 = "{" + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "}";
						vectorVal2 = vectorVal2.Replace(TEXT("Y"), TEXT(",Y"));
						vectorVal2 = vectorVal2.Replace(TEXT("Z"), TEXT(",Z"));
						res = "[" + res + vectorVal2 + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType() == "FRotator")
				{
					TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(parent);
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						FString vectorVal = "{" + (*ArrayOfValues)[g].ToString() + "}";
						vectorVal = vectorVal.Replace(TEXT("Y"), TEXT(",Y"));
						vectorVal = vectorVal.Replace(TEXT("R"), TEXT(",R"));
						res += vectorVal + ",";
					}
					if (ArrayOfValues->Num() > 0) {
						FString vectorVal2 = "{" + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "}";
						vectorVal2 = vectorVal2.Replace(TEXT("Y"), TEXT(",Y"));
						vectorVal2 = vectorVal2.Replace(TEXT("R"), TEXT(",R"));
						res = "[" + res + vectorVal2 + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType() == "FColor")
				{
					TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(parent);
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						FString vectorVal = "{" + (*ArrayOfValues)[g].ToString() + "}";
						vectorVal = vectorVal.Replace(TEXT("G"), TEXT(",G"));
						vectorVal = vectorVal.Replace(TEXT("B"), TEXT(",B"));
						vectorVal = vectorVal.Replace(TEXT("A"), TEXT(",A"));
						res += vectorVal + ",";
					}
					if (ArrayOfValues->Num() > 0) {
						FString vectorVal2 = "{" + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "}";
						vectorVal2 = vectorVal2.Replace(TEXT("G"), TEXT(",G"));
						vectorVal2 = vectorVal2.Replace(TEXT("B"), TEXT(",B"));
						vectorVal2 = vectorVal2.Replace(TEXT("A"), TEXT(",A"));
						res = "[" + res + vectorVal2 + "]";
					}
					else {
						res = "[]";
					}
				}
				else if (innerProp->GetCPPType().StartsWith("A") || innerProp->GetCPPType().StartsWith("U") || innerProp->GetCPPType().StartsWith("TSubclassOf")) {
					TArray<UObject*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UObject*>>(parent);
					res = "";
					for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
						if ((*ArrayOfValues)[g]) {
							m_variablesObject.Add((*ArrayOfValues)[g]);
							res += "%%" + (*ArrayOfValues)[g]->GetName() + ",";
						}
						else {
							res += "null,";
						}
					}
					if (ArrayOfValues->Num() > 0) {
						if ((*ArrayOfValues)[ArrayOfValues->Num() - 1]) {
							m_variablesObject.Add((*ArrayOfValues)[ArrayOfValues->Num() - 1]);
							res = "[" + res + "%%" + (*ArrayOfValues)[ArrayOfValues->Num() - 1]->GetName() + "]";
						}
						else {
							res += "["+ res + "null]";
						}
					}
					else {
						res = "[]";
					}
				}
			}
			else if (It->GetCPPType().StartsWith("F")) {
				UStructProperty* It4 = Cast<UStructProperty>(It);
				void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
				if (secondary != "") {
					UProperty* It6 = FindField<UProperty>(It4->Struct, *secondary);
					if (It6) {
						if (It6->GetCPPType() == "float") {
							UFloatProperty* It5 = Cast<UFloatProperty>(It6);
							float v = It5->GetPropertyValue_InContainer(dir2, 0);
							res = FString::SanitizeFloat(v);
						}
						else if (It6->GetCPPType() == "int32") {
							UIntProperty* It5 = Cast<UIntProperty>(It6);
							int v = It5->GetPropertyValue_InContainer(dir2, 0);
							res = FString::FromInt(v);
						}
						else if (It6->GetCPPType() == "FString") {
							UStrProperty* It5 = Cast<UStrProperty>(It6);
							FString v = It5->GetPropertyValue_InContainer(dir2, 0);
							res = v;
						}
						else if (It6->GetCPPType() == "bool") {
							UBoolProperty* It5 = Cast<UBoolProperty>(It6);
							bool b = It5->GetPropertyValue_InContainer(dir2, 0);
							res = b ? "true" : "false";
						}
						else if (It6->GetCPPType() == "FName") {
							UNameProperty* It5 = Cast<UNameProperty>(It6);
							FName n = It5->GetPropertyValue_InContainer(dir2, 0);
							res = n.ToString();
						}
						else if (It6->GetCPPType() == "FVector") {
							UStructProperty* It5 = Cast<UStructProperty>(It6);
							FVector vectorVal = { 0.f, 0.f, 0.f };
							void * dir3 = static_cast<uint8_t*>(dir2) + It5->GetOffset_ForInternal();
							It5->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
							res = "{" + vectorVal.ToString() + "}";
							res = res.Replace(TEXT("Y"), TEXT(",Y"));
							res = res.Replace(TEXT("Z"), TEXT(",Z"));
						}
						else if (It6->GetCPPType() == "FRotator") {
							UStructProperty* It5 = Cast<UStructProperty>(It6);
							FRotator vectorVal = { 0.f, 0.f, 0.f };
							void * dir3 = static_cast<uint8_t*>(dir2) + It5->GetOffset_ForInternal();
							It5->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
							res = "{" + vectorVal.ToString() + "}";
							res = res.Replace(TEXT("Y"), TEXT(",Y"));
							res = res.Replace(TEXT("R"), TEXT(",R"));
						}
						else if (It6->GetCPPType() == "FColor") {
							UStructProperty* It5 = Cast<UStructProperty>(It6);
							FColor vectorVal = { 0, 0, 0, 0 };
							void * dir3 = static_cast<uint8_t*>(dir2) + It5->GetOffset_ForInternal();
							It5->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
							res = "{" + vectorVal.ToString() + "}";
							res = res.Replace(TEXT("G"), TEXT(",G"));
							res = res.Replace(TEXT("B"), TEXT(",B"));
							res = res.Replace(TEXT("A"), TEXT(",A"));
						}
						else if (It->GetCPPType().StartsWith("E")) {
							UEnumProperty* It5 = Cast<UEnumProperty>(It6);
							uint8* enumVal = It5->ContainerPtrToValuePtr<uint8>(dir2);
							//value.Split("##", &nonsS, &value);
							res = FString::FromInt((int)(*enumVal));
						}
						else if (It6->GetCPPType().StartsWith("A") || It6->GetCPPType().StartsWith("U") || It6->GetCPPType().StartsWith("TSubclassOf")) {
							UObjectProperty* It5 = Cast<UObjectProperty>(It6);
							UObject* vectorVal = It5->GetPropertyValue_InContainer(dir2, 0);
							if (vectorVal) {
								m_variablesObject.Add(vectorVal);
								res = "%%" + vectorVal->GetName();
							}
							else {
								res = "null";
							}
						}
						else if (It6->GetCPPType() == "TArray") {
							UArrayProperty* arrProp = Cast<UArrayProperty>(It6);
							UProperty* innerProp = arrProp->Inner;
							if (innerProp->GetCPPType() == "float")
							{
								TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									res += FString::SanitizeFloat((*ArrayOfValues)[g]) + ",";
								}
								if (ArrayOfValues->Num() > 0) {
									res = "[" + res + FString::SanitizeFloat((*ArrayOfValues)[ArrayOfValues->Num() - 1]) + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType() == "int32")
							{
								TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									res += FString::FromInt((*ArrayOfValues)[g]) + ",";
								}
								if (ArrayOfValues->Num() > 0) {
									res = "[" + res + FString::FromInt((*ArrayOfValues)[ArrayOfValues->Num() - 1]) + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType() == "FString")
							{
								TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
								res = "\"";
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									res += (*ArrayOfValues)[g] + "\",\"";
								}
								if (ArrayOfValues->Num() > 0) {
									res = "[" + res + (*ArrayOfValues)[ArrayOfValues->Num() - 1] + "\"" + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType() == "bool")
							{
								TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									res += (*ArrayOfValues)[g] ? "true" : "false";
									res += ",";
								}
								if (ArrayOfValues->Num() > 0) {
									res = "[" + res + FString::FromInt((*ArrayOfValues)[ArrayOfValues->Num() - 1]) + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType() == "FName")
							{
								TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
								res = "\"";
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									res += (*ArrayOfValues)[g].ToString() + "\",\"";
								}
								if (ArrayOfValues->Num() > 0) {
									res = "[" + res + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "\"" + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType() == "FVector")
							{
								TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									FString vectorVal = "{" + (*ArrayOfValues)[g].ToString() + "}";
									vectorVal = vectorVal.Replace(TEXT("Y"), TEXT(",Y"));
									vectorVal = vectorVal.Replace(TEXT("Z"), TEXT(",Z"));
									res += vectorVal + ",";
								}
								if (ArrayOfValues->Num() > 0) {
									FString vectorVal2 = "{" + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "}";
									vectorVal2 = vectorVal2.Replace(TEXT("Y"), TEXT(",Y"));
									vectorVal2 = vectorVal2.Replace(TEXT("Z"), TEXT(",Z"));
									res = "[" + res + vectorVal2 + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType() == "FRotator")
							{
								TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									FString vectorVal = "{" + (*ArrayOfValues)[g].ToString() + "}";
									vectorVal = vectorVal.Replace(TEXT("Y"), TEXT(",Y"));
									vectorVal = vectorVal.Replace(TEXT("R"), TEXT(",R"));
									res += vectorVal + ",";
								}
								if (ArrayOfValues->Num() > 0) {
									FString vectorVal2 = "{" + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "}";
									vectorVal2 = vectorVal2.Replace(TEXT("Y"), TEXT(",Y"));
									vectorVal2 = vectorVal2.Replace(TEXT("R"), TEXT(",R"));
									res = "[" + res + vectorVal2 + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType() == "FColor")
							{
								TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									FString vectorVal = "{" + (*ArrayOfValues)[g].ToString() + "}";
									vectorVal = vectorVal.Replace(TEXT("G"), TEXT(",G"));
									vectorVal = vectorVal.Replace(TEXT("B"), TEXT(",B"));
									vectorVal = vectorVal.Replace(TEXT("A"), TEXT(",A"));
									res += vectorVal + ",";
								}
								if (ArrayOfValues->Num() > 0) {
									FString vectorVal2 = "{" + (*ArrayOfValues)[ArrayOfValues->Num() - 1].ToString() + "}";
									vectorVal2 = vectorVal2.Replace(TEXT("G"), TEXT(",G"));
									vectorVal2 = vectorVal2.Replace(TEXT("B"), TEXT(",B"));
									vectorVal2 = vectorVal2.Replace(TEXT("A"), TEXT(",A"));
									res = "[" + res + vectorVal2 + "]";
								}
								else {
									res = "[]";
								}
							}
							else if (innerProp->GetCPPType().StartsWith("A") || innerProp->GetCPPType().StartsWith("U") || innerProp->GetCPPType().StartsWith("TSubclassOf")) {
								TArray<UObject*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UObject*>>(dir2);
								res = "";
								for (int g = 0; g < ArrayOfValues->Num() - 1; g++) {
									if ((*ArrayOfValues)[g]) {
										m_variablesObject.Add((*ArrayOfValues)[g]);
										res += "%%" + (*ArrayOfValues)[g]->GetName() + ",";
									}
									else {
										res += "null,";
									}
								}
								if (ArrayOfValues->Num() > 0) {
									if ((*ArrayOfValues)[ArrayOfValues->Num() - 1]) {
										m_variablesObject.Add((*ArrayOfValues)[ArrayOfValues->Num() - 1]);
										res = "[" + res + "%%" + (*ArrayOfValues)[ArrayOfValues->Num() - 1]->GetName() + "]";
									}
									else {
										res += "["+ res + "null]";
									}
								}
								else {
									res = "[]";
								}
							}
						}
					}
				}
			}
		}
	}
	if (foundV) {
		return res;
	}
	else {
		return "__NOTFOUND__";
	}
}

FString UInteractComponent::DivideString(FString str, FString* var1, FString* var2, bool equals /*= false*/)
{
	FString symbol = "";
	if (equals) {
		if (str.Split("=", var1, var2)) {
			symbol = "=";
		}
	}
	else {
		if (str.TrimStartAndEnd().Split("+", var1, var2)) {
			symbol = "+";
		}
		else if (str.TrimStartAndEnd().Split("*", var1, var2)) {
			symbol = "*";
		}
		else if (str.TrimStartAndEnd().Split("/", var1, var2)) {
			symbol = "/";
		}
		else if (str.TrimStartAndEnd().Split("%", var1, var2)) {
			symbol = "%";
		}
		else if (str.TrimStartAndEnd().Split("^", var1, var2)) {
			symbol = "^";
		}
		else if (str.TrimStartAndEnd().Split("&&", var1, var2)) {
			symbol = "&&";
		}
		else if (str.TrimStartAndEnd().Split("||", var1, var2)) {
			symbol = "||";
		}
		else if (str.TrimStartAndEnd().Split("->mod", var1, var2)) {
			symbol = "->mod";
		}
		else if (str.TrimStartAndEnd().Split("->dist->", var1, var2)) {
			symbol = "->dist->";
		}
		else if (str.TrimStartAndEnd().Split("->norm", var1, var2)) {
			symbol = "->norm";
		}
		else if (str.TrimStartAndEnd().Split("->size", var1, var2)) {
			symbol = "->size";
		}
		else if (str.TrimStartAndEnd().Split("->push->", var1, var2)) {
			symbol = "->push->";
		}
		else if (str.TrimStartAndEnd().Split("->remove->", var1, var2)) {
			symbol = "->remove->";
		}
		else if (str.TrimStartAndEnd().Split("->class", var1, var2)) {
			symbol = "->class";
		}
		else if (str.TrimStartAndEnd().Split("->name", var1, var2)) {
			symbol = "->name";
		}
		else if (str.TrimStartAndEnd().Split("->tags", var1, var2)) {
			symbol = "->tags";
		}
		else if (str.TrimStartAndEnd().Split("->sqrt", var1, var2)) {
			symbol = "->sqrt";
		}
		else if (str.TrimStartAndEnd().Split("->", var1, var2)) {
			symbol = "->";
		}
		else {
			FString nons = "";
			FString oVal = "";
			FString otherVal = "";
			if (str.Contains("{") || str.Contains("}")) {
				str.TrimStartAndEnd().Split("}", &otherVal, &oVal);
				otherVal.TrimStartAndEnd().Split("{", &nons, &otherVal);
				if (otherVal.Contains("-")) {
					symbol = DivideString(oVal, var1, var2, equals);
				}
				else if (str.TrimStartAndEnd().Split("-", var1, var2)) {
					symbol = "-";
				}
			}
			else if (str.TrimStartAndEnd().Split("-", var1, var2)) {
				symbol = "-";
			}
		}
	}
	return symbol;
}

void UInteractComponent::SetInternalVariable(AActor *actor, FSetInternalVariable& data, UInteractComponent* interactComponent)
{
	AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	if (acto) {
		void* parent = reinterpret_cast<void*>(acto);
		UClass* targetClass = acto->GetClass();
		UActorComponent* compo = UInteractComponent::GetActorComponent(interactComponent, acto, data.specificComponent, data.componentTag);
		if (compo) {
			targetClass = compo->GetClass();
			parent = reinterpret_cast<void*>(compo);
		}
		FString value = interactComponent->FindVariable(data.variableValue.TrimStartAndEnd());
		if (value == "") {
			value = interactComponent->CalculateValue(data.variableValue.TrimStartAndEnd(), false);
		}
		interactComponent->SetVariableValue(parent, data.variableName.TrimStartAndEnd(), value.TrimStartAndEnd(), targetClass);
	}
}

void UInteractComponent::ExecuteInternalFunction(AActor *actor, FExecuteInternalFunction& data, UInteractComponent* interactComponent)
{
	FString custDirVal = FString::Printf(TEXT("[%s] %s (%d)"), *FString(__FUNCTION__), *FString(__FILE__), __LINE__);
	ISM_ULOG_EDIT("%s", Warning, *custDirVal);
	UClass* specificClass = nullptr;
	UObject* specificActor = nullptr;
	UFunction* function = nullptr;
	if (data.selectActor) {
		AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
		if (acto) {
			UActorComponent* compo = UInteractComponent::GetActorComponent(interactComponent, acto, data.specificComponent, data.componentTag);
			if (compo) {
				specificActor = compo;
				specificClass = compo->GetClass();
			}
			else {
				specificActor = acto;
				specificClass = acto->GetClass();
			}
			function = specificActor->FindFunction(data.functionName);
		}
	}
	else {
		specificClass = data.specificClass;
		if (specificClass) {
			specificActor = specificClass->ClassDefaultObject;
			function = specificActor->FindFunction(data.functionName);
		}
	}
	if (function) {
		TArray<FString> parsedStrings;
		for (int i = 0; i < data.parameters.Num(); i++) {
			FString value = interactComponent->FindVariable(data.parameters[i].TrimStartAndEnd());
			if (value == "") {
				value = interactComponent->CalculateValue(data.parameters[i].TrimStartAndEnd(), false);
			}
			parsedStrings.Add(value);
		}

		if (function->NumParms < parsedStrings.Num()) {
			return;
		}

		uint8 *Buffer = (uint8*)FMemory_Alloca(function->ParmsSize);
		FMemory::Memzero(Buffer, function->ParmsSize);
		int indString = 0;
		int index = 0;
		for (TFieldIterator<UProperty> PropIt(function, EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
		{
			UProperty* Property = *PropIt;
			if (Property->HasAnyPropertyFlags(CPF_OutParm)) {
				index += Property->GetSize();
				continue;
			}
			if (Property->GetCPPType() == "float") {
				UFloatProperty* fIt2 = Cast<UFloatProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						float val = UInteractCore::GetValue(parsedStrings[indString]);
						FMemory::Memcpy(Buffer + index, &val, sizeof(float));
						index += sizeof(float);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType() == "int32") {
				UIntProperty* fIt2 = Cast<UIntProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						int val = (int)UInteractCore::GetValue(parsedStrings[indString]);
						FMemory::Memcpy(Buffer + index, &val, sizeof(int));
						index += sizeof(int);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType() == "FString") {
				UStrProperty* fIt2 = Cast<UStrProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						FMemory::Memcpy(Buffer + index, &parsedStrings[indString], sizeof(FString));
						index += sizeof(FString);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType() == "bool") {
				UBoolProperty* fIt2 = Cast<UBoolProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						bool val = parsedStrings[indString] == "true" ? true : false;
						FMemory::Memcpy(Buffer + index, &val, sizeof(bool));
						index += sizeof(bool);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType() == "FName") {
				UNameProperty* fIt2 = Cast<UNameProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						FName val = FName(*parsedStrings[indString]);
						FMemory::Memcpy(Buffer + index, &val, sizeof(FName));
						index += sizeof(FName);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType() == "FVector") {
				UStructProperty* fIt2 = Cast<UStructProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						FVector vectorVal = { 0.f, 0.f, 0.f };
						if (parsedStrings[indString].StartsWith("{") && parsedStrings[indString].EndsWith("}")) {
							parsedStrings[indString].Replace(TEXT("{"), TEXT(""));
							parsedStrings[indString].Replace(TEXT("}"), TEXT(""));
						}
						vectorVal.InitFromString(parsedStrings[indString].ToUpper());
						FMemory::Memcpy(Buffer + index, &vectorVal, sizeof(FVector));
						index += sizeof(FVector);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType() == "FRotator") {
				UStructProperty* fIt2 = Cast<UStructProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						FRotator vectorVal = { 0.f, 0.f, 0.f };
						if (parsedStrings[indString].StartsWith("{") && parsedStrings[indString].EndsWith("}")) {
							parsedStrings[indString].Replace(TEXT("{"), TEXT(""));
							parsedStrings[indString].Replace(TEXT("}"), TEXT(""));
						}
						vectorVal.InitFromString(parsedStrings[indString].ToUpper());
						FMemory::Memcpy(Buffer + index, &vectorVal, sizeof(FRotator));
						index += sizeof(FRotator);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType() == "FColor") {
				UStructProperty* fIt2 = Cast<UStructProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						FColor vectorVal = { 0, 0, 0, 0 };
						if (parsedStrings[indString].StartsWith("{") && parsedStrings[indString].EndsWith("}")) {
							parsedStrings[indString].Replace(TEXT("{"), TEXT(""));
							parsedStrings[indString].Replace(TEXT("}"), TEXT(""));
						}
						vectorVal.InitFromString(parsedStrings[indString].ToUpper());
						FMemory::Memcpy(Buffer + index, &vectorVal, sizeof(FColor));
						index += sizeof(FColor);
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType().StartsWith("U") || Property->GetCPPType().StartsWith("A") || Property->GetCPPType().StartsWith("TSubclassOf")) {
				UObjectProperty* fIt2 = Cast<UObjectProperty>(Property);
				if (fIt2) {
					if (parsedStrings.IsValidIndex(indString)) {
						FString nonsS = "";
						UObject* vectorVal = nullptr;
						parsedStrings[indString].Split("%%", &nonsS, &parsedStrings[indString]);
						for (int h = 0; h < interactComponent->m_variablesObject.Num(); h++) {
							if (interactComponent->m_variablesObject[h]->GetName() == parsedStrings[indString]) {
								vectorVal = interactComponent->m_variablesObject[h];
								break;
							}
						}
						if (vectorVal) {
							FMemory::Memcpy(Buffer + index, &vectorVal, vectorVal->GetClass()->GetStructureSize());
							index += vectorVal->GetClass()->GetStructureSize();
						}
						else {
							FMemory::Memcpy(Buffer + index, &vectorVal, sizeof(void*));
							index += sizeof(void*);
						}
					}
					indString++;
					continue;
				}
			}
			else if (Property->GetCPPType().StartsWith("E")) {
				UEnumProperty* It4 = Cast<UEnumProperty>(Property);
				if (It4) {
					if (parsedStrings.IsValidIndex(indString)) {
						int val = (int)UInteractCore::GetValue(parsedStrings[indString]);
						FMemory::Memcpy(Buffer + index, &val, It4->GetSize());
						index += It4->GetSize();
					}
					indString++;
					continue;
				}
			}

		}

		specificActor->ProcessEvent(function, Buffer);


		// 		uint8* outValueAddr = Property->ContainerPtrToValuePtr<uint8>(Buffer);
		//		bool* pReturn = (bool*)outValueAddr;

		for (TFieldIterator<UProperty> PropIt(function, EFieldIteratorFlags::ExcludeSuper); PropIt; ++PropIt)
		{
			UProperty* Property = *PropIt;
			if (Property->HasAnyPropertyFlags(CPF_OutParm)) {
				if (Property->GetCPPType() == "float") {
					UFloatProperty* fIt2 = Cast<UFloatProperty>(Property);
					if (fIt2) {
						interactComponent->ReturnValue(&data.customReturn, fIt2->GetPropertyValue(Buffer));
						continue;
					}
				}
				else if (Property->GetCPPType() == "int32") {
					UIntProperty* fIt2 = Cast<UIntProperty>(Property);
					if (fIt2) {
						interactComponent->ReturnValue(&data.customReturn, fIt2->GetPropertyValue(Buffer));
						continue;
					}
				}
				else if (Property->GetCPPType() == "FString") {
					UStrProperty* fIt2 = Cast<UStrProperty>(Property);
					if (fIt2) {
						interactComponent->ReturnValue(&data.customReturn, fIt2->GetPropertyValue(Buffer));
						continue;
					}
				}
				else if (Property->GetCPPType() == "bool") {
					UBoolProperty* fIt2 = Cast<UBoolProperty>(Property);
					if (fIt2) {
						interactComponent->ReturnValue(&data.customReturn, fIt2->GetPropertyValue(Buffer));
						continue;
					}
				}
				else if (Property->GetCPPType() == "FName") {
					UNameProperty* fIt2 = Cast<UNameProperty>(Property);
					if (fIt2) {
						interactComponent->ReturnValue(&data.customReturn, fIt2->GetPropertyValue(Buffer));
						continue;
					}
				}
				else if (Property->GetCPPType() == "FVector") {
					UStructProperty* fIt2 = Cast<UStructProperty>(Property);
					if (fIt2) {
						FVector vectorVal = { 0.f, 0.f, 0.f };
						void* dir2 = reinterpret_cast<void*>(Buffer);
						void * dir3 = static_cast<uint8_t*>(dir2) + fIt2->GetOffset_ForInternal();
						fIt2->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
						interactComponent->ReturnValue(&data.customReturn, vectorVal);
						continue;
					}
				}
				else if (Property->GetCPPType() == "FRotator") {
					UStructProperty* fIt2 = Cast<UStructProperty>(Property);
					if (fIt2) {
						FRotator vectorVal = { 0.f, 0.f, 0.f };
						void* dir2 = reinterpret_cast<void*>(Buffer);
						void * dir3 = static_cast<uint8_t*>(dir2) + fIt2->GetOffset_ForInternal();
						fIt2->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
						interactComponent->ReturnValue(&data.customReturn, vectorVal);
						continue;
					}
				}
				else if (Property->GetCPPType() == "FColor") {
					UStructProperty* fIt2 = Cast<UStructProperty>(Property);
					if (fIt2) {
						FColor vectorVal = { 0, 0, 0, 0 };
						void* dir2 = reinterpret_cast<void*>(Buffer);
						void * dir3 = static_cast<uint8_t*>(dir2) + fIt2->GetOffset_ForInternal();
						fIt2->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
						interactComponent->ReturnValue(&data.customReturn, vectorVal);
						continue;
					}
				}
				else if (Property->GetCPPType().StartsWith("U") || Property->GetCPPType().StartsWith("A") || Property->GetCPPType().StartsWith("TSubclassOf")) {
					UObjectProperty* fIt2 = Cast<UObjectProperty>(Property);
					if (fIt2) {
						interactComponent->ReturnValue(&data.customReturn, fIt2->GetPropertyValue(Buffer));
						continue;
					}
				}
				else if (Property->GetCPPType().StartsWith("E")) {
					UEnumProperty* It4 = Cast<UEnumProperty>(Property);
					if (It4) {
						uint8* enumVal = It4->ContainerPtrToValuePtr<uint8>(Buffer);
						interactComponent->ReturnValue(&data.customReturn, (int)(*enumVal));
						continue;
					}
				}
			}

		}

		CONFIRM_RETURN(interactComponent, data, FExecuteInternalFunction)
			//FMemory::Free(Buffer);

	}
}

void UInteractComponent::CallInteractDelegate(AActor* actor, EActionList function, EDelegateList delegateName, FString infoData, int stageIndex, int functionIndexV, EActionStageType stageType, EAction actionButton, FString intName, bool onlyThis, EActionListCore functionCore /*= EActionListCore::None*/, EDelegateListCore delegateNameCore /*= EDelegateListCore::None*/)
{
	EActionStageType auxTempType = tempType;
	int auxFunctIndex = functionIndexV;
	int auxCurrentStage = m_currentStage;
	EAction auxTempAction = tempAction;
	for (int i = 0; i < delegateAuxFunctions.Num(); i++) {
		UInteractComponent* interactComp = nullptr;
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), delegateAuxFunctions[i].interactCompClass, FoundActors);
		for (int g = 0; g < FoundActors.Num(); g++) {
			if (FoundActors[g]->GetName() == delegateAuxFunctions[i].interactCompActorName) {
				interactComp = Cast<UInteractComponent>(FoundActors[g]->GetComponentByClass(UInteractComponent::StaticClass()));
				break;
			}
		}
		if (interactComp) {
			if (delegateAuxFunctions[i].delegateName != EDelegateList::None && delegateAuxFunctions[i].delegateName == delegateName) {
				if (intName == "" || delegateAuxFunctions[i].interactName == "" || delegateAuxFunctions[i].interactName == intName) {
					if (!onlyThis || (onlyThis && GetOwner()->GetName() == delegateAuxFunctions[i].currentInteractName)) {
						if (delegateAuxFunctions[i].infoData == "" || delegateAuxFunctions[i].infoData == infoData) {
							if (delegateAuxFunctions[i].stageIndex == -1 || delegateAuxFunctions[i].stageIndex == stageIndex) {
								if (delegateAuxFunctions[i].functionIndex == -1 || delegateAuxFunctions[i].functionIndex == functionIndexV) {
									if (delegateAuxFunctions[i].stageType == EActionStageType::None || delegateAuxFunctions[i].stageType == stageType) {
										if (delegateAuxFunctions[i].actionButton == EAction::None || delegateAuxFunctions[i].actionButton == actionButton) {
											if ((delegateAuxFunctions[i].specificFunction != EActionList::None && delegateAuxFunctions[i].specificFunction == function)
												|| (delegateAuxFunctions[i].specificFunctionCore != EActionListCore::None && delegateAuxFunctions[i].specificFunctionCore == functionCore)
												|| (delegateAuxFunctions[i].specificFunction == EActionList::None && delegateAuxFunctions[i].specificFunctionCore == EActionListCore::None)) {
												if (interactComp->interactDataArray.IsValidIndex(delegateAuxFunctions[i].stageFunction)) {
													tempType = delegateAuxFunctions[i].stageTypeFunction;
													tempAction = delegateAuxFunctions[i].buttonTypeFunction;
													m_currentStage = delegateAuxFunctions[i].stageFunction;
													if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Pre) {
														int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
														int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction.Num();

														for (int j = startIndex; j < endIndex; j++) {
															functionIndex = j;
															EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j].enumNameCore;
															EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j].enumName;
															if (tempEnumNameCore != EActionListCore::None) {
																CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j], this);
															}
															else {
																CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j], this);
															}
														}
													}
													else if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Init) {
														int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
														int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction.Num();

														for (int j = startIndex; j < endIndex; j++) {
															functionIndex = j;
															EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j].enumNameCore;
															EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j].enumName;
															if (tempEnumNameCore != EActionListCore::None) {
																CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j], this);
															}
															else {
																CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j], this);
															}
														}
													}
													else if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Exit) {
														int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
														int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction.Num();

														for (int j = startIndex; j < endIndex; j++) {
															functionIndex = j;
															EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j].enumNameCore;
															EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j].enumName;
															if (tempEnumNameCore != EActionListCore::None) {
																CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j], this);
															}
															else {
																CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j], this);
															}
														}
													}
													else if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Interact) {
														for (int f = 0; f < interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions.Num(); f++) {
															if (interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].actionButton == delegateAuxFunctions[i].buttonTypeFunction) {
																int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
																int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction.Num();

																for (int j = startIndex; j < endIndex; j++) {
																	functionIndex = j;
																	EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j].enumNameCore;
																	EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j].enumName;
																	if (tempEnumNameCore != EActionListCore::None) {
																		CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j], this);
																	}
																	else {
																		CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j], this);
																	}
																}
																break;
															}
														}
													}

													if (!delegateAuxFunctions[i].isPermanent) {
														delegateAuxFunctions.RemoveAt(i);
														i--;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else if (delegateAuxFunctions[i].delegateNameCore != EDelegateListCore::None && delegateAuxFunctions[i].delegateNameCore == delegateNameCore) {
				if (intName == "" || delegateAuxFunctions[i].interactName == "" || delegateAuxFunctions[i].interactName == intName) {
					if (delegateAuxFunctions[i].infoData == "" || delegateAuxFunctions[i].infoData == infoData) {
						if (!onlyThis || (onlyThis && GetOwner()->GetName() == delegateAuxFunctions[i].currentInteractName)) {
							if (delegateAuxFunctions[i].stageIndex == -1 || delegateAuxFunctions[i].stageIndex == stageIndex) {
								if (delegateAuxFunctions[i].functionIndex == -1 || delegateAuxFunctions[i].functionIndex == functionIndexV) {
									if (delegateAuxFunctions[i].stageType == EActionStageType::None || delegateAuxFunctions[i].stageType == stageType) {
										if (delegateAuxFunctions[i].actionButton == EAction::None || delegateAuxFunctions[i].actionButton == actionButton) {
											if ((delegateAuxFunctions[i].specificFunction != EActionList::None && delegateAuxFunctions[i].specificFunction == function)
												|| (delegateAuxFunctions[i].specificFunctionCore != EActionListCore::None && delegateAuxFunctions[i].specificFunctionCore == functionCore)
												|| (delegateAuxFunctions[i].specificFunction == EActionList::None && delegateAuxFunctions[i].specificFunctionCore == EActionListCore::None)) {
												if (interactComp->interactDataArray.IsValidIndex(delegateAuxFunctions[i].stageFunction)) {
													tempType = delegateAuxFunctions[i].stageTypeFunction;
													tempAction = delegateAuxFunctions[i].buttonTypeFunction;
													m_currentStage = delegateAuxFunctions[i].stageFunction;
													if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Pre) {
														int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
														int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction.Num();

														for (int j = startIndex; j < endIndex; j++) {
															functionIndex = j;
															EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j].enumNameCore;
															EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j].enumName;
															if (tempEnumNameCore != EActionListCore::None) {
																CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j], this);
															}
															else {
																CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].preStageFunction[j], this);
															}
														}
													}
													else if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Init) {
														int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
														int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction.Num();

														for (int j = startIndex; j < endIndex; j++) {
															functionIndex = j;
															EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j].enumNameCore;
															EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j].enumName;
															if (tempEnumNameCore != EActionListCore::None) {
																CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j], this);
															}
															else {
																CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.initFunction[j], this);
															}
														}
													}
													else if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Exit) {
														int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
														int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction.Num();

														for (int j = startIndex; j < endIndex; j++) {
															functionIndex = j;
															EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j].enumNameCore;
															EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j].enumName;
															if (tempEnumNameCore != EActionListCore::None) {
																CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j], this);
															}
															else {
																CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].capsuleTriggerStruct.exitFunction[j], this);
															}
														}
													}
													else if (delegateAuxFunctions[i].stageTypeFunction == EActionStageType::Interact) {
														for (int f = 0; f < interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions.Num(); f++) {
															if (interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].actionButton == delegateAuxFunctions[i].buttonTypeFunction) {
																int startIndex = delegateAuxFunctions[i].startFunctionIndex == -1 ? 0 : delegateAuxFunctions[i].startFunctionIndex;
																int endIndex = delegateAuxFunctions[i].endFunctionIndex != -1 && interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction.IsValidIndex(delegateAuxFunctions[i].endFunctionIndex + 1) ? delegateAuxFunctions[i].endFunctionIndex + 1 : interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction.Num();

																for (int j = startIndex; j < endIndex; j++) {
																	functionIndex = j;
																	EActionListCore tempEnumNameCore = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j].enumNameCore;
																	EActionList tempEnumName = interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j].enumName;
																	if (tempEnumNameCore != EActionListCore::None) {
																		CallDelegateCoreFunction(tempEnumNameCore, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j], this);
																	}
																	else {
																		CallDelegateFunction(tempEnumName, actor, interactComp->interactDataArray[delegateAuxFunctions[i].stageFunction].actions[f].stageFunction[j], this);
																	}
																}
																break;
															}
														}
													}

													if (!delegateAuxFunctions[i].isPermanent) {
														delegateAuxFunctions.RemoveAt(i);
														i--;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	tempType = auxTempType;
	functionIndex = auxFunctIndex;
	m_currentStage = auxCurrentStage;
	tempAction = auxTempAction;
}

void UInteractComponent::CallExternalDelegateFunction(AActor *actor, FCallExternalDelegateFunction& data, UInteractComponent* interactComponent)
{
	AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	if (acto) {
		UInteractComponent* interactComp = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
		if (interactComp) {
			interactComp->CallInteractDelegate(actor, EActionList::None, data.delegateName, data.specificData, interactComp->m_currentStage, interactComp->functionIndex, interactComp->tempType, interactComp->tempAction, interactComponent->GetOwner()->GetName(), true, EActionListCore::callExternalDelegateFunction, data.delegateNameCore);
		}
	}
	else {
		interactComponent->CallInteractDelegate(actor, EActionList::None, data.delegateName, data.specificData, interactComponent->m_currentStage, interactComponent->functionIndex, interactComponent->tempType, interactComponent->tempAction, interactComponent->GetOwner()->GetName(), false, EActionListCore::callExternalDelegateFunction, data.delegateNameCore);
	}
}

void UInteractComponent::LoadStreamLevelInteract(AActor *actor, FLoadStreamLevelInteract& data, UInteractComponent* interactComponent)
{
	datasStreamLoad.Empty();
	actorsStreamLoad.Empty();
	for (TObjectIterator<AActor> acto; acto; ++acto)
	{
		if (ISM_ISVALID(acto) && ISM_ISVALID(acto->GetWorld())) {
			FString levelName = "";
			ULevel* level2 = acto->GetLevel();
			if (level2) {
				levelName = acto->GetLevel()->GetOuter() ? acto->GetLevel()->GetOuter()->GetName() : acto->GetLevel()->GetName();
				if (levelName == acto->GetWorld()->GetName()) {
					levelName = acto->GetLevel()->GetName();
				}
			}
			if (levelName == data.levelName.ToString()) {
				UInteractComponent* intComp = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
				if (intComp) {
					intComp->ImportInteractDataInternal();
					datasStreamLoad.Add(intComp->interactDataArray);
					actorsStreamLoad.Add(acto->GetName());
					intComp->interactDataArray.Empty();
				}
			}
		}
	}
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = interactComponent;
	LatentActionInfo.ExecutionFunction = "ResetBPInteract";
	LatentActionInfo.UUID = 123;
	LatentActionInfo.Linkage = 1;
	UGameplayStatics::LoadStreamLevel(interactComponent, data.levelName, true, true, LatentActionInfo);
}

void UInteractComponent::ResetBPInteract() {
	for (TObjectIterator<AActor> acto2; acto2; ++acto2)
	{
		if (ISM_ISVALID(acto2) && ISM_ISVALID(acto2->GetWorld())) {
			FString levelName = "";
			ULevel* level2 = acto2->GetLevel();
			if (level2) {
				levelName = acto2->GetLevel()->GetOuter() ? acto2->GetLevel()->GetOuter()->GetName() : acto2->GetLevel()->GetName();
				if (levelName == acto2->GetWorld()->GetName()) {
					levelName = acto2->GetLevel()->GetName();
				}
			}
			UInteractComponent* intComp = Cast<UInteractComponent>(acto2->GetComponentByClass(UInteractComponent::StaticClass()));
			if (intComp) {
				for (int g = 0; g < actorsStreamLoad.Num(); g++) {
					if (actorsStreamLoad[g] == acto2->GetName()) {
						intComp->interactDataArray = datasStreamLoad[g];
//						intComp->ImportInteractDataInternal();
//						intComp->InitializeTooltip();
//						intComp->StartInteract();
					}
				}
			}
		}
	}
}

void UInteractComponent::UnloadStreamLevelInteract(AActor *actor, FUnloadStreamLevelInteract& data, UInteractComponent* interactComponent)
{
	for (TObjectIterator<AActor> acto; acto; ++acto)
	{
		if (ISM_ISVALID(acto) && ISM_ISVALID(acto->GetWorld())) {
			FString levelName = "";
			ULevel* level2 = acto->GetLevel();
			if (level2) {
				levelName = acto->GetLevel()->GetOuter() ? acto->GetLevel()->GetOuter()->GetName() : acto->GetLevel()->GetName();
				if (levelName == acto->GetWorld()->GetName()) {
					levelName = acto->GetLevel()->GetName();
				}
			}
			if (levelName == data.levelName.ToString()) {
				UInteractComponent* intComp = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
				if (intComp) {
					UInteractComponent::JumpStageGlobal(*acto, "", 0.f, -1, true);
				}
			}
		}
	}
	UGameplayStatics::UnloadStreamLevel(interactComponent, data.levelName, FLatentActionInfo(), true);
}

void UInteractComponent::WhenFunction(AActor *actor, FWhenFunction& data, UInteractComponent* interactComponent)
{
	FString interactName = "";
	if (!data.executeAlways) {
		AActor* currentActo = UInteractComponent::GetCurrentActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
		if (currentActo) {
			interactName = currentActo->GetName();
		}
	}
	UInteractComponent* interactComp = interactComponent;
	AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	if (!acto) {
		acto = interactComponent->GetOwner();
	}

	EActionListCore specificFunctionCore = data.specificFunctionCore;

	EActionList specificFunction = data.specificFunction;

	int stageIndex = data.stageIndex;

	int functionIndex = data.functionIndex;

	EActionStageType stageType = data.stageType;

	EAction buttonType = data.buttonType;


	if (!data.specifyData) {
		specificFunctionCore = EActionListCore::None;

		specificFunction = EActionList::None;

		stageIndex = -1;

		functionIndex = -1;

		stageType = EActionStageType::None;

		buttonType = EAction::None;
	}

	int startFunctionIndex = data.startFunctionIndex;

	int endFunctionIndex = data.endFunctionIndex;

	EActionStageType stageTypeFunction = data.stageTypeFunction;

	EAction buttonTypeFunction = data.buttonTypeFunction;

	if (!data.specifyFunction) {
		startFunctionIndex = -1;

		endFunctionIndex = -1;

		stageTypeFunction = EActionStageType::Pre;

		buttonTypeFunction = EAction::None;
	}

	FDelegateInteractData delegateData = { data.delegateName, data.delegateNameCore, data.specificData, stageIndex, functionIndex, stageType, buttonType, specificFunction, specificFunctionCore, data.stageFunction, data.isPermanent, interactComponent->m_currentStage, interactComponent->functionIndex, interactComponent->tempType, interactComponent->tempAction, startFunctionIndex, endFunctionIndex, stageTypeFunction, buttonTypeFunction, interactName, acto->GetClass(), acto->GetName(), interactComponent->GetOwner()->GetName() };
	for (int i = 0; i < delegateAuxFunctions.Num(); i++) {
		if (delegateAuxFunctions[i].currentStageIndex == interactComponent->m_currentStage && delegateAuxFunctions[i].currentFunctionIndex == interactComponent->functionIndex && delegateAuxFunctions[i].currentStageType == interactComponent->tempType && delegateAuxFunctions[i].currentActionButton == interactComponent->tempAction && delegateAuxFunctions[i].interactName == interactName && delegateAuxFunctions[i].interactCompClass->GetName() == acto->GetClass()->GetName() && delegateAuxFunctions[i].interactCompActorName == acto->GetName() && delegateAuxFunctions[i].currentInteractName == interactComponent->GetOwner()->GetName()) {
			return;
		}
	}
	delegateAuxFunctions.Add(delegateData);
}

void UInteractComponent::GetInternalVariable(AActor *actor, FGetInternalVariable& data, UInteractComponent* interactComponent)
{
	if (data.selectActor) {
		AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
		if (acto) {
			void* parent = reinterpret_cast<void*>(acto);
			UClass* targetClass = acto->GetClass();
			UActorComponent* compo = UInteractComponent::GetActorComponent(interactComponent, acto, data.specificComponent, data.componentTag);
			if (compo) {
				targetClass = compo->GetClass();
				parent = reinterpret_cast<void*>(compo);
			}
			FString varRes = interactComponent->FindLocalVariable(parent, targetClass, data.variableName.TrimStartAndEnd());
			if (varRes != "__NOTFOUND__") {
				interactComponent->ReturnValue(&data.customReturn, varRes, false);
				CONFIRM_RETURN(interactComponent, data, FGetInternalVariable)
			}
		}
	}
	else {
		if (data.specificClass) {
			void* parent = reinterpret_cast<void*>(data.specificClass->ClassDefaultObject);
			UClass* targetClass = data.specificClass;

			FString varRes = interactComponent->FindLocalVariable(parent, targetClass, data.variableName.TrimStartAndEnd());
			if (varRes != "__NOTFOUND__") {
				interactComponent->ReturnValue(&data.customReturn, varRes, false);
				CONFIRM_RETURN(interactComponent, data, FGetInternalVariable)
			}
		}
	}
}

void UInteractComponent::GetExternalVariable(AActor* actor, FGetExternalVariable& data, UInteractComponent* interactComponent)
{
	UInteractComponent* intComp = nullptr;
	AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	if (acto) {
		intComp = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
	}
	if (intComp) {
		FString value = intComp->FindVariable(data.variableNameExternal);
		TArray<FBaseCreateEditVariable> variables;
		FBaseCreateEditVariable var = { false, data.variableNameInternal, value };
		variables.Add(var);
		FCreateEditVariable dat = { false, {}, variables };
		UInteractComponent::CreateEditVariable(actor, dat, interactComponent);
	}
}

void UInteractComponent::CreateEditVariable(AActor *actor, FCreateEditVariable& data, UInteractComponent* interactComponent)
{
	UInteractComponent* intComp = nullptr;
	if (data.doOnExternalActor) {
		AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
		if (acto) {
			intComp = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
		}
	}
	if (!intComp) {
		intComp = interactComponent;
	}
	for (int j = 0; j < data.variables.Num(); j++) {
		int iVar = -1;
		for (int i = 0; i < intComp->m_sharedVariablesArray.Num(); i++) {
			if (intComp->m_sharedVariablesArray[i].Contains(data.variables[j].variableName.TrimStartAndEnd() + "=")) {
				iVar = i;
				break;
			}
		}
		FString val = data.variables[j].variableValue.TrimStartAndEnd();
		FString part1 = "";
		FString part2 = "";
		FString symbol = interactComponent->DivideString(val, &part1, &part2);
		FString secondSymbol = "";
		FString noneS = "";
		if (symbol != "") {
			if (part1.TrimStartAndEnd().StartsWith("_") || part1.TrimStartAndEnd().StartsWith("!_")) {
				if (part1.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					part1.Split("!", &noneS, &part1);
				}
				else {
					secondSymbol = "";
				}
				part1 = interactComponent->FindVariable(part1.TrimStartAndEnd());
			}
			if (secondSymbol != "") {
				val = secondSymbol + part1;
				secondSymbol = "";
			}
			else {
				val = interactComponent->CalculateValue(part1.TrimStartAndEnd(), true);
			}
		}
		else {
			if (val.TrimStartAndEnd().StartsWith("_") || val.TrimStartAndEnd().StartsWith("!_")) {
				if (val.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					val.Split("!", &noneS, &val);
				}
				else {
					secondSymbol = "";
				}
				val = interactComponent->FindVariable(val.TrimStartAndEnd());
			}
			if (secondSymbol != "") {
				val = secondSymbol + interactComponent->CalculateValue(val.TrimStartAndEnd(), true);
				secondSymbol = "";
			}
			val = interactComponent->CalculateValue(val.TrimStartAndEnd(), true);
		}
		while (symbol != "") {
			FString prov = part2;
			if (prov.TrimStartAndEnd().StartsWith("_") || prov.TrimStartAndEnd().StartsWith("!_")) {
				if (prov.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					prov.Split("!", &noneS, &prov);
				}
				prov = interactComponent->FindVariable(prov);
			}
			val = val + symbol;
			symbol = interactComponent->DivideString(part2, &part1, &part2);
			if (part1.TrimStartAndEnd().StartsWith("_") || part1.TrimStartAndEnd().StartsWith("!_")) {
				if (part1.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					part1.Split("!", &noneS, &part1);
				}
				part1 = interactComponent->FindVariable(part1);
			}
			if (symbol != "") {
				if (secondSymbol != "") {
					val = val + secondSymbol + interactComponent->CalculateValue(part1.TrimStartAndEnd(), true);
					secondSymbol = "";
				}
				else {
					val = val + interactComponent->CalculateValue(part1.TrimStartAndEnd(), true);
				}
			}
			else {
				if (secondSymbol != "") {
					val = val + secondSymbol + interactComponent->CalculateValue(prov.TrimStartAndEnd(), true);
					secondSymbol = "";
				}
				else {
					val = val + interactComponent->CalculateValue(prov.TrimStartAndEnd(), true);
				}
			}

			val = interactComponent->CalculateValue(val.TrimStartAndEnd(), true);
		}

		if (val != "") {
			FString nonsS = "";
			FString valS = "";
			FString nonsS2 = "";
			if (data.variables[j].destroy && iVar != -1) {
				intComp->m_sharedVariablesArray[iVar].Split("=", &nonsS, &valS);
				if (valS.TrimStartAndEnd().StartsWith("%%")) {
					valS.Split("%%", &nonsS2, &valS);
					for (int x = 0; x < intComp->m_variablesObject.Num(); x++) {
						if (valS == intComp->m_variablesObject[x]->GetName()) {
							intComp->m_variablesObject.RemoveAt(x);
							break;
						}
					}
				}
				else if (valS.TrimStartAndEnd().StartsWith("[")) {
					valS.Replace(TEXT("["), TEXT(""));
					valS.Replace(TEXT("]"), TEXT(""));
					while (valS.Split(",", &nonsS, &valS)) {
						if (nonsS.TrimStartAndEnd().StartsWith("%%")) {
							nonsS.Split("%%", &nonsS2, &nonsS);
							for (int x = 0; x < intComp->m_variablesObject.Num(); x++) {
								if (nonsS.TrimStartAndEnd() == intComp->m_variablesObject[x]->GetName()) {
									intComp->m_variablesObject.RemoveAt(x);
									break;
								}
							}
						}
					}
					if (valS.TrimStartAndEnd().StartsWith("%%")) {
						valS.Split("%%", &nonsS2, &valS);
						for (int x = 0; x < intComp->m_variablesObject.Num(); x++) {
							if (valS.TrimStartAndEnd() == intComp->m_variablesObject[x]->GetName()) {
								intComp->m_variablesObject.RemoveAt(x);
								break;
							}
						}
					}
				}
				intComp->m_sharedVariablesArray.RemoveAt(iVar);
			}
			else if (iVar != -1) {
				intComp->m_sharedVariablesArray[iVar].Split("=", &nonsS, &valS);
				if (valS.TrimStartAndEnd().StartsWith("%%")) {
					valS.Split("%%", &nonsS2, &valS);
					for (int x = 0; x < intComp->m_variablesObject.Num(); x++) {
						if (valS.TrimStartAndEnd() == intComp->m_variablesObject[x]->GetName()) {
							intComp->m_variablesObject.RemoveAt(x);
							break;
						}
					}
				}
				else if (valS.TrimStartAndEnd().StartsWith("[")) {
					valS.Replace(TEXT("["), TEXT(""));
					valS.Replace(TEXT("]"), TEXT(""));
					while (valS.Split(",", &nonsS, &valS)) {
						if (nonsS.TrimStartAndEnd().StartsWith("%%")) {
							nonsS.Split("%%", &nonsS2, &nonsS);
							for (int x = 0; x < intComp->m_variablesObject.Num(); x++) {
								if (nonsS.TrimStartAndEnd() == intComp->m_variablesObject[x]->GetName()) {
									intComp->m_variablesObject.RemoveAt(x);
									break;
								}
							}
						}
					}
					if (valS.TrimStartAndEnd().StartsWith("%%")) {
						valS.Split("%%", &nonsS2, &valS);
						for (int x = 0; x < intComp->m_variablesObject.Num(); x++) {
							if (valS.TrimStartAndEnd() == intComp->m_variablesObject[x]->GetName()) {
								intComp->m_variablesObject.RemoveAt(x);
								break;
							}
						}
					}
				}
				intComp->m_sharedVariablesArray[iVar] = data.variables[j].variableName.TrimStartAndEnd() + "=" + val;
			}
			else {
				intComp->m_sharedVariablesArray.Push(data.variables[j].variableName.TrimStartAndEnd() + "=" + val);
			}
		}
	}
}

void UInteractComponent::UpdateVariables(void* parent, FString variables, UScriptStruct* structClass, UInteractComponent* interactComponent)
{
	FString continueString = variables;
	FString variableString = "";
	while (continueString.Split(";", &variableString, &continueString)) {
		FString varName = "";
		FString varValue = "";
		variableString = variableString.TrimStartAndEnd();
		interactComponent->DivideString(variableString, &varName, &varValue, true);
		FString part1 = "";
		FString part2 = "";
		FString symbol = interactComponent->DivideString(varValue, &part1, &part2);
		FString secondSymbol = "";
		FString noneS = "";
		bool prub = false;
		if (varName.TrimStartAndEnd().StartsWith("_")) {
			prub = true;
		}
		if (symbol != "") {
			if (part1.TrimStartAndEnd().StartsWith("_") || part1.TrimStartAndEnd().StartsWith("!_")) {
				if (part1.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					part1.Split("!", &noneS, &part1);
				}
				else {
					secondSymbol = "";
				}
				part1 = interactComponent->FindVariable(part1);
				if (!prub) {
					FString otherPart1S = "";
					FString otherPart2S = part1;
					while (otherPart2S.Split("\"", &otherPart1S, &otherPart2S)) {
						otherPart2S = otherPart1S + otherPart2S;
						otherPart1S = "";
					}
					part1 = otherPart1S + otherPart2S;
				}
			}
			else if (part1.TrimStartAndEnd().StartsWith("$")) {
				part1 = interactComponent->FindLocalVariable(parent, structClass, part1);
			}
			if (secondSymbol != "") {
				varValue = secondSymbol + interactComponent->CalculateValue(part1.TrimStartAndEnd(), true);
				secondSymbol = "";
			}
			else {
				varValue = interactComponent->CalculateValue(part1.TrimStartAndEnd(), true);
			}
		}
		else {
			if (varValue.TrimStartAndEnd().StartsWith("_") || varValue.TrimStartAndEnd().StartsWith("!_")) {
				if (varValue.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					varValue.Split("!", &noneS, &varValue);
				}
				else {
					secondSymbol = "";
				}
				varValue = interactComponent->FindVariable(varValue.TrimStartAndEnd());
				if (!prub) {
					FString otherPart1S = "";
					FString otherPart2S = varValue;
					while (otherPart2S.Split("\"", &otherPart1S, &otherPart2S)) {
						otherPart2S = otherPart1S + otherPart2S;
						otherPart1S = "";
					}
					varValue = otherPart1S + otherPart2S;
				}
			}
			else if (varValue.TrimStartAndEnd().StartsWith("$")) {
				varValue = interactComponent->FindLocalVariable(parent, structClass, varValue.TrimStartAndEnd());
			}
			if (secondSymbol != "") {
				varValue = secondSymbol + interactComponent->CalculateValue(varValue.TrimStartAndEnd(), true);
				secondSymbol = "";
			}
			varValue = interactComponent->CalculateValue(varValue.TrimStartAndEnd(), prub);
		}
		while (symbol != "") {
			FString prov = part2;
			if (prov.TrimStartAndEnd().StartsWith("_") || prov.TrimStartAndEnd().StartsWith("!_")) {
				if (prov.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					prov.Split("!", &noneS, &prov);
				}
				prov = interactComponent->FindVariable(prov);
				if (!prub) {
					FString otherPart1S = "";
					FString otherPart2S = prov;
					while (otherPart2S.Split("\"", &otherPart1S, &otherPart2S)) {
						otherPart2S = otherPart1S + otherPart2S;
						otherPart1S = "";
					}
					prov = otherPart1S + otherPart2S;
				}
			}
			else if (prov.TrimStartAndEnd().StartsWith("$")) {
				prov = interactComponent->FindLocalVariable(parent, structClass, prov);
			}
			varValue = varValue + symbol;
			symbol = interactComponent->DivideString(part2, &part1, &part2);
			if (part1.TrimStartAndEnd().StartsWith("_") || part1.TrimStartAndEnd().StartsWith("!_")) {
				if (part1.TrimStartAndEnd().StartsWith("!")) {
					secondSymbol = "!";
					part1.Split("!", &noneS, &part1);
				}
				part1 = interactComponent->FindVariable(part1);
				if (!prub) {
					FString otherPart1S = "";
					FString otherPart2S = part1;
					while (otherPart2S.Split("\"", &otherPart1S, &otherPart2S)) {
						otherPart2S = otherPart1S + otherPart2S;
						otherPart1S = "";
					}
					part1 = otherPart1S + otherPart2S;
				}
			}
			else if (part1.TrimStartAndEnd().StartsWith("$")) {
				part1 = interactComponent->FindLocalVariable(parent, structClass, part1);
			}
			if (symbol != "") {
				if (secondSymbol != "") {
					varValue = varValue + secondSymbol + interactComponent->CalculateValue(part1.TrimStartAndEnd(), true);
					secondSymbol = "";
				}
				else {
					varValue = varValue + interactComponent->CalculateValue(part1.TrimStartAndEnd(), true);
				}
			}
			else {
				if (secondSymbol != "") {
					varValue = varValue + secondSymbol + interactComponent->CalculateValue(prov.TrimStartAndEnd(), true);
					secondSymbol = "";
				}
				else {
					varValue = varValue + interactComponent->CalculateValue(prov.TrimStartAndEnd(), true);
				}
			}

			varValue = interactComponent->CalculateValue(varValue.TrimStartAndEnd(), prub);
		}
		if (varValue != "") {
			if (varName.TrimStartAndEnd().StartsWith("$")) {
				FString noneS1 = "";
				varName.Split("$", &noneS1, &varName);
				void* strct = nullptr;

				interactComponent->SetVariableValue(parent, varName.TrimStartAndEnd(), varValue.TrimStartAndEnd(), structClass, strct);
			}
			else if (varName.TrimStartAndEnd().StartsWith("_")) {
				FString noneS1 = "";
				varName.Split("_", &noneS1, &varName);
				interactComponent->UpdateSharedVariable(varName.TrimStartAndEnd(), varValue.TrimStartAndEnd());
			}
		}
	}
}



void UInteractComponent::UpdateVariables(UPARAM(ref) FCustom& parent, FString variables, FCustom structClass)
{
	UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&parent), variables, structClass.StaticStruct(), this);
}

void UInteractComponent::SetVariableValue(void* parent, FString property, FString value, UStruct* structClass, void* structValue)
{
	FString nonsS = "";
	FString name = property;
	FString secondary = "";
	if (property.Contains("::")) {
		property.Split("::", &name, &secondary);
	}
	else if (property.Contains("->")) {
		property.Split("->", &name, &secondary);
		secondary = CalculateValue(secondary.TrimStartAndEnd(), false);
	}
	UProperty* It = FindField<UProperty>(structClass, *name);

	if (It) {
		if (It->GetCPPType() == "float") {
			UFloatProperty* It4 = Cast<UFloatProperty>(It);
			It4->SetPropertyValue_InContainer(parent, UInteractCore::GetValue(value), 0);
		}
		else if (It->GetCPPType() == "int32") {
			UIntProperty* It4 = Cast<UIntProperty>(It);
			It4->SetPropertyValue_InContainer(parent, (int)UInteractCore::GetValue(value), 0);
		}
		else if (It->GetCPPType() == "FString") {
			UStrProperty* It4 = Cast<UStrProperty>(It);
			It4->SetPropertyValue_InContainer(parent, value, 0);
		}
		else if (It->GetCPPType() == "bool") {
			UBoolProperty* It4 = Cast<UBoolProperty>(It);
			It4->SetPropertyValue_InContainer(parent, value == "true" ? true : false, 0);
		}
		else if (It->GetCPPType() == "FName") {
			UNameProperty* It4 = Cast<UNameProperty>(It);
			It4->SetPropertyValue_InContainer(parent, FName(*value), 0);
		}
		else if (It->GetCPPType() == "FVector") {
			UStructProperty* It4 = Cast<UStructProperty>(It);
			FVector vectorVal = { 0.f, 0.f, 0.f };
			if (value.StartsWith("{") && value.EndsWith("}")) {
				value.Replace(TEXT("{"), TEXT(""));
				value.Replace(TEXT("}"), TEXT(""));
			}
			vectorVal.InitFromString(value.ToUpper());
			void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
			It4->CopyValuesInternal(dir2, reinterpret_cast<void*>(&vectorVal), 1);
		}
		else if (It->GetCPPType() == "FRotator") {
			UStructProperty* It4 = Cast<UStructProperty>(It);
			FRotator vectorVal = { 0.f, 0.f, 0.f };
			if (value.StartsWith("{") && value.EndsWith("}")) {
				value.Replace(TEXT("{"), TEXT(""));
				value.Replace(TEXT("}"), TEXT(""));
			}
			vectorVal.InitFromString(value.ToUpper());
			void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
			It4->CopyValuesInternal(dir2, reinterpret_cast<void*>(&vectorVal), 1);
		}
		else if (It->GetCPPType() == "FColor") {
			UStructProperty* It4 = Cast<UStructProperty>(It);
			FColor vectorVal = { 0, 0, 0, 0 };
			if (value.StartsWith("{") && value.EndsWith("}")) {
				value.Replace(TEXT("{"), TEXT(""));
				value.Replace(TEXT("}"), TEXT(""));
			}
			vectorVal.InitFromString(value.ToUpper());
			void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
			It4->CopyValuesInternal(dir2, reinterpret_cast<void*>(&vectorVal), 1);
		}
		else if (It->GetCPPType().StartsWith("A") || It->GetCPPType().StartsWith("U") || It->GetCPPType().StartsWith("TSubclassOf")) {
			UObjectProperty* It4 = Cast<UObjectProperty>(It);
			UObject* vectorVal = nullptr;
			value.Split("%%", &nonsS, &value);
			for (int h = 0; h < m_variablesObject.Num(); h++) {
				if (m_variablesObject[h]->GetName() == value) {
					vectorVal = m_variablesObject[h];
					break;
				}
			}
			It4->SetPropertyValue_InContainer(parent, vectorVal, 0);
		}
		else if (It->GetCPPType().StartsWith("E")) {
			UEnumProperty* It4 = Cast<UEnumProperty>(It);
			uint8* enumVal = It4->ContainerPtrToValuePtr<uint8>(parent);
			//value.Split("##", &nonsS, &value);
			*enumVal = (int)UInteractCore::GetValue(value);
		}
		else if (It->GetCPPType() == "TArray") {
			UArrayProperty* arrProp = Cast<UArrayProperty>(It);
			UProperty* innerProp = arrProp->Inner;
			FString continueString = "";
			FString variableString = "";
			FString noneS = "";
			if (innerProp->GetCPPType() == "float")
			{
				TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<float> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					while (continueString.Split(",", &variableString, &continueString)) {
						newArray.Add(UInteractCore::GetValue(CalculateValue(variableString, this)));
					}
					newArray.Add(UInteractCore::GetValue(CalculateValue(continueString, this)));
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						(*ArrayOfValues)[ind] = UInteractCore::GetValue(CalculateValue(value, this));
					}
				}
			}
			else if (innerProp->GetCPPType() == "int32")
			{
				TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<int> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					while (continueString.Split(",", &variableString, &continueString)) {
						newArray.Add((int)UInteractCore::GetValue(CalculateValue(variableString, this)));
					}
					newArray.Add((int)UInteractCore::GetValue(CalculateValue(continueString, this)));
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						(*ArrayOfValues)[ind] = (int)UInteractCore::GetValue(CalculateValue(value, this));
					}
				}
			}
			else if (innerProp->GetCPPType() == "FString")
			{
				TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<FString> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					while (continueString.Split("\",", &variableString, &continueString)) {
						newArray.Add(CalculateValue(variableString + "\"", this));
					}
					newArray.Add(CalculateValue(continueString, this));
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						(*ArrayOfValues)[ind] = CalculateValue(value, this);
					}
				}
			}
			else if (innerProp->GetCPPType() == "bool")
			{
				TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<bool> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					while (continueString.Split(",", &variableString, &continueString)) {
						newArray.Add(CalculateValue(variableString, this) == "true" ? true : false);
					}
					newArray.Add(CalculateValue(continueString, this) == "true" ? true : false);
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						(*ArrayOfValues)[ind] = CalculateValue(value, this) == "true" ? true : false;
					}
				}
			}
			else if (innerProp->GetCPPType() == "FName")
			{
				TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<FName> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					while (continueString.Split("\",", &variableString, &continueString)) {
						newArray.Add(FName(*CalculateValue(variableString + "\"", this)));
					}
					newArray.Add(FName(*CalculateValue(continueString, this)));
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						(*ArrayOfValues)[ind] = FName(*CalculateValue(value, this));
					}
				}
			}
			else if (innerProp->GetCPPType() == "FVector")
			{
				TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<FVector> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					FVector vectr = { 0.f, 0.f, 0.f };
					while (continueString.Split("},", &variableString, &continueString)) {
						vectr.InitFromString(CalculateValue(variableString + "}", this));
						newArray.Add(vectr);
					}
					vectr.InitFromString(CalculateValue(continueString, this));
					newArray.Add(vectr);
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						FVector vectr = { 0.f, 0.f, 0.f };
						vectr.InitFromString(CalculateValue(value, this));
						(*ArrayOfValues)[ind] = vectr;
					}
				}
			}
			else if (innerProp->GetCPPType() == "FRotator")
			{
				TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<FRotator> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					FRotator vectr = { 0.f, 0.f, 0.f };
					while (continueString.Split("},", &variableString, &continueString)) {
						vectr.InitFromString(CalculateValue(variableString + "}", this));
						newArray.Add(vectr);
					}
					vectr.InitFromString(CalculateValue(continueString, this));
					newArray.Add(vectr);
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						FRotator vectr = { 0.f, 0.f, 0.f };
						vectr.InitFromString(CalculateValue(value, this));
						(*ArrayOfValues)[ind] = vectr;
					}
				}
			}
			else if (innerProp->GetCPPType() == "FColor")
			{
				TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<FColor> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					FColor vectr = { 0, 0, 0, 0 };
					while (continueString.Split("},", &variableString, &continueString)) {
						vectr.InitFromString(CalculateValue(variableString + "}", this));
						newArray.Add(vectr);
					}
					vectr.InitFromString(CalculateValue(continueString, this));
					newArray.Add(vectr);
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						FColor vectr = { 0, 0, 0, 0 };
						vectr.InitFromString(CalculateValue(value, this));
						(*ArrayOfValues)[ind] = vectr;
					}
				}
			}
			else if (innerProp->GetCPPType().StartsWith("A") || innerProp->GetCPPType().StartsWith("U") || innerProp->GetCPPType().StartsWith("TSubclassOf")) {
				TArray<UObject*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UObject*>>(parent);
				if (secondary == "") {
					continueString = value;
					TArray<UObject*> newArray;
					continueString.Split("[", &noneS, &continueString);
					continueString.Split("]", &continueString, &noneS);
					while (continueString.Split(",", &variableString, &continueString)) {
						variableString.Split("%%", &nonsS, &variableString);
						variableString = CalculateValue(variableString, this);
						for (int h = 0; h < m_variablesObject.Num(); h++) {
							if (m_variablesObject[h]->GetName() == variableString) {
								newArray.Add(m_variablesObject[h]);
								break;
							}
						}
					}
					continueString.Split("%%", &nonsS, &continueString);
					continueString = CalculateValue(continueString, this);
					for (int h = 0; h < m_variablesObject.Num(); h++) {
						if (m_variablesObject[h]->GetName() == continueString) {
							newArray.Add(m_variablesObject[h]);
							break;
						}
					}
					ArrayOfValues->Empty();
					ArrayOfValues->Append(newArray);
				}
				else {
					int ind = (int)UInteractCore::GetValue(secondary);
					if (ArrayOfValues->IsValidIndex(ind)) {
						value.Split("%%", &nonsS, &continueString);
						continueString = CalculateValue(continueString, this);
						for (int h = 0; h < m_variablesObject.Num(); h++) {
							if (m_variablesObject[h]->GetName() == continueString) {
								(*ArrayOfValues)[ind] = m_variablesObject[h];
								break;
							}
						}
					}
				}
			}
		}
		else if (It->GetCPPType().StartsWith("F")) {
			UStructProperty* It4 = Cast<UStructProperty>(It);
			void * dir2 = static_cast<uint8_t*>(parent) + It4->GetOffset_ForInternal();
			if (secondary != "") {
				UProperty* It3 = FindField<UProperty>(It4->Struct, *secondary);
				if (It3) {
					if (It3->GetCPPType() == "float") {
						UFloatProperty* It5 = Cast<UFloatProperty>(It3);
						It5->SetPropertyValue_InContainer(dir2, UInteractCore::GetValue(value), 0);
					}
					else if (It3->GetCPPType() == "int32") {
						UIntProperty* It5 = Cast<UIntProperty>(It3);
						It5->SetPropertyValue_InContainer(dir2, (int)UInteractCore::GetValue(value), 0);
					}
					else if (It3->GetCPPType() == "FString") {
						UStrProperty* It5 = Cast<UStrProperty>(It3);
						It5->SetPropertyValue_InContainer(dir2, value, 0);
					}
					else if (It3->GetCPPType() == "bool") {
						UBoolProperty* It5 = Cast<UBoolProperty>(It3);
						It5->SetPropertyValue_InContainer(dir2, value == "true" ? true : false, 0);
					}
					else if (It3->GetCPPType() == "FName") {
						UNameProperty* It5 = Cast<UNameProperty>(It3);
						It5->SetPropertyValue_InContainer(dir2, FName(*value), 0);
					}
					else if (It3->GetCPPType() == "FVector") {
						UStructProperty* It5 = Cast<UStructProperty>(It3);
						FVector vectorVal = { 0.f, 0.f, 0.f };
						if (value.StartsWith("{") && value.EndsWith("}")) {
							value.Replace(TEXT("{"), TEXT(""));
							value.Replace(TEXT("}"), TEXT(""));
						}
						vectorVal.InitFromString(value.ToUpper());
						void * dir3 = static_cast<uint8_t*>(dir2) + It5->GetOffset_ForInternal();
						It5->CopyValuesInternal(dir3, reinterpret_cast<void*>(&vectorVal), 1);
					}
					else if (It3->GetCPPType() == "FRotator") {
						UStructProperty* It5 = Cast<UStructProperty>(It3);
						FRotator vectorVal = { 0.f, 0.f, 0.f };
						if (value.StartsWith("{") && value.EndsWith("}")) {
							value.Replace(TEXT("{"), TEXT(""));
							value.Replace(TEXT("}"), TEXT(""));
						}
						vectorVal.InitFromString(value.ToUpper());
						void * dir3 = static_cast<uint8_t*>(dir2) + It5->GetOffset_ForInternal();
						It5->CopyValuesInternal(dir3, reinterpret_cast<void*>(&vectorVal), 1);
					}
					else if (It3->GetCPPType() == "FColor") {
						UStructProperty* It5 = Cast<UStructProperty>(It3);
						FColor vectorVal = { 0, 0, 0, 0 };
						if (value.StartsWith("{") && value.EndsWith("}")) {
							value.Replace(TEXT("{"), TEXT(""));
							value.Replace(TEXT("}"), TEXT(""));
						}
						vectorVal.InitFromString(value.ToUpper());
						void * dir3 = static_cast<uint8_t*>(dir2) + It5->GetOffset_ForInternal();
						It5->CopyValuesInternal(dir3, reinterpret_cast<void*>(&vectorVal), 1);
					}
					else if (It->GetCPPType().StartsWith("E")) {
						UEnumProperty* It5 = Cast<UEnumProperty>(It3);
						uint8* enumVal = It5->ContainerPtrToValuePtr<uint8>(dir2);
						//value.Split("##", &nonsS, &value);
						*enumVal = (int)UInteractCore::GetValue(value);
					}
					else if (It3->GetCPPType().StartsWith("A") || It3->GetCPPType().StartsWith("U") || It3->GetCPPType().StartsWith("TSubclassOf")) {
						UObjectProperty* It5 = Cast<UObjectProperty>(It3);
						UObject* vectorVal = nullptr;
						value.Split("%%", &nonsS, &value);
						for (int h = 0; h < m_variablesObject.Num(); h++) {
							if (m_variablesObject[h]->GetName() == value) {
								vectorVal = m_variablesObject[h];
								break;
							}
						}
						It5->SetPropertyValue_InContainer(dir2, vectorVal, 0);
					}
					else if (It3->GetCPPType() == "TArray") {
						UArrayProperty* arrProp = Cast<UArrayProperty>(It3);
						UProperty* innerProp = arrProp->Inner;
						FString continueString = "";
						FString variableString = "";
						FString noneS = "";
						if (innerProp->GetCPPType() == "float")
						{
							TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
							continueString = value;
							TArray<float> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							while (continueString.Split(",", &variableString, &continueString)) {
								newArray.Add(UInteractCore::GetValue(CalculateValue(variableString, this)));
							}
							newArray.Add(UInteractCore::GetValue(CalculateValue(continueString, this)));
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);

						}
						else if (innerProp->GetCPPType() == "int32")
						{
							TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
							continueString = value;
							TArray<int> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							while (continueString.Split(",", &variableString, &continueString)) {
								newArray.Add((int)UInteractCore::GetValue(CalculateValue(variableString, this)));
							}
							newArray.Add((int)UInteractCore::GetValue(CalculateValue(continueString, this)));
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
						else if (innerProp->GetCPPType() == "FString")
						{
							TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
							continueString = value;
							TArray<FString> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							while (continueString.Split("\",", &variableString, &continueString)) {
								newArray.Add(CalculateValue(variableString + "\"", this));
							}
							newArray.Add(CalculateValue(continueString, this));
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
						else if (innerProp->GetCPPType() == "bool")
						{
							TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
							continueString = value;
							TArray<bool> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							while (continueString.Split(",", &variableString, &continueString)) {
								newArray.Add(CalculateValue(variableString, this) == "true" ? true : false);
							}
							newArray.Add(CalculateValue(continueString, this) == "true" ? true : false);
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
						else if (innerProp->GetCPPType() == "FName")
						{
							TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
							continueString = value;
							TArray<FName> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							while (continueString.Split("\",", &variableString, &continueString)) {
								newArray.Add(FName(*CalculateValue(variableString + "\"", this)));
							}
							newArray.Add(FName(*CalculateValue(continueString, this)));
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
						else if (innerProp->GetCPPType() == "FVector")
						{
							TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
							continueString = value;
							TArray<FVector> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							FVector vectr = { 0.f, 0.f, 0.f };
							while (continueString.Split("},", &variableString, &continueString)) {
								vectr.InitFromString(CalculateValue(variableString + "}", this));
								newArray.Add(vectr);
							}
							vectr.InitFromString(CalculateValue(continueString, this));
							newArray.Add(vectr);
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
						else if (innerProp->GetCPPType() == "FRotator")
						{
							TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
							continueString = value;
							TArray<FRotator> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							FRotator vectr = { 0.f, 0.f, 0.f };
							while (continueString.Split("},", &variableString, &continueString)) {
								vectr.InitFromString(CalculateValue(variableString + "}", this));
								newArray.Add(vectr);
							}
							vectr.InitFromString(CalculateValue(continueString, this));
							newArray.Add(vectr);
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
						else if (innerProp->GetCPPType() == "FColor")
						{
							TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
							continueString = value;
							TArray<FColor> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							FColor vectr = { 0, 0, 0, 0 };
							while (continueString.Split("},", &variableString, &continueString)) {
								vectr.InitFromString(CalculateValue(variableString + "}", this));
								newArray.Add(vectr);
							}
							vectr.InitFromString(CalculateValue(continueString, this));
							newArray.Add(vectr);
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
						else if (innerProp->GetCPPType().StartsWith("A") || innerProp->GetCPPType().StartsWith("U") || innerProp->GetCPPType().StartsWith("TSubclassOf")) {
							TArray<UObject*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UObject*>>(dir2);
							continueString = value;
							TArray<UObject*> newArray;
							continueString.Split("[", &noneS, &continueString);
							continueString.Split("]", &continueString, &noneS);
							while (continueString.Split(",", &variableString, &continueString)) {
								variableString.Split("%%", &nonsS, &variableString);
								variableString = CalculateValue(variableString, this);
								for (int h = 0; h < m_variablesObject.Num(); h++) {
									if (m_variablesObject[h]->GetName() == variableString) {
										newArray.Add(m_variablesObject[h]);
										break;
									}
								}
							}
							continueString.Split("%%", &nonsS, &continueString);
							continueString = CalculateValue(continueString, this);
							for (int h = 0; h < m_variablesObject.Num(); h++) {
								if (m_variablesObject[h]->GetName() == continueString) {
									newArray.Add(m_variablesObject[h]);
									break;
								}
							}
							ArrayOfValues->Empty();
							ArrayOfValues->Append(newArray);
						}
					}
				}
			}
		}
	}
}

bool UInteractComponent::CheckRaycast(AActor* actor, FCheckRaycast& data, UInteractComponent* interactComponent)
{
	FVector start = { 0.f, 0.f,0.f };
	FVector location = data.relativeFinalOffset;
	AActor* act1 = nullptr;
	if (data.actor.enumName != EGetActorList::None) {
		int val = (int)data.actor.enumName - 1;
		if (val >= 0) {
			act1 = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(false, 0));
		}
	}

	if (act1) {
		USceneComponent* compRot = nullptr;
		if (IsValid(data.specificChild)) {
			TSet<UActorComponent*> comps;
			comps = act1->GetComponents();
			for (int i = 0; i < comps.Array().Num(); i++) {
				USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
				if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
					if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
						compRot = scene;
					}
				}
				else {
					if (scene && scene->GetClass() == data.specificChild) {
						compRot = scene;
					}
				}
			}
		}
		else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
			TSet<UActorComponent*> comps;
			comps = act1->GetComponents();
			for (int i = 0; i < comps.Array().Num(); i++) {
				USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
				if (scene && scene->ComponentHasTag(data.componentTag)) {
					compRot = scene;
				}
			}
		}

		FVector dirVect = { 0.f, 0.f, 0.f };
		if (data.direction == EVectorDirection::Forward) {
			if (compRot) {
				dirVect = compRot->GetForwardVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
			else if (act1) {
				dirVect = act1->GetActorForwardVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
		}
		else if (data.direction == EVectorDirection::Backward) {
			if (compRot) {
				dirVect = -compRot->GetForwardVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
			else if (act1) {
				dirVect = -act1->GetActorForwardVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
		}
		else if (data.direction == EVectorDirection::Up) {
			if (compRot) {
				dirVect = compRot->GetUpVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
			else if (act1) {
				dirVect = act1->GetActorUpVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
		}
		else if (data.direction == EVectorDirection::Down) {
			if (compRot) {
				dirVect = -compRot->GetUpVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
			else if (act1) {
				dirVect = -act1->GetActorUpVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
		}
		else if (data.direction == EVectorDirection::Right) {
			if (compRot) {
				dirVect = compRot->GetRightVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
			else if (act1) {
				dirVect = act1->GetActorRightVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
		}
		else if (data.direction == EVectorDirection::Left) {
			if (compRot) {
				dirVect = -compRot->GetRightVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
			else if (act1) {
				dirVect = -act1->GetActorRightVector();
				dirVect.Normalize();
				location += dirVect * data.distance;
			}
		}

		if (compRot) {
			location += compRot->GetComponentLocation();
			start = compRot->GetComponentLocation() + data.relativeInitOffset;
		}
		else if (act1) {
			location += act1->GetActorLocation();
			start = act1->GetActorLocation() + data.relativeInitOffset;
		}


		//Delete return
		FString nonsS2 = "";
		FString retValHit = "";
		for (int h = 0; h < data.customReturn.Num(); h++) {
			data.customReturn[h].Split("%%", &nonsS2, &retValHit);
			for (int y = 0; y < interactComponent->m_variablesObject.Num(); y++) {
				if (retValHit == interactComponent->m_variablesObject[y]->GetName()) {
					interactComponent->m_variablesObject.RemoveAt(y);
					break;
				}
			}
		}


		TArray<FHitResult> Hit;
		TArray<TEnumAsByte<EObjectTypeQuery>> objType;
		bool res = false;
		if (data.raycastType == ERaycastType::Line) {
			if (data.collisionChannel == ECollisionRayCastType::AllDynamic) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
			}
			else if (data.collisionChannel == ECollisionRayCastType::AllStatic) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Pawn) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Pawn));
			}
			else if (data.collisionChannel == ECollisionRayCastType::PhysicsBody) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Visibility) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Visibility));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Camera) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Camera));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Destructible) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Destructible));
			}
			TArray<AActor*> ignoredActors;
			ignoredActors.Add(act1);
			res = UKismetSystemLibrary::LineTraceMultiForObjects(interactComponent->GetWorld(), start, location, objType, true, ignoredActors, data.debug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
				Hit, true);
			if (res) {
				bool prov = false;
				for (int i = 0; i < Hit.Num(); i++) {
					if ((data.selectedActors.Contains(Hit[i].Actor->GetClass()) && data.ignoreActors) || (!data.selectedActors.Contains(Hit[i].Actor->GetClass()) && !data.ignoreActors)) {
						continue;
					}
					else if ((data.selectedComponents.Contains(Hit[i].Component->GetClass()) && data.ignoreComponents) || (!data.selectedComponents.Contains(Hit[i].Component->GetClass()) && !data.ignoreComponents)) {
						continue;
					}
					else {
						prov = true;
						if (!IsValid(data.targetActorClass) && data.targetActorTag == NAME_None && !IsValid(data.targetComponentClass) && data.targetComponentTag == NAME_None) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
					}
					if (IsValid(data.targetActorClass)) {
						if (Hit[i].Actor->GetClass() == data.targetActorClass) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}
					if (data.targetActorTag != NAME_None) {
						if (Hit[i].Actor->Tags.Contains(data.targetActorTag)) {
							//FString resVa = "";
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}
					if (IsValid(data.targetComponentClass)) {
						if (Hit[i].Component->GetClass() == data.targetComponentClass) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}
					if (data.targetComponentTag != NAME_None) {
						if (Hit[i].Component->ComponentTags.Contains(data.targetComponentTag)) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}
				}
				if (!prov) {
					res = false;
				}
				else {
					for (int f = 0; f < Hit.Num(); f++) {
						AActor* hitActor = Hit[f].Actor.Get();
						interactComponent->ReturnValue(&data.customReturn, hitActor);
					}
					CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
				}
			}
		}
		else if (data.raycastType == ERaycastType::Capsule) {
			if (data.collisionChannel == ECollisionRayCastType::AllDynamic) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
			}
			else if (data.collisionChannel == ECollisionRayCastType::AllStatic) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Pawn) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Pawn));
			}
			else if (data.collisionChannel == ECollisionRayCastType::PhysicsBody) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Visibility) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Visibility));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Camera) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Camera));
			}
			else if (data.collisionChannel == ECollisionRayCastType::Destructible) {
				objType.Push(UEngineTypes::ConvertToObjectType(ECC_Destructible));
			}
			TArray<AActor*> ignoredActors;
			ignoredActors.Add(act1);
			res = UKismetSystemLibrary::CapsuleTraceMultiForObjects(interactComponent->GetWorld(), start, location, data.raycastRadius, data.raycastLength, objType, true, ignoredActors, data.debug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
				Hit, true);
			if (res) {
				bool prov = false;
				for (int i = 0; i < Hit.Num(); i++) {
					if ((data.selectedActors.Contains(Hit[i].Actor->GetClass()) && data.ignoreActors) || (!data.selectedActors.Contains(Hit[i].Actor->GetClass()) && !data.ignoreActors)) {
						continue;
					}
					else if ((data.selectedComponents.Contains(Hit[i].Component->GetClass()) && data.ignoreComponents) || (!data.selectedComponents.Contains(Hit[i].Component->GetClass()) && !data.ignoreComponents)) {
						continue;
					}
					else {
						prov = true;
						if (!IsValid(data.targetActorClass) && data.targetActorTag == NAME_None && !IsValid(data.targetComponentClass) && data.targetComponentTag == NAME_None) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
					}
					if (IsValid(data.targetActorClass)) {
						if (Hit[i].Actor->GetClass() == data.targetActorClass) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}
					if (data.targetActorTag != NAME_None) {
						if (Hit[i].Actor->Tags.Contains(data.targetActorTag)) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}
					if (IsValid(data.targetComponentClass)) {
						if (Hit[i].Component->GetClass() == data.targetComponentClass) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}
					if (data.targetComponentTag != NAME_None) {
						if (Hit[i].Component->ComponentTags.Contains(data.targetComponentTag)) {
							for (int f = 0; f < Hit.Num(); f++) {
								AActor* hitActor = Hit[f].Actor.Get();
								interactComponent->ReturnValue(&data.customReturn, hitActor);
							}
							CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
								return true;
						}
						else {
							res = false;
							continue;
						}
					}

				}
				if (!prov) {
					res = false;
				}
				else {
					for (int f = 0; f < Hit.Num(); f++) {
						AActor* hitActor = Hit[f].Actor.Get();
						interactComponent->ReturnValue(&data.customReturn, hitActor);
					}
					CONFIRM_RETURN(interactComponent, data, FCheckRaycast)
				}
			}
		}
		return res;
	}
	return false;
}

bool UInteractComponent::CheckTooltipVisibility(AActor* actor, FCheckTooltipVisibility& data, UInteractComponent* interactComponent)
{
	if (data.specificTooltip) {
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions.IsValidIndex(data.selectTooltip)) {
			if (IsValid(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[data.selectTooltip].m_WidgetComponent) && IsValid(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[data.selectTooltip].m_WidgetComponent->GetUserWidgetObject())) {
				UTooltip* tooltip = Cast<UTooltip>(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[data.selectTooltip].m_WidgetComponent->GetUserWidgetObject());
				if (tooltip) {
					if (data.isVisible) {
						return tooltip->GetButton()->ColorAndOpacity.A >= 0.01f;
					}
					else {
						return tooltip->GetButton()->ColorAndOpacity.A <= 0.01f;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < interactComponent->interactDataArray[interactComponent->m_currentStage].actions.Num(); i++) {
			if (IsValid(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
				UTooltip* tooltip = Cast<UTooltip>(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
				if (tooltip) {
					if (data.isVisible) {
						if (tooltip->GetButton()->ColorAndOpacity.A < 0.01f) {
							return false;
						}
					}
					else {
						if (tooltip->GetButton()->ColorAndOpacity.A > 0.01f) {
							return false;
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool UInteractComponent::CheckIfActorExists(AActor* actor, FCheckIfActorExists& data, UInteractComponent* interactComponent)
{
	if (data.actor.enumName != EGetActorList::None) {
		int val = (int)data.actor.enumName - 1;
		AActor* acto = nullptr;
		if (val >= 0) {
			acto = interactComponent->actorFunctionMap[val].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(false, 0));
		}
		if (acto) {
			return true;
		}
	}
	return false;
}

UInteractComponent* UInteractComponent::GetInteractBP(UClass* interactOwnerClass)
{
	if (interactOwnerClass) {
		AActor* actorInteractBP = interactOwnerClass->GetDefaultObject<AActor>();
		if (actorInteractBP) {
			TArray<UObject*> usc;
			interactOwnerClass->CollectDefaultSubobjects(usc, true);
			for (int h = 0; h < usc.Num(); h++) {
				UInteractComponent* intc = Cast<UInteractComponent>(usc[h]->GetOuter());
				if (intc) {
					return intc;
				}
			}
		}
	}
	return nullptr;
}

void UInteractComponent::StartInteract()
{
	ImportInteractDataInternal();
	InitializeTooltip();

	//PRESTAGE
	m_currentStage = (int)UInteractCore::GetValue(m_initStage, interactDataArray.Num() - 1);
	m_sharedFunctionObjects.Empty();
	m_sharedVariablesArray.Empty();
	m_variablesArray.Empty();
	m_variablesObject.Empty();
	//Si spawnea un objeto esto se reinciaría por lo tanto no es recomendable
	//delegateAuxFunctions.Empty();
	m_currentStage = UInteractCore::GetValue(m_initStage, interactDataArray.Num() - 1);;
	functionIndex = 0;
	if (IInteractInterface* intInt = Cast<IInteractInterface>(GetOwner())) {
		m_actorMesh = intInt->GetActorMesh();
	}
	else {
		TArray<UActorComponent*> ActorCArray = GetOwner()->GetComponentsByClass(UMeshComponent::StaticClass());
		for (int i = 0; i < ActorCArray.Num(); i++) {
			m_actorMesh.Add(Cast<UMeshComponent>(ActorCArray[i]));
		}
	}
	tempActor = nullptr;
	tempDelayFunction = false;
	tempDoOnceDelay = false;
	tempJumpStage = false;
	tempJumpFunction = false;

	UpdateStageAtributes();

	interactBP = UInteractComponent::GetInteractBP(GetOwner()->GetClass());

	if (!doOnceStart) {
		FTimerDelegate ticksDel;
		ticksDel.BindUFunction(this, FName("CalculateTicks"), m_ticksTime);
		GetWorld()->GetTimerManager().SetTimer(ticksHandle, ticksDel, m_ticksTime, true);
	}

	if (interactDataArray.IsValidIndex(m_currentStage)) {
		while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].preStageFunction.Num())
		{
			if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].preStageFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].preStageFunction[functionIndex].enumNameCore != EActionListCore::None) {
				tempAction = EAction::None;
				tempType = EActionStageType::Pre;
				int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
				int value = ((int)interactDataArray[m_currentStage].preStageFunction[functionIndex].enumName) - 1;
				int valueCore = ((int)interactDataArray[m_currentStage].preStageFunction[functionIndex].enumNameCore) - 1;
				if (valueCore >= 0) {
					interactFunctionMap[valueCore].Execute(this, tempActor, EAction::None, functionIndex, EActionStageType::Pre, -1, -1, -1, false);
				}
				else if (value >= 0) {
					interactFunctionMap[value + maxE - 1].Execute(this, tempActor, EAction::None, functionIndex, EActionStageType::Pre, -1, -1, -1, false);
				}
			}
			if (stopStage || tempDelayFunction) {
				break;
			}
			functionIndex++;
		}
	}
	m_actorOutsideEnterNames.Empty();
	m_actorOutsideExitNames.Empty();
	m_actorInsideEnterNames.Empty();
	m_actorInsideExitNames.Empty();
	m_actorTriggerEnterNames.Empty();
	m_actorTriggerExitNames.Empty();
	tempActor = nullptr;
	doOnceStart = true;
}

void UInteractComponent::ImportInteractData(FString text)
{
	UProperty* Prop = UInteractComponent::StaticClass()->FindPropertyByName(TEXT("interactDataArray"));
	if (Prop->ImportText(*text, &interactDataArray, PPF_None, nullptr))
	{
		//Import sucessful
	}
}

FString UInteractComponent::ExportInteractData()
{
	FString Output = "";
	if (UProperty* Prop = UInteractComponent::StaticClass()->FindPropertyByName(TEXT("interactDataArray")))
	{
		Prop->ExportText_Direct(Output, &interactDataArray, nullptr, this, (PPF_ExportsNotFullyQualified | PPF_Copy | PPF_Delimited | PPF_IncludeTransient), nullptr);
	}
	return Output;
}

void UInteractComponent::ImportInteractDataInternal()
{
	if (interactDataArrayString == "NOT_VALUE") {
		ExportInteractDataInternal();
	}
	if (interactDataArrayString != "NOT_VALUE") {
		ImportInteractData(interactDataArrayString);
	}
}

void UInteractComponent::ExportInteractDataInternal()
{
	if (interactDataArray.Num() != 0) {
		interactDataArrayString = ExportInteractData();
	}
	interactDataArray.Empty();
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();
	StartInteract();
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!tooltipLoadedViewport) {
		for (int j = 0; j < interactDataArray.Num(); j++)
		{
			for (int i = 0; i < interactDataArray[j].actions.Num(); i++)
			{
				if (IsValid(interactDataArray[j].actions[i].m_WidgetComponent)) {
					UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[j].actions[i].m_WidgetComponent->GetUserWidgetObject());

					if (m_TooltipWidget) {
						if (interactDataArray[j].actions[i].widgetInScreen) {
							interactDataArray[j].actions[i].m_WidgetComponent->ToggleVisibility(false);
							m_TooltipWidget->AddToViewport();
							m_TooltipWidget->SetTooltipLocation(interactDataArray[j].actions[i].widgetLocation);
							m_TooltipWidget->SetTooltipSize(interactDataArray[j].actions[i].widgetSize);
						}
					}
				}
			}
		}
		tooltipLoadedViewport = true;
		SetComponentTickEnabled(false);
	}
}

void UInteractComponent::TriggerCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		if (interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num() == 0) {
			if (m_actorTriggerEnterNames.Contains(OtherActor->GetName())) {
				m_actorTriggerEnterNames.Remove(OtherActor->GetName());
			}
		}
		if (!m_actorTriggerExitNames.Contains(OtherActor->GetName())) {
			tempDelayFunction = false;
			tempDoOnceDelay = false;

			if (interactDataArray[m_currentStage].capsuleTriggerStruct.tag.ToString() != "" && interactDataArray[m_currentStage].capsuleTriggerStruct.tag != NAME_None) {
				if (interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass != nullptr) {
					if (OtherActor->IsA(interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass) && OtherActor->ActorHasTag(interactDataArray[m_currentStage].capsuleTriggerStruct.tag)
						&& interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerExitNames.Add(OtherActor->GetName());
							if (m_actorTriggerEnterNames.Contains(OtherActor->GetName())) {
								m_actorTriggerEnterNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;

								tempType = EActionStageType::Exit;
								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
				else {
					//IInteractInterface* intAct = Cast<IInteractInterface>(OtherActor);
					if (OtherActor && OtherActor->ActorHasTag(interactDataArray[m_currentStage].capsuleTriggerStruct.tag) && interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerExitNames.Add(OtherActor->GetName());
							if (m_actorTriggerEnterNames.Contains(OtherActor->GetName())) {
								m_actorTriggerEnterNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;
								tempType = EActionStageType::Exit;

								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
			}
			else {
				if (interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass != nullptr) {
					if (OtherActor->IsA(interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass) && interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerExitNames.Add(OtherActor->GetName());
							if (m_actorTriggerEnterNames.Contains(OtherActor->GetName())) {
								m_actorTriggerEnterNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;
								tempType = EActionStageType::Exit;

								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
				else {
					//IInteractInterface* intAct = Cast<IInteractInterface>(OtherActor);
					if (OtherActor && interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerExitNames.Add(OtherActor->GetName());
							if (m_actorTriggerEnterNames.Contains(OtherActor->GetName())) {
								m_actorTriggerEnterNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;
								tempType = EActionStageType::Exit;

								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Exit, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
			}
		}
	}
}

void UInteractComponent::TriggerCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		if (interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num() == 0) {
			if (m_actorTriggerExitNames.Contains(OtherActor->GetName())) {
				m_actorTriggerExitNames.Remove(OtherActor->GetName());
			}
		}
		if (!m_actorTriggerEnterNames.Contains(OtherActor->GetName())) {
			tempDelayFunction = false;
			tempDoOnceDelay = false;
			if (interactDataArray[m_currentStage].capsuleTriggerStruct.tag.ToString() != "" && interactDataArray[m_currentStage].capsuleTriggerStruct.tag != NAME_None) {
				if (interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass != nullptr) {
					if (OtherActor->IsA(interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass) && OtherActor->ActorHasTag(interactDataArray[m_currentStage].capsuleTriggerStruct.tag)
						&& interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerEnterNames.Add(OtherActor->GetName());
							if (m_actorTriggerExitNames.Contains(OtherActor->GetName())) {
								m_actorTriggerExitNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;
								tempType = EActionStageType::Init;

								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
				else {
					//IInteractInterface* intAct = Cast<IInteractInterface>(OtherActor);
					if (OtherActor && OtherActor->ActorHasTag(interactDataArray[m_currentStage].capsuleTriggerStruct.tag) && interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerEnterNames.Add(OtherActor->GetName());
							if (m_actorTriggerExitNames.Contains(OtherActor->GetName())) {
								m_actorTriggerExitNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;
								tempType = EActionStageType::Init;

								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
			}
			else {
				if (interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass != nullptr) {
					if (OtherActor->IsA(interactDataArray[m_currentStage].capsuleTriggerStruct.actorClass) && interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerEnterNames.Add(OtherActor->GetName());
							if (m_actorTriggerExitNames.Contains(OtherActor->GetName())) {
								m_actorTriggerExitNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;
								tempType = EActionStageType::Init;

								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
				else {
					//IInteractInterface* intAct = Cast<IInteractInterface>(OtherActor);
					if (OtherActor && interactDataArray.IsValidIndex(m_currentStage))
					{
						if (interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num() > 0) {
							tempActor = OtherActor;
							functionIndex = 0;
							m_actorTriggerEnterNames.Add(OtherActor->GetName());
							if (m_actorTriggerExitNames.Contains(OtherActor->GetName())) {
								m_actorTriggerExitNames.Remove(OtherActor->GetName());
							}
						}
						while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction.Num())
						{
							if (GetWorld() && GetOwner() && interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore != EActionListCore::None) {
								tempAction = EAction::None;
								tempType = EActionStageType::Init;

								int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
								int value = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumName) - 1;
								int valueCore = ((int)interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction[functionIndex].enumNameCore) - 1;
								if (valueCore >= 0) {
									interactFunctionMap[valueCore].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
								else if (value >= 0) {
									interactFunctionMap[value + maxE - 1].Execute(this, OtherActor, EAction::None, functionIndex, EActionStageType::Init, -1, -1, -1, false);
								}
							}
							if (stopStage || tempDelayFunction) {
								break;
							}
							functionIndex++;
						}
					}
				}
			}
		}
	}

}

void UInteractComponent::ExternalInteract(AActor* actor, FExternalInteract& data, UInteractComponent* interactComponent)
{
	if (data.jumpToStage) {
		AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
		UInteractComponent::JumpStageGlobal(acto, data.goToStage.stageName, data.goToStage.delay, data.goToStage.stageNumber, data.goToStage.cancelTimers);
	}
	else {
		if (data.doLikePlayer) {
			TArray<UPrimitiveComponent*> InteractiveItems;
			interactComponent->GetOwner()->GetOverlappingComponents(InteractiveItems);
			UInteractComponent* minItem = nullptr;
			TArray<UInteractComponent*> highestPriorityOverlappingItem;
			bool sameButton = false;

			for (UPrimitiveComponent* itemC : InteractiveItems)
			{
				if (itemC->GetOwner()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) && itemC->ComponentHasTag("Interact") && itemC->GetOwner()->GetName() != interactComponent->GetOwner()->GetName())
				{
					sameButton = false;
					//IInteractInterface* itemClass = Cast<IInteractInterface>(itemC->GetOwner());

					UActorComponent* actorComponent = itemC->GetOwner()->GetComponentByClass(UInteractComponent::StaticClass());
					UInteractComponent* interactComponent1 = Cast<UInteractComponent>(actorComponent);

					if (interactComponent1->interactDataArray.Num() > 0 && interactComponent1->interactDataArray.IsValidIndex(interactComponent1->m_currentStage)) {
						for (int i = 0; i < interactComponent1->interactDataArray[interactComponent1->m_currentStage].actions.Num(); i++) {
							if (interactComponent1->interactDataArray[interactComponent1->m_currentStage].actions[i].actionButton == data.actionButton.actionButton) {
								sameButton = true;
							}
						}
					}
					if (interactComponent1) {
						if (interactComponent1->GetPriority() == EPriority::None) {
							continue;
						}
						if (interactComponent1->GetPriority() == EPriority::Min && sameButton) {
							minItem = interactComponent1;
							continue;
						}
						if (interactComponent1->GetPriority() == EPriority::Max && sameButton) {
							highestPriorityOverlappingItem.Empty();
							highestPriorityOverlappingItem.Add(interactComponent1);
							break;
						}
						if (sameButton && (highestPriorityOverlappingItem.Num() <= 0 || (highestPriorityOverlappingItem[0]->GetPriority() < interactComponent1->GetPriority()) || (!highestPriorityOverlappingItem[0]->GetPriorityRepeat() && interactComponent1->GetPriorityRepeat())))
						{
							highestPriorityOverlappingItem.Empty();
							highestPriorityOverlappingItem.Add(interactComponent1);
						}
						else if (sameButton && highestPriorityOverlappingItem.Num() > 0 && highestPriorityOverlappingItem[0]->GetPriority() == interactComponent1->GetPriority() && (highestPriorityOverlappingItem[0]->GetPriorityRepeat() && interactComponent1->GetPriorityRepeat())) {
							highestPriorityOverlappingItem.Add(interactComponent1);
						}
					}
				}
			}
			if (highestPriorityOverlappingItem.Num() > 0) {
				for (int i = 0; i < highestPriorityOverlappingItem.Num(); i++) {
					AActor* actAct = Cast<AActor>(highestPriorityOverlappingItem[i]);
					if (actAct && actAct->GetName() != interactComponent->GetOwner()->GetName()) {
						highestPriorityOverlappingItem[i]->Interact(interactComponent->GetOwner(), data.actionButton);
					}
				}
			}
			else if (minItem) {
				AActor* actAct = Cast<AActor>(minItem);
				if (actAct && actAct->GetName() != interactComponent->GetOwner()->GetName()) {
					minItem->Interact(interactComponent->GetOwner(), data.actionButton);
				}
			}
		}
		else {
			TArray<AActor*> foundActors;
			if (data.actor.enumName != EGetActorList::None) {
				int val = (int)data.actor.enumName - 1;
				AActor* act = nullptr;
				if (val >= 0) {
					act = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
				}
				if (act) {
					if (data.overlapping) {
						interactComponent->GetOwner()->GetOverlappingActors(foundActors);
						if (foundActors.Contains(act)) {
							UActorComponent* actorComponent = act->GetOwner()->GetComponentByClass(UInteractComponent::StaticClass());
							UInteractComponent* actAct = Cast<UInteractComponent>(actorComponent);
							if (actAct && act->GetName() != interactComponent->GetOwner()->GetName()) {
								actAct->Interact(interactComponent->GetOwner(), data.actionButton);
							}
						}
					}
					else {
						UActorComponent* actorComponent = act->GetOwner()->GetComponentByClass(UInteractComponent::StaticClass());
						UInteractComponent* actAct = Cast<UInteractComponent>(actorComponent);
						if (actAct && act->GetName() != interactComponent->GetOwner()->GetName()) {
							actAct->Interact(interactComponent->GetOwner(), data.actionButton);
						}
					}
				}
			}
			else {
				interactComponent->GetOwner()->GetOverlappingActors(foundActors);
				for (int i = 0; i < foundActors.Num(); i++) {
					UActorComponent* actorComponent = foundActors[i]->GetOwner()->GetComponentByClass(UInteractComponent::StaticClass());
					UInteractComponent* actAct = Cast<UInteractComponent>(actorComponent);
					if (actAct && foundActors[i]->GetName() != interactComponent->GetOwner()->GetName()) {
						actAct->Interact(interactComponent->GetOwner(), data.actionButton);
					}
				}
			}
		}
	}
}

void UInteractComponent::DestroyEntity(AActor* actor, FDestroyEntity& data, UInteractComponent* interactComponent)
{
	int val = (int)data.actor.enumName - 1;
	AActor* acto = nullptr;
	if (val >= 0) {
		acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
	}
	if (acto) {
		UInteractComponent* inter = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
		if (inter) {
			inter->stopStage = true;
			inter->readyForNextStage = false;
		}
		IInteractInterface* intAct = Cast<IInteractInterface>(acto);
		if(intAct){
			intAct->DestroyActor();
		}
		else {
			acto->Destroy();
		}
	}
}

void UInteractComponent::TeleportTo(AActor* actor, FTeleportTo& data, UInteractComponent* interactComponent)
{
	AActor* act = UInteractComponent::GetCurrentActor(interactComponent, data.actor2, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	TArray<AActor*> actors;
	if (act != nullptr) {
		if (data.actor.enumName != EGetActorList::None) {
			int val = (int)data.actor.enumName - 1;
			AActor* acto = nullptr;
			if (val >= 0) {
				acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
			}
			if (acto) {
				FVector newVector = acto->GetActorLocation();
				if (data.targetActorDirection != EVectorDirection::None) {
					FVector directionVector = { 0.f, 0.f, 0.f };
					if (data.targetActorDirection == EVectorDirection::Forward) {
						directionVector = acto->GetActorForwardVector();
					}
					else if (data.targetActorDirection == EVectorDirection::Backward) {
						directionVector = acto->GetActorForwardVector()*-1.f;
					}
					else if (data.targetActorDirection == EVectorDirection::Right) {
						directionVector = acto->GetActorRightVector();
					}
					else if (data.targetActorDirection == EVectorDirection::Left) {
						directionVector = acto->GetActorRightVector()*-1.f;
					}
					else if (data.targetActorDirection == EVectorDirection::Up) {
						directionVector = acto->GetActorUpVector();
					}
					else if (data.targetActorDirection == EVectorDirection::Down) {
						directionVector = acto->GetActorUpVector()*-1.f;
					}
					directionVector.Normalize();
					directionVector = directionVector * data.distanceDirection;
					newVector.Set(newVector.X + directionVector.X, newVector.Y + directionVector.Y, newVector.Z + directionVector.Z);
				}
				newVector.Set(newVector.X + data.location.X, newVector.Y + data.location.Y, newVector.Z + data.location.Z);
				act->SetActorLocation(newVector);
			}
		}
		else {
			act->SetActorLocation(data.location);
		}
	}
}

void UInteractComponent::MoveToFunction(AActor *actor, FMoveToFunction& data, UInteractComponent* interactComponent)
{
	data.savedState.tempAction = interactComponent->tempAction;
	data.savedState.tempStage = interactComponent->tempType;
	data.savedState.tempTooltipIndex = interactComponent->tempTooltipIndex;
	data.savedState.tempFunctionIndex = interactComponent->functionIndex;
	data.savedState.tempCurrentStage = interactComponent->m_currentStage;
	data.savedState.tempInElse = interactComponent->tempInElse;

	if (data.forceMove) {
		for (int i = 0; i < interactComponent->m_registerTimerArray.Num(); i++) {
			if (interactComponent->m_registerTimerArray[i].Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::moveToFunction))) {
				interactComponent->m_registerTimerArray.RemoveAt(i);
			}
		}
	}

	if ((data.forceMove && !interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::moveToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) || !data.forceMove) {
		if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::moveToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
			interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::moveToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
		}
		FVector locVec = { 0.f, 0.f, 0.f };
		if (!data.movableTarget) {
			FVector location = data.location;
			AActor* act1 = UInteractComponent::GetCurrentActor(interactComponent, data.actor2, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
			USceneComponent* compRot = nullptr;
			if (IsValid(data.specificChild) && act1) {
				TSet<UActorComponent*> comps;
				comps = act1->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
						if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
							compRot = scene;
						}
					}
					else {
						if (scene && scene->GetClass() == data.specificChild) {
							compRot = scene;
						}
					}
				}
			}
			else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
				TSet<UActorComponent*> comps;
				comps = act1->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (scene && scene->ComponentHasTag(data.componentTag)) {
						compRot = scene;
					}
				}
			}

			if (data.actor.enumName != EGetActorList::None) {
				int val = (int)data.actor.enumName - 1;
				AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor2, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
				FVector dirVect = { 0.f, 0.f, 0.f };
				if (data.direction == EVectorDirection::Forward) {
					if (act1 && (!acto || act1->GetName() == acto->GetName())) {
						dirVect = act1->GetActorForwardVector();
						dirVect.Normalize();
						location += dirVect * data.distanceDirection;
					}
					else if (acto) {
						location += acto->GetActorLocation();
					}
				}
				else if (data.direction == EVectorDirection::Backward) {
					if (act1 && (!acto || act1->GetName() == acto->GetName())) {
						dirVect = -act1->GetActorForwardVector();
						dirVect.Normalize();
						location += dirVect * data.distanceDirection;
					}
					else if (acto) {
						location += acto->GetActorLocation();
					}
				}
				else if (data.direction == EVectorDirection::Up) {
					if (act1 && (!acto || act1->GetName() == acto->GetName())) {
						dirVect = act1->GetActorUpVector();
						dirVect.Normalize();
						location += dirVect * data.distanceDirection;
					}
					else if (acto) {
						location += acto->GetActorLocation();
					}
				}
				else if (data.direction == EVectorDirection::Down) {
					if (act1 && (!acto || act1->GetName() == acto->GetName())) {
						dirVect = -act1->GetActorUpVector();
						dirVect.Normalize();
						location += dirVect * data.distanceDirection;
					}
					else if (acto) {
						location += acto->GetActorLocation();
					}
				}
				else if (data.direction == EVectorDirection::Right) {
					if (act1 && (!acto || act1->GetName() == acto->GetName())) {
						dirVect = act1->GetActorRightVector();
						dirVect.Normalize();
						location += dirVect * data.distanceDirection;
					}
					else if (acto) {
						location += acto->GetActorLocation();
					}
				}
				else if (data.direction == EVectorDirection::Left) {
					if (act1 && (!acto || act1->GetName() == acto->GetName())) {
						dirVect = -act1->GetActorRightVector();
						dirVect.Normalize();
						location += dirVect * data.distanceDirection;
					}
					else if (acto) {
						location += acto->GetActorLocation();
					}
				}
				if (compRot) {
					location += compRot->GetComponentLocation();
				}
				else if (act1) {
					location += act1->GetActorLocation();
				}
				data.actorDebug = acto;
			}
			locVec = location;
		}
		FMoveToFunction moveFunc = { data.actor2, data.actor, data.specificChild, data.componentTag, data.curve, locVec,
			data.direction, data.distanceDirection, data.tickTime, data.speedAmount, data.toleranceError, data.movableTarget, data.targetV, data.actorDebug, data.moveHandle, data.savedState };
		interactComponent->MoveToFunctionTimer(actor, moveFunc);
	}
}

void UInteractComponent::MoveToFunctionTimer(AActor* actor, FMoveToFunction& data)
{
	if (GetWorld() && GetOwner() && m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::moveToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		FFunctionActorData intActor = { EGetActorList::getThisActor, {}, {data.actor2.getThisActor.getOwner} };
		FVector location = { 0.f, 0.f, 0.f };
		FVector direction = { 1.f, 1.f, 1.f };
		FVector usLocation = { 0.f, 0.f, 0.f };
		AActor* act1 = UInteractComponent::GetCurrentActor(this, data.actor2, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, data.savedState.tempInElse, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage);
		if (act1) {
			USceneComponent* compRot = nullptr;
			if (IsValid(data.specificChild)) {
				TSet<UActorComponent*> comps;
				comps = act1->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
						if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
							compRot = scene;
						}
					}
					else {
						if (scene && scene->GetClass() == data.specificChild) {
							compRot = scene;
						}
					}
				}
			}
			else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
				TSet<UActorComponent*> comps;
				comps = act1->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (scene && scene->ComponentHasTag(data.componentTag)) {
						compRot = scene;
					}
				}
			}

			if (!compRot) {
				usLocation = act1->GetActorLocation();
			}
			else {
				usLocation = compRot->GetComponentLocation();
			}

			if (data.movableTarget) {
				if (data.actor.enumName != EGetActorList::None) {
					int val = (int)data.actor.enumName - 1;
					AActor* acto = nullptr;
					if (val >= 0) {
						acto = actorFunctionMap[val].Execute(this, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage, FExtraActorData(data.savedState.tempInElse, 0));
					}

					FVector dirVect = { 0.f, 0.f, 0.f };
					if (data.direction == EVectorDirection::Forward) {
						if (act1 && (!acto || act1->GetName() == acto->GetName())) {
							dirVect = act1->GetActorForwardVector();
							dirVect.Normalize();
							location += dirVect * data.distanceDirection;
						}
						else if (acto) {
							location += acto->GetActorLocation();
						}
					}
					else if (data.direction == EVectorDirection::Backward) {
						if (act1 && (!acto || act1->GetName() == acto->GetName())) {
							dirVect = -act1->GetActorForwardVector();
							dirVect.Normalize();
							location += dirVect * data.distanceDirection;
						}
						else if (acto) {
							location += acto->GetActorLocation();
						}
					}
					else if (data.direction == EVectorDirection::Up) {
						if (act1 && (!acto || act1->GetName() == acto->GetName())) {
							dirVect = act1->GetActorUpVector();
							dirVect.Normalize();
							location += dirVect * data.distanceDirection;
						}
						else if (acto) {
							location += acto->GetActorLocation();
						}
					}
					else if (data.direction == EVectorDirection::Down) {
						if (act1 && (!acto || act1->GetName() == acto->GetName())) {
							dirVect = -act1->GetActorUpVector();
							dirVect.Normalize();
							location += dirVect * data.distanceDirection;
						}
						else if (acto) {
							location += acto->GetActorLocation();
						}
					}
					else if (data.direction == EVectorDirection::Right) {
						if (act1 && (!acto || act1->GetName() == acto->GetName())) {
							dirVect = act1->GetActorRightVector();
							dirVect.Normalize();
							location += dirVect * data.distanceDirection;
						}
						else if (acto) {
							location += acto->GetActorLocation();
						}
					}
					else if (data.direction == EVectorDirection::Left) {
						if (act1 && (!acto || act1->GetName() == acto->GetName())) {
							dirVect = -act1->GetActorRightVector();
							dirVect.Normalize();
							location += dirVect * data.distanceDirection;
						}
						else if (acto) {
							location += acto->GetActorLocation();
						}
					}
					location += data.location;
					if (compRot) {
						location += compRot->GetComponentLocation();
					}
					else if (act1) {
						location += act1->GetActorLocation();
					}
					data.actorDebug = acto;
				}
			}
			else {
				location = data.location;
			}
			direction = location - usLocation;
			float distance = direction.Size();
			if (distance > data.toleranceError) {
				direction.Normalize();
				float dil = 1.f;
				if (act1) {
					dil = act1->CustomTimeDilation;
				}
				FVector res = direction * data.speedAmount*dil;
				FSetTransformActor transf = { intActor, data.specificChild, data.componentTag, true, data.curve, true, res, false, {0.f, 0.f, 0.f}, false, {0.f, 0.f, 0.f}, NaN, NaN, NaN, act1 };
				UInteractComponent::SetTransformActor(actor, transf, this);
				FTimerDelegate moveDel;
				moveDel.BindUFunction(this, FName("MoveToFunctionTimer"), actor, data);
				if (dil == 0.f) {
					GetWorld()->GetTimerManager().SetTimer(data.moveHandle, moveDel, data.tickTime, false);
				}
				else {
					GetWorld()->GetTimerManager().SetTimer(data.moveHandle, moveDel, data.tickTime*1.0 / dil, false);
				}
			}
			else {
				m_registerTimerArray.Remove("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::moveToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
			}
		}
	}
}

void UInteractComponent::RotateToFunction(AActor *actor, FRotateToFunction& data, UInteractComponent* interactComponent)
{
	data.savedState.tempAction = interactComponent->tempAction;
	data.savedState.tempStage = interactComponent->tempType;
	data.savedState.tempTooltipIndex = interactComponent->tempTooltipIndex;
	data.savedState.tempFunctionIndex = interactComponent->functionIndex;
	data.savedState.tempCurrentStage = interactComponent->m_currentStage;
	data.savedState.tempInElse = interactComponent->tempInElse;

	if (data.forceRotate) {
		for (int i = 0; i < interactComponent->m_registerTimerArray.Num(); i++) {
			if (interactComponent->m_registerTimerArray[i].Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::rotateToFunction))) {
				interactComponent->m_registerTimerArray.RemoveAt(i);
			}
		}
	}

	if ((data.forceRotate && !interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::rotateToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) || !data.forceRotate) {
		if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::rotateToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
			interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::rotateToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
		}
		AActor* act = UInteractComponent::GetCurrentActor(interactComponent, data.actor2, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);

		FRotator dataRot = { 0.f, 0.f, 0.f };
		if (!data.movableTarget) {
			USceneComponent* compRot = nullptr;
			if (IsValid(data.specificChild) && act) {
				TSet<UActorComponent*> comps;
				comps = act->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
						if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
							compRot = scene;
						}
					}
					else {
						if (scene && scene->GetClass() == data.specificChild) {
							compRot = scene;
						}
					}
				}
			}
			else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
				TSet<UActorComponent*> comps;
				comps = act->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (scene && scene->ComponentHasTag(data.componentTag)) {
						compRot = scene;
					}
				}
			}

			FRotator rotation = { 0.f, 0.f, 0.f };
			if (data.actor.enumName != EGetActorList::None) {
				int val = (int)data.actor.enumName - 1;
				AActor* acto = nullptr;
				if (val >= 0) {
					acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage, FExtraActorData(data.savedState.tempInElse, 0));
				}
				if (acto) {
					if (act) {
						if (compRot == nullptr) {
							if (act->GetName() == acto->GetName()) {
								rotation = act->GetActorRotation();
							}
							else {
								rotation = UKismetMathLibrary::FindLookAtRotation(act->GetActorLocation(), acto->GetActorLocation());
							}
						}
						else {
							if (compRot->GetOwner()->GetName() == acto->GetName()) {
								rotation = compRot->GetComponentRotation();
							}
							else {
								rotation = UKismetMathLibrary::FindLookAtRotation(compRot->GetComponentLocation(), acto->GetActorLocation());
							}
						}
						rotation.Add(data.rotation.Pitch, data.rotation.Yaw, data.rotation.Roll);
					}
				}
				data.actorDebug = acto;
			}
			dataRot = rotation;
		}
		FRotateToFunction rotFunc = { data.actor2, data.actor, data.specificChild, data.componentTag, dataRot, data.tickTime, data.speedAmount,
			data.toleranceError, data.movableTarget, data.targetV, data.actorDebug, data.rotateHandle, data.savedState };
		interactComponent->RotateToFunctionTimer(actor, rotFunc);

	}

}

void UInteractComponent::RotateToFunctionTimer(AActor* actor, FRotateToFunction& data)
{
	float fpsActuales = ALevelManager::GetFPS();
	float delta = 60.0f / fpsActuales;
	if (fpsActuales > 0 && fpsActuales < 60.0f) {
		data.deltaTimeAlt = delta * data.tickTime * data.timerSpeedRate;
	}
	else {
		data.deltaTimeAlt = data.tickTime * data.timerSpeedRate;
	}

	if (GetWorld() && GetOwner() && m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::rotateToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		FFunctionActorData intActor = { EGetActorList::getThisActor, {}, {data.actor2.getThisActor.getOwner} };
		FRotator rotation = { 0.f, 0.f, 0.f };
		AActor* act = UInteractComponent::GetCurrentActor(this, data.actor2, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, data.savedState.tempInElse, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage);

		USceneComponent* compRot = nullptr;
		if (IsValid(data.specificChild)) {
			TSet<UActorComponent*> comps;
			comps = act->GetComponents();
			for (int i = 0; i < comps.Array().Num(); i++) {
				USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
				if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
					if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
						compRot = scene;
					}
				}
				else {
					if (scene && scene->GetClass() == data.specificChild) {
						compRot = scene;
					}
				}
			}
		}
		else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
			TSet<UActorComponent*> comps;
			comps = act->GetComponents();
			for (int i = 0; i < comps.Array().Num(); i++) {
				USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
				if (scene && scene->ComponentHasTag(data.componentTag)) {
					compRot = scene;
				}
			}
		}

		if (data.movableTarget) {
			rotation = data.rotation;
			if (data.actor.enumName != EGetActorList::None) {
				int val = (int)data.actor.enumName - 1;
				AActor* acto = nullptr;
				if (val >= 0) {
					acto = actorFunctionMap[val].Execute(this, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage, FExtraActorData(data.savedState.tempInElse, 0));
				}
				if (acto) {
					if (act) {
						if (compRot == nullptr) {
							if (act->GetName() == acto->GetName()) {
								rotation = act->GetActorRotation();
							}
							else {
								rotation = UKismetMathLibrary::FindLookAtRotation(act->GetActorLocation(), acto->GetActorLocation());
							}
						}
						else {
							if (compRot->GetOwner()->GetName() == acto->GetName()) {
								rotation = compRot->GetComponentRotation();
							}
							else {
								rotation = UKismetMathLibrary::FindLookAtRotation(compRot->GetComponentLocation(), acto->GetActorLocation());
							}
						}
						rotation.Add(data.rotation.Pitch, data.rotation.Yaw, data.rotation.Roll);
					}
				}
				data.actorDebug = acto;
			}
		}
		else {
			rotation = data.rotation;
		}
		FRotator newRot = { 0.f, 0.f, 0.f };

		float dil = 1.f;
		if (act) {
			dil = act->CustomTimeDilation;
		}
		if (!compRot) {
			if (dil == 0.f) {
				newRot = UKismetMathLibrary::RInterpTo(act->GetActorRotation(), rotation, data.deltaTimeAlt, data.speedAmount);
			}
			else {
				newRot = UKismetMathLibrary::RInterpTo(act->GetActorRotation(), rotation, data.deltaTimeAlt*1.0 / dil, data.speedAmount*dil);
			}
			if (!rotation.Equals(act->GetActorRotation(), data.toleranceError)) {
				act->AddActorLocalRotation(FQuat(newRot - act->GetActorRotation()));
				FTimerDelegate moveDel;
				moveDel.BindUFunction(this, FName("RotateToFunctionTimer"), actor, data);
				if (dil == 0.f) {
					GetWorld()->GetTimerManager().SetTimer(data.rotateHandle, moveDel, data.tickTime, false);
				}
				else {
					GetWorld()->GetTimerManager().SetTimer(data.rotateHandle, moveDel, data.tickTime*1.0 / dil, false);
				}
			}
			else {
				m_registerTimerArray.Remove("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::rotateToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
			}
		}
		else {
			if (dil == 0.f) {
				newRot = UKismetMathLibrary::RInterpTo(compRot->GetComponentRotation(), rotation, data.deltaTimeAlt, data.speedAmount);
			}
			else {
				newRot = UKismetMathLibrary::RInterpTo(compRot->GetComponentRotation(), rotation, data.deltaTimeAlt*1.0 / dil, data.speedAmount*dil);
			}
			if (!rotation.Equals(compRot->GetComponentRotation(), data.toleranceError)) {
				compRot->AddLocalRotation(FQuat(newRot - compRot->GetComponentRotation()));
				FTimerDelegate moveDel;
				moveDel.BindUFunction(this, FName("RotateToFunctionTimer"), actor, data);
				if (dil == 0.f) {
					GetWorld()->GetTimerManager().SetTimer(data.rotateHandle, moveDel, data.tickTime, false);
				}
				else {
					GetWorld()->GetTimerManager().SetTimer(data.rotateHandle, moveDel, data.tickTime*1.0 / dil, false);
				}
			}
			else {
				m_registerTimerArray.Remove("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::rotateToFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
			}
		}
	}
}

void UInteractComponent::SpawnerFunction(AActor *actor, FSpawnerFunction& data, UInteractComponent* interactComponent)
{

	FTransform Transform = FTransform();
	USceneComponent* compRot = nullptr;

	float random = FMath::RandRange(0.0f, 100.0f);

	UInteractComponent* intBPSpawn = nullptr;
	TArray<FInteractData> dataSpawn;

	if (!data.spawnAsChild) {
		if (data.actor.enumName != EGetActorList::None) {
			int val = (int)data.actor.enumName - 1;
			AActor* pivotActor = nullptr;
			if (val >= 0) {
				pivotActor = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));

				if (pivotActor) {
					if (IsValid(data.specificChild)) {
						TSet<UActorComponent*> comps;
						comps = pivotActor->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++) {
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (data.componentTag.ToString() != "" && data.componentTag != NAME_None && (scene && scene->GetClass() == data.specificChild) || scene->ComponentHasTag(data.componentTag)) {
								compRot = scene;
							}
						}
					}
					else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None)
					{
						TSet<UActorComponent*> comps;
						comps = pivotActor->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++)
						{
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (scene && scene->ComponentHasTag(data.componentTag))
							{
								compRot = scene;
							}
						}
					}
				}
			}

			if (compRot)
			{
				Transform.SetLocation(data.position + compRot->GetComponentLocation());
				Transform.SetRotation(FQuat(data.rotation + compRot->GetComponentRotation()));
				intBPSpawn = UInteractComponent::GetInteractBP(data.singleSpawn);
				if (intBPSpawn) {
					intBPSpawn->ImportInteractDataInternal();
					dataSpawn = intBPSpawn->interactDataArray;
					intBPSpawn->interactDataArray.Empty();
				}
				data.spawned = pivotActor->GetWorld()->SpawnActorDeferred<AActor>(data.singleSpawn, Transform, pivotActor, pivotActor->Instigator, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			}
			else if (pivotActor)
			{
				Transform.SetLocation(data.position + pivotActor->GetActorLocation());
				Transform.SetRotation(FQuat(data.rotation + pivotActor->GetActorRotation()));
				intBPSpawn = UInteractComponent::GetInteractBP(data.singleSpawn);
				if (intBPSpawn) {
					intBPSpawn->ImportInteractDataInternal();
					dataSpawn = intBPSpawn->interactDataArray;
					intBPSpawn->interactDataArray.Empty();
				}
				data.spawned = pivotActor->GetWorld()->SpawnActorDeferred<AActor>(data.singleSpawn, Transform, pivotActor, pivotActor->Instigator, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			}

		}
		else {

			Transform.SetLocation(data.position);
			Transform.SetRotation(FQuat(data.rotation));
			intBPSpawn = UInteractComponent::GetInteractBP(data.singleSpawn);
			if (intBPSpawn) {
				intBPSpawn->ImportInteractDataInternal();
				dataSpawn = intBPSpawn->interactDataArray;
				intBPSpawn->interactDataArray.Empty();
			}
			data.spawned = interactComponent->GetWorld()->SpawnActorDeferred<AActor>(data.singleSpawn, Transform, interactComponent->GetOwner(), interactComponent->GetOwner()->Instigator, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		}

		if (data.spawned) {
			data.spawned->FinishSpawning(Transform);
			if (intBPSpawn) {
				intBPSpawn->interactDataArray = dataSpawn;
			}
			UInteractComponent* intCompThis = Cast<UInteractComponent>(data.spawned->GetComponentByClass(UInteractComponent::StaticClass()));
			if (intCompThis) {
				intCompThis->interactDataArray = dataSpawn;
	  			intCompThis->InitializeTooltip();
				IPlayerInterface* player = Cast<IPlayerInterface>(UGameplayStatics::GetPlayerCharacter(intCompThis->GetWorld(), 0));
				if (player) {
					player->ExecuteNearTriggers();
				}
			}
			data.spawned->Tags = data.spawnedActorTags;
			interactComponent->ReturnValue(&data.customReturn, data.spawned);
			CONFIRM_RETURN(interactComponent, data, FSpawnerFunction)
		}
	}
	else if (data.spawnAsChild)
	{
		TSet<UActorComponent*> comps;
		comps = interactComponent->GetOwner()->GetComponents();
		for (int i = 0; i < comps.Array().Num(); i++) {
			UChildActorComponent* spawnChild = Cast<UChildActorComponent>(comps.Array()[i]);
			if (data.componentTag.ToString() != "" && data.componentTag != NAME_None && spawnChild) {
				if (spawnChild->ComponentTags.Num() > 0 && spawnChild->ComponentHasTag(data.componentTag))
				{
					intBPSpawn = UInteractComponent::GetInteractBP(data.singleSpawn);
					if (intBPSpawn) {
						intBPSpawn->ImportInteractDataInternal();
						dataSpawn = intBPSpawn->interactDataArray;
						intBPSpawn->interactDataArray.Empty();
					}
					spawnChild->SetChildActorClass(data.singleSpawn);
					spawnChild->CreateChildActor();
					if (intBPSpawn) {
						intBPSpawn->interactDataArray = dataSpawn;
					}
					UInteractComponent* intCompThis = Cast<UInteractComponent>(spawnChild->GetChildActor()->GetComponentByClass(UInteractComponent::StaticClass()));
					if (intCompThis) {
						intCompThis->interactDataArray = dataSpawn;
 						intCompThis->InitializeTooltip();
						IPlayerInterface* player = Cast<IPlayerInterface>(UGameplayStatics::GetPlayerCharacter(intCompThis->GetWorld(), 0));
						if (player) {
							player->ExecuteNearTriggers();
						}
					}
					spawnChild->SetWorldScale3D({ 1,1,1 });
					spawnChild->ComponentTags.Append(data.spawnedActorTags);
				}
			}
		}
	}
	

}

void UInteractComponent::RelocateFunction(AActor *actor, FRelocateFunction& data, UInteractComponent* interactComponent) {
	data.savedState.tempAction = interactComponent->tempAction;
	data.savedState.tempStage = interactComponent->tempType;
	data.savedState.tempTooltipIndex = interactComponent->tempTooltipIndex;
	data.savedState.tempFunctionIndex = interactComponent->functionIndex;
	data.savedState.tempCurrentStage = interactComponent->m_currentStage;
	data.savedState.tempInElse = interactComponent->tempInElse;
	data.actor.enumName = EGetActorList::getThisActor;
	//Si no existe en el registro, se registra porque se tiene que usar
	if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::relocateFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::relocateFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
	}

	interactComponent->RelocateFunctionTimer(actor, data);
}


void UInteractComponent::RelocateFunctionTimer(AActor *actor, FRelocateFunction& data) {
	ACharacter *player = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	float fpsActuales = ALevelManager::GetFPS();
	float delta = 60.0f / fpsActuales;
	if (fpsActuales > 0 && fpsActuales < 60.0f) {
		data.deltaTimeAlt = delta * data.DeltaTime * data.timerSpeedRate;
	}
	else {
		data.deltaTimeAlt = data.DeltaTime * data.timerSpeedRate;
	}
	if (player && GetWorld() && GetOwner() && m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::relocateFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		if (data.actor.enumName != EGetActorList::None) {
			int val = (int)data.actor.enumName - 1;
			AActor* m_actor = nullptr;
			if (val >= 0) {
				m_actor = actorFunctionMap[val].Execute(this, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage, FExtraActorData(data.savedState.tempInElse, 0));
			}
			TArray<UActorComponent*> arrows;
				if (m_actor) {
					arrows = m_actor->GetComponentsByTag(UArrowComponent::StaticClass(), data.arrowTag);
				}

				if (arrows.Num() != 0) {
					UArrowComponent* relocateArrow = Cast <UArrowComponent>(arrows[0]);

					if (relocateArrow) {

						float m_abs = UKismetMathLibrary::Abs(relocateArrow->GetComponentLocation().X - player->GetActorLocation().X);
						FVector direction = relocateArrow->GetComponentLocation() - player->GetActorLocation();
						if (m_abs > data.errorMarginLocation && !data.isRotating) {
							//AddMovementInput
							player->AddMovementInput(direction, 1.0f, data.forceMovement);
							FTimerDelegate ObjDel;
							ObjDel.BindUFunction(this, FName("RelocateFunctionTimer"), actor, data);
							GetWorld()->GetTimerManager().SetTimer(data.relocateFunctionTimer, ObjDel, data.DeltaTime, false);
						}
						else {
							if (data.rotateWhenFinish) {
								data.isRotating = true;
								FRotator playerRotation = player->GetControlRotation();
								FRotator arrowRotation = relocateArrow->GetComponentRotation();
								FRotator totalRotation = UKismetMathLibrary::RInterpTo(playerRotation, arrowRotation, data.deltaTimeAlt, data.rotationSpeed);
								if (totalRotation.Equals(arrowRotation, data.errorMarginRotation)) {
									//Finish
									if (data.nextStageRelocateDone != -1) {
										this->NextStage(data.nextStageRelocateDone);
									}
								}
								else {
									player->GetController()->SetControlRotation(totalRotation);

									FTimerDelegate ObjDel;
									ObjDel.BindUFunction(this, FName("RelocateFunctionTimer"), actor, data);
									GetWorld()->GetTimerManager().SetTimer(data.relocateFunctionTimer, ObjDel, data.DeltaTime, false);
								}
							}
							else {
								if (data.nextStageRelocateDone != -1) {
									this->NextStage(data.nextStageRelocateDone);
								}
							}
						}


					}
				}
			//}
		}
	}
}

void UInteractComponent::ObjectInspector(AActor *actor, FObjectInspector& data, UInteractComponent* interactComponent)
{
	data.savedState.tempAction = interactComponent->tempAction;
	data.savedState.tempStage = interactComponent->tempType;
	data.savedState.tempTooltipIndex = interactComponent->tempTooltipIndex;
	data.savedState.tempFunctionIndex = interactComponent->functionIndex;
	data.savedState.tempCurrentStage = interactComponent->m_currentStage;
	data.savedState.tempInElse = interactComponent->tempInElse;
	//Si no existe en el registro, se registra porque se tiene que usar
	if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerList"), ETimerList::objectInspector) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerList"), ETimerList::objectInspector) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
	}
	interactComponent->ObjectInspectorTimer(actor, data);
}
void UInteractComponent::ObjectInspectorTimer(AActor *actor, FObjectInspector& data)
{
	
	ACharacter *player = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	float fpsActuales = ALevelManager::GetFPS();
	float delta = 60.0f / fpsActuales;
	if (fpsActuales > 0 && fpsActuales < 60.0f) {
		data.deltaTimeAlt = delta * data.DeltaTime * data.timerSpeedRate;
	}
	else {
		data.deltaTimeAlt = data.DeltaTime* data.timerSpeedRate;
	}

	if (player) {
		//TODO faltaria hacer aqui lo de items ya ch
		//activateLigths = true;
		//FVector start = player->getCameraComponent()->getDefaultCamera()->GetComponentLocation();
		//FVector forwardCameraVector = player->getCameraComponent()->getDefaultCamera()->GetForwardVector() * 500;
		//FVector end = start + forwardCameraVector;
		//TArray<class AActor*> ActorsToIgnore;
		//FHitResult OutHit;
		//if (data.debugSphereOn) {
		//	UKismetSystemLibrary::SphereTraceSingle(player->GetWorld(), start, end, data.sphereSize, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHit, false);
		//}
		//else {
		//	UKismetSystemLibrary::SphereTraceSingle(player->GetWorld(), start, end, data.sphereSize, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, false);
		//}

		//if (data.canInspectionate && GetWorld() && GetOwner() && m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerList"), ETimerList::objectInspector) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		//	/*	FString TheFloatStr = FString::SanitizeFloat(player->getYCameraValue());
		//		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);*/
		//	player->setCameraIsBlocked(true);
		//	int idIsDoing = -1;
		//	for (int i = 0; i < m_variablesArray.Num(); i++) {
		//		if (m_variablesArray[i].Contains("Id=IsInspection:")) {
		//			idIsDoing = i;
		//		}
		//	}

		//	if (data.actor.enumName != EGetActorList::None) {
		//		int val = (int)data.actor.enumName - 1;
		//		AActor* objToInspect = nullptr;
		//		if (val >= 0) {
		//			objToInspect = actorFunctionMap[val].Execute(this, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage, data.savedState.tempInElse);
		//		}
		//		AItem* itemActor = Cast<AItem>(objToInspect);
		//		if (itemActor) {
		//			TArray<UActorComponent*> arrows = itemActor->GetComponentsByTag(UArrowComponent::StaticClass(), "PointArrow");
		//			UArrowComponent* pointArrow = Cast <UArrowComponent>(arrows[0]);
		//			if (pointArrow) {
		//				TArray<UMeshComponent*> actorMeshes = itemActor->GetActorMesh();
		//				if (actorMeshes[0] && !data.foundingClue) {
		//					UMeshComponent* mesh = actorMeshes[0];

		//					if (!doOnceInspector) {
		//						doOnceInspector = true;
		//						initPosGlobal = mesh->GetComponentLocation();
		//						initRotGlobal = mesh->GetComponentRotation();
		//					}
		//					data.objectRotator = mesh->GetComponentRotation();
		//					//Turn inspectable object left or right
		//					data.makeRotatorOfAxis = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, player->getXCameraValue() * data.rotationVelocityLeftRight * -1.0f);
		//					mesh->AddWorldRotation(data.makeRotatorOfAxis);

		//					//Turn inspectable object up or down
		//					float rotationFactorX = 0.0f;
		//					float rotationFactorY = 0.0f;
		//					FRotator camera = player->getCameraComponent()->getDefaultCamera()->GetComponentRotation();
		//					if (UKismetMathLibrary::InRange_FloatFloat(camera.Yaw, -90.0f, 0.0f)) {
		//						rotationFactorX = UKismetMathLibrary::MapRangeClamped(camera.Yaw, -90.0f, 0.0f, 1.0f, 0.0f);
		//						rotationFactorY = UKismetMathLibrary::MapRangeClamped(camera.Yaw, 0.0f, -90.0f, 1.0f, 0.0f);
		//						//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, "CUADRANTE 0");
		//					}
		//					else if (UKismetMathLibrary::InRange_FloatFloat(camera.Yaw, -180.0f, -90.0f)) {
		//						rotationFactorX = UKismetMathLibrary::MapRangeClamped(camera.Yaw, -180.0f, -90.0f, 1.0f, 0.0f);
		//						rotationFactorY = UKismetMathLibrary::MapRangeClamped(camera.Yaw, -90.0f, -180.0f, 0.0f, -1.0f);
		//						//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, "CUADRANTE 1");
		//					}
		//					else if (UKismetMathLibrary::InRange_FloatFloat(camera.Yaw, 90.0f, 180.0f)) {
		//						rotationFactorX = UKismetMathLibrary::MapRangeClamped(camera.Yaw, 90.0f, 180.0f, -1.0f, 0.0f);
		//						rotationFactorY = UKismetMathLibrary::MapRangeClamped(camera.Yaw, 90.0f, 180.0f, 0.0f, -1.0f);
		//						//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, "CUADRANTE 2");
		//					}
		//					else if (UKismetMathLibrary::InRange_FloatFloat(camera.Yaw, 0.0f, 90.0f)) {
		//						rotationFactorX = UKismetMathLibrary::MapRangeClamped(camera.Yaw, 0.0f, 90.0f, 0.0f, -1.0f);
		//						rotationFactorY = UKismetMathLibrary::MapRangeClamped(camera.Yaw, 0.0f, 90.0f, 1.0f, 0.0f);
		//						//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, "CUADRANTE 3");
		//					}

		//					data.makeRotatorOfAxis = UKismetMathLibrary::MakeRotator(player->getYCameraValue() * rotationFactorX * data.rotationVelocityUpDown, player->getYCameraValue()*rotationFactorY * data.rotationVelocityUpDown, 0.0f);
		//					mesh->AddWorldRotation(data.makeRotatorOfAxis);

		//					//Turn inspectable object in Z axis
		//					data.makeRotatorOfAxis = UKismetMathLibrary::MakeRotator(player->getXLeftJoystickValue() * rotationFactorY * data.rotationVelocityRoll, player->getXLeftJoystickValue() * rotationFactorX * data.rotationVelocityRoll*-1.0f, 0.0f);
		//					mesh->AddWorldRotation(data.makeRotatorOfAxis);

		//					//Go to te vector point
		//					FVector VectorLerp = UKismetMathLibrary::VLerp(mesh->GetComponentLocation(), pointArrow->GetComponentLocation(), data.deltaTimeAlt);
		//					mesh->SetWorldLocation(VectorLerp);
		//				}

		//				UMeshComponent* clueMesh = nullptr;
		//				for (int k = 0; k < actorMeshes.Num(); k++) {
		//					if (actorMeshes[k]->ComponentTags.Contains("clue")) {
		//						clueMesh = actorMeshes[k];
		//						break;
		//					}
		//				}
		//				if (clueMesh) {
		//					if (OutHit.GetComponent() == clueMesh) {
		//						/*auto inicio = chrono::steady_clock::now();
		//						auto duration = chrono::duration_cast<chrono::seconds>(end - start).count();*/
		//						if (data.contadorObservador >= data.timePastUntilFoundClue) {
		//							data.foundingClue = true;
		//							FVector cameraPos = player->getCameraComponent()->getDefaultCamera()->GetComponentLocation();
		//							FVector lineForward = player->getCameraComponent()->getDefaultCamera()->GetForwardVector() * data.clueDistancePosition;
		//							FVector clueFinalPos = cameraPos + lineForward;
		//							FVector clueLerp = UKismetMathLibrary::VLerp(clueMesh->GetComponentLocation(), clueFinalPos, data.deltaTimeAlt);
		//							clueMesh->SetWorldLocation(clueLerp);
		//							if (data.nextStageClueFound != -1) {
		//								UInteractComponent::JumpStageGlobal(GetOwner(), "", 0.0f, data.nextStageClueFound, false);
		//								//this->NextStage(data.nextStageClueFound);
		//							}
		//							if (data.contadorClue >= data.timePastUntilClueDisappear) {
		//								data.contadorClue = 0;
		//								data.contadorObservador = 0;
		//								clueMesh->SetVisibility(false);
		//								clueMesh->SetCollisionProfileName(TEXT("OverlapAll"));
		//								tempDelay = 0.0f;
		//								data.foundingClue = false;
		//							}
		//							else {
		//								data.contadorClue += GetWorld()->GetDeltaSeconds();
		//							}

		//						}
		//						else {
		//							data.contadorObservador += GetWorld()->GetDeltaSeconds();
		//						}
		//					}
		//					else {
		//						data.contadorObservador = 0;
		//						data.contadorClue = 0;
		//					}
		//				}
		//			}
		//		}

		//	}

		//	if (idIsDoing == -1) {
		//		FTimerDelegate ObjDel;
		//		ObjDel.BindUFunction(this, FName("ObjectInspectorTimer"), actor, data);
		//		GetWorld()->GetTimerManager().SetTimer(data.ObjInspectorTimer, ObjDel, data.DeltaTime, false);
		//	}
		//	else {
		//		doOnceInspector = false;
		//		player->setCameraIsBlocked(false);
		//		m_variablesArray.RemoveAt(idIsDoing, 1, true);
		//		data.finishingCounter = 5;
		//		activateLigths = false;
		//		data.contadorObservador = 0;
		//	}
		//}
		//else {
		//	int idIsDoing = -1;
		//	for (int i = 0; i < m_variablesArray.Num(); i++) {
		//		if (m_variablesArray[i].Contains("Id=IsInspection:")) {
		//			idIsDoing = i;
		//		}
		//	}


		//	if (idIsDoing == -1) {
		//		FString playerIsBloqued = "Id=IsInspection:" + FString::FromInt((int)data.canInspectionate);
		//		m_variablesArray.Add(playerIsBloqued);
		//		idIsDoing = m_variablesArray.Num() - 1;
		//	}

		//	if (data.actor.enumName != EGetActorList::None) {
		//		int val = (int)data.actor.enumName - 1;
		//		AActor* objToInspect = nullptr;
		//		if (val >= 0) {
		//			objToInspect = actorFunctionMap[val].Execute(this, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage, data.savedState.tempInElse);
		//		}
		//		AItem* itemActor = Cast<AItem>(objToInspect);
		//		if (itemActor) {
		//			TArray<UMeshComponent*> actorMeshes = itemActor->GetActorMesh();
		//			if (actorMeshes[0]) {
		//				UMeshComponent* mesh = actorMeshes[0];
		//				if (mesh->GetComponentRotation().Equals(initRotGlobal, 2.f) && mesh->GetComponentLocation().Equals(initPosGlobal, 1.f)) {
		//					FRotator objectLerpRot = UKismetMathLibrary::RLerp(mesh->GetComponentRotation(), initRotGlobal, data.deltaTimeAlt, true);
		//					mesh->SetWorldRotation(objectLerpRot);
		//					FVector VectorLerp = UKismetMathLibrary::VLerp(mesh->GetComponentLocation(), initPosGlobal, data.deltaTimeAlt);
		//					mesh->SetWorldLocation(VectorLerp);
		//					data.finishingCounter--;
		//				}
		//				else {
		//					FRotator objectLerpRot = UKismetMathLibrary::RLerp(mesh->GetComponentRotation(), initRotGlobal, data.deltaTimeAlt, true);
		//					mesh->SetWorldRotation(objectLerpRot);
		//					FVector VectorLerp = UKismetMathLibrary::VLerp(mesh->GetComponentLocation(), initPosGlobal, data.deltaTimeAlt);
		//					mesh->SetWorldLocation(VectorLerp);
		//				}
		//			}
		//		}
		//	}

		//	if (data.finishingCounter != 0) {
		//		FTimerDelegate ObjDel;
		//		ObjDel.BindUFunction(this, FName("ObjectInspectorTimer"), actor, data);
		//		GetWorld()->GetTimerManager().SetTimer(data.ObjInspectorTimer, ObjDel, data.DeltaTime, false);
		//	}
		//	else {
		//		doOnceInspector = false;
		//		player->setCameraIsBlocked(false);
		//		m_variablesArray.RemoveAt(idIsDoing, 1, true);
		//		data.finishingCounter = 5;
		//		activateLigths = false;
		//	}
		//}
	}
}
void UInteractComponent::CurveLineTrace(AActor *actor, FCurveLineTrace& data, UInteractComponent* interactComponent)
{
	data.savedState.tempAction = interactComponent->tempAction;
	data.savedState.tempStage = interactComponent->tempType;
	data.savedState.tempTooltipIndex = interactComponent->tempTooltipIndex;
	data.savedState.tempFunctionIndex = interactComponent->functionIndex;
	data.savedState.tempCurrentStage = interactComponent->m_currentStage;
	data.savedState.tempInElse = interactComponent->tempInElse;
	//Si no existe en el registro, se registra porque se tiene que usar
	if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerList"), ETimerList::curveLineTrace) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerList"), ETimerList::curveLineTrace) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
	}
	interactComponent->CurveLineTraceTimer(actor, data);
}
void UInteractComponent::CurveLineTraceTimer(AActor *actor, FCurveLineTrace& data)
{

	ACharacter* character = Cast<ACharacter>(actor);

	if (UInteractComponent::IsTimerRegister(this, ETimerList::curveLineTrace, data.savedState.tempCurrentStage, data.savedState.tempFunctionIndex, data.savedState.tempStage, data.savedState.tempAction)) {

		FVector actorForwardVector;
		FRotator cameraRot;

		if (data.actor.enumName != EGetActorList::None) {
			int val = (int)data.actor.enumName - 1;
			AActor* actorToUse = nullptr;
			if (val >= 0) {
				actorToUse = actorFunctionMap[val].Execute(this, actor, data.savedState.tempAction, data.savedState.tempTooltipIndex, data.savedState.tempStage, -1, data.savedState.tempFunctionIndex, data.savedState.tempCurrentStage, FExtraActorData(data.savedState.tempInElse, 0));
			}
			//ISM_PRINTLOG(actorForwardVector.ToString())

			if (actorToUse) {

			if (!character) {
				character = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
				actorForwardVector = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();
			}
			else {
				actorForwardVector = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();
				cameraRot = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation();
			}
				//operaciones para la curva de bezier
				FVector op1 = actorForwardVector * data.CP1Float;
				FVector op2 = actorForwardVector * data.CP2Float;
				FVector op3 = actorForwardVector * data.CPEndFloat;

				FVector op4 = op1 + actorToUse->GetActorLocation();
				FVector op5 = op2 + actorToUse->GetActorLocation();
				FVector op6 = op3 + actorToUse->GetActorLocation();

				FVector CP1 = op4 + data.CP1Vector;
				FVector CP2 = op5 + data.CP2Vector;
				FVector CPEnd = op6 + data.CPEndVector;

				FVector CPStart = actorToUse->GetActorLocation() + data.distancedOffsetToStartDrawing;
				CPEnd = actorToUse->GetActorLocation() + data.CPEndVector;
				//Curva de Bezier
				data.beamComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), data.particles, CPStart, FRotator::ZeroRotator, true);
				data.m_BeamsArray.Add(data.beamComp);
				if (data.beamComp) {
					data.beamComp->SetBeamSourcePoint(0, CPStart, 0);
					data.beamComp->SetBeamTargetPoint(0, CPEnd, 0);
				}
			}
			FTimerDelegate ObjDel;
			ObjDel.BindUFunction(this, FName("PlaneLineTraceTimer"), actor, data);
			GetWorld()->GetTimerManager().SetTimer(data.m_Timer, ObjDel, data.DeltaTime, false);
		}
	}
}

void UInteractComponent::SetRagdoll(AActor* actor, FSetRagdoll& data, UInteractComponent* interactComponent)
{
	ACharacter* character;
	if (data.m_actor == nullptr)
	{
		data.m_actor = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	}
	if (data.m_actor)
	{
		character = Cast<ACharacter>(data.m_actor);

		if (character)
		{
			character->GetMesh()->SetAllBodiesNotifyRigidBodyCollision(true);
			character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			character->GetMesh()->UpdateKinematicBonesToAnim(character->GetMesh()->GetComponentSpaceTransforms(), ETeleportType::TeleportPhysics, true);
			character->GetMesh()->SetSimulatePhysics(true);
			character->GetMesh()->RefreshBoneTransforms();
			character->GetMesh()->SetGenerateOverlapEvents(true);
			character->GetMesh()->SetShouldUpdatePhysicsVolume(true);
			character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			character->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
		}
	}
	else
	{
		FString debugText = FString(TEXT("FUNCTION: SetRagdoll || ERROR: The Target Actor is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		ISM_ULOG_EDIT("%s", Error, *debugText);
	}
}

void UInteractComponent::AddImpulseFunction(AActor* actor, FAddImpulseFunction& data, UInteractComponent* interactComponent)
{
	ACharacter* character = Cast<ACharacter>(actor);

	if (!character)
	{
		character = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(interactComponent->GetWorld(), 0));
	}
	if (!character)
	{
		FString debugText = FString(TEXT("FUNCTION: AddImpulseFunction || ERROR: The Actor that applies the impulse is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		ISM_ULOG_EDIT("%s", Error, *debugText);
	}

	FName boneName = FName(TEXT("None"));
	switch (data.ImpulsedBone)
	{
	case EBone_Socket::ROOT:
		boneName = FName(TEXT("root"));
		break;
	case EBone_Socket::PELVIS:
		boneName = FName(TEXT("Pelvis"));
		break;
	case EBone_Socket::SPINE_01:
		boneName = FName(TEXT("spine_01"));
		break;
	case EBone_Socket::SPINE_02:
		boneName = FName(TEXT("spine_02"));
		break;
	case EBone_Socket::SPINE_03:
		boneName = FName(TEXT("spine_01"));
		break;
	case EBone_Socket::NECK:
		boneName = FName(TEXT("neck_01"));
		break;
	case EBone_Socket::HEAD:
		boneName = FName(TEXT("head"));
		break;
	case EBone_Socket::TOPHEAD:
		boneName = FName(TEXT("End"));
		break;
	case EBone_Socket::L_CLAVICLE:
		boneName = FName(TEXT("clavicle_l"));
		break;
	case EBone_Socket::L_UPPERARM:
		boneName = FName(TEXT("upperarm_l"));
		break;
	case EBone_Socket::L_LOWERARM:
		boneName = FName(TEXT("lowerarm_l"));
		break;
	case EBone_Socket::L_HAND:
		boneName = FName(TEXT("hand_l"));
		break;
	case EBone_Socket::L_INDEX_01:
		boneName = FName(TEXT("index_01_l"));
		break;
	case EBone_Socket::L_INDEX_02:
		boneName = FName(TEXT("index_02_l"));
		break;
	case EBone_Socket::L_INDEX_03:
		boneName = FName(TEXT("index_03_l"));
		break;
	case EBone_Socket::L_INDEX_04:
		boneName = FName(TEXT("LeftHandIndex4"));
		break;
	case EBone_Socket::R_CLAVICLE:
		boneName = FName(TEXT("clavicle_r"));
		break;
	case EBone_Socket::R_UPPERARM:
		boneName = FName(TEXT("upperarm_r"));
		break;
	case EBone_Socket::R_LOWERARM:
		boneName = FName(TEXT("lowerarm_r"));
		break;
	case EBone_Socket::R_HAND:
		boneName = FName(TEXT("hand_r"));
		break;
	case EBone_Socket::R_INDEX_01:
		boneName = FName(TEXT("index_01_r"));
		break;
	case EBone_Socket::R_INDEX_02:
		boneName = FName(TEXT("index_02_r"));
		break;
	case EBone_Socket::R_INDEX_03:
		boneName = FName(TEXT("index_03_r"));
		break;
	case EBone_Socket::R_INDEX_04:
		boneName = FName(TEXT("RightHandIndex4"));
		break;
	case EBone_Socket::L_THIGH:
		boneName = FName(TEXT("thigh_l"));
		break;
	case EBone_Socket::L_CALF:
		boneName = FName(TEXT("calf_l"));
		break;
	case EBone_Socket::L_FOOT:
		boneName = FName(TEXT("foot_l"));
		break;
	case EBone_Socket::L_BALL:
		boneName = FName(TEXT("ball_l"));
		break;
	case EBone_Socket::L_TOECAP:
		boneName = FName(TEXT("End_001"));
		break;
	case EBone_Socket::R_THIGH:
		boneName = FName(TEXT("thigh_r"));
		break;
	case EBone_Socket::R_CALF:
		boneName = FName(TEXT("calf_r"));
		break;
	case EBone_Socket::R_FOOT:
		boneName = FName(TEXT("foot_r"));
		break;
	case EBone_Socket::R_BALL:
		boneName = FName(TEXT("ball_r"));
		break;
	case EBone_Socket::R_TOECAP:
		boneName = FName(TEXT("End_002"));
		break;
	default:
		break;
	}

	if (data.m_actor == nullptr)
	{
		data.m_actor = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	}

	if (data.m_actor)
	{
		IInteractInterface* intInt = Cast<IInteractInterface>(data.m_actor);
		ACharacter* otherCharacter = nullptr;
		AActor* otherItem = nullptr;
		TArray <UMeshComponent *> m_itemMeshes;
		FVector dirVec = { 0.f, 0.f, 0.f };
		FVector finalVec = { 0.f, 0.f, 0.f };

		//if (intInt) {
		if (data.m_actor->IsA(ACharacter::StaticClass()))
		{
			otherCharacter = Cast<ACharacter>(data.m_actor);
			otherCharacter->GetMesh()->SetSimulatePhysics(true);

		}
		else if (data.m_actor->IsA(AActor::StaticClass()))
		{
			otherItem = Cast<AActor>(data.m_actor);
			if (intInt) {
				m_itemMeshes = intInt->GetActorMesh();
			}
			else {
				TArray<UActorComponent*> ActorCArray = actor->GetComponentsByClass(UMeshComponent::StaticClass());
				for (int i = 0; i < ActorCArray.Num(); i++) {
					m_itemMeshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
				}
			}
			for (int i = 0; i < m_itemMeshes.Num(); i++)
			{
				m_itemMeshes[i]->SetSimulatePhysics(true);
			}
		}
		else
		{
			TArray<UMeshComponent*> meshes;
			TArray<UActorComponent*> ActorCArray = data.m_actor->GetComponentsByClass(UMeshComponent::StaticClass());
			for (int i = 0; i < ActorCArray.Num(); i++) {
				meshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
			}
			for (int i = 0; i < meshes.Num(); i++)
			{
				meshes[i]->SetSimulatePhysics(true);
			}
		}
		//}

		if (data.BaseDirections)
		{
			if (otherCharacter)
			{
				if (data.Direction == EVectorDirection::Forward)
				{
					dirVec = otherCharacter->GetActorForwardVector();
					dirVec.Normalize();
					finalVec = dirVec * data.DistanceDirection;
				}
				else if (data.Direction == EVectorDirection::Backward)
				{
					dirVec = -otherCharacter->GetActorForwardVector();
					dirVec.Normalize();
					finalVec = dirVec * data.DistanceDirection;
				}
				else if (data.Direction == EVectorDirection::Up)
				{
					dirVec = otherCharacter->GetActorUpVector();
					dirVec.Normalize();
					finalVec = dirVec * data.DistanceDirection;
				}
				else if (data.Direction == EVectorDirection::Down)
				{
					dirVec = -otherCharacter->GetActorUpVector();
					dirVec.Normalize();
					finalVec = dirVec * data.DistanceDirection;
				}
				else if (data.Direction == EVectorDirection::Right)
				{
					dirVec = otherCharacter->GetActorRightVector();
					dirVec.Normalize();
					finalVec = dirVec * data.DistanceDirection;
				}
				else if (data.Direction == EVectorDirection::Left)
				{
					dirVec = -otherCharacter->GetActorRightVector();
					dirVec.Normalize();
					finalVec = dirVec * data.DistanceDirection;
				}

				switch (data.ImpulseType)
				{
				case EImpulseType::AddImpulseAtLocation:
					otherCharacter->GetMesh()->AddImpulseAtLocation(finalVec, otherCharacter->GetActorLocation(), boneName);
					break;
				case EImpulseType::AddImpulse:
					otherCharacter->GetMesh()->AddImpulse(finalVec, boneName);
					break;
				case EImpulseType::AddAngularImpulseInDegrees:
					otherCharacter->GetMesh()->AddAngularImpulseInDegrees(finalVec, boneName);
					break;
				case EImpulseType::AddAngularImpulseInRadians:
					otherCharacter->GetMesh()->AddAngularImpulseInRadians(finalVec, boneName);
					break;
				case EImpulseType::AddForceAtLocation:
					otherCharacter->GetMesh()->AddForceAtLocation(finalVec, otherCharacter->GetActorLocation(), boneName);
					break;
				case EImpulseType::AddForceAtLocationLocal:
					otherCharacter->GetMesh()->AddForceAtLocationLocal(finalVec, otherCharacter->GetActorLocation(), boneName);
					break;
				case EImpulseType::AddForce:
					otherCharacter->GetMesh()->AddForce(finalVec, boneName);
					break;
				default:
					break;
				}
			}
			else if (otherItem)
			{
				for (int i = 0; i < m_itemMeshes.Num(); i++)
				{
					if (data.Direction == EVectorDirection::Forward)
					{
						dirVec = otherItem->GetActorForwardVector();
						dirVec.Normalize();
						finalVec = dirVec * data.DistanceDirection;
					}
					else if (data.Direction == EVectorDirection::Backward)
					{
						dirVec = -otherItem->GetActorForwardVector();
						dirVec.Normalize();
						finalVec = dirVec * data.DistanceDirection;
					}
					else if (data.Direction == EVectorDirection::Up)
					{
						dirVec = otherItem->GetActorUpVector();
						dirVec.Normalize();
						finalVec = dirVec * data.DistanceDirection;
					}
					else if (data.Direction == EVectorDirection::Down)
					{
						dirVec = -otherItem->GetActorUpVector();
						dirVec.Normalize();
						finalVec = dirVec * data.DistanceDirection;
					}
					else if (data.Direction == EVectorDirection::Right)
					{
						dirVec = otherItem->GetActorRightVector();
						dirVec.Normalize();
						finalVec = dirVec * data.DistanceDirection;
					}
					else if (data.Direction == EVectorDirection::Left)
					{
						dirVec = -otherItem->GetActorRightVector();
						dirVec.Normalize();
						finalVec = dirVec * data.DistanceDirection;
					}

					switch (data.ImpulseType)
					{
					case EImpulseType::AddImpulseAtLocation:
						m_itemMeshes[i]->AddImpulseAtLocation(finalVec, otherItem->GetActorLocation());
						break;
					case EImpulseType::AddImpulse:
						m_itemMeshes[i]->AddImpulse(finalVec);
						break;
					case EImpulseType::AddAngularImpulseInDegrees:
						m_itemMeshes[i]->AddAngularImpulseInDegrees(finalVec);
						break;
					case EImpulseType::AddAngularImpulseInRadians:
						m_itemMeshes[i]->AddAngularImpulseInRadians(finalVec);
						break;
					case EImpulseType::AddForceAtLocation:
						m_itemMeshes[i]->AddForceAtLocation(finalVec, otherItem->GetActorLocation());
						break;
					case EImpulseType::AddForceAtLocationLocal:
						m_itemMeshes[i]->AddForceAtLocationLocal(finalVec, otherItem->GetActorLocation());
						break;
					case EImpulseType::AddForce:
						m_itemMeshes[i]->AddForce(finalVec);
						break;
					default:
						break;
					}
				}
			}
		}
		else
		{
			if (otherCharacter && character)
			{
				dirVec = character->GetActorForwardVector();
				dirVec.Normalize();
				finalVec = dirVec * data.DistanceDirection;

				switch (data.ImpulseType)
				{
				case EImpulseType::AddImpulseAtLocation:
					otherCharacter->GetMesh()->AddImpulseAtLocation(finalVec, character->GetActorLocation(), boneName);
					break;
				case EImpulseType::AddImpulse:
					otherCharacter->GetMesh()->AddImpulse(finalVec, boneName);
					break;
				case EImpulseType::AddAngularImpulseInDegrees:
					otherCharacter->GetMesh()->AddAngularImpulseInDegrees(finalVec, boneName);
					break;
				case EImpulseType::AddAngularImpulseInRadians:
					otherCharacter->GetMesh()->AddAngularImpulseInRadians(finalVec, boneName);
					break;
				case EImpulseType::AddForceAtLocation:
					otherCharacter->GetMesh()->AddForceAtLocation(finalVec, character->GetActorLocation(), boneName);
					break;
				case EImpulseType::AddForceAtLocationLocal:
					otherCharacter->GetMesh()->AddForceAtLocationLocal(finalVec, character->GetActorLocation(), boneName);
					break;
				case EImpulseType::AddForce:
					otherCharacter->GetMesh()->AddForce(finalVec, boneName);
					break;
				default:
					break;
				}
			}
			else if (otherItem && character)
			{
				for (int i = 0; i < m_itemMeshes.Num(); i++)
				{
					dirVec = character->GetActorForwardVector();
					dirVec.Normalize();
					finalVec = dirVec * data.DistanceDirection;
					switch (data.ImpulseType)
					{
					case EImpulseType::AddImpulseAtLocation:
						m_itemMeshes[i]->AddImpulseAtLocation(finalVec, character->GetActorLocation());
						break;
					case EImpulseType::AddImpulse:
						m_itemMeshes[i]->AddImpulse(finalVec);
						break;
					case EImpulseType::AddAngularImpulseInDegrees:
						m_itemMeshes[i]->AddAngularImpulseInDegrees(finalVec);
						break;
					case EImpulseType::AddAngularImpulseInRadians:
						m_itemMeshes[i]->AddAngularImpulseInRadians(finalVec);
						break;
					case EImpulseType::AddForceAtLocation:
						m_itemMeshes[i]->AddForceAtLocation(finalVec, character->GetActorLocation());
						break;
					case EImpulseType::AddForceAtLocationLocal:
						m_itemMeshes[i]->AddForceAtLocationLocal(finalVec, character->GetActorLocation());
						break;
					case EImpulseType::AddForce:
						m_itemMeshes[i]->AddForce(finalVec);
						break;
					default:
						break;
					}
				}
			}
			else
			{
				FString debugText = FString(TEXT("FUNCTION: AddImpulseFunction || ERROR: The Actor that makes the impulse or the Actor that makes the impulse or both are NULL. || INTERACT OWNER ACTOR: "));
				FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
				debugText.Append(ObjectName);
				FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
				debugText.Append(debugText2);
				FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
				debugText.Append(ObjectName2);

				//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
				ISM_ULOG_EDIT("%s", Error, *debugText);
			}
		}
	}
	else
	{
		FString debugText = FString(TEXT("FUNCTION: AddImpulseFunction || ERROR: The Target Actor is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		ISM_ULOG_EDIT("%s", Error, *debugText);
	}
}

void UInteractComponent::SetCollisionSettings(AActor *actor, FSetCollisionSettings& data, UInteractComponent* interactComponent)
{
	if (data.m_actor == nullptr)
	{
		data.m_actor = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	}
	if (data.m_actor)
	{
//  		if (data.m_actor->IsA(AChPlayer::StaticClass()))
// 		{
// 			AChPlayer* player = Cast<AChPlayer>(data.m_actor);
// 			if (player)
// 			{
// 				if (data.ChangeMeshCollision)
// 				{
// 					USkeletalMeshComponent* m_playerMesh = player->GetMesh();
// 
// 					m_playerMesh->SetNotifyRigidBodyCollision(data.MeshCollisionSettings.SimGenHitEvents);
// 					m_playerMesh->SetGenerateOverlapEvents(data.MeshCollisionSettings.GenOverlapEvents);
// 
// 					// SET CAN CHARACTER STEP UP ON
// 					switch (data.MeshCollisionSettings.CanCharacterStepUpOn)
// 					{
// 					case ECanCharacterStepUpOn::NO:
// 						m_playerMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
// 						break;
// 					case ECanCharacterStepUpOn::YES:
// 						m_playerMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
// 						break;
// 					case ECanCharacterStepUpOn::OWNER:
// 						m_playerMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
// 						break;
// 					default:
// 						break;
// 					}
// 					if (!data.MeshCollisionSettings.CustomPreset)
// 					{
// 						// SET COLLISION PRESETS
// 						switch (data.MeshCollisionSettings.CollisionPresets)
// 						{
// 						case ECollisionPresets::NOCOLLISION:
// 							m_playerMesh->SetCollisionProfileName("NoCollision");
// 							break;
// 						case ECollisionPresets::BLOCKALL:
// 							m_playerMesh->SetCollisionProfileName("BlockAll");
// 							break;
// 						case ECollisionPresets::OVERLAPALL:
// 							m_playerMesh->SetCollisionProfileName("OverlapAll");
// 							break;
// 						case ECollisionPresets::BLOCKALLDYNAMIC:
// 							m_playerMesh->SetCollisionProfileName("BlockAllDynamic");
// 							break;
// 						case ECollisionPresets::OVERLAPALLDYNAMIC:
// 							m_playerMesh->SetCollisionProfileName("OverlapAllDynamic");
// 							break;
// 						case ECollisionPresets::IGNOREONLYPAWN:
// 							m_playerMesh->SetCollisionProfileName("IgnoreOnlyPawn");
// 							break;
// 						case ECollisionPresets::OVERLAPONLYPAWN:
// 							m_playerMesh->SetCollisionProfileName("OverlapOnlyPawn");
// 							break;
// 						case ECollisionPresets::PAWN:
// 							m_playerMesh->SetCollisionProfileName("Pawn");
// 							break;
// 						case ECollisionPresets::SPECTATOR:
// 							m_playerMesh->SetCollisionProfileName("Spectator");
// 							break;
// 						case ECollisionPresets::CHARACTERMESH:
// 							m_playerMesh->SetCollisionProfileName("CharacterMesh");
// 							break;
// 						case ECollisionPresets::PHYSICSACTOR:
// 							m_playerMesh->SetCollisionProfileName("PhysicsActor");
// 							break;
// 						case ECollisionPresets::DESTRUCTIBLE:
// 							m_playerMesh->SetCollisionProfileName("Destructible");
// 							break;
// 						case ECollisionPresets::INVISIBLEWALL:
// 							m_playerMesh->SetCollisionProfileName("InvisibleWall");
// 							break;
// 						case ECollisionPresets::INVISIBLEWALLDYNAMIC:
// 							m_playerMesh->SetCollisionProfileName("InvisibleWallDynamic");
// 							break;
// 						case ECollisionPresets::TRIGGER:
// 							m_playerMesh->SetCollisionProfileName("Trigger");
// 							break;
// 						case ECollisionPresets::RAGDOLL:
// 							m_playerMesh->SetCollisionProfileName("Ragdoll");
// 							break;
// 						case ECollisionPresets::VEHICLE:
// 							m_playerMesh->SetCollisionProfileName("Vehicle");
// 							break;
// 						case ECollisionPresets::UI:
// 							m_playerMesh->SetCollisionProfileName("UI");
// 							break;
// 						default:
// 							break;
// 						}
// 					}
// 					else
// 					{
// 						// SET COLLISION ENABLED
// 						switch (data.MeshCollisionSettings.CustomCollisionPreset.CollisionEnabled)
// 						{
// 						case ECollisionEnabledType::NOCOLLISION:
// 							m_playerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 							break;
// 						case ECollisionEnabledType::QUERYONLY:
// 							m_playerMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
// 							break;
// 						case ECollisionEnabledType::PHYSICSONLY:
// 							m_playerMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
// 							break;
// 						case ECollisionEnabledType::COLLISIONENABLED:
// 							m_playerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 							break;
// 						default:
// 							break;
// 						}
// 
// 						// SET OBJECT TYPE
// 						switch (data.MeshCollisionSettings.CustomCollisionPreset.ObjectType)
// 						{
// 						case ECollisionObjectType::VISIBILITY:
// 							m_playerMesh->SetCollisionObjectType(ECC_Visibility);
// 							break;
// 						case ECollisionObjectType::CAMERA:
// 							m_playerMesh->SetCollisionObjectType(ECC_Camera);
// 							break;
// 						case ECollisionObjectType::WORLDSTATIC:
// 							m_playerMesh->SetCollisionObjectType(ECC_WorldStatic);
// 							break;
// 						case ECollisionObjectType::WORLDDYNAMIC:
// 							m_playerMesh->SetCollisionObjectType(ECC_WorldDynamic);
// 							break;
// 						case ECollisionObjectType::PAWN:
// 							m_playerMesh->SetCollisionObjectType(ECC_Pawn);
// 							break;
// 						case ECollisionObjectType::PHYSICSBODY:
// 							m_playerMesh->SetCollisionObjectType(ECC_PhysicsBody);
// 							break;
// 						case ECollisionObjectType::VEHICLE:
// 							m_playerMesh->SetCollisionObjectType(ECC_Vehicle);
// 							break;
// 						case ECollisionObjectType::DESTRUCTIBLE:
// 							m_playerMesh->SetCollisionObjectType(ECC_Destructible);
// 							break;
// 						default:
// 							break;
// 						}
// 
// 						// SET ALL CHANNELS RESPONSE TO A COLLISION TYPE
// 						if (data.MeshCollisionSettings.CustomCollisionPreset.AllChannelsResponseTo)
// 						{
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.CollisionResponses)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToAllChannels(ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 						}
// 						// SET COLLISION RESPONSE TO SPECIFIC CHANNEL
// 						else
// 						{
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.Visibility)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.Camera)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.WorldStatic)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.WorldDynamic)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.Pawn)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.PhysicsBody)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.Vehicle)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.MeshCollisionSettings.CustomCollisionPreset.Destructible)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerMesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 						}
// 					}
// 				}
// 				if (data.ChangeShapeCollision)
// 				{
// 					UCapsuleComponent* m_playerCapsule = player->GetCapsuleComponent();
// 
// 					m_playerCapsule->SetNotifyRigidBodyCollision(data.ShapeCollisionSettings.SimGenHitEvents);
// 					m_playerCapsule->SetGenerateOverlapEvents(data.ShapeCollisionSettings.GenOverlapEvents);
// 
// 					// SET CAN CHARACTER STEP UP ON
// 					switch (data.ShapeCollisionSettings.CanCharacterStepUpOn)
// 					{
// 					case ECanCharacterStepUpOn::NO:
// 						m_playerCapsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
// 						break;
// 					case ECanCharacterStepUpOn::YES:
// 						m_playerCapsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
// 						break;
// 					case ECanCharacterStepUpOn::OWNER:
// 						m_playerCapsule->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
// 						break;
// 					default:
// 						break;
// 					}
// 					if (!data.ShapeCollisionSettings.CustomPreset)
// 					{
// 						// SET COLLISION PRESETS
// 						switch (data.ShapeCollisionSettings.CollisionPresets)
// 						{
// 						case ECollisionPresets::NOCOLLISION:
// 							m_playerCapsule->SetCollisionProfileName("NoCollision");
// 							break;
// 						case ECollisionPresets::BLOCKALL:
// 							m_playerCapsule->SetCollisionProfileName("BlockAll");
// 							break;
// 						case ECollisionPresets::OVERLAPALL:
// 							m_playerCapsule->SetCollisionProfileName("OverlapAll");
// 							break;
// 						case ECollisionPresets::BLOCKALLDYNAMIC:
// 							m_playerCapsule->SetCollisionProfileName("BlockAllDynamic");
// 							break;
// 						case ECollisionPresets::OVERLAPALLDYNAMIC:
// 							m_playerCapsule->SetCollisionProfileName("OverlapAllDynamic");
// 							break;
// 						case ECollisionPresets::IGNOREONLYPAWN:
// 							m_playerCapsule->SetCollisionProfileName("IgnoreOnlyPawn");
// 							break;
// 						case ECollisionPresets::OVERLAPONLYPAWN:
// 							m_playerCapsule->SetCollisionProfileName("OverlapOnlyPawn");
// 							break;
// 						case ECollisionPresets::PAWN:
// 							m_playerCapsule->SetCollisionProfileName("Pawn");
// 							break;
// 						case ECollisionPresets::SPECTATOR:
// 							m_playerCapsule->SetCollisionProfileName("Spectator");
// 							break;
// 						case ECollisionPresets::CHARACTERMESH:
// 							m_playerCapsule->SetCollisionProfileName("CharacterMesh");
// 							break;
// 						case ECollisionPresets::PHYSICSACTOR:
// 							m_playerCapsule->SetCollisionProfileName("PhysicsActor");
// 							break;
// 						case ECollisionPresets::DESTRUCTIBLE:
// 							m_playerCapsule->SetCollisionProfileName("Destructible");
// 							break;
// 						case ECollisionPresets::INVISIBLEWALL:
// 							m_playerCapsule->SetCollisionProfileName("InvisibleWall");
// 							break;
// 						case ECollisionPresets::INVISIBLEWALLDYNAMIC:
// 							m_playerCapsule->SetCollisionProfileName("InvisibleWallDynamic");
// 							break;
// 						case ECollisionPresets::TRIGGER:
// 							m_playerCapsule->SetCollisionProfileName("Trigger");
// 							break;
// 						case ECollisionPresets::RAGDOLL:
// 							m_playerCapsule->SetCollisionProfileName("Ragdoll");
// 							break;
// 						case ECollisionPresets::VEHICLE:
// 							m_playerCapsule->SetCollisionProfileName("Vehicle");
// 							break;
// 						case ECollisionPresets::UI:
// 							m_playerCapsule->SetCollisionProfileName("UI");
// 							break;
// 						default:
// 							break;
// 						}
// 					}
// 					else
// 					{
// 						// SET COLLISION ENABLED
// 						switch (data.ShapeCollisionSettings.CustomCollisionPreset.CollisionEnabled)
// 						{
// 						case ECollisionEnabledType::NOCOLLISION:
// 							m_playerCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 							break;
// 						case ECollisionEnabledType::QUERYONLY:
// 							m_playerCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
// 							break;
// 						case ECollisionEnabledType::PHYSICSONLY:
// 							m_playerCapsule->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
// 							break;
// 						case ECollisionEnabledType::COLLISIONENABLED:
// 							m_playerCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 							break;
// 						default:
// 							break;
// 						}
// 
// 						// SET OBJECT TYPE
// 						switch (data.ShapeCollisionSettings.CustomCollisionPreset.ObjectType)
// 						{
// 						case ECollisionObjectType::VISIBILITY:
// 							m_playerCapsule->SetCollisionObjectType(ECC_Visibility);
// 							break;
// 						case ECollisionObjectType::CAMERA:
// 							m_playerCapsule->SetCollisionObjectType(ECC_Camera);
// 							break;
// 						case ECollisionObjectType::WORLDSTATIC:
// 							m_playerCapsule->SetCollisionObjectType(ECC_WorldStatic);
// 							break;
// 						case ECollisionObjectType::WORLDDYNAMIC:
// 							m_playerCapsule->SetCollisionObjectType(ECC_WorldDynamic);
// 							break;
// 						case ECollisionObjectType::PAWN:
// 							m_playerCapsule->SetCollisionObjectType(ECC_Pawn);
// 							break;
// 						case ECollisionObjectType::PHYSICSBODY:
// 							m_playerCapsule->SetCollisionObjectType(ECC_PhysicsBody);
// 							break;
// 						case ECollisionObjectType::VEHICLE:
// 							m_playerCapsule->SetCollisionObjectType(ECC_Vehicle);
// 							break;
// 						case ECollisionObjectType::DESTRUCTIBLE:
// 							m_playerCapsule->SetCollisionObjectType(ECC_Destructible);
// 							break;
// 						default:
// 							break;
// 						}
// 
// 						// SET ALL CHANNELS RESPONSE TO A COLLISION TYPE
// 						if (data.ShapeCollisionSettings.CustomCollisionPreset.AllChannelsResponseTo)
// 						{
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.CollisionResponses)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToAllChannels(ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToAllChannels(ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 						}
// 						// SET COLLISION RESPONSE TO SPECIFIC CHANNEL
// 						else
// 						{
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.Visibility)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.Camera)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.WorldStatic)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.WorldDynamic)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.Pawn)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.PhysicsBody)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.Vehicle)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 							switch (data.ShapeCollisionSettings.CustomCollisionPreset.Destructible)
// 							{
// 							case ECollisionType::IGNORE:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
// 								break;
// 							case ECollisionType::OVERLAP:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
// 								break;
// 							case ECollisionType::BLOCK:
// 								m_playerCapsule->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
// 								break;
// 							default:
// 								break;
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
		if (IInteractInterface* intInt = Cast<IInteractInterface>(data.m_actor))
		{
			AActor* otherCharacter = Cast<AActor>(data.m_actor);
			if (otherCharacter)
			{
				if (data.ChangeMeshCollision)
				{
					TArray<UMeshComponent *> m_otherCharacterMeshes = intInt->GetActorMesh();

					if (m_otherCharacterMeshes.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterMeshes.Num(); i++)
						{
							m_otherCharacterMeshes[i]->SetNotifyRigidBodyCollision(data.MeshCollisionSettings.SimGenHitEvents);
							m_otherCharacterMeshes[i]->SetGenerateOverlapEvents(data.MeshCollisionSettings.GenOverlapEvents);

							// SET CAN CHARACTER STEP UP ON
							switch (data.MeshCollisionSettings.CanCharacterStepUpOn)
							{
							case ECanCharacterStepUpOn::NO:
								m_otherCharacterMeshes[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
								break;
							case ECanCharacterStepUpOn::YES:
								m_otherCharacterMeshes[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
								break;
							case ECanCharacterStepUpOn::OWNER:
								m_otherCharacterMeshes[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
								break;
							default:
								break;
							}
							if (!data.MeshCollisionSettings.CustomPreset)
							{
								// SET COLLISION PRESETS
								switch (data.MeshCollisionSettings.CollisionPresets)
								{
								case ECollisionPresets::NOCOLLISION:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("NoCollision");
									break;
								case ECollisionPresets::BLOCKALL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("BlockAll");
									break;
								case ECollisionPresets::OVERLAPALL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("OverlapAll");
									break;
								case ECollisionPresets::BLOCKALLDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("BlockAllDynamic");
									break;
								case ECollisionPresets::OVERLAPALLDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("OverlapAllDynamic");
									break;
								case ECollisionPresets::IGNOREONLYPAWN:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("IgnoreOnlyPawn");
									break;
								case ECollisionPresets::OVERLAPONLYPAWN:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("OverlapOnlyPawn");
									break;
								case ECollisionPresets::PAWN:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Pawn");
									break;
								case ECollisionPresets::SPECTATOR:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Spectator");
									break;
								case ECollisionPresets::CHARACTERMESH:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("CharacterMesh");
									break;
								case ECollisionPresets::PHYSICSACTOR:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("PhysicsActor");
									break;
								case ECollisionPresets::DESTRUCTIBLE:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Destructible");
									break;
								case ECollisionPresets::INVISIBLEWALL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("InvisibleWall");
									break;
								case ECollisionPresets::INVISIBLEWALLDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("InvisibleWallDynamic");
									break;
								case ECollisionPresets::TRIGGER:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Trigger");
									break;
								case ECollisionPresets::RAGDOLL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Ragdoll");
									break;
								case ECollisionPresets::VEHICLE:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Vehicle");
									break;
								case ECollisionPresets::UI:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("UI");
									break;
								default:
									break;
								}
							}
							else
							{
								// SET COLLISION ENABLED
								switch (data.MeshCollisionSettings.CustomCollisionPreset.CollisionEnabled)
								{
								case ECollisionEnabledType::NOCOLLISION:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
									break;
								case ECollisionEnabledType::QUERYONLY:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
									break;
								case ECollisionEnabledType::PHYSICSONLY:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
									break;
								case ECollisionEnabledType::COLLISIONENABLED:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
									break;
								default:
									break;
								}

								// SET OBJECT TYPE
								switch (data.MeshCollisionSettings.CustomCollisionPreset.ObjectType)
								{
								case ECollisionObjectType::VISIBILITY:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Visibility);
									break;
								case ECollisionObjectType::CAMERA:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Camera);
									break;
								case ECollisionObjectType::WORLDSTATIC:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_WorldStatic);
									break;
								case ECollisionObjectType::WORLDDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_WorldDynamic);
									break;
								case ECollisionObjectType::PAWN:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Pawn);
									break;
								case ECollisionObjectType::PHYSICSBODY:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_PhysicsBody);
									break;
								case ECollisionObjectType::VEHICLE:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Vehicle);
									break;
								case ECollisionObjectType::DESTRUCTIBLE:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Destructible);
									break;
								default:
									break;
								}

								// SET ALL CHANNELS RESPONSE TO A COLLISION TYPE
								if (data.MeshCollisionSettings.CustomCollisionPreset.AllChannelsResponseTo)
								{
									switch (data.MeshCollisionSettings.CustomCollisionPreset.CollisionResponses)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToAllChannels(ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToAllChannels(ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToAllChannels(ECR_Block);
										break;
									default:
										break;
									}
								}
								// SET COLLISION RESPONSE TO SPECIFIC CHANNEL
								else
								{
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Visibility)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Camera)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.WorldStatic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.WorldDynamic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Pawn)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.PhysicsBody)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Vehicle)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Destructible)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
										break;
									default:
										break;
									}
								}
							}
						}
					}
				}
				if (data.ChangeShapeCollision)
				{
					TArray<UShapeComponent *> m_otherCharacterCollisions = intInt->GetActorCollision();

					if (m_otherCharacterCollisions.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterCollisions.Num(); i++)
						{
							m_otherCharacterCollisions[i]->SetNotifyRigidBodyCollision(data.ShapeCollisionSettings.SimGenHitEvents);
							m_otherCharacterCollisions[i]->SetGenerateOverlapEvents(data.ShapeCollisionSettings.GenOverlapEvents);

							// SET CAN CHARACTER STEP UP ON
							switch (data.ShapeCollisionSettings.CanCharacterStepUpOn)
							{
							case ECanCharacterStepUpOn::NO:
								m_otherCharacterCollisions[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
								break;
							case ECanCharacterStepUpOn::YES:
								m_otherCharacterCollisions[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
								break;
							case ECanCharacterStepUpOn::OWNER:
								m_otherCharacterCollisions[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
								break;
							default:
								break;
							}
							if (!data.ShapeCollisionSettings.CustomPreset)
							{
								// SET COLLISION PRESETS
								switch (data.ShapeCollisionSettings.CollisionPresets)
								{
								case ECollisionPresets::NOCOLLISION:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("NoCollision");
									break;
								case ECollisionPresets::BLOCKALL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("BlockAll");
									break;
								case ECollisionPresets::OVERLAPALL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("OverlapAll");
									break;
								case ECollisionPresets::BLOCKALLDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("BlockAllDynamic");
									break;
								case ECollisionPresets::OVERLAPALLDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("OverlapAllDynamic");
									break;
								case ECollisionPresets::IGNOREONLYPAWN:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("IgnoreOnlyPawn");
									break;
								case ECollisionPresets::OVERLAPONLYPAWN:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("OverlapOnlyPawn");
									break;
								case ECollisionPresets::PAWN:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Pawn");
									break;
								case ECollisionPresets::SPECTATOR:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Spectator");
									break;
								case ECollisionPresets::CHARACTERMESH:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("CharacterMesh");
									break;
								case ECollisionPresets::PHYSICSACTOR:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("PhysicsActor");
									break;
								case ECollisionPresets::DESTRUCTIBLE:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Destructible");
									break;
								case ECollisionPresets::INVISIBLEWALL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("InvisibleWall");
									break;
								case ECollisionPresets::INVISIBLEWALLDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("InvisibleWallDynamic");
									break;
								case ECollisionPresets::TRIGGER:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Trigger");
									break;
								case ECollisionPresets::RAGDOLL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Ragdoll");
									break;
								case ECollisionPresets::VEHICLE:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Vehicle");
									break;
								case ECollisionPresets::UI:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("UI");
									break;
								default:
									break;
								}
							}
							else
							{
								// SET COLLISION ENABLED
								switch (data.ShapeCollisionSettings.CustomCollisionPreset.CollisionEnabled)
								{
								case ECollisionEnabledType::NOCOLLISION:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
									break;
								case ECollisionEnabledType::QUERYONLY:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
									break;
								case ECollisionEnabledType::PHYSICSONLY:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
									break;
								case ECollisionEnabledType::COLLISIONENABLED:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
									break;
								default:
									break;
								}

								// SET OBJECT TYPE
								switch (data.ShapeCollisionSettings.CustomCollisionPreset.ObjectType)
								{
								case ECollisionObjectType::VISIBILITY:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Visibility);
									break;
								case ECollisionObjectType::CAMERA:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Camera);
									break;
								case ECollisionObjectType::WORLDSTATIC:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_WorldStatic);
									break;
								case ECollisionObjectType::WORLDDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_WorldDynamic);
									break;
								case ECollisionObjectType::PAWN:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Pawn);
									break;
								case ECollisionObjectType::PHYSICSBODY:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_PhysicsBody);
									break;
								case ECollisionObjectType::VEHICLE:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Vehicle);
									break;
								case ECollisionObjectType::DESTRUCTIBLE:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Destructible);
									break;
								default:
									break;
								}

								// SET ALL CHANNELS RESPONSE TO A COLLISION TYPE
								if (data.ShapeCollisionSettings.CustomCollisionPreset.AllChannelsResponseTo)
								{
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.CollisionResponses)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToAllChannels(ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToAllChannels(ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToAllChannels(ECR_Block);
										break;
									default:
										break;
									}
								}
								// SET COLLISION RESPONSE TO SPECIFIC CHANNEL
								else
								{
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Visibility)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Camera)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.WorldStatic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.WorldDynamic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Pawn)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.PhysicsBody)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Vehicle)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Destructible)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
										break;
									default:
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		else 
		{
			AActor* otherCharacter = Cast<AActor>(data.m_actor);
			if (otherCharacter)
			{
				if (data.ChangeMeshCollision)
				{
					TArray<UMeshComponent*> m_otherCharacterMeshes;
					TArray<UActorComponent*> ActorCArray = data.m_actor->GetComponentsByClass(UMeshComponent::StaticClass());
					for (int i = 0; i < ActorCArray.Num(); i++) {
						m_otherCharacterMeshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
					}

					if (m_otherCharacterMeshes.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterMeshes.Num(); i++)
						{
							m_otherCharacterMeshes[i]->SetNotifyRigidBodyCollision(data.MeshCollisionSettings.SimGenHitEvents);
							m_otherCharacterMeshes[i]->SetGenerateOverlapEvents(data.MeshCollisionSettings.GenOverlapEvents);

							// SET CAN CHARACTER STEP UP ON
							switch (data.MeshCollisionSettings.CanCharacterStepUpOn)
							{
							case ECanCharacterStepUpOn::NO:
								m_otherCharacterMeshes[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
								break;
							case ECanCharacterStepUpOn::YES:
								m_otherCharacterMeshes[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
								break;
							case ECanCharacterStepUpOn::OWNER:
								m_otherCharacterMeshes[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
								break;
							default:
								break;
							}
							if (!data.MeshCollisionSettings.CustomPreset)
							{
								// SET COLLISION PRESETS
								switch (data.MeshCollisionSettings.CollisionPresets)
								{
								case ECollisionPresets::NOCOLLISION:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("NoCollision");
									break;
								case ECollisionPresets::BLOCKALL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("BlockAll");
									break;
								case ECollisionPresets::OVERLAPALL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("OverlapAll");
									break;
								case ECollisionPresets::BLOCKALLDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("BlockAllDynamic");
									break;
								case ECollisionPresets::OVERLAPALLDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("OverlapAllDynamic");
									break;
								case ECollisionPresets::IGNOREONLYPAWN:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("IgnoreOnlyPawn");
									break;
								case ECollisionPresets::OVERLAPONLYPAWN:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("OverlapOnlyPawn");
									break;
								case ECollisionPresets::PAWN:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Pawn");
									break;
								case ECollisionPresets::SPECTATOR:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Spectator");
									break;
								case ECollisionPresets::CHARACTERMESH:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("CharacterMesh");
									break;
								case ECollisionPresets::PHYSICSACTOR:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("PhysicsActor");
									break;
								case ECollisionPresets::DESTRUCTIBLE:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Destructible");
									break;
								case ECollisionPresets::INVISIBLEWALL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("InvisibleWall");
									break;
								case ECollisionPresets::INVISIBLEWALLDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("InvisibleWallDynamic");
									break;
								case ECollisionPresets::TRIGGER:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Trigger");
									break;
								case ECollisionPresets::RAGDOLL:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Ragdoll");
									break;
								case ECollisionPresets::VEHICLE:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("Vehicle");
									break;
								case ECollisionPresets::UI:
									m_otherCharacterMeshes[i]->SetCollisionProfileName("UI");
									break;
								default:
									break;
								}
							}
							else
							{
								// SET COLLISION ENABLED
								switch (data.MeshCollisionSettings.CustomCollisionPreset.CollisionEnabled)
								{
								case ECollisionEnabledType::NOCOLLISION:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
									break;
								case ECollisionEnabledType::QUERYONLY:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
									break;
								case ECollisionEnabledType::PHYSICSONLY:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
									break;
								case ECollisionEnabledType::COLLISIONENABLED:
									m_otherCharacterMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
									break;
								default:
									break;
								}

								// SET OBJECT TYPE
								switch (data.MeshCollisionSettings.CustomCollisionPreset.ObjectType)
								{
								case ECollisionObjectType::VISIBILITY:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Visibility);
									break;
								case ECollisionObjectType::CAMERA:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Camera);
									break;
								case ECollisionObjectType::WORLDSTATIC:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_WorldStatic);
									break;
								case ECollisionObjectType::WORLDDYNAMIC:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_WorldDynamic);
									break;
								case ECollisionObjectType::PAWN:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Pawn);
									break;
								case ECollisionObjectType::PHYSICSBODY:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_PhysicsBody);
									break;
								case ECollisionObjectType::VEHICLE:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Vehicle);
									break;
								case ECollisionObjectType::DESTRUCTIBLE:
									m_otherCharacterMeshes[i]->SetCollisionObjectType(ECC_Destructible);
									break;
								default:
									break;
								}

								// SET ALL CHANNELS RESPONSE TO A COLLISION TYPE
								if (data.MeshCollisionSettings.CustomCollisionPreset.AllChannelsResponseTo)
								{
									switch (data.MeshCollisionSettings.CustomCollisionPreset.CollisionResponses)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToAllChannels(ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToAllChannels(ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToAllChannels(ECR_Block);
										break;
									default:
										break;
									}
								}
								// SET COLLISION RESPONSE TO SPECIFIC CHANNEL
								else
								{
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Visibility)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Camera)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.WorldStatic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.WorldDynamic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Pawn)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.PhysicsBody)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Vehicle)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.MeshCollisionSettings.CustomCollisionPreset.Destructible)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterMeshes[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
										break;
									default:
										break;
									}
								}
							}
						}
					}
				}
				if (data.ChangeShapeCollision)
				{
					TArray<UShapeComponent*> m_otherCharacterCollisions;
					TArray<UActorComponent*> ActorBArray = data.m_actor->GetComponentsByTag(UBoxComponent::StaticClass(), "ModifiableShape");
					TArray<UActorComponent*> ActorSArray = data.m_actor->GetComponentsByTag(USphereComponent::StaticClass(), "ModifiableShape");
					TArray<UActorComponent*> ActorCArray = data.m_actor->GetComponentsByTag(UCapsuleComponent::StaticClass(), "ModifiableShape");

					if (ActorBArray.Num() != 0)
					{
						for (int i = 0; i < ActorBArray.Num(); i++) {
							m_otherCharacterCollisions.Add(Cast<UShapeComponent>(ActorBArray[i]));
						}
					}
					if (ActorSArray.Num() != 0)
					{
						for (int i = 0; i < ActorSArray.Num(); i++) {
							m_otherCharacterCollisions.Add(Cast<UShapeComponent>(ActorSArray[i]));
						}
					}
					if (ActorCArray.Num() != 0)
					{
						for (int i = 0; i < ActorCArray.Num(); i++) {
							m_otherCharacterCollisions.Add(Cast<UShapeComponent>(ActorCArray[i]));
						}
					}

					if (m_otherCharacterCollisions.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterCollisions.Num(); i++)
						{
							m_otherCharacterCollisions[i]->SetNotifyRigidBodyCollision(data.ShapeCollisionSettings.SimGenHitEvents);
							m_otherCharacterCollisions[i]->SetGenerateOverlapEvents(data.ShapeCollisionSettings.GenOverlapEvents);

							// SET CAN CHARACTER STEP UP ON
							switch (data.ShapeCollisionSettings.CanCharacterStepUpOn)
							{
							case ECanCharacterStepUpOn::NO:
								m_otherCharacterCollisions[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
								break;
							case ECanCharacterStepUpOn::YES:
								m_otherCharacterCollisions[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
								break;
							case ECanCharacterStepUpOn::OWNER:
								m_otherCharacterCollisions[i]->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
								break;
							default:
								break;
							}
							if (!data.ShapeCollisionSettings.CustomPreset)
							{
								// SET COLLISION PRESETS
								switch (data.ShapeCollisionSettings.CollisionPresets)
								{
								case ECollisionPresets::NOCOLLISION:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("NoCollision");
									break;
								case ECollisionPresets::BLOCKALL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("BlockAll");
									break;
								case ECollisionPresets::OVERLAPALL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("OverlapAll");
									break;
								case ECollisionPresets::BLOCKALLDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("BlockAllDynamic");
									break;
								case ECollisionPresets::OVERLAPALLDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("OverlapAllDynamic");
									break;
								case ECollisionPresets::IGNOREONLYPAWN:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("IgnoreOnlyPawn");
									break;
								case ECollisionPresets::OVERLAPONLYPAWN:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("OverlapOnlyPawn");
									break;
								case ECollisionPresets::PAWN:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Pawn");
									break;
								case ECollisionPresets::SPECTATOR:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Spectator");
									break;
								case ECollisionPresets::CHARACTERMESH:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("CharacterMesh");
									break;
								case ECollisionPresets::PHYSICSACTOR:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("PhysicsActor");
									break;
								case ECollisionPresets::DESTRUCTIBLE:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Destructible");
									break;
								case ECollisionPresets::INVISIBLEWALL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("InvisibleWall");
									break;
								case ECollisionPresets::INVISIBLEWALLDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("InvisibleWallDynamic");
									break;
								case ECollisionPresets::TRIGGER:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Trigger");
									break;
								case ECollisionPresets::RAGDOLL:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Ragdoll");
									break;
								case ECollisionPresets::VEHICLE:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("Vehicle");
									break;
								case ECollisionPresets::UI:
									m_otherCharacterCollisions[i]->SetCollisionProfileName("UI");
									break;
								default:
									break;
								}
							}
							else
							{
								// SET COLLISION ENABLED
								switch (data.ShapeCollisionSettings.CustomCollisionPreset.CollisionEnabled)
								{
								case ECollisionEnabledType::NOCOLLISION:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
									break;
								case ECollisionEnabledType::QUERYONLY:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
									break;
								case ECollisionEnabledType::PHYSICSONLY:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
									break;
								case ECollisionEnabledType::COLLISIONENABLED:
									m_otherCharacterCollisions[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
									break;
								default:
									break;
								}

								// SET OBJECT TYPE
								switch (data.ShapeCollisionSettings.CustomCollisionPreset.ObjectType)
								{
								case ECollisionObjectType::VISIBILITY:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Visibility);
									break;
								case ECollisionObjectType::CAMERA:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Camera);
									break;
								case ECollisionObjectType::WORLDSTATIC:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_WorldStatic);
									break;
								case ECollisionObjectType::WORLDDYNAMIC:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_WorldDynamic);
									break;
								case ECollisionObjectType::PAWN:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Pawn);
									break;
								case ECollisionObjectType::PHYSICSBODY:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_PhysicsBody);
									break;
								case ECollisionObjectType::VEHICLE:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Vehicle);
									break;
								case ECollisionObjectType::DESTRUCTIBLE:
									m_otherCharacterCollisions[i]->SetCollisionObjectType(ECC_Destructible);
									break;
								default:
									break;
								}

								// SET ALL CHANNELS RESPONSE TO A COLLISION TYPE
								if (data.ShapeCollisionSettings.CustomCollisionPreset.AllChannelsResponseTo)
								{
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.CollisionResponses)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToAllChannels(ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToAllChannels(ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToAllChannels(ECR_Block);
										break;
									default:
										break;
									}
								}
								// SET COLLISION RESPONSE TO SPECIFIC CHANNEL
								else
								{
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Visibility)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Camera)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.WorldStatic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.WorldDynamic)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Pawn)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.PhysicsBody)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Vehicle)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
										break;
									default:
										break;
									}
									switch (data.ShapeCollisionSettings.CustomCollisionPreset.Destructible)
									{
									case ECollisionType::IGNORE:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
										break;
									case ECollisionType::OVERLAP:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
										break;
									case ECollisionType::BLOCK:
										m_otherCharacterCollisions[i]->SetCollisionResponseToChannel(ECC_Destructible, ECR_Block);
										break;
									default:
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		FString debugText = FString(TEXT("FUNCTION: SetCollisionSettings || ERROR: The Target Actor is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
		ISM_ULOG_EDIT("%s", Error, *debugText);
	}
}

void UInteractComponent::SlowTimeFunction(AActor* actor, FSlowTimeFunction& data, UInteractComponent* interactComponent)
{
	if (data.ChangeWorldTime)
	{
		UGameplayStatics::SetGlobalTimeDilation(interactComponent->GetWorld(), data.WorldTimeDilation);
	}

	if (data.m_actor == nullptr)
	{
		data.m_actor = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	}
	if (data.m_actor)
	{
// 		if (data.m_actor->IsA(ACh::StaticClass()))
// 		{
/*		ACh* character = Cast<ACh>(data.m_actor);*/
		if (data.ChangeCharacterTime)
		{
			data.m_actor->CustomTimeDilation = data.CharacterTimeDilation;
		}
/*		}*/
// 		else if (data.m_actor->IsA(AItem::StaticClass()))
// 		{
// 			AItem* item = Cast<AItem>(data.m_actor);
// 			if (item && data.ChangeCharacterTime)
// 			{
// 				item->CustomTimeDilation = data.CharacterTimeDilation;
// 			}
// 		}
// 		else
// 		{
// 			FString debugText = FString(TEXT("FUNCTION: SlowTimeFunction || WARNING: The Target Actor doesnt inherits from Ch or a Item. || INTERACT OWNER ACTOR: "));
// 			FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
// 			debugText.Append(ObjectName);
// 			FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
// 			debugText.Append(debugText2);
// 			FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
// 			debugText.Append(ObjectName2);
// 
// 			//ISM_PRINTLOG_EDIT(debugText, 10.0f, FColor::Yellow);
// 			ISM_ULOG_EDIT("%s", Warning, *debugText);
// 		}
	}
	else
	{
		FString debugText = FString(TEXT("FUNCTION: SlowTimeFunction || ERROR: The Target Actor is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
		ISM_ULOG_EDIT("%s", Error, *debugText);
	}
}

void UInteractComponent::AttachToObject(AActor* actor, FAttachToObject& data, UInteractComponent* interactComponent)
{
	FName socketName = FName(TEXT("None"));
	FAttachmentTransformRules attachmentTransformRules = FAttachmentTransformRules(data.AttachLocationRule, data.AttachRotationRule, data.AttachScaleRule, data.WeldSimulatedBodies);
	FDetachmentTransformRules detachmentTransformRules = FDetachmentTransformRules(data.DetachLocationRule, data.DetachRotationRule, data.DetachScaleRule, data.CallModify);

	if (data.CustomSocket)
	{
		socketName = data.CustomSocketToAttach;
	}
	else
	{
		switch (data.SocketToAttach)
		{
		case EBone_Socket::ROOT:
			socketName = FName(TEXT("Root_Socket"));
			break;
		case EBone_Socket::PELVIS:
			socketName = FName(TEXT("Pelvis_Socket"));
			break;
		case EBone_Socket::SPINE_01:
			socketName = FName(TEXT("01_Spine_Socket"));
			break;
		case EBone_Socket::SPINE_02:
			socketName = FName(TEXT("02_Spine_Socket"));
			break;
		case EBone_Socket::SPINE_03:
			socketName = FName(TEXT("03_Spine_Socket"));
			break;
		case EBone_Socket::NECK:
			socketName = FName(TEXT("Neck_Socket"));
			break;
		case EBone_Socket::HEAD:
			socketName = FName(TEXT("Head_Socket"));
			break;
		case EBone_Socket::TOPHEAD:
			socketName = FName(TEXT("TopHead_Socket"));
			break;
		case EBone_Socket::L_CLAVICLE:
			socketName = FName(TEXT("L_Clavicle_Socket"));
			break;
		case EBone_Socket::L_UPPERARM:
			socketName = FName(TEXT("L_Upperarm_Socket"));
			break;
		case EBone_Socket::L_LOWERARM:
			socketName = FName(TEXT("L_Lowerarm_Socket"));
			break;
		case EBone_Socket::L_HAND:
			socketName = FName(TEXT("L_Hand_Socket"));
			break;
		case EBone_Socket::L_INDEX_01:
			socketName = FName(TEXT("01_L_Index_Socket"));
			break;
		case EBone_Socket::L_INDEX_02:
			socketName = FName(TEXT("02_L_Index_Socket"));
			break;
		case EBone_Socket::L_INDEX_03:
			socketName = FName(TEXT("03_L_Index_Socket"));
			break;
		case EBone_Socket::L_INDEX_04:
			socketName = FName(TEXT("04_L_Index_Socket"));
			break;
		case EBone_Socket::R_CLAVICLE:
			socketName = FName(TEXT("R_Clavicle_Socket"));
			break;
		case EBone_Socket::R_UPPERARM:
			socketName = FName(TEXT("R_Upperarm_Socket"));
			break;
		case EBone_Socket::R_LOWERARM:
			socketName = FName(TEXT("R_Lowerarm_Socket"));
			break;
		case EBone_Socket::R_HAND:
			socketName = FName(TEXT("R_Hand_Socket"));
			break;
		case EBone_Socket::R_INDEX_01:
			socketName = FName(TEXT("01_R_Index_Socket"));
			break;
		case EBone_Socket::R_INDEX_02:
			socketName = FName(TEXT("02_R_Index_Socket"));
			break;
		case EBone_Socket::R_INDEX_03:
			socketName = FName(TEXT("03_R_Index_Socket"));
			break;
		case EBone_Socket::R_INDEX_04:
			socketName = FName(TEXT("04_R_Index_Socket"));
			break;
		case EBone_Socket::L_THIGH:
			socketName = FName(TEXT("L_Thigh_Socket"));
			break;
		case EBone_Socket::L_CALF:
			socketName = FName(TEXT("L_Calf_Socket"));
			break;
		case EBone_Socket::L_FOOT:
			socketName = FName(TEXT("L_Foot_Socket"));
			break;
		case EBone_Socket::L_BALL:
			socketName = FName(TEXT("L_Ball_Socket"));
			break;
		case EBone_Socket::L_TOECAP:
			socketName = FName(TEXT("L_Toecap_Socket"));
			break;
		case EBone_Socket::R_THIGH:
			socketName = FName(TEXT("R_Thigh_Socket"));
			break;
		case EBone_Socket::R_CALF:
			socketName = FName(TEXT("R_Calf_Socket"));
			break;
		case EBone_Socket::R_FOOT:
			socketName = FName(TEXT("R_Foot_Socket"));
			break;
		case EBone_Socket::R_BALL:
			socketName = FName(TEXT("R_Ball_Socket"));
			break;
		case EBone_Socket::R_TOECAP:
			socketName = FName(TEXT("R_Toecap_Socket"));
			break;
		default:
			break;
		}
	}

	if (data.m_actor == nullptr)
	{
		data.m_actor = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
		data.m_currentActor = UInteractComponent::GetCurrentActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	}
	if (data.m_actor)
	{
		if (data.m_component == nullptr)
		{
			data.m_component = UInteractComponent::GetSceneComponent(interactComponent, data.m_actor, data.specificChild, data.componentTag);
		}

		if (data.DoOnActor)
		{
			if (data.Attach)
			{
				if (data.actor.getThisActor.getOwner == true)
				{
					if (Cast<ACharacter>(data.m_currentActor))
					{
						ACharacter* othercharacter = Cast<ACharacter>(data.m_currentActor);

						if (othercharacter)
						{
							data.m_actor->AttachToComponent(othercharacter->GetMesh(), attachmentTransformRules, socketName);
						}
						else
						{
							FString debugText = FString(TEXT("FUNCTION: AttachToObject || ERROR: The Attachment Actor is NULL. || INTERACT OWNER ACTOR: "));
							FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName);
							FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
							debugText.Append(debugText2);
							FString ObjectName2 = (data.m_currentActor != NULL) ? data.m_currentActor->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName2);

							//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
							ISM_ULOG_EDIT("%s", Error, *debugText);
						}
					}
					else
					{
						if (data.m_currentActor)
						{
							data.m_actor->AttachToComponent(data.m_currentActor->GetRootComponent(), attachmentTransformRules);
						}
						else
						{
							FString debugText = FString(TEXT("FUNCTION: AttachToObject || ERROR: The Attachment Actor is NULL. || INTERACT OWNER ACTOR: "));
							FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName);
							FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
							debugText.Append(debugText2);
							FString ObjectName2 = (data.m_currentActor != NULL) ? data.m_currentActor->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName2);

							//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
							ISM_ULOG_EDIT("%s", Error, *debugText);
						}
					}
				}
				else
				{
					if (interactComponent->GetOwner()->IsA(ACharacter::StaticClass()))
					{
						ACharacter* character = Cast<ACharacter>(interactComponent->GetOwner());

						if (character)
						{
							data.m_actor->AttachToComponent(character->GetMesh(), attachmentTransformRules, socketName);
						}
						else
						{
							FString debugText = FString(TEXT("FUNCTION: AttachToObject || ERROR: The Attachment Actor is NULL. || INTERACT OWNER ACTOR: "));
							FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName);
							FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
							debugText.Append(debugText2);
							FString ObjectName2 = (data.m_currentActor != NULL) ? data.m_currentActor->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName2);

							//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
							ISM_ULOG_EDIT("%s", Error, *debugText);
						}
					}
					else
					{
						if (interactComponent->GetOwner())
						{
							data.m_actor->AttachToComponent(interactComponent->GetOwner()->GetRootComponent(), attachmentTransformRules);
						}
						else
						{
							FString debugText = FString(TEXT("FUNCTION: AttachToObject || ERROR: The Attachment Actor is NULL. || INTERACT OWNER ACTOR: "));
							FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName);
							FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
							debugText.Append(debugText2);
							FString ObjectName2 = (data.m_currentActor != NULL) ? data.m_currentActor->GetName() : FString(TEXT("None"));
							debugText.Append(ObjectName2);

							//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
							ISM_ULOG_EDIT("%s", Error, *debugText);
						}
					}
				}
			}
			else
			{
				data.m_actor->DetachFromActor(detachmentTransformRules);
			}
		}
		else
		{
			if (data.Attach)
			{
				data.m_component->DetachFromComponent(detachmentTransformRules);
				if (Cast<ACharacter>(data.m_currentActor))
				{
					ACharacter* othercharacter = Cast<ACharacter>(data.m_currentActor);

					if (othercharacter)
					{
						data.m_component->AttachToComponent(othercharacter->GetMesh(), attachmentTransformRules, socketName);
					}
					else
					{
						FString debugText = FString(TEXT("FUNCTION: AttachToObject || ERROR: The Attachment Actor is NULL. || INTERACT OWNER ACTOR: "));
						FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
						debugText.Append(ObjectName);
						FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
						debugText.Append(debugText2);
						FString ObjectName2 = (data.m_currentActor != NULL) ? data.m_currentActor->GetName() : FString(TEXT("None"));
						debugText.Append(ObjectName2);

						//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
						ISM_ULOG_EDIT("%s", Error, *debugText);
					}
				}
				else
				{
					if (data.m_currentActor)
					{
						data.m_component->AttachToComponent(data.m_currentActor->GetRootComponent(), attachmentTransformRules);
					}
					else
					{
						FString debugText = FString(TEXT("FUNCTION: AttachToObject || ERROR: The Attachment Actor is NULL. || INTERACT OWNER ACTOR: "));
						FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
						debugText.Append(ObjectName);
						FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
						debugText.Append(debugText2);
						FString ObjectName2 = (data.m_currentActor != NULL) ? data.m_currentActor->GetName() : FString(TEXT("None"));
						debugText.Append(ObjectName2);

						//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
						ISM_ULOG_EDIT("%s", Error, *debugText);
					}
				}
			}
			else
			{
				data.m_component->DetachFromComponent(detachmentTransformRules);
			}
		}
	}
	else
	{
		FString debugText = FString(TEXT("FUNCTION: AttachToObject || ERROR: The Target Actor is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (data.m_currentActor != NULL) ? data.m_currentActor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
		ISM_ULOG_EDIT("%s", Error, *debugText);
	}
}

void UInteractComponent::PlaySequence(AActor* actor, FPlaySequence& data, UInteractComponent* interactComponent)
{
	ALevelSequenceActor* OutActor;
	ULevelSequencePlayer* sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(interactComponent->GetWorld(), data.levelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);

	if (sequencePlayer) {
		sequencePlayer->Play();
	}

}


void UInteractComponent::CancelTimer(AActor *actor, FCancelTimer& data, UInteractComponent* interactComponent)
{
	if (data.specificTimer != ETimerList::None) {
		for (int i = 0; i < interactComponent->m_registerTimerArray.Num(); i++) {
			if (data.specificFunctionIndex != -1 && data.specificStage != -1 && data.doOnce) {
				if (interactComponent->m_registerTimerArray[i].Contains("Id=" + GETENUMSTRING(TEXT("ETimerList"), data.specificTimer) + "-" + FString::FromInt(data.specificStage) + "-" + FString::FromInt(data.specificFunctionIndex) + "-" + FString::FromInt((int)data.specificStageType) + "-" + FString::FromInt((int)data.specificAction))) {
					interactComponent->m_registerTimerArray.RemoveAt(i);
					break;
				}
			}
			else {
				if (interactComponent->m_registerTimerArray[i].Contains("Id=" + GETENUMSTRING(TEXT("ETimerList"), data.specificTimer))) {
					interactComponent->m_registerTimerArray.RemoveAt(i);
					if (data.doOnce) {
						break;
					}
					i--;
				}
			}
		}
	}
	else if (data.specificTimerCore != ETimerListCore::None) {
		for (int i = 0; i < interactComponent->m_registerTimerArray.Num(); i++) {
			if (data.specificFunctionIndex != -1 && data.specificStage != -1 && data.doOnce) {
				if (interactComponent->m_registerTimerArray[i].Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), data.specificTimerCore) + "-" + FString::FromInt(data.specificStage) + "-" + FString::FromInt(data.specificFunctionIndex) + "-" + FString::FromInt((int)data.specificStageType) + "-" + FString::FromInt((int)data.specificAction))) {
					interactComponent->m_registerTimerArray.RemoveAt(i);
					break;
				}
			}
			else {
				if (interactComponent->m_registerTimerArray[i].Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), data.specificTimerCore))) {
					interactComponent->m_registerTimerArray.RemoveAt(i);
					if (data.doOnce) {
						break;
					}
					i--;
				}
			}
		}
	}
	else {
		interactComponent->m_registerTimerArray.Empty();
	}
}


void UInteractComponent::RotateLookAtActor(AActor* actor, FRotateLookAtActor& data, UInteractComponent* interactComponent)
{
	AActor* act = UInteractComponent::GetCurrentActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	TArray<AActor*> actors;
// 	if (data.currentActor.customActor) {
// 		if (data.currentActor.currentTag.ToString() != "" && data.currentActor.currentTag != NAME_None) {
// 			TArray<AActor*> actors1;
// 			UGameplayStatics::GetAllActorsWithTag(interactComponent->GetWorld(), data.currentActor.currentTag, actors1);
// 			for (int i = 0; i < actors1.Num(); i++) {
// 				if ((actors1[i]->GetClass() == data.currentActor.currentActor) || !data.currentActor.currentActor) {
// 					act = actors1[i];
// 					break;
// 				}
// 			}
// 		}
// 		else {
// 			TArray<AActor*> actors1;
// 			UGameplayStatics::GetAllActorsOfClass(interactComponent->GetWorld(), data.currentActor.currentActor, actors1);
// 			if (actors1.Num() > 0) {
// 				act = actors1[0];
// 			}
// 		}
// 	}
// 	else {
// 		if (data.currentActor.doOnOwner) {
// 			act = interactComponent->GetOwner();
// 		}
// 		else if (actor) {
// 			act = actor;
// 		}
// 	}
	if (act != nullptr) {
		if (data.actor.enumName != EGetActorList::None) {
			int val = (int)data.actor.enumName - 1;
			AActor* acto = nullptr;
			if (val >= 0) {
				acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
			}
			if (acto) {
				FRotator rot = { 0.f, 0.f, 0.f };
				FVector directionVector = { 0.f, 0.f, 0.f };
				USceneComponent* sceneC = UInteractComponent::GetSceneComponent(interactComponent, acto, data.specificChild, data.componentTag);
				if (data.targetActorDirection != EVectorDirection::None) {
					if (sceneC) {
						rot = sceneC->GetComponentRotation();
					}
					else {
						rot = acto->GetActorRotation();
					}
					if (data.targetActorDirection == EVectorDirection::Backward) {
						if (sceneC) {
							directionVector = sceneC->GetForwardVector()*-1.f;
						}
						else {
							directionVector = acto->GetActorForwardVector()*-1.f;
						}
						directionVector.Normalize();
						UKismetMathLibrary::ComposeRotators(rot, { directionVector.X*180.f,directionVector.Y*180.f, directionVector.Z*180.f });
					}
					else if (data.targetActorDirection == EVectorDirection::Right) {
						if (sceneC) {
							directionVector = sceneC->GetRightVector();
						}
						else {
							directionVector = acto->GetActorRightVector();
						}
						directionVector.Normalize();
						UKismetMathLibrary::ComposeRotators(rot, { directionVector.X*90.f,directionVector.Y*90.f, directionVector.Z*90.f });
					}
					else if (data.targetActorDirection == EVectorDirection::Left) {
						if (sceneC) {
							directionVector = sceneC->GetRightVector()*1.f;
						}
						else {
							directionVector = acto->GetActorRightVector()*1.f;
						}
						directionVector.Normalize();
						UKismetMathLibrary::ComposeRotators(rot, { directionVector.X*90.f,directionVector.Y*90.f, directionVector.Z*90.f });
					}
					else if (data.targetActorDirection == EVectorDirection::Up) {
						if (sceneC) {
							directionVector = sceneC->GetUpVector();
						}
						else {
							directionVector = acto->GetActorUpVector();
						}
						directionVector.Normalize();
						UKismetMathLibrary::ComposeRotators(rot, { directionVector.X*90.f,directionVector.Y*90.f, directionVector.Z*90.f });
					}
					else if (data.targetActorDirection == EVectorDirection::Down) {
						if (sceneC) {
							directionVector = sceneC->GetUpVector()*1.f;
						}
						else {
							directionVector = acto->GetActorUpVector()*1.f;
						}
						directionVector.Normalize();
						UKismetMathLibrary::ComposeRotators(rot, { directionVector.X*90.f,directionVector.Y*90.f, directionVector.Z*90.f });
					}
				}
				else {
					if (sceneC) {
						rot = UKismetMathLibrary::FindLookAtRotation(act->GetActorLocation(), sceneC->GetComponentLocation());
					}
					else {
						rot = UKismetMathLibrary::FindLookAtRotation(act->GetActorLocation(), acto->GetActorLocation());
					}
				}
				rot += data.rotation;
				FRotator finalRot = rot;
				FRotator thisRot = act->GetActorRotation();
				if (data.rotateOnlyOn == ESpecificRotAxis::Pitch) {
					finalRot = FRotator(rot.Pitch, thisRot.Yaw, thisRot.Roll);
				}
				else if (data.rotateOnlyOn == ESpecificRotAxis::Yaw) {
					finalRot = FRotator(thisRot.Pitch, rot.Yaw, thisRot.Roll);
				}
				else if (data.rotateOnlyOn == ESpecificRotAxis::Roll) {
					finalRot = FRotator(thisRot.Pitch, thisRot.Yaw, rot.Roll);
				}
				act->SetActorRotation(finalRot);
			}
		}
		else {
			act->SetActorRotation(data.rotation);
		}
	}
}

void UInteractComponent::SetVisibilityActor(AActor* actor, FSetVisibilityActor& data, UInteractComponent* interactComponent)
{
	if (data.actor.enumName == EGetActorList::None) {
		if (data.doOnActor) {
			interactComponent->GetOwner()->SetActorHiddenInGame(!data.visible);
			if (data.propagateChildren) {
				TSet<UActorComponent*> comps;
				comps = interactComponent->GetOwner()->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (scene && scene->GetClass() != UCapsuleComponent::StaticClass()) {
						scene->SetHiddenInGame(!data.visible);
					}
				}
			}
		}
		if (IsValid(data.specificChild)) {
			TSet<UActorComponent*> comps;
			comps = interactComponent->GetOwner()->GetComponents();
			for (int i = 0; i < comps.Array().Num(); i++) {
				USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
				if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
					if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
						scene->SetHiddenInGame(!data.visible);
					}
				}
				else {
					if (scene && scene->GetClass() == data.specificChild) {
						scene->SetHiddenInGame(!data.visible);
					}
				}
			}
		}
		else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
			TSet<UActorComponent*> comps;
			comps = interactComponent->GetOwner()->GetComponents();
			for (int i = 0; i < comps.Array().Num(); i++) {
				USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
				if (scene && scene->ComponentHasTag(data.componentTag)) {
					scene->SetHiddenInGame(!data.visible);
				}
			}
		}
	}
	else {
		int val = (int)data.actor.enumName - 1;
		AActor* acto = nullptr;
		if (val >= 0) {
			acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
		}
		if (acto) {
			if (data.doOnActor) {
				acto->SetActorHiddenInGame(!data.visible);
				if (data.propagateChildren) {
					TSet<UActorComponent*> comps;
					comps = acto->GetComponents();
					for (int i = 0; i < comps.Array().Num(); i++) {
						USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
						if (scene && scene->GetClass() != UCapsuleComponent::StaticClass()) {
							scene->SetHiddenInGame(!data.visible);
						}
					}
				}
			}
			if (IsValid(data.specificChild)) {
				TSet<UActorComponent*> comps;
				comps = acto->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
						if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
							scene->SetHiddenInGame(!data.visible);
						}
					}
					else {
						if (scene && scene->GetClass() == data.specificChild) {
							scene->SetHiddenInGame(!data.visible);
						}
					}
				}
			}
			else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
				TSet<UActorComponent*> comps;
				comps = acto->GetComponents();
				for (int i = 0; i < comps.Array().Num(); i++) {
					USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
					if (scene && scene->ComponentHasTag(data.componentTag)) {
						scene->SetHiddenInGame(!data.visible);
					}
				}
			}
		}
	}

}

void UInteractComponent::SetTransformActor(AActor* actor, FSetTransformActor& data, UInteractComponent* interactComponent)
{
	if (data.actor.enumName != EGetActorList::None) {
		int val = (int)data.actor.enumName - 1;
		AActor* acto = nullptr;
		if (val >= 0) {
			if (data.actorDebug) {
				acto = data.actorDebug;
			}
			else {
				acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
			}
		}
		if (acto) {
			if (data.setRelative) {
				if (!data.curve.curve) {
					if (data.setLocation) {
						if (IsValid(data.specificChild)) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
										FVector newPos = scene->GetComponentLocation();
										newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y, newPos.Z + data.location.Z);
										scene->SetWorldLocation(newPos);
									}
								}
								else {
									if (scene && scene->GetClass() == data.specificChild) {
										FVector newPos = scene->GetComponentLocation();
										newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y, newPos.Z + data.location.Z);
										scene->SetWorldLocation(newPos);
									}
								}
							}
						}
						else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (scene && scene->ComponentHasTag(data.componentTag)) {
									FVector newPos = scene->GetComponentLocation();
									newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y, newPos.Z + data.location.Z);
									scene->SetWorldLocation(newPos);
								}
							}
						}
						else {
							FVector newPos = acto->GetActorLocation();
							newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y, newPos.Z + data.location.Z);
							acto->SetActorLocation(newPos);
						}
					}
					if (data.setRotation) {
						if (IsValid(data.specificChild)) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
										scene->AddLocalRotation(data.rotation);
									}
								}
								else {
									if (scene && scene->GetClass() == data.specificChild) {
										scene->AddLocalRotation(data.rotation);
									}
								}
							}
						}
						else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (scene && scene->ComponentHasTag(data.componentTag)) {
									scene->AddLocalRotation(data.rotation);
								}
							}
						}
						else {
							acto->AddActorLocalRotation(data.rotation);
						}
					}
					if (data.setScale) {
						if (IsValid(data.specificChild)) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
										FVector newPos = scene->GetComponentScale();
										newPos += data.scale;
										scene->SetWorldScale3D(newPos);
									}
								}
								else {
									if (scene && scene->GetClass() == data.specificChild) {
										FVector newPos = scene->GetComponentScale();
										newPos += data.scale;
										scene->SetWorldScale3D(newPos);
									}
								}
							}
						}
						else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (scene && scene->ComponentHasTag(data.componentTag)) {
									FVector newPos = scene->GetComponentScale();
									newPos += data.scale;
									scene->SetWorldScale3D(newPos);
								}
							}
						}
						else {
							FVector newPos = acto->GetActorScale();
							newPos += data.scale;
							acto->SetActorScale3D(newPos);
						}
					}
				}
				else {
					data.curve.curve->GetTimeRange(data.minTimeVal, data.maxTimeVal);
					if (data.acumTime == NaN) {
						data.acumTime = 0.f;
					}
					data.acumTime += data.curve.updateTime;
					if (data.acumTime <= data.maxTimeVal) {
						float val1 = data.curve.curve->GetFloatValue(data.acumTime);
						float val2 = data.acumTime;
						if (data.curve.negateFirstCoordinate) {
							val1 *= -data.curve.updateAmount;
						}
						else {
							val1 *= data.curve.updateAmount;
						}
						if (data.curve.negateSecondCoordinate) {
							val2 *= -data.curve.updateAmount;
						}
						else {
							val2 *= data.curve.updateAmount;
						}
						if (data.curve.curveCoordinates == ECurveCoordinates::XY) {
							if (data.setLocation) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FVector newPos = scene->GetComponentLocation();
												newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y + val2, newPos.Z + data.location.Z);
												scene->SetWorldLocation(newPos);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FVector newPos = scene->GetComponentLocation();
												newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y + val2, newPos.Z + data.location.Z);
												scene->SetWorldLocation(newPos);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FVector newPos = scene->GetComponentLocation();
											newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y + val2, newPos.Z + data.location.Z);
											scene->SetWorldLocation(newPos);
										}
									}
								}
								else {
									FVector newPos = acto->GetActorLocation();
									newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y + val2, newPos.Z + data.location.Z);
									acto->SetActorLocation(newPos);
								}
							}
							if (data.setRotation) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FRotator newPos = data.rotation;
												newPos.Add(val1, val2, 0.f);
												scene->AddLocalRotation(data.rotation);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FRotator newPos = data.rotation;
												newPos.Add(val1, val2, 0.f);
												scene->AddLocalRotation(data.rotation);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FRotator newPos = data.rotation;
											newPos.Add(val1, val2, 0.f);
											scene->AddLocalRotation(data.rotation);
										}
									}
								}
								else {
									FRotator newPos = data.rotation;
									newPos.Add(val1, val2, 0.f);
									acto->AddActorLocalRotation(data.rotation);
								}
							}
							if (data.setScale) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FVector newPos = scene->GetComponentScale();
												newPos.Set(data.scale.X + val1, data.scale.Y + val2, data.scale.Z);
												scene->SetWorldScale3D(newPos);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FVector newPos = scene->GetComponentScale();
												newPos.Set(data.scale.X + val1, data.scale.Y + val2, data.scale.Z);
												scene->SetWorldScale3D(newPos);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FVector newPos = scene->GetComponentScale();
											newPos.Set(data.scale.X + val1, data.scale.Y + val2, data.scale.Z);
											scene->SetWorldScale3D(newPos);
										}
									}
								}
								else {
									FVector newPos = acto->GetActorScale();
									newPos.Set(data.scale.X + val1, data.scale.Y + val2, data.scale.Z);
									acto->SetActorScale3D(newPos);
								}
							}
						}
						else if (data.curve.curveCoordinates == ECurveCoordinates::XZ) {
							if (data.setLocation) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FVector newPos = scene->GetComponentLocation();
												newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y, newPos.Z + data.location.Z + val2);
												scene->SetWorldLocation(newPos);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FVector newPos = scene->GetComponentLocation();
												newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y, newPos.Z + data.location.Z + val2);
												scene->SetWorldLocation(newPos);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FVector newPos = scene->GetComponentLocation();
											newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y, newPos.Z + data.location.Z + val2);
											scene->SetWorldLocation(newPos);
										}
									}
								}
								else {
									FVector newPos = acto->GetActorLocation();
									newPos.Set(newPos.X + data.location.X + val1, newPos.Y + data.location.Y, newPos.Z + data.location.Z + val2);
									acto->SetActorLocation(newPos);
								}
							}
							if (data.setRotation) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FRotator newPos = data.rotation;
												newPos.Add(val1, 0.f, val2);
												scene->AddLocalRotation(data.rotation);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FRotator newPos = data.rotation;
												newPos.Add(val1, 0.f, val2);
												scene->AddLocalRotation(data.rotation);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FRotator newPos = data.rotation;
											newPos.Add(val1, 0.f, val2);
											scene->AddLocalRotation(data.rotation);
										}
									}
								}
								else {
									FRotator newPos = data.rotation;
									newPos.Add(val1, 0.f, val2);
									acto->AddActorLocalRotation(data.rotation);
								}
							}
							if (data.setScale) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FVector newPos = scene->GetComponentScale();
												newPos.Set(data.scale.X + val1, data.scale.Y, data.scale.Z + val2);
												scene->SetWorldScale3D(newPos);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FVector newPos = scene->GetComponentScale();
												newPos.Set(data.scale.X + val1, data.scale.Y, data.scale.Z + val2);
												scene->SetWorldScale3D(newPos);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FVector newPos = scene->GetComponentScale();
											newPos.Set(data.scale.X + val1, data.scale.Y, data.scale.Z + val2);
											scene->SetWorldScale3D(newPos);
										}
									}
								}
								else {
									FVector newPos = acto->GetActorScale();
									newPos.Set(data.scale.X + val1, data.scale.Y, data.scale.Z + val2);
									acto->SetActorScale3D(newPos);
								}
							}
						}
						else if (data.curve.curveCoordinates == ECurveCoordinates::YZ) {
							if (data.setLocation) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FVector newPos = scene->GetComponentLocation();
												newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y + val1, newPos.Z + data.location.Z + val2);
												scene->SetWorldLocation(newPos);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FVector newPos = scene->GetComponentLocation();
												newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y + val1, newPos.Z + data.location.Z + val2);
												scene->SetWorldLocation(newPos);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FVector newPos = scene->GetComponentLocation();
											newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y + val1, newPos.Z + data.location.Z + val2);
											scene->SetWorldLocation(newPos);
										}
									}
								}
								else {
									FVector newPos = acto->GetActorLocation();
									newPos.Set(newPos.X + data.location.X, newPos.Y + data.location.Y + val1, newPos.Z + data.location.Z + val2);
									acto->SetActorLocation(newPos);
								}
							}
							if (data.setRotation) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FRotator newPos = data.rotation;
												newPos.Add(0.f, val1, val2);
												scene->AddLocalRotation(data.rotation);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FRotator newPos = data.rotation;
												newPos.Add(0.f, val1, val2);
												scene->AddLocalRotation(data.rotation);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FRotator newPos = data.rotation;
											newPos.Add(0.f, val1, val2);
											scene->AddLocalRotation(data.rotation);
										}
									}
								}
								else {
									FRotator newPos = data.rotation;
									newPos.Add(0.f, val1, val2);
									acto->AddActorLocalRotation(data.rotation);
								}
							}
							if (data.setScale) {
								if (IsValid(data.specificChild)) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
											if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
												FVector newPos = scene->GetComponentScale();
												newPos.Set(data.scale.X, data.scale.Y + val1, data.scale.Z + val2);
												scene->SetWorldScale3D(newPos);
											}
										}
										else {
											if (scene && scene->GetClass() == data.specificChild) {
												FVector newPos = scene->GetComponentScale();
												newPos.Set(data.scale.X, data.scale.Y + val1, data.scale.Z + val2);
												scene->SetWorldScale3D(newPos);
											}
										}
									}
								}
								else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
									TSet<UActorComponent*> comps;
									comps = acto->GetComponents();
									for (int i = 0; i < comps.Array().Num(); i++) {
										USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
										if (scene && scene->ComponentHasTag(data.componentTag)) {
											FVector newPos = scene->GetComponentScale();
											newPos.Set(data.scale.X, data.scale.Y + val1, data.scale.Z + val2);
											scene->SetWorldScale3D(newPos);
										}
									}
								}
								else {
									FVector newPos = acto->GetActorScale();
									newPos.Set(data.scale.X, data.scale.Y + val1, data.scale.Z + val2);
									acto->SetActorScale3D(newPos);
								}
							}
						}
					}
					else {
						data.acumTime = NaN;
						data.minTimeVal = NaN;
						data.maxTimeVal = NaN;
					}
				}
			}
			else {
				if (data.setLocation) {
					if (IsValid(data.specificChild)) {
						TSet<UActorComponent*> comps;
						comps = acto->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++) {
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
								if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
									scene->SetWorldLocation(data.location);
								}
							}
							else {
								if (scene && scene->GetClass() == data.specificChild) {
									scene->SetWorldLocation(data.location);
								}
							}
						}
					}
					else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
						TSet<UActorComponent*> comps;
						comps = acto->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++) {
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (scene && scene->ComponentHasTag(data.componentTag)) {
								scene->SetWorldLocation(data.location);
							}
						}
					}
					else {
						acto->SetActorLocation(data.location);
					}
				}
				if (data.setRotation) {
					if (IsValid(data.specificChild)) {
						TSet<UActorComponent*> comps;
						comps = acto->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++) {
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
								if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
									scene->SetWorldRotation(data.rotation);
								}
							}
							else {
								if (scene && scene->GetClass() == data.specificChild) {
									scene->SetWorldRotation(data.rotation);
								}
							}
						}
					}
					else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
						TSet<UActorComponent*> comps;
						comps = acto->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++) {
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (scene && scene->ComponentHasTag(data.componentTag)) {
								scene->SetWorldRotation(data.rotation);
							}
						}
					}
					else {
						acto->SetActorRotation(data.rotation);
					}
				}
				if (data.setScale) {
					if (IsValid(data.specificChild)) {
						TSet<UActorComponent*> comps;
						comps = acto->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++) {
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
								if (scene && scene->GetClass() == data.specificChild && scene->ComponentHasTag(data.componentTag)) {
									scene->SetWorldScale3D(data.scale);
								}
							}
							else {
								if (scene && scene->GetClass() == data.specificChild) {
									scene->SetWorldScale3D(data.scale);
								}
							}
						}
					}
					else if (data.componentTag.ToString() != "" && data.componentTag != NAME_None) {
						TSet<UActorComponent*> comps;
						comps = acto->GetComponents();
						for (int i = 0; i < comps.Array().Num(); i++) {
							USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
							if (scene && scene->ComponentHasTag(data.componentTag)) {
								scene->SetWorldScale3D(data.scale);
							}
						}
					}
					else {
						acto->SetActorScale3D(data.scale);
					}
				}
			}
		}
	}
}

void UInteractComponent::PlaySoundFunction(AActor* actor, FPlaySoundFunction& data, UInteractComponent* interactComponent)
{
	UAudioComponent* audComp = nullptr;
	if (data.sound) {
		if (data.atLocation) {
			FVector loc = data.offsetLocation;
			USoundAttenuation* AttenuationSettings = NewObject<USoundAttenuation>();
			FSoundAttenuationSettings soundAtt = FSoundAttenuationSettings();
			soundAtt.AbsorptionMethod = data.attenuationType == ESoundAttenuationFunction::Linear ? EAirAbsorptionMethod::Linear : EAirAbsorptionMethod::CustomCurve;
			soundAtt.AttenuationShape = EAttenuationShape::Capsule;
			soundAtt.FalloffDistance = data.falloutDistance;
			if (data.attenuationType == ESoundAttenuationFunction::Linear) {
				soundAtt.DistanceAlgorithm = EAttenuationDistanceModel::Linear;
			}
			else if (data.attenuationType == ESoundAttenuationFunction::Inverse) {
				soundAtt.DistanceAlgorithm = EAttenuationDistanceModel::Inverse;
			}
			else if (data.attenuationType == ESoundAttenuationFunction::Logarithmic) {
				soundAtt.DistanceAlgorithm = EAttenuationDistanceModel::Logarithmic;
			}
			else if (data.attenuationType == ESoundAttenuationFunction::LogReverse) {
				soundAtt.DistanceAlgorithm = EAttenuationDistanceModel::LogReverse;
			}
			else if (data.attenuationType == ESoundAttenuationFunction::NaturalSound) {
				soundAtt.DistanceAlgorithm = EAttenuationDistanceModel::NaturalSound;
			}
			else if (data.attenuationType == ESoundAttenuationFunction::CustomCurve) {
				soundAtt.DistanceAlgorithm = EAttenuationDistanceModel::Custom;
			}
			AttenuationSettings->Attenuation = soundAtt;
			if (data.actor.enumName != EGetActorList::None) {
				int val = (int)data.actor.enumName - 1;
				AActor* acto = nullptr;
				if (val >= 0) {
					acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
				}
				if (acto) {
					FVector newVector = acto->GetActorLocation();
					if (data.targetActorDirection != EVectorDirection::None) {
						FVector directionVector = { 0.f, 0.f, 0.f };
						if (data.targetActorDirection == EVectorDirection::Forward) {
							directionVector = acto->GetActorForwardVector();
						}
						else if (data.targetActorDirection == EVectorDirection::Backward) {
							directionVector = acto->GetActorForwardVector()*-1.f;
						}
						else if (data.targetActorDirection == EVectorDirection::Right) {
							directionVector = acto->GetActorRightVector();
						}
						else if (data.targetActorDirection == EVectorDirection::Left) {
							directionVector = acto->GetActorRightVector()*-1.f;
						}
						else if (data.targetActorDirection == EVectorDirection::Up) {
							directionVector = acto->GetActorUpVector();
						}
						else if (data.targetActorDirection == EVectorDirection::Down) {
							directionVector = acto->GetActorUpVector()*-1.f;
						}
						directionVector.Normalize();
						directionVector = directionVector * data.distanceDirection;
						newVector.Set(newVector.X + directionVector.X, newVector.Y + directionVector.Y, newVector.Z + directionVector.Z);
					}
					newVector.Set(newVector.X + data.offsetLocation.X, newVector.Y + data.offsetLocation.Y, newVector.Z + data.offsetLocation.Z);
					loc = newVector;
					if (data.attachToActor) {
						USceneComponent* otherCompRot = nullptr;
						if (IsValid(data.attachedComponent)) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (data.attachedComponentTag.ToString() != "" && data.attachedComponentTag != NAME_None) {
									if (scene && scene->GetClass() == data.attachedComponent && scene->ComponentHasTag(data.attachedComponentTag)) {
										otherCompRot = scene;
										//scene->AddLocalRotation(data.rotation);
									}
								}
								else {
									if (scene && scene->GetClass() == data.attachedComponent) {
										otherCompRot = scene;
										//scene->AddLocalRotation(data.rotation);
									}
								}
							}
						}
						else if (data.attachedComponentTag.ToString() != "" && data.attachedComponentTag != NAME_None) {
							TSet<UActorComponent*> comps;
							comps = acto->GetComponents();
							for (int i = 0; i < comps.Array().Num(); i++) {
								USceneComponent* scene = Cast<USceneComponent>(comps.Array()[i]);
								if (scene && scene->ComponentHasTag(data.attachedComponentTag)) {
									otherCompRot = scene;
									//scene->AddLocalRotation(data.rotation);
								}
							}
						}
						if (otherCompRot) {
							audComp = UGameplayStatics::SpawnSoundAttached(data.sound, otherCompRot, data.attachedPointName, loc, EAttachLocation::KeepRelativeOffset, true, data.volumeMultiplier, data.pitchMultiplier, data.startTime, AttenuationSettings);
							if (audComp) {
								interactComponent->m_sharedFunctionObjects.Add(Cast<UObject>(audComp));
							}
							return;
						}
					}
				}
			}
			if (data.overrideValues) {
				audComp = UGameplayStatics::SpawnSoundAtLocation(interactComponent->GetWorld(), data.sound, loc, { 0.f, 0.f, 0.f }, data.volumeMultiplier, data.pitchMultiplier, data.startTime, AttenuationSettings);
			}
			else {
				audComp = UGameplayStatics::SpawnSoundAtLocation(interactComponent->GetWorld(), data.sound, loc, { 0.f, 0.f, 0.f });
			}
		}
		else {
			if (data.overrideValues) {
				audComp = UGameplayStatics::SpawnSound2D(interactComponent->GetWorld(), data.sound, data.volumeMultiplier, data.pitchMultiplier, data.startTime);
			}
			else {
				audComp = UGameplayStatics::SpawnSound2D(interactComponent->GetWorld(), data.sound);
			}
		}
		if (audComp) {
			interactComponent->m_sharedFunctionObjects.Add(Cast<UObject>(audComp));
			data.audioComponent = audComp;
			data.savedState.tempCurrentStage = interactComponent->m_currentStage;
			data.savedState.tempFunctionIndex = interactComponent->functionIndex;
			data.savedState.tempStage = interactComponent->tempType;
			data.savedState.tempAction = interactComponent->tempAction;
			if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::playSoundFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
				interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::playSoundFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
			}
			interactComponent->PlaySoundTimer(actor, data);
		}
	}
}

void UInteractComponent::PlaySoundTimer(AActor *actor, FPlaySoundFunction& data)
{
	if (GetWorld() && GetOwner() &&m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::playSoundFunction) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		if (data.audioComponent) {
			if (!data.tempOnce) {
				data.tempOnce = true;
				FTimerDelegate soundDel;
				soundDel.BindUFunction(this, FName("PlaySoundTimer"), actor, data);
				GetWorld()->GetTimerManager().SetTimer(data.soundTimer, soundDel, data.audioComponent->Sound->GetDuration(), false);
			}
			else {
				for (int i = 0; i < m_sharedFunctionObjects.Num(); i++) {
					if (m_sharedFunctionObjects[i]->IsValidLowLevel()) {
						UAudioComponent* audComp = Cast<UAudioComponent>(m_sharedFunctionObjects[i]);
						if (audComp) {
							if (data.sound) {
								if (audComp->Sound->GetName() == data.sound->GetName()) {
									if (data.isLoop) {
										if (data.audioComponent->IsValidLowLevel()) {
											data.audioComponent->Play();
										}
										FTimerDelegate soundDel;
										soundDel.BindUFunction(this, FName("PlaySoundTimer"), actor, data);
										GetWorld()->GetTimerManager().SetTimer(data.soundTimer, soundDel, data.audioComponent->Sound->GetDuration(), false);
									}
									break;
								}
							}
						}
					}
					else {
						m_sharedFunctionObjects.RemoveAt(i);
					}
				}
			}
		}
	}
}

void UInteractComponent::StopSoundFunction(AActor* actor, FStopSoundFunction& data, UInteractComponent* interactComponent)
{
	for (int i = 0; i < interactComponent->m_sharedFunctionObjects.Num(); i++) {
		if (interactComponent->m_sharedFunctionObjects[i]->IsValidLowLevel()) {
			UAudioComponent* audComp = Cast<UAudioComponent>(interactComponent->m_sharedFunctionObjects[i]);
			if (audComp) {
				if (data.sound) {
					if (audComp->Sound->GetName() == data.sound->GetName()) {
						audComp->DestroyComponent();
						interactComponent->m_sharedFunctionObjects.RemoveAt(i);
						if (!data.destroyAll) {
							break;
						}
					}
				}
			}
		}
	}
}

bool UInteractComponent::CheckIfSoundIsPlaying(AActor* actor, FCheckIfSoundIsPlaying& data, UInteractComponent* interactComponent)
{
	for (int i = 0; i < interactComponent->m_sharedFunctionObjects.Num(); i++) {
		if (interactComponent->m_sharedFunctionObjects[i]->IsValidLowLevel()) {
			UAudioComponent* audComp = Cast<UAudioComponent>(interactComponent->m_sharedFunctionObjects[i]);
			if (audComp) {
				if (data.sound) {
					if (audComp->Sound->GetName() == data.sound->GetName()) {
						return audComp->IsPlaying();
					}
				}
			}
		}
	}
	return false;
}


bool UInteractComponent::PlaceholderCheck(AActor* actor, FPlaceholderCheck& data, UInteractComponent* interactComponent)
{
	return true;
}

void UInteractComponent::ModifySoundFunction(AActor* actor, FModifySoundFunction& data, UInteractComponent* interactComponent)
{
	for (int i = 0; i < interactComponent->m_sharedFunctionObjects.Num(); i++) {
		if (interactComponent->m_sharedFunctionObjects[i]->IsValidLowLevel()) {
			UAudioComponent* audComp = Cast<UAudioComponent>(interactComponent->m_sharedFunctionObjects[i]);
			if (audComp) {
				if (data.sound) {
					if (audComp->Sound->GetName() == data.sound->GetName()) {
						if (data.setVolumeMultiplier) {
							audComp->SetVolumeMultiplier(data.volumeMultiplier);
						}
						if (data.setPitchMultiplier) {
							audComp->SetPitchMultiplier(data.pitchMultiplier);
						}
						if (!data.doOnAll) {
							break;
						}
					}
				}
			}
		}
	}
}

void UInteractComponent::CameraEffect(AActor* actor, FCameraEffect& data, UInteractComponent* interactComponent)
{
	ACharacter* character = Cast<ACharacter>(actor);
	if (!character) {
		character = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(interactComponent->GetWorld(), 0));
	}
	if (character) {
		if (data.selectEffect == ECameraEffect::Fade) {
			UGameplayStatics::GetPlayerCameraManager(interactComponent->GetWorld(), 0)->StartCameraFade(data.fadeEffect.fromAlpha, data.fadeEffect.toAlpha, data.fadeEffect.duration, data.fadeEffect.color, data.fadeEffect.fadeAudio, true);
		}
		else if (data.selectEffect == ECameraEffect::DepthOfField) {
/*
			character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.bOverride_DepthOfFieldMethod = 1;
			character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = 1;
			character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.bOverride_DepthOfFieldFocalRegion = 1;

			character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_Gaussian;
			character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.DepthOfFieldFocalDistance = data.depthOfFieldEffect.FocalDistance;
			character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.DepthOfFieldFocalRegion = data.depthOfFieldEffect.FocalRegion;*/
		}
		else if (data.selectEffect == ECameraEffect::CancelDepthOfField) {

			//character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.bOverride_DepthOfFieldMethod = 0;
			//character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = 0;
			//character->getCameraComponent()->getDefaultCamera()->PostProcessSettings.bOverride_DepthOfFieldFocalRegion = 0;
		}
	}
}

void UInteractComponent::OpenLevelFunction(AActor* actor, FOpenLevelFunction& data, UInteractComponent* interactComponent)
{
	if (data.restartLevel) {
		UGameplayStatics::OpenLevel(interactComponent->GetWorld(), FName(*interactComponent->GetWorld()->GetName()), false, data.options);
	}
	else {
		UGameplayStatics::OpenLevel(interactComponent->GetWorld(), data.levelName, false, data.options);
	}
}

void UInteractComponent::ChangePriority(AActor* actor, FChangePriority& data, UInteractComponent* interactComponent)
{
	int val = (int)data.actor.enumName - 1;
	AActor* acto = nullptr;
	if (val >= 0) {
		acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
	}
	if (acto) {
		UInteractComponent* interactComp = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
		if (interactComp) {
			if (interactComp->interactDataArray.IsValidIndex(data.stage)) {
				interactComp->interactDataArray[data.stage].priorityStage = data.priority;
				interactComp->interactDataArray[data.stage].doOtherInteract = data.doOtherInteracts;
			}
		}
	}
}

void UInteractComponent::ExecCommand(AActor *actor, FExecCommand& data, UInteractComponent* interactComponent) {
	GEngine->Exec(interactComponent->GetWorld(), *data.command);
}

void UInteractComponent::ExecuteTrigger(AActor* actor, FExecuteTrigger& data, UInteractComponent* interactComponent)
{
	AActor* intActor = UInteractComponent::GetCurrentActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
// 	if (data.currentActor.customActor) {
// 		if (data.currentActor.currentTag.ToString() != "" && data.currentActor.currentTag != NAME_None) {
// 			TArray<AActor*> actors;
// 			UGameplayStatics::GetAllActorsWithTag(interactComponent->GetWorld(), data.currentActor.currentTag, actors);
// 			for (int i = 0; i < actors.Num(); i++) {
// 				if ((actors[i]->GetClass() == data.currentActor.currentActor) || !data.currentActor.currentActor) {
// 					intActor = actors[i];
// 					break;
// 				}
// 			}
// 		}
// 		else {
// 			TArray<AActor*> actors;
// 			UGameplayStatics::GetAllActorsOfClass(interactComponent->GetWorld(), data.currentActor.currentActor, actors);
// 			if (actors.Num() > 0) {
// 				intActor = actors[0];
// 			}
// 		}
// 	}
// 	else {
// 		if (data.currentActor.doOnOwner) {
// 			intActor = interactComponent->GetOwner();
// 		}
// 		else if (actor) {
// 			intActor = actor;
// 		}
// 	}

	if (intActor) {
		if (data.actor.enumName != EGetActorList::None) {
			AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
			if (acto) {
				UInteractComponent* intComp = Cast<UInteractComponent>(intActor->GetComponentByClass(UInteractComponent::StaticClass()));
				if (intComp) {
					if (data.enterTrigger == ETriggerSelect::Enter) {
						intComp->TriggerCapsuleOverlapBegin(nullptr, acto, nullptr, 0, false, {});
					}
					else if (data.enterTrigger == ETriggerSelect::Exit) {
						intComp->TriggerCapsuleOverlapEnd(nullptr, acto, nullptr, 0);
					}
					if (data.enterHighlight == ETriggerSelect::Enter) {
						intComp->OutsideCapsuleOverlapBegin(nullptr, acto, nullptr, 0, false, {});
					}
					else if (data.enterHighlight == ETriggerSelect::Exit) {
						intComp->OutsideCapsuleOverlapEnd(nullptr, acto, nullptr, 0);
					}
					if (data.enterInside == ETriggerSelect::Enter) {
						intComp->InsideCapsuleOverlapBegin(nullptr, acto, nullptr, 0, false, {});
					}
					else  if (data.enterInside == ETriggerSelect::Exit) {
						intComp->InsideCapsuleOverlapEnd(nullptr, acto, nullptr, 0);
					}
				}
			}
		}
	}
}

void UInteractComponent::AddWidgetToViewport(AActor* actor, FAddWidgetToViewport& data, UInteractComponent* interactComponent)
{
	if (data.specificWidget) {
		UUserWidget* widget = CreateWidget<UUserWidget>(interactComponent->GetWorld(), data.specificWidget);
		if (widget) {
			widget->AddToViewport(data.zOrder);
		}
	}
}

void UInteractComponent::PlayWidgetAnimation(AActor *actor, FPlayWidgetAnimation& data, UInteractComponent* interactComponent)
{
	if (data.specificWidget) {
		TArray<UUserWidget*> widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(interactComponent->GetWorld(), widgets, data.specificWidget);
		UWidgetAnimation* widgetAnim = nullptr;
		for (int i = 0; i < widgets.Num(); i++) {
			UProperty* prop = widgets[i]->GetClass()->PropertyLink;

			// Run through all properties of this class to find any widget animations
			while (prop != nullptr)
			{
				// Only interested in object properties
				if (prop->GetClass() == UObjectProperty::StaticClass())
				{
					UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

					// Only want the properties that are widget animations
					if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
					{
						UObject* object = objectProp->GetObjectPropertyValue_InContainer(widgets[i]);

						widgetAnim = Cast<UWidgetAnimation>(object);

						if (widgetAnim != nullptr) {
							FString name = data.animationName + "_INST";

							if (widgetAnim->GetName() == name) {
								if (data.selectMode == EWidgetAnimMode::Forward) {
									widgets[i]->PlayAnimation(widgetAnim, data.startTime, data.numLoops, EUMGSequencePlayMode::Forward, data.speed);
								}
								else if (data.selectMode == EWidgetAnimMode::Reverse) {
									widgets[i]->PlayAnimation(widgetAnim, data.startTime, data.numLoops, EUMGSequencePlayMode::Reverse, data.speed);
								}
								else if (data.selectMode == EWidgetAnimMode::PingPong) {
									widgets[i]->PlayAnimation(widgetAnim, data.startTime, data.numLoops, EUMGSequencePlayMode::PingPong, data.speed);
								}
								break;
							}
						}
					}
				}

				prop = prop->PropertyLinkNext;
			}
		}
	}
}

void UInteractComponent::ShowHideOutlineShine(AActor *actor, FShowHideOutlineShine& data, UInteractComponent* interactComponent)
{
	AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);

	if (acto) {
		if (data.setOutline) {
			for (int i = 0; i < interactComponent->m_actorMesh.Num(); i++) {
				if (interactComponent->m_actorMesh[i] && data.outlineColor != EOutlineColor::None && interactComponent->m_actorMesh[i]->ComponentHasTag(FName(TEXT("light")))) {
					interactComponent->m_actorMesh[i]->SetCustomDepthStencilValue((uint8)data.outlineColor);
					interactComponent->m_actorMesh[i]->SetRenderCustomDepth(true);
				}
				else {
					interactComponent->m_actorMesh[i]->SetRenderCustomDepth(false);
				}
			}
		}
		if (interactComponent->interactDataArray.IsValidIndex(interactComponent->m_currentStage)) {
			if (data.outlineValue != EOutlineColor::None) {
				interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleHighlightStruct.m_CapsuleColor = data.outlineValue;
			}
		}
		if (data.setShine) {
			interactComponent->isShining = data.showShine;
			for (int i = 0; i < interactComponent->m_actorMesh.Num(); i++) {
				if (interactComponent->m_actorMesh[i] && interactComponent->m_actorMesh[i]->ComponentHasTag(FName(TEXT("shine")))) {
					UMaterialInterface* mat = interactComponent->m_actorMesh[i]->GetMaterial(0);
					if (mat) {
						UMaterialInstanceDynamic* matInst = Cast<UMaterialInstanceDynamic>(mat);
						if (!matInst) {
							matInst = UMaterialInstanceDynamic::Create(mat, interactComponent);
						}
						if (matInst) {
							matInst->SetScalarParameterValue(FName(TEXT("RimShader")), data.showShine ? 1 : 0);
							interactComponent->m_actorMesh[i]->SetMaterial(0, matInst);
						}
					}
				}
			}
		}
		if (data.shineValue == ESetBoolValue::E_True) {
			interactComponent->isShining = true;
		}
		else if (data.shineValue == ESetBoolValue::E_False) {
			interactComponent->isShining = false;
		}
	}
}

void UInteractComponent::RemoveWidgetFunction(AActor* actor, FRemoveWidgetFunction& data, UInteractComponent* interactComponent)
{
	if (data.specificWidget) {
		TArray<UUserWidget*> widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(interactComponent->GetWorld(), widgets, data.specificWidget);
		for (int i = 0; i < widgets.Num(); i++) {
			widgets[i]->RemoveFromParent();
			if (data.doOnce) {
				break;
			}
		}
	}
}

void UInteractComponent::SetPhysicsSettings(AActor* actor, FSetPhysicsSettings& data, UInteractComponent* interactComponent)
{
	if (data.m_actor == nullptr)
	{
		data.m_actor = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	}
	if (data.m_actor)
	{
// 		if (data.m_actor->IsA(AChPlayer::StaticClass()))
// 		{
// 			AChPlayer* player = Cast<AChPlayer>(data.m_actor);
// 			if (player)
// 			{
// 				if (data.ChangeMeshPhysics)
// 				{
// 					USkeletalMeshComponent* m_playerMesh = player->GetMesh();
// 
// 					if (m_playerMesh)
// 					{
// 						m_playerMesh->SetSimulatePhysics(data.MeshPhysicsSettings.SimulatePhysics);
// 					}
// 				}
// 				if (data.ChangeShapePhysics)
// 				{
// 					UCapsuleComponent* m_playerCapsule = player->GetCapsuleComponent();
// 
// 					if (m_playerCapsule)
// 					{
// 						m_playerCapsule->SetSimulatePhysics(data.ShapePhysicsSettings.SimulatePhysics);
// 					}
// 				}
// 			}
// 		}
		if (IInteractInterface* intInt = Cast<IInteractInterface>(data.m_actor))
		{
			AActor* otherCharacter = data.m_actor;
			if (otherCharacter)
			{
				if (data.ChangeMeshPhysics)
				{
					TArray<UMeshComponent *> m_otherCharacterMeshes = intInt->GetActorMesh();

					if (m_otherCharacterMeshes.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterMeshes.Num(); i++)
						{
							m_otherCharacterMeshes[i]->SetSimulatePhysics(data.MeshPhysicsSettings.SimulatePhysics);
						}
					}
				}
				if (data.ChangeShapePhysics)
				{
					TArray<UShapeComponent *> m_otherCharacterCollisions = intInt->GetActorCollision();

					if (m_otherCharacterCollisions.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterCollisions.Num(); i++)
						{
							m_otherCharacterCollisions[i]->SetSimulatePhysics(data.ShapePhysicsSettings.SimulatePhysics);
						}
					}
				}
			}
		}
		else
		{
			AActor* otherCharacter = data.m_actor;
			if (otherCharacter)
			{
				if (data.ChangeMeshPhysics)
				{
					TArray<UMeshComponent*> m_otherCharacterMeshes;
					TArray<UActorComponent*> ActorCArray = data.m_actor->GetComponentsByClass(UMeshComponent::StaticClass());
					for (int i = 0; i < ActorCArray.Num(); i++) {
						m_otherCharacterMeshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
					}

					if (m_otherCharacterMeshes.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterMeshes.Num(); i++)
						{
							m_otherCharacterMeshes[i]->SetSimulatePhysics(data.MeshPhysicsSettings.SimulatePhysics);
						}
					}
				}
				if (data.ChangeShapePhysics)
				{
					TArray<UShapeComponent*> m_otherCharacterCollisions;
					TArray<UActorComponent*> ActorBArray = data.m_actor->GetComponentsByTag(UBoxComponent::StaticClass(), "ModifiableShape");
					TArray<UActorComponent*> ActorSArray = data.m_actor->GetComponentsByTag(USphereComponent::StaticClass(), "ModifiableShape");
					TArray<UActorComponent*> ActorCArray = data.m_actor->GetComponentsByTag(UCapsuleComponent::StaticClass(), "ModifiableShape");

					if (ActorBArray.Num() != 0)
					{
						for (int i = 0; i < ActorBArray.Num(); i++) {
							m_otherCharacterCollisions.Add(Cast<UShapeComponent>(ActorBArray[i]));
						}
					}
					if (ActorSArray.Num() != 0)
					{
						for (int i = 0; i < ActorSArray.Num(); i++) {
							m_otherCharacterCollisions.Add(Cast<UShapeComponent>(ActorSArray[i]));
						}
					}
					if (ActorCArray.Num() != 0)
					{
						for (int i = 0; i < ActorCArray.Num(); i++) {
							m_otherCharacterCollisions.Add(Cast<UShapeComponent>(ActorCArray[i]));
						}
					}

					if (m_otherCharacterCollisions.Num() != 0)
					{
						for (int i = 0; i < m_otherCharacterCollisions.Num(); i++)
						{
							m_otherCharacterCollisions[i]->SetSimulatePhysics(data.ShapePhysicsSettings.SimulatePhysics);
						}
					}
				}
			}
		}
	}
	else
	{
		FString debugText = FString(TEXT("FUNCTION: SetPhysicsSettings || ERROR: The Target Actor is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
		ISM_ULOG_EDIT("%s", Error, *debugText);
	}
}

void UInteractComponent::AddTagFunction(AActor* actor, FAddTagFunction& data, UInteractComponent* interactComponent)
{
	if (data.m_actor == nullptr)
	{
		data.m_actor = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	}
	if (data.m_actor)
	{
		if (data.m_component == nullptr)
		{
			data.m_component = UInteractComponent::GetSceneComponent(interactComponent, data.m_actor, data.specificChild, data.componentTag);
		}
		if (data.addTag) {
			for (int i = 0; i < data.m_actorTag.Num(); i++)
			{
				data.m_actor->Tags.Add(data.m_actorTag[i]);
			}
		}
		else
		{
			for (int i = 0; i < data.m_actorTag.Num(); i++)
			{
				if (data.m_actor->Tags.Contains(data.m_actorTag[i])) {
					data.m_actor->Tags.Remove(data.m_actorTag[i]);
				}
			}
		}

		if (data.m_component)
		{
			if (data.addTag) {

				for (int i = 0; i < data.m_componentTag.Num(); i++)
				{
					data.m_component->ComponentTags.Add(data.m_componentTag[i]);
				}
			}
			else
			{
				for (int i = 0; i < data.m_componentTag.Num(); i++)
				{
					if (data.m_component->ComponentTags.Contains(data.m_componentTag[i])) {
						data.m_component->ComponentTags.Remove(data.m_componentTag[i]);
					}
				}
			}
		}
		else
		{
			FString debugText = FString(TEXT("FUNCTION: SetPhysicsSettings || ERROR: The Target Component is NULL. || INTERACT OWNER ACTOR: "));
			FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
			debugText.Append(ObjectName);
			FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
			debugText.Append(debugText2);
			FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
			debugText.Append(ObjectName2);

			//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
			ISM_ULOG_EDIT("%s", Error, *debugText);
		}
	}
	else
	{
		FString debugText = FString(TEXT("FUNCTION: AddTagFunction || ERROR: The Target Actor is NULL. || INTERACT OWNER ACTOR: "));
		FString ObjectName = (interactComponent->GetOwner() != NULL) ? interactComponent->GetOwner()->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName);
		FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
		debugText.Append(debugText2);
		FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
		debugText.Append(ObjectName2);

		//ISM_PRINTLOG_EDIT(debugText, 20.0f, FColor::Red);
		ISM_ULOG_EDIT("%s", Error, *debugText);
	}
}

void UInteractComponent::SetMaterialMesh(AActor* actor, FSetMaterialMesh& data, UInteractComponent* interactComponent)
{
	AActor* acto = UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	if (acto) {
		IInteractInterface* item = Cast<IInteractInterface>(acto);
		if (item) {
			if (item->GetActorMesh().IsValidIndex(data.meshIndex)) {
				item->GetActorMesh()[data.meshIndex]->SetMaterial(data.elementIndex, data.material);
			}
		}
		else {
			TArray<UMeshComponent*> meshes;
			TArray<UActorComponent*> ActorCArray = acto->GetComponentsByClass(UMeshComponent::StaticClass());
			for (int i = 0; i < ActorCArray.Num(); i++) {
				meshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
			}
			if (meshes.IsValidIndex(data.meshIndex)) {
				meshes[data.meshIndex]->SetMaterial(data.elementIndex, data.material);
			}
		}
// 		ACh* charact = Cast<ACh>(acto);
// 		if (charact) {
// 			if (charact->GetActorMesh().IsValidIndex(data.meshIndex)) {
// 				charact->GetActorMesh()[data.meshIndex]->SetMaterial(data.elementIndex, data.material);
// 			}
// 		}
	}
}

void UInteractComponent::JumpFunction(AActor* actor, FJumpFunction& data, UInteractComponent* interactComponent)
{
	if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction.Num() > 0) {
		int value = (int)UInteractCore::GetValue(data.function, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction.Num() - 1);
		interactComponent->functionIndex = value - 1;
		interactComponent->tempJumpFunction = true;
	}
}

void UInteractComponent::CancelJumpStage(AActor* actor, FCancelJumpStage& data, UInteractComponent* interactComponent)
{
	interactComponent->stopStage = false;
	interactComponent->tempJumpStage = false;
	interactComponent->tempJumpFunction = false;
	interactComponent->tempDelay = -1;
	interactComponent->tempTooltip = -2;
	if (interactComponent->GetWorld()) {
		interactComponent->GetWorld()->GetTimerManager().ClearTimer(interactComponent->nextHandle);
	}
}

void UInteractComponent::BreakFunction(AActor* actor, FBreakFunction& data, UInteractComponent* interactComponent)
{
	if (interactComponent->tempType == EActionStageType::Pre) {
		interactComponent->functionIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction.Num();
	}
	else if (interactComponent->tempType == EActionStageType::Init) {
		interactComponent->functionIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction.Num();
	}
	else if (interactComponent->tempType == EActionStageType::Exit) {
		interactComponent->functionIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction.Num();
	}
	else if (interactComponent->tempType == EActionStageType::Interact) {
		interactComponent->functionIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].actions[interactComponent->tempTooltipIndex].stageFunction.Num();
	}
	interactComponent->stopStage = true;
}

void UInteractComponent::PlayAnimCharacter(AActor* actor, FPlayAnimCharacter& data, UInteractComponent* interactComponent)
{
	ACharacter* character = Cast<ACharacter>(UInteractComponent::GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse));

	if (character) {
		data.characterBP = character->GetMesh()->GetAnimInstance()->GetFullName();
		character->PlayAnimMontage(data.animation, data.playRate, NAME_None);
		data.savedState.tempCurrentStage = interactComponent->m_currentStage;
		data.savedState.tempFunctionIndex = interactComponent->functionIndex;
		data.savedState.tempStage = interactComponent->tempType;
		data.savedState.tempAction = interactComponent->tempAction;
		data.duration = data.animation->GetPlayLength()/data.playRate;
		data.notifyEvents = data.animation->Notifies;
		if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::playAnimCharacter) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
			interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::playAnimCharacter) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
		}
		interactComponent->PlayAnimCharacterTimer(character, data);
	}
}

void UInteractComponent::PlayAnimCharacterTimer(ACharacter* actor, FPlayAnimCharacter& data)
{
	float fpsActuales = ALevelManager::GetFPS();
	float delta = 60.0f / fpsActuales;
	if (fpsActuales > 0 && fpsActuales < 60.0f) {
		data.deltaTimeAlt = delta * data.deltaTime * data.timerSpeedRate;
	}
	else {
		data.deltaTimeAlt = data.deltaTime * data.timerSpeedRate;
	}

	if (GetWorld() && GetOwner() && m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::playAnimCharacter) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		if (actor) {
			data.time += data.deltaTime;
			if(!data.doOnce){
				data.doOnce = true;
				FString noneS = "";
				FString nameC = "";
				data.characterBP.Split("/", &nameC, &noneS);
				FString nam = nameC.TrimStartAndEnd();
				nam.RemoveFromEnd("_C");
				FString resName = "AnimBlueprint'/Game/Characters/AnimBP/" + nam + "." + nameC.TrimStartAndEnd() + "'";
				actor->GetMesh()->SetAnimInstanceClass(StaticLoadClass(UObject::StaticClass(), nullptr, *resName));
			}
			if (data.notifyTime != -1.f && data.time >= data.notifyTime && !data.notifyOnce) {
				CallInteractDelegate(actor, EActionList::None, EDelegateList::None, data.notifyInfo, data.savedState.tempCurrentStage, data.savedState.tempFunctionIndex, data.savedState.tempStage, data.savedState.tempAction, GetOwner()->GetName(), false, EActionListCore::playAnimCharacter, EDelegateListCore::onAnimationTimer);
				data.notifyOnce = true;
			}
			for (int x = 0; x < data.notifyEvents.Num(); x++) {
				if (data.time >= data.notifyEvents[x].GetTriggerTime()) {
					CallInteractDelegate(actor, EActionList::None, EDelegateList::None, data.notifyEvents[x].GetNotifyEventName().ToString(), data.savedState.tempCurrentStage, data.savedState.tempFunctionIndex, data.savedState.tempStage, data.savedState.tempAction, GetOwner()->GetName(), false, EActionListCore::playAnimCharacter, EDelegateListCore::onAnimationNotification);
					data.notifyEvents.RemoveAt(x);
					x--;
				}
			}
			if (data.time < data.duration || data.looping) {
				if (data.time >= data.duration && data.looping && actor) {
					actor->PlayAnimMontage(data.animation, data.playRate, NAME_None);
					data.time = 0.f;
				}
				FTimerDelegate animationDel;
				animationDel.BindUFunction(this, FName("PlayAnimCharacterTimer"), actor, data);
				GetWorld()->GetTimerManager().SetTimer(data.animationTimer, animationDel, data.deltaTime, false);
			}
		}
	}
}

void UInteractComponent::DelayTimer(AActor* actor, FDelayFunction& data, int functIndex, bool tempDoOnceDel, int teTooltipIndex)
{
	TArray<FFunctionData> siz;
	if (tempType == EActionStageType::Pre) {
		siz = interactDataArray[m_currentStage].preStageFunction;
	}
	else if (tempType == EActionStageType::Interact) {
		if (teTooltipIndex != -1) {
			siz = interactDataArray[m_currentStage].actions[teTooltipIndex].stageFunction;
		}
		else {
			siz = interactDataArray[m_currentStage].actions[tempTooltipIndex].stageFunction;
		}
	}
	else if (tempType == EActionStageType::Init) {
		siz = interactDataArray[m_currentStage].capsuleTriggerStruct.initFunction;
	}
	else if (tempType == EActionStageType::Exit) {
		siz = interactDataArray[m_currentStage].capsuleTriggerStruct.exitFunction;
	}
	if (!tempDoOnceDel) {
		tempDelayFunction = false;
		data.ready = false;

		for (int i = 0; i < siz.Num(); i++) {
			if (siz[i].enumNameCore == EActionListCore::delayFunction) {
				functIndex = i + 1;
				break;
			}
		}
	}
	stopStage = false;

	int indFunc = functIndex;
	functionIndex = functIndex;
	while (indFunc < data.functions.Num())
	{
		if (functionIndex < siz.Num() && siz[functionIndex].enumNameCore != EActionListCore::delayFunction) {
			if (teTooltipIndex != -1) {
				tempTooltipIndex = teTooltipIndex;
				data.functions[indFunc].Execute(this, actor, tempAction, teTooltipIndex, tempType, -1, -1, -1, tempInElse);
			}
			else {
				data.functions[indFunc].Execute(this, actor, tempAction, tempTooltipIndex, tempType, -1, -1, -1, tempInElse);
			}
		}
		else if (functionIndex < siz.Num() && siz[functionIndex].enumNameCore == EActionListCore::delayFunction) {
			if (!data.isAffectedBySlowTime) {
				float dil = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
				FTimerDelegate delayDel;
				delayDel.BindUFunction(this, FName("DelayTimer"), actor, data, indFunc + 1, true, teTooltipIndex);
				if (dil == 0.f) {
					GetWorld()->GetTimerManager().SetTimer(data.delayHandle, delayDel, UInteractCore::GetValue(siz[functionIndex].delayFunction.delayAmount)*dil, false);
				}
				else {
					GetWorld()->GetTimerManager().SetTimer(data.delayHandle, delayDel, UInteractCore::GetValue(siz[functionIndex].delayFunction.delayAmount), false);
				}
			}
			else {
				FTimerDelegate delayDel;
				delayDel.BindUFunction(this, FName("DelayTimer"), actor, data, indFunc + 1, true, teTooltipIndex);
				GetWorld()->GetTimerManager().SetTimer(data.delayHandle, delayDel, UInteractCore::GetValue(siz[functionIndex].delayFunction.delayAmount), false);
			}
			stopStage = true;
		}

		functionIndex++;
		indFunc++;

		if (tempJumpFunction) {
			indFunc = functionIndex;
			tempJumpFunction = false;
		}

		if (indFunc >= data.functions.Num()) {
			data.ready = true;
		}
		if (stopStage) {
			break;
		}
	}

	if (data.ready) {
		data.functions.Empty();
		if (readyForNextStage && tempType == EActionStageType::Interact) {
			if (teTooltipIndex != -1) {
				int newStageint = (int)UInteractCore::GetValue(interactDataArray[m_currentStage].actions[teTooltipIndex].m_nextStage);
				if (tempTooltip != -2) {
					NextStage(tempTooltip, interactDataArray[m_currentStage].actions[teTooltipIndex]);
				}
				else {
					NextStage(newStageint, interactDataArray[m_currentStage].actions[teTooltipIndex]);
				}
			}
			else {
				int newStageint = (int)UInteractCore::GetValue(interactDataArray[m_currentStage].actions[tempTooltipIndex].m_nextStage);
				if (tempTooltip != -2) {
					NextStage(tempTooltip, interactDataArray[m_currentStage].actions[tempTooltipIndex]);
				}
				else {
					NextStage(newStageint, interactDataArray[m_currentStage].actions[tempTooltipIndex]);
				}
			}
		}
		data.ready = false;
	}
}


void UInteractComponent::DelayFunction(AActor* actor, FDelayFunction& data, UInteractComponent* interactComponent)
{
	interactComponent->GetWorld()->GetTimerManager().ClearTimer(data.delayHandle);
	interactComponent->stopStage = true;
	data.functions.Empty();
	TArray<FFunctionData> functions;
	interactComponent->tempDelayFunction = true;
	if (interactComponent->tempType == EActionStageType::Pre) {
		functions = interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction;
	}
	else if (interactComponent->tempType == EActionStageType::Interact) {
		functions = interactComponent->interactDataArray[interactComponent->m_currentStage].actions[interactComponent->tempTooltipIndex].stageFunction;
	}
	else if (interactComponent->tempType == EActionStageType::Init) {
		functions = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction;
	}
	else if (interactComponent->tempType == EActionStageType::Exit) {
		functions = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction;
	}

	interactComponent->tempDelayWrapper = data.functions.Num();

	for (int i = 0; i < functions.Num(); i++) {

		int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
		int value = ((int)functions[i].enumName) - 1;
		int valueCore = ((int)functions[i].enumNameCore) - 1;
		if (valueCore >= 0) {
			data.functions.Push(interactComponent->interactFunctionMap[valueCore]);
		}
		else if (value >= 0) {
			data.functions.Push(interactComponent->interactFunctionMap[value + maxE - 1]);
		}
	}

	if (!data.isAffectedBySlowTime) {
		float dil = UGameplayStatics::GetGlobalTimeDilation(interactComponent->GetWorld());
		FTimerDelegate delayDel;
		delayDel.BindUFunction(interactComponent, FName("DelayTimer"), actor, data, interactComponent->functionIndex + 1, false, interactComponent->tempTooltipIndex);
		float val2 = UInteractCore::GetValue(data.delayAmount);
		if (val2 != 0.f) {
			interactComponent->GetWorld()->GetTimerManager().SetTimer(data.delayHandle, delayDel, val2*dil, false);
		}
		else {
			interactComponent->GetWorld()->GetTimerManager().SetTimer(data.delayHandle, delayDel, val2, false);
		}
	}
	else {
		FTimerDelegate delayDel;
		delayDel.BindUFunction(interactComponent, FName("DelayTimer"), actor, data, interactComponent->functionIndex + 1, false, interactComponent->tempTooltipIndex);
		interactComponent->GetWorld()->GetTimerManager().SetTimer(data.delayHandle, delayDel, UInteractCore::GetValue(data.delayAmount), false);
	}
}

void UInteractComponent::SetTooltipLocationSize(AActor* actor, FSetTooltipLocationSize& data, UInteractComponent* interactComponent)
{
	AActor* acto = GetTargetActor(interactComponent, data.actor, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, interactComponent->tempInElse);
	if (acto) {
		UActorComponent* intComp = acto->GetComponentByClass(UInteractComponent::StaticClass());
		if (intComp) {
			UInteractComponent* interactComp = Cast<UInteractComponent>(intComp);
			if (data.specificStage != -1) {
				if (interactComp->interactDataArray.IsValidIndex(data.specificStage)) {
					for (int i = 0; i < interactComp->interactDataArray[data.specificStage].actions.Num(); i++) {
						if (data.setLocation) {
							if (IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
								UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
								if (m_TooltipWidget) {

									UInteractCore::checkCurrentPlatform();
									if (!interactComp->tooltipLoaded) {
										for (int j = 0; j < interactComp->interactDataArray.Num(); j++)
										{
											for (int k = 0; k < interactComp->interactDataArray[j].actions.Num(); k++)
											{
												if (IsValid(interactComp->interactDataArray[j].actions[k].m_WidgetComponent)) {
													UTooltip* m_TooltipWidget1 = Cast<UTooltip>(interactComp->interactDataArray[j].actions[k].m_WidgetComponent->GetUserWidgetObject());

													if (m_TooltipWidget1) {
														interactComp->interactDataArray[j].actions[k].m_button = UInteractCore::GetActionButtonImage(interactComp->interactDataArray[j].actions[k].actionButton);
														m_TooltipWidget1->SetButtonTexture(interactComp->interactDataArray[j].actions[k].m_button);
														m_TooltipWidget1->SetIconTexture(interactComp->interactDataArray[j].actions[k].m_icon);
														m_TooltipWidget1->SetCircleTexture(interactComp->interactDataArray[j].actions[k].m_circle);
														m_TooltipWidget1->SetTooltipSize(interactComp->interactDataArray[j].actions[k].widgetSize);
														if (interactComp->interactDataArray[j].actions[k].m_quickTimeEvent) {
															m_TooltipWidget1->delayQuickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_delayQuickEventTime);
															m_TooltipWidget1->quickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_quickEventTime);
															m_TooltipWidget1->speedOtherAnims = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_tooltipAnimSpeed);
														}
														else {
															m_TooltipWidget1->delayQuickEventTime = 0.f;
															m_TooltipWidget1->quickEventTime = 0.f;
															m_TooltipWidget1->speedOtherAnims = 0.f;
														}
													}
												}
											}
										}
										interactComp->tooltipLoaded = true;
									}
									if (data.specificTooltip == EAction::None) {
										if (interactComp->interactDataArray[data.specificStage].actions[i].widgetInScreen) {
											m_TooltipWidget->SetTooltipLocation(data.location);
										}
										else {
											FTransform transform = FTransform();
											transform.SetLocation(data.location);
											interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->SetRelativeTransform(transform);
										}
									}
									else if (data.specificTooltip == interactComp->interactDataArray[data.specificStage].actions[i].actionButton) {
										if (interactComp->interactDataArray[data.specificStage].actions[i].widgetInScreen) {
											m_TooltipWidget->SetTooltipLocation(data.location);
										}
										else {
											FTransform transform = FTransform();
											transform.SetLocation(data.location);
											interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->SetRelativeTransform(transform);
										}
									}
								}
							}
						}
						if (data.setSize) {
							if (IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
								UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
								if (m_TooltipWidget) {

									UInteractCore::checkCurrentPlatform();
									if (!interactComp->tooltipLoaded) {
										for (int j = 0; j < interactComp->interactDataArray.Num(); j++)
										{
											for (int k = 0; k < interactComp->interactDataArray[j].actions.Num(); k++)
											{
												if (IsValid(interactComp->interactDataArray[j].actions[k].m_WidgetComponent)) {
													UTooltip* m_TooltipWidget1 = Cast<UTooltip>(interactComp->interactDataArray[j].actions[k].m_WidgetComponent->GetUserWidgetObject());

													if (m_TooltipWidget1) {
														interactComp->interactDataArray[j].actions[k].m_button = UInteractCore::GetActionButtonImage(interactComp->interactDataArray[j].actions[k].actionButton);
														m_TooltipWidget1->SetButtonTexture(interactComp->interactDataArray[j].actions[k].m_button);
														m_TooltipWidget1->SetIconTexture(interactComp->interactDataArray[j].actions[k].m_icon);
														m_TooltipWidget1->SetCircleTexture(interactComp->interactDataArray[j].actions[k].m_circle);
														m_TooltipWidget1->SetTooltipSize(interactComp->interactDataArray[j].actions[k].widgetSize);
														if (interactComp->interactDataArray[j].actions[k].m_quickTimeEvent) {
															m_TooltipWidget1->delayQuickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_delayQuickEventTime);
															m_TooltipWidget1->quickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_quickEventTime);
															m_TooltipWidget1->speedOtherAnims = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_tooltipAnimSpeed);
														}
														else {
															m_TooltipWidget1->delayQuickEventTime = 0.f;
															m_TooltipWidget1->quickEventTime = 0.f;
															m_TooltipWidget1->speedOtherAnims = 0.f;
														}
													}
												}
											}
										}
										interactComp->tooltipLoaded = true;
									}
									if (data.specificTooltip == EAction::None) {
										m_TooltipWidget->SetTooltipSize(data.location);
									}
									else if (data.specificTooltip == interactComp->interactDataArray[data.specificStage].actions[i].actionButton) {
										m_TooltipWidget->SetTooltipSize(data.location);
									}
								}
							}
						}
					}
				}
			}
			else {
				if (interactComp->interactDataArray.IsValidIndex(interactComp->m_currentStage)) {
					for (int i = 0; i < interactComp->interactDataArray[interactComp->m_currentStage].actions.Num(); i++) {
						if (data.setLocation) {
							if (IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
								UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
								if (m_TooltipWidget) {

									UInteractCore::checkCurrentPlatform();
									if (!interactComp->tooltipLoaded) {
										for (int j = 0; j < interactComp->interactDataArray.Num(); j++)
										{
											for (int k = 0; k < interactComp->interactDataArray[j].actions.Num(); k++)
											{
												if (IsValid(interactComp->interactDataArray[j].actions[k].m_WidgetComponent)) {
													UTooltip* m_TooltipWidget1 = Cast<UTooltip>(interactComp->interactDataArray[j].actions[k].m_WidgetComponent->GetUserWidgetObject());

													if (m_TooltipWidget1) {
														interactComp->interactDataArray[j].actions[k].m_button = UInteractCore::GetActionButtonImage(interactComp->interactDataArray[j].actions[k].actionButton);
														m_TooltipWidget1->SetButtonTexture(interactComp->interactDataArray[j].actions[k].m_button);
														m_TooltipWidget1->SetIconTexture(interactComp->interactDataArray[j].actions[k].m_icon);
														m_TooltipWidget1->SetCircleTexture(interactComp->interactDataArray[j].actions[k].m_circle);
														m_TooltipWidget1->SetTooltipSize(interactComp->interactDataArray[j].actions[k].widgetSize);
														if (interactComp->interactDataArray[j].actions[k].m_quickTimeEvent) {
															m_TooltipWidget1->delayQuickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_delayQuickEventTime);
															m_TooltipWidget1->quickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_quickEventTime);
															m_TooltipWidget1->speedOtherAnims = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_tooltipAnimSpeed);
														}
														else {
															m_TooltipWidget1->delayQuickEventTime = 0.f;
															m_TooltipWidget1->quickEventTime = 0.f;
															m_TooltipWidget1->speedOtherAnims = 0.f;
														}

													}
												}
											}
										}
										interactComp->tooltipLoaded = true;
									}
									if (data.specificTooltip == EAction::None) {
										if (interactComp->interactDataArray[interactComp->m_currentStage].actions[i].widgetInScreen) {
											m_TooltipWidget->SetTooltipLocation(data.location);
										}
										else {
											FTransform transform = FTransform();
											transform.SetLocation(data.location);
											interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->SetRelativeTransform(transform);
										}
									}
									else if (data.specificTooltip == interactComp->interactDataArray[interactComp->m_currentStage].actions[i].actionButton) {
										if (interactComp->interactDataArray[interactComp->m_currentStage].actions[i].widgetInScreen) {
											m_TooltipWidget->SetTooltipLocation(data.location);
										}
										else {
											FTransform transform = FTransform();
											transform.SetLocation(data.location);
											interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->SetRelativeTransform(transform);
										}
									}
								}
							}
						}
						if (data.setSize) {
							if (IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
								UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
								if (m_TooltipWidget) {

									UInteractCore::checkCurrentPlatform();
									if (!interactComp->tooltipLoaded) {
										for (int j = 0; j < interactComp->interactDataArray.Num(); j++)
										{
											for (int k = 0; k < interactComp->interactDataArray[j].actions.Num(); k++)
											{
												if (IsValid(interactComp->interactDataArray[j].actions[k].m_WidgetComponent)) {
													UTooltip* m_TooltipWidget1 = Cast<UTooltip>(interactComp->interactDataArray[j].actions[k].m_WidgetComponent->GetUserWidgetObject());

													if (m_TooltipWidget1) {
														interactComp->interactDataArray[j].actions[k].m_button = UInteractCore::GetActionButtonImage(interactComp->interactDataArray[j].actions[k].actionButton);
														m_TooltipWidget1->SetButtonTexture(interactComp->interactDataArray[j].actions[k].m_button);
														m_TooltipWidget1->SetIconTexture(interactComp->interactDataArray[j].actions[k].m_icon);
														m_TooltipWidget1->SetCircleTexture(interactComp->interactDataArray[j].actions[k].m_circle);
														m_TooltipWidget1->SetTooltipSize(interactComp->interactDataArray[j].actions[k].widgetSize);
														if (interactComp->interactDataArray[j].actions[k].m_quickTimeEvent) {
															m_TooltipWidget1->delayQuickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_delayQuickEventTime);
															m_TooltipWidget1->quickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_quickEventTime);
															m_TooltipWidget1->speedOtherAnims = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_tooltipAnimSpeed);
														}
														else {
															m_TooltipWidget1->delayQuickEventTime = 0.f;
															m_TooltipWidget1->quickEventTime = 0.f;
															m_TooltipWidget1->speedOtherAnims = 0.f;
														}
													}
												}
											}
										}
										interactComp->tooltipLoaded = true;
									}
									if (data.specificTooltip == EAction::None) {
										m_TooltipWidget->SetTooltipSize(data.location);
									}
									else if (data.specificTooltip == interactComp->interactDataArray[interactComp->m_currentStage].actions[i].actionButton) {
										m_TooltipWidget->SetTooltipSize(data.location);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void UInteractComponent::ShowHideTooltip(AActor *actor, FShowHideTooltip& data, UInteractComponent* interactComponent)
{
	if (data.actor.enumName != EGetActorList::None) {
		int val = (int)data.actor.enumName - 1;
		AActor* acto = nullptr;
		if (val >= 0) {
			acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
		}
		if (acto) {
			UActorComponent* intComp = acto->GetComponentByClass(UInteractComponent::StaticClass());
			if (intComp) {
				UInteractComponent* interactComp = Cast<UInteractComponent>(intComp);
				if (data.specificStage != -1) {
					if (interactComp->interactDataArray.IsValidIndex(data.specificStage)) {
						for (int i = 0; i < interactComp->interactDataArray[data.specificStage].actions.Num(); i++) {
							if (data.showTooltip) {
								if (IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
									UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
									//PROVISIONAL POR LOS QTE (HAY QUE PROBAR LOS ITEMS)
									if (m_TooltipWidget) {
										UInteractCore::checkCurrentPlatform();

										if (!interactComp->tooltipLoaded) {
											for (int j = 0; j < interactComp->interactDataArray.Num(); j++)
											{
												for (int k = 0; k < interactComp->interactDataArray[j].actions.Num(); k++)
												{
													if (IsValid(interactComp->interactDataArray[j].actions[k].m_WidgetComponent)) {
														UTooltip* m_TooltipWidget1 = Cast<UTooltip>(interactComp->interactDataArray[j].actions[k].m_WidgetComponent->GetUserWidgetObject());

														if (m_TooltipWidget1) {
															interactComp->interactDataArray[j].actions[k].m_button = UInteractCore::GetActionButtonImage(interactComp->interactDataArray[j].actions[k].actionButton);
															m_TooltipWidget1->SetButtonTexture(interactComp->interactDataArray[j].actions[k].m_button);
															m_TooltipWidget1->SetIconTexture(interactComp->interactDataArray[j].actions[k].m_icon);
															m_TooltipWidget1->SetCircleTexture(interactComp->interactDataArray[j].actions[k].m_circle);
															m_TooltipWidget1->SetTooltipSize(interactComp->interactDataArray[j].actions[k].widgetSize);
															if (interactComp->interactDataArray[j].actions[k].m_quickTimeEvent) {
																m_TooltipWidget1->delayQuickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_delayQuickEventTime);
																m_TooltipWidget1->quickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_quickEventTime);
																m_TooltipWidget1->speedOtherAnims = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_tooltipAnimSpeed);
															}
															else {
																m_TooltipWidget1->delayQuickEventTime = 0.f;
																m_TooltipWidget1->quickEventTime = 0.f;
																m_TooltipWidget1->speedOtherAnims = 0.f;
															}
														}
													}
												}
											}
											interactComp->tooltipLoaded = true;
										}

										if (data.specificTooltip == EAction::None) {
											if ((interactComp->tempSameState && interactComp->tempAction == interactComp->interactDataArray[data.specificStage].actions[i].actionButton) || interactComp->tempAction == EAction::None || !interactComp->tempSameState) {
												m_TooltipWidget->Animate(data.tooltipAnimSpeed);
												if (data.setTooltipVisibilityValue) {
													interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = true;
												}
												if (data.setTooltipInteractValue) {
													interactComp->interactDataArray[data.specificStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
												}
											}
										}
										else if (data.specificTooltip == interactComp->interactDataArray[data.specificStage].actions[i].actionButton) {
											m_TooltipWidget->Animate(data.tooltipAnimSpeed);
											if (data.setTooltipVisibilityValue) {
												interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = true;
											}
											if (data.setTooltipInteractValue) {
												interactComp->interactDataArray[data.specificStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
											}
										}
									}
								}
							}
							else {
								if (IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
									UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[data.specificStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
									if (data.specificTooltip == EAction::None) {
										if (m_TooltipWidget && (interactComp->tempSameState && interactComp->tempAction == interactComp->interactDataArray[data.specificStage].actions[i].actionButton) || interactComp->tempAction == EAction::None || !interactComp->tempSameState) {
											m_TooltipWidget->StopAnimate(data.tooltipAnimSpeed);
											if (data.setTooltipVisibilityValue) {
												interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = false;
											}
											if (data.setTooltipInteractValue) {
												interactComp->interactDataArray[data.specificStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
											}
										}
									}
									else if (data.specificTooltip == interactComp->interactDataArray[data.specificStage].actions[i].actionButton) {
										m_TooltipWidget->StopAnimate(data.tooltipAnimSpeed);
										if (data.setTooltipVisibilityValue) {
											interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = false;
										}
										if (data.setTooltipInteractValue) {
											interactComp->interactDataArray[data.specificStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
										}
									}
								}
							}
							if (data.specificTooltip == EAction::None) {
								if ((interactComp->tempSameState && interactComp->tempAction == interactComp->interactDataArray[data.specificStage].actions[i].actionButton) || interactComp->tempAction == EAction::None || !interactComp->tempSameState) {
									if (data.tooltipVisibilityValue == ESetBoolValue::E_True) {
										interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = true;
									}
									else if (data.tooltipVisibilityValue == ESetBoolValue::E_False) {
										interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = false;
									}
								}
							}
							else {
								if (data.tooltipVisibilityValue == ESetBoolValue::E_True) {
									interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = true;
								}
								else if (data.tooltipVisibilityValue == ESetBoolValue::E_False) {
									interactComp->interactDataArray[data.specificStage].actions[i].m_showTooltip = false;
								}
							}
						}
					}
				}
				else {
					if (interactComp->interactDataArray.IsValidIndex(interactComp->m_currentStage)) {
						for (int i = 0; i < interactComp->interactDataArray[interactComp->m_currentStage].actions.Num(); i++) {
							if (data.showTooltip) {
								if (IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
									UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
									if (m_TooltipWidget) {
										UInteractCore::checkCurrentPlatform();

										if (!interactComp->tooltipLoaded) {
											for (int j = 0; j < interactComp->interactDataArray.Num(); j++)
											{
												for (int k = 0; k < interactComp->interactDataArray[j].actions.Num(); k++)
												{
													if (IsValid(interactComp->interactDataArray[j].actions[k].m_WidgetComponent)) {
														UTooltip* m_TooltipWidget1 = Cast<UTooltip>(interactComp->interactDataArray[j].actions[k].m_WidgetComponent->GetUserWidgetObject());

														if (m_TooltipWidget1) {
															interactComp->interactDataArray[j].actions[k].m_button = UInteractCore::GetActionButtonImage(interactComp->interactDataArray[j].actions[k].actionButton);
															m_TooltipWidget1->SetButtonTexture(interactComp->interactDataArray[j].actions[k].m_button);
															m_TooltipWidget1->SetIconTexture(interactComp->interactDataArray[j].actions[k].m_icon);
															m_TooltipWidget1->SetCircleTexture(interactComp->interactDataArray[j].actions[k].m_circle);
															m_TooltipWidget1->SetTooltipSize(interactComp->interactDataArray[j].actions[k].widgetSize);
															if (interactComp->interactDataArray[j].actions[k].m_quickTimeEvent) {
																m_TooltipWidget1->delayQuickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_delayQuickEventTime);
																m_TooltipWidget1->quickEventTime = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_quickEventTime);
																m_TooltipWidget1->speedOtherAnims = UInteractCore::GetValue(interactComp->interactDataArray[j].actions[k].m_tooltipAnimSpeed);
															}
															else {
																m_TooltipWidget1->delayQuickEventTime = 0.f;
																m_TooltipWidget1->quickEventTime = 0.f;
																m_TooltipWidget1->speedOtherAnims = 0.f;
															}
														}
													}
												}
											}
											interactComp->tooltipLoaded = true;
										}
										if (data.specificTooltip == EAction::None) {
											if ((interactComp->tempSameState && interactComp->tempAction == interactComp->interactDataArray[interactComp->m_currentStage].actions[i].actionButton) || interactComp->tempAction == EAction::None || !interactComp->tempSameState) {
												m_TooltipWidget->Animate(data.tooltipAnimSpeed);
												if (data.setTooltipVisibilityValue) {
													interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = true;
												}
												if (data.setTooltipInteractValue) {
													interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
												}
											}
										}
										else if (data.specificTooltip == interactComp->interactDataArray[interactComp->m_currentStage].actions[i].actionButton) {
											m_TooltipWidget->Animate(data.tooltipAnimSpeed);
											if (data.setTooltipVisibilityValue) {
												interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = true;
											}
											if (data.setTooltipInteractValue) {
												interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
											}
										}
									}
								}
							}
							else {
								if (IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
									UTooltip* m_TooltipWidget = Cast<UTooltip>(interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
									if (data.specificTooltip == EAction::None) {
										if (m_TooltipWidget && (interactComp->tempSameState && interactComp->tempAction == interactComp->interactDataArray[interactComp->m_currentStage].actions[i].actionButton) || interactComp->tempAction == EAction::None || !interactComp->tempSameState) {
											m_TooltipWidget->StopAnimate(data.tooltipAnimSpeed);
											if (data.setTooltipVisibilityValue) {
												interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = false;
											}
											if (data.setTooltipInteractValue) {
												interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
											}
										}
									}
									else if (data.specificTooltip == interactComp->interactDataArray[interactComp->m_currentStage].actions[i].actionButton) {
										m_TooltipWidget->StopAnimate(data.tooltipAnimSpeed);
										if (data.setTooltipVisibilityValue) {
											interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = false;
										}
										if (data.setTooltipInteractValue) {
											interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_interactIfTooltipIsHidden = data.interactIfTooltipHidden;
										}
									}
								}
							}
							if (data.specificTooltip == EAction::None) {
								if ((interactComp->tempSameState && interactComp->tempAction == interactComp->interactDataArray[interactComp->m_currentStage].actions[i].actionButton) || interactComp->tempAction == EAction::None || !interactComp->tempSameState) {
									if (data.tooltipVisibilityValue == ESetBoolValue::E_True) {
										interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = true;
									}
									else if (data.tooltipVisibilityValue == ESetBoolValue::E_False) {
										interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = false;
									}
								}
							}
							else {
								if (data.tooltipVisibilityValue == ESetBoolValue::E_True) {
									interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = true;
								}
								else if (data.tooltipVisibilityValue == ESetBoolValue::E_False) {
									interactComp->interactDataArray[interactComp->m_currentStage].actions[i].m_showTooltip = false;
								}
							}
						}
					}
				}
			}
		}
	}
}

void UInteractComponent::InsideCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPlayerInterface* MyActor = Cast<IPlayerInterface>(OtherActor);
	if (MyActor && interactDataArray.IsValidIndex(m_currentStage) && !m_actorInsideEnterNames.Contains(OtherActor->GetName()))
	{
		m_actorInsideEnterNames.Add(OtherActor->GetName());
		if (m_actorInsideExitNames.Contains(OtherActor->GetName())) {
			m_actorInsideExitNames.Remove(OtherActor->GetName());
		}
		tempInteractAvailable = true;
		tempActor = OtherActor;
		int maxInd = -1;
		for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++) {
			if (interactDataArray[m_currentStage].actions[i].m_quickTimeEvent) {
				float delVal = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_nextStageDelay);
				if (maxInd == -1 || delVal > UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_nextStageDelay)) {
					maxInd = i;
				}
			}
		}
		if (maxInd != -1) {
			float delVal = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_nextStageDelay);
			UInteractComponent::JumpStageGlobal(GetOwner(), "", delVal, UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_nextFailStage));
		}
		if (!firstTimeTooltip) {
			FTimerDelegate TooltipDel;
			TooltipDel.BindUFunction(this, FName("ShowTooltip"));
			GetWorld()->GetTimerManager().SetTimer(tooltipHandle, TooltipDel, 0.1f, false);
			HideTooltip();
			ShowTooltip();
		}
		else {
			ShowTooltip();
			//tempActor = nullptr;
		}
		ShowInnerOutline();
		if (interactDataArray[m_currentStage].capsuleInteractStruct.m_isShining) {
			ShowShine();
		}
		else {
			HideShine();
		}
	}
}

void UInteractComponent::InsideCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPlayerInterface* MyActor = Cast<IPlayerInterface>(OtherActor);
	if (MyActor && interactDataArray.IsValidIndex(m_currentStage) && !m_actorInsideExitNames.Contains(OtherActor->GetName()))
	{
		m_actorInsideExitNames.Add(OtherActor->GetName());
		if (m_actorInsideEnterNames.Contains(OtherActor->GetName())) {
			m_actorInsideEnterNames.Remove(OtherActor->GetName());
		}
		tempInteractAvailable = false;
		tempActor = OtherActor;
		tempSameState = false;
		int maxInd = -1;
		for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++) {
			if (interactDataArray[m_currentStage].actions[i].m_quickTimeEvent) {
				float delVal = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_nextStageDelay);
				if (maxInd == -1 || delVal > UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_nextStageDelay)) {
					maxInd = i;
				}
			}
		}
		if (maxInd != -1) {
			FCancelJumpStage cancelJ = {};
			UInteractComponent::CancelJumpStage(tempActor, cancelJ, this);
		}
		HideTooltipHard();
		if (interactDataArray.IsValidIndex(m_currentStage) && (interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleSize.X < interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleSize.X
			|| interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleSize.Y < interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleSize.Y)) {
			ShowOutsideOutline();
			if (interactDataArray[m_currentStage].capsuleHighlightStruct.m_isShining) {
				ShowShine();
			}
			else {
				HideShine();
			}
		}
		else {
			HideOutline();
			HideShine();
		}
		//tempActor = nullptr;
	}
}

void UInteractComponent::ShowShine()
{
	if (!isShining && (interactDataArray[m_currentStage].capsuleInteractStruct.m_isShining || interactDataArray[m_currentStage].capsuleHighlightStruct.m_isShining)) {
		isShining = true;
		for (int i = 0; i < m_actorMesh.Num(); i++) {
			UStaticMeshComponent* statiMesh = Cast<UStaticMeshComponent>(m_actorMesh[i]);
			if (m_actorMesh[i] && m_actorMesh[i]->ComponentHasTag(FName(TEXT("shine"))) && ((statiMesh && statiMesh->GetStaticMesh()) || !statiMesh )) {
				UMaterialInterface* mat = m_actorMesh[i]->GetMaterial(0);
				if (mat) {
					UMaterialInstanceDynamic* matInst = Cast<UMaterialInstanceDynamic>(mat);
					if (!matInst) {
						//auxMat = mat;
						matInst = UMaterialInstanceDynamic::Create(mat, this);
					}

					if (matInst) {
						matInst->SetScalarParameterValue(FName(TEXT("RimShader")), 1);
						m_actorMesh[i]->SetMaterial(0, matInst);
					}
				}
			}
		}
	}
}

void UInteractComponent::HideShine()
{
	if (isShining && (interactDataArray[m_currentStage].capsuleInteractStruct.m_isShining || interactDataArray[m_currentStage].capsuleHighlightStruct.m_isShining)) {
		isShining = false;
		for (int i = 0; i < m_actorMesh.Num(); i++) {
			if (m_actorMesh[i] && m_actorMesh[i]->ComponentHasTag(FName(TEXT("shine")))) {
				UMaterialInterface* mat = m_actorMesh[i]->GetMaterial(0);
				if (mat) {
					UMaterialInstanceDynamic* matInst = Cast<UMaterialInstanceDynamic>(mat);
					if (matInst) {
						matInst->SetScalarParameterValue(FName(TEXT("RimShader")), 0);
						m_actorMesh[i]->SetMaterial(0, matInst);
					}

				}
			}
		}
	}
}

void UInteractComponent::OutsideCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPlayerInterface* MyActor = Cast<IPlayerInterface>(OtherActor);
	if (MyActor && !m_actorOutsideEnterNames.Contains(OtherActor->GetName()) && interactDataArray.IsValidIndex(m_currentStage))
	{
		m_actorOutsideEnterNames.Add(OtherActor->GetName());
		if (m_actorOutsideExitNames.Contains(OtherActor->GetName())) {
			m_actorOutsideExitNames.Remove(OtherActor->GetName());
		}
		tempActor = OtherActor;
		if (interactDataArray.IsValidIndex(m_currentStage) && (interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleSize.X < interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleSize.X
			|| interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleSize.Y < interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleSize.Y)) {
			ShowOutsideOutline();
			if (interactDataArray[m_currentStage].capsuleHighlightStruct.m_isShining) {
				ShowShine();
			}
		}
	}
}

void UInteractComponent::OutsideCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPlayerInterface* MyActor = Cast<IPlayerInterface>(OtherActor);
	if (MyActor && !m_actorOutsideExitNames.Contains(OtherActor->GetName()) && interactDataArray.IsValidIndex(m_currentStage))
	{
		m_actorOutsideExitNames.Add(OtherActor->GetName());
		if (m_actorOutsideEnterNames.Contains(OtherActor->GetName())) {
			m_actorOutsideEnterNames.Remove(OtherActor->GetName());
		}
		tempActor = OtherActor;
		HideOutline();
		HideShine();
	}
}

void UInteractComponent::ShowTooltip()
{
	if ((tempActor && Cast<IPlayerInterface>(tempActor) || (tempType == EActionStageType::Interact || tempType == EActionStageType::Exit || tempType == EActionStageType::Init || (tempType == EActionStageType::Pre && tempHoldingButton))) && tempInteractAvailable) {
		if (interactDataArray.IsValidIndex(m_currentStage)) {
			for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++)
			{
				if (IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
					UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
					if (m_TooltipWidget && interactDataArray[m_currentStage].actions[i].m_showTooltip) {
						auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
						if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
						{
							UInteractCore::currentPlatform = UInteractConfig::defaultGamepadPlatform;
						}
						else {
							UInteractCore::currentPlatform = UInteractConfig::defaultPlatform;

						}
						interactDataArray[m_currentStage].actions[i].m_button = UInteractCore::GetActionButtonImage(interactDataArray[m_currentStage].actions[i].actionButton);

						if (!tooltipLoaded) {
							for (int j = 0; j < interactDataArray.Num(); j++)
							{
								for (int k = 0; k < interactDataArray[j].actions.Num(); k++)
								{
									if (IsValid(interactDataArray[j].actions[k].m_WidgetComponent)) {
										UTooltip* m_TooltipWidget1 = Cast<UTooltip>(interactDataArray[j].actions[k].m_WidgetComponent->GetUserWidgetObject());

										if (m_TooltipWidget1) {
											interactDataArray[j].actions[k].m_button = UInteractCore::GetActionButtonImage(interactDataArray[j].actions[k].actionButton);
											m_TooltipWidget1->SetButtonTexture(interactDataArray[j].actions[k].m_button);
											m_TooltipWidget1->SetIconTexture(interactDataArray[j].actions[k].m_icon);
											m_TooltipWidget1->SetCircleTexture(interactDataArray[j].actions[k].m_circle);
											m_TooltipWidget1->SetTooltipSize(interactDataArray[j].actions[k].widgetSize);
											if (interactDataArray[j].actions[k].m_quickTimeEvent) {
												m_TooltipWidget1->delayQuickEventTime = UInteractCore::GetValue(interactDataArray[j].actions[k].m_delayQuickEventTime);
												m_TooltipWidget1->quickEventTime = UInteractCore::GetValue(interactDataArray[j].actions[k].m_quickEventTime);
												m_TooltipWidget1->speedOtherAnims = UInteractCore::GetValue(interactDataArray[j].actions[k].m_tooltipAnimSpeed);
											}
											else {
												m_TooltipWidget1->delayQuickEventTime = 0.f;
												m_TooltipWidget1->quickEventTime = 0.f;
												m_TooltipWidget1->speedOtherAnims = 0.f;
											}
										}
									}
								}
							}
							tooltipLoaded = true;
						}
						if (interactDataArray[m_currentStage].actions[i].m_quickTimeEvent) {
							m_TooltipWidget->delayQuickEventTime = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_delayQuickEventTime);
							m_TooltipWidget->quickEventTime = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_quickEventTime);
							m_TooltipWidget->speedOtherAnims = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_tooltipAnimSpeed);
						}
						else {
							m_TooltipWidget->delayQuickEventTime = 0.f;
							m_TooltipWidget->quickEventTime = 0.f;
							m_TooltipWidget->speedOtherAnims = 0.f;
						}

						//Para ocultar solo el seleccionado si es el mismoe estado
						if ((tempSameState && tempAction == interactDataArray[m_currentStage].actions[i].actionButton) || tempAction == EAction::None || !tempSameState) {
							m_TooltipWidget->Animate(UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_tooltipAnimSpeed));
						}

						if (!firstTimeTooltip) {
							firstTimeTooltip = true;
						}
					}
				}
			}
		}
	}
}


void UInteractComponent::HideTooltipHard()
{
	for (int t = 0; t < interactDataArray.Num(); t++) {
		for (int f = 0; f < interactDataArray[t].actions.Num(); f++) {
			if (IsValid(interactDataArray[t].actions[f].m_WidgetComponent) && IsValid(interactDataArray[t].actions[f].m_WidgetComponent->GetUserWidgetObject())) {
				UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[t].actions[f].m_WidgetComponent->GetUserWidgetObject());
				if (m_TooltipWidget) {
					m_TooltipWidget->StopFunctions(true, 1.f, true);
				}
			}
		}
	}
}


void UInteractComponent::HideTooltip()
{
	if ((tempActor && Cast<IPlayerInterface>(tempActor)) || (tempType == EActionStageType::Interact || tempType == EActionStageType::Exit || tempType == EActionStageType::Init || (tempType == EActionStageType::Pre && tempHoldingButton))) {
		if (interactDataArray.IsValidIndex(m_currentStage)) {
			bool qteBool = HasAnyQTE();
			for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++)
			{
				if (IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
					UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());

					if (m_TooltipWidget && (interactDataArray[m_currentStage].actions[i].m_showTooltip || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f) && (tempSameState && tempAction == interactDataArray[m_currentStage].actions[i].actionButton) || tempAction == EAction::None || !tempSameState) {
						m_TooltipWidget->StopAnimate(UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_tooltipAnimSpeed));
					}
					if (qteBool && (m_TooltipWidget && (interactDataArray[m_currentStage].actions[i].m_showTooltip || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f) && (tempAction != interactDataArray[m_currentStage].actions[i].actionButton))) {
						m_TooltipWidget->StopFunctions();
					}
				}
			}
		}
	}

}

void UInteractComponent::ShowInnerOutline()
{
	if (tempActor && Cast<IPlayerInterface>(tempActor)) {
		if (interactDataArray.IsValidIndex(m_currentStage)) {
			for (int i = 0; i < m_actorMesh.Num(); i++) {
				if (m_actorMesh[i] && interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleColor != EOutlineColor::None && m_actorMesh[i]->ComponentHasTag(FName(TEXT("light")))) {
					m_actorMesh[i]->SetCustomDepthStencilValue((uint8)interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleColor);
					m_actorMesh[i]->SetRenderCustomDepth(true);
				}
				else {
					m_actorMesh[i]->SetRenderCustomDepth(false);
				}
			}
		}
	}
}

void UInteractComponent::ShowOutsideOutline()
{
	if (tempActor && Cast<IPlayerInterface>(tempActor)) {
		if (interactDataArray.IsValidIndex(m_currentStage)) {
			for (int i = 0; i < m_actorMesh.Num(); i++) {
				if (m_actorMesh[i] && interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleColor != EOutlineColor::None && m_actorMesh[i]->ComponentHasTag(FName(TEXT("light")))) {
					m_actorMesh[i]->SetCustomDepthStencilValue((uint8)interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleColor);
					m_actorMesh[i]->SetRenderCustomDepth(true);
				}
				else {
					m_actorMesh[i]->SetRenderCustomDepth(false);
				}
			}
		}
	}
}

void UInteractComponent::HideOutline()
{
	if (tempActor && Cast<IPlayerInterface>(tempActor)) {
		for (int i = 0; i < m_actorMesh.Num(); i++) {
			m_actorMesh[i]->SetRenderCustomDepth(false);
		}
	}
}

void UInteractComponent::InitializeTooltip()
{
	//TOOLTIP
	for (int j = 0; j < interactDataArray.Num(); j++)
	{
		for (int i = 0; i < interactDataArray[j].actions.Num(); i++)
		{
			FString str = "InteractWidgetComponent:%" + FString::FromInt(j) + "-" + FString::FromInt(i);
			for (int z = 0; z < widgetComps.Num(); z++) {
				if (ISM_ISVALID(widgetComps[z]) && widgetComps[z]->GetName().Contains(str)) {
					interactDataArray[j].actions[i].m_WidgetComponent = widgetComps[z];
					UUserWidget* userWidg = CreateWidget(GetWorld(), StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/TooltipWidget.TooltipWidget_C'")));
					interactDataArray[j].actions[i].m_WidgetComponent->SetWidget(userWidg);
					if (!GIsEditor || !WITH_EDITOR) {
						if(!interactDataArray[j].actions[i].widgetInScreen){
							interactDataArray[j].actions[i].m_WidgetComponent->AddViewportInteract(userWidg);
						}
					}
					break;
				}
			}

			if (IsValid(interactDataArray[j].actions[i].m_WidgetComponent)) {
				FTransform transform = FTransform();
				transform.SetLocation(interactDataArray[j].actions[i].widgetLocation);
				interactDataArray[j].actions[i].m_WidgetComponent->SetRelativeTransform(transform);
			}
		}
	}
}

void UInteractComponent::Next(int stage, FTooltipData data) {

	if (stage == -1) {
		if (GetWorld()) {
			GetWorld()->GetTimerManager().ClearTimer(ticksHandle);
		}
		FCancelTimer cancelStruct = { ETimerListCore::None, ETimerList::None, false, -1.f, -1.f };
		UInteractComponent::CancelTimer(nullptr, cancelStruct, this);

		for (int t = 0; t < interactDataArray.Num(); t++) {
			for (int f = 0; f < interactDataArray[t].actions.Num(); f++) {
				if (IsValid(interactDataArray[t].actions[f].m_WidgetComponent) && IsValid(interactDataArray[t].actions[f].m_WidgetComponent->GetUserWidgetObject())) {
					UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[t].actions[f].m_WidgetComponent->GetUserWidgetObject());
					if (m_TooltipWidget) {
						m_TooltipWidget->StopFunctions(true);
					}
				}
			}
		}

		for (int i = 0; i < m_actorMesh.Num(); i++) {
			if (m_actorMesh[i] && m_actorMesh[i]->ComponentHasTag(FName(TEXT("shine")))) {
				UMaterialInterface* mat = m_actorMesh[i]->GetMaterial(0);
				if (mat) {
					UMaterialInstanceDynamic* matInst = Cast<UMaterialInstanceDynamic>(mat);
					if (matInst) {
						matInst->SetScalarParameterValue(FName(TEXT("RimShader")), 0);
						m_actorMesh[i]->SetMaterial(0, matInst);
					}

				}
			}
			m_actorMesh[i]->SetRenderCustomDepth(false);
		}

		if (GetOwner()) {
			for (int z = 0; z < delegateAuxFunctions.Num(); z++) {
				if (delegateAuxFunctions[z].currentInteractName == GetOwner()->GetName()) {
					delegateAuxFunctions.RemoveAt(z);
					z--;
				}
			}
		}

		m_innerCapsule->SetCapsuleSize(0.f, 0.f);
		m_outsideCapsule->SetCapsuleSize(0.f, 0.f);
		m_triggerCapsule->SetCapsuleSize(0.f, 0.f);

		doOnceStart = false;
		m_currentStage = -1;
		interactDataArray.Empty();
		return;
	}
	else if (stage == -3) {


		for (int t = 0; t < interactDataArray.Num(); t++) {
			for (int f = 0; f < interactDataArray[t].actions.Num(); f++) {
				if (IsValid(interactDataArray[t].actions[f].m_WidgetComponent) && IsValid(interactDataArray[t].actions[f].m_WidgetComponent->GetUserWidgetObject())) {
					UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[t].actions[f].m_WidgetComponent->GetUserWidgetObject());
					if (m_TooltipWidget) {
						m_TooltipWidget->StopFunctions(true);
					}
				}
			}
		}

		for (int i = 0; i < m_actorMesh.Num(); i++) {
			if (m_actorMesh[i] && m_actorMesh[i]->ComponentHasTag(FName(TEXT("shine")))) {
				UMaterialInterface* mat = m_actorMesh[i]->GetMaterial(0);
				if (mat) {
					UMaterialInstanceDynamic* matInst = Cast<UMaterialInstanceDynamic>(mat);
					if (matInst) {
						matInst->SetScalarParameterValue(FName(TEXT("RimShader")), 0);
						m_actorMesh[i]->SetMaterial(0, matInst);
					}

				}
			}
			m_actorMesh[i]->SetRenderCustomDepth(false);
		}
		return;
	}

	bool hideTooltip = false;
	bool hideOutline = false;
	bool holdingDown = false;
	m_actorInsideEnterNames.Empty();
	m_actorInsideExitNames.Empty();
	m_actorTriggerEnterNames.Empty();
	m_actorTriggerExitNames.Empty();
	if (tempType == EActionStageType::Interact || tempType == EActionStageType::Pre) {
		hideTooltip = data.m_hideTooltip;
		hideOutline = data.m_hideOutline;
		holdingDown = data.m_holdingDown;
	}
	if (data.m_holdingDown) {
		tempHoldingButton = data.m_holdingDown;
	}
	if ((tempTooltip == -2 || ((tempType == EActionStageType::Exit || tempType == EActionStageType::Init || tempType == EActionStageType::Interact || (tempActor && Cast<IPlayerInterface>(tempActor))) && (tempTooltip != -2 && !tempJumpStage))) && !(tempSameState && !hideTooltip)) {
		if (((!hideTooltip || tempTooltip != -2) || tempSameState || holdingDown) && data.m_showTooltip) {
			HideTooltip();
		}
		if (!hideOutline) {
			HideOutline();
		}
		HideShine();
	}
	m_currentStage = stage;
	UpdateStageAtributes();
	if ((tempTooltip == -2 || ((tempType == EActionStageType::Exit || tempType == EActionStageType::Init || tempType == EActionStageType::Interact || (tempActor && Cast<IPlayerInterface>(tempActor))) && ((tempTooltip != -2 && !tempJumpStage) || (HasAnyQTE())))) && !((tempSameState && !tempJumpStage) && !hideTooltip)) {
		bool doneF = false;
		ShowTooltip();
		if (tempActor && Cast<IPlayerInterface>(tempActor)) {
			TArray<UPrimitiveComponent*> InteractiveItems;
			tempActor->GetOverlappingComponents(InteractiveItems);
			for (int f = 0; f < InteractiveItems.Num(); f++) {
				if (InteractiveItems[f]->GetName() == m_innerCapsule->GetName() && interactDataArray[m_currentStage].capsuleInteractStruct.m_isShining) {
					ShowShine();
					ShowInnerOutline();
					doneF = true;
					break;
				}
			}
			if (!doneF) {
				for (int f = 0; f < InteractiveItems.Num(); f++) {
					if (InteractiveItems[f]->GetName() == m_outsideCapsule->GetName() && interactDataArray[m_currentStage].capsuleHighlightStruct.m_isShining) {
						ShowShine();
						ShowOutsideOutline();
						doneF = true;
						break;
					}
				}
			}
		}
		if (!doneF) {
			HideShine();
		}
	}
	tempHoldingButton = false;
	tempTooltip = -2;
	tempDelay = -1;
	tempSameState = false;
	stopStage = false;
	tempType = EActionStageType::None;
	functionIndex = 0;
	//tempActor = nullptr;
	tempTooltipIndex = -1;
	tempDoOnceDelay = false;
	readyForNextStage = false;
	tempInElse = false;
	tempJumpStage = false;
	tempJumpFunction = false;
	//tempIndex = 0;
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		if (tempActor) {
			int maxInd = -1;
			for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++) {
				if (interactDataArray[m_currentStage].actions[i].m_quickTimeEvent) {
					float delVal = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_nextStageDelay);
					if (maxInd == -1 || delVal > UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_nextStageDelay)) {
						maxInd = i;
					}
				}
			}
			if (maxInd != -1) {
				float delVal = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_delayQuickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_quickEventTime) + UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_nextStageDelay);
				UInteractComponent::JumpStageGlobal(GetOwner(), "", delVal, UInteractCore::GetValue(interactDataArray[m_currentStage].actions[maxInd].m_nextFailStage));
			}
		}
		while (interactDataArray.IsValidIndex(m_currentStage) && functionIndex < interactDataArray[m_currentStage].preStageFunction.Num())
		{
			if (interactDataArray[m_currentStage].preStageFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].preStageFunction[functionIndex].enumNameCore != EActionListCore::None) {
				tempAction = EAction::None;
				tempType = EActionStageType::Pre;

				int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
				int value = ((int)interactDataArray[m_currentStage].preStageFunction[functionIndex].enumName) - 1;
				int valueCore = ((int)interactDataArray[m_currentStage].preStageFunction[functionIndex].enumNameCore) - 1;
				if (valueCore >= 0) {
					interactFunctionMap[valueCore].Execute(this, tempActor, EAction::None, functionIndex, EActionStageType::Pre, -1, -1, -1, false);
				}
				else if (value >= 0) {
					interactFunctionMap[value + maxE - 1].Execute(this, tempActor, EAction::None, functionIndex, EActionStageType::Pre, -1, -1, -1, false);
				}
			}
			if (stopStage) {
				break;
			}
			functionIndex++;
		}
	}
}

void UInteractComponent::NextStage(int stage, FTooltipData data)
{
	if (!tempDelayFunction) {
		if (GetWorld()) {
			GetWorld()->GetTimerManager().ClearTimer(functionPressingHandle);
			GetWorld()->GetTimerManager().ClearTimer(pressingHandle);
		}
		float delay = 0.f;
		bool hideTooltip = false;
		bool hideOutline = false;
		delay = UInteractCore::GetValue(data.m_nextStageDelay);
		if (tempType == EActionStageType::Interact || (tempActor && Cast<IPlayerInterface>(tempActor))) {
			hideTooltip = data.m_hideTooltip;
			hideOutline = data.m_hideOutline;
		}
		if (tempDelay != -1) {
			delay = tempDelay;
		}
		tempSameState = stage == m_currentStage;
		if (stage >= 0 && stage < interactDataArray.Num()) {

			if (tempType == EActionStageType::Interact) {
				if (HasAnyQTE()) {
					if (!IsQTEAvailable()) {
						stage = UInteractCore::GetValue(data.m_nextFailStage, interactDataArray.Num() - 1);
					}
					tempSameState = stage == m_currentStage;
					if (tempSameState) {
						StopQTEStage();
					}
				}
			}
			if (!data.m_holdingDown) {
				if (hideTooltip && tempTooltip == -2) {
					HideTooltip();
				}
				if (hideOutline) {
					HideOutline();
				}
			}
			if (delay > 0.f) {
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("Next"), stage, data);
				if (GetWorld()) {
					GetWorld()->GetTimerManager().SetTimer(nextHandle, TimerDel, delay, false);
				}
			}
			else {
				Next(stage, data);
			}

		}
		else if (stage == -1) {
			if (delay > 0.f) {
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("Next"), stage, data);
				if (GetWorld()) {
					GetWorld()->GetTimerManager().SetTimer(nextHandle, TimerDel, delay, false);
				}
			}
			else {
				Next(stage, data);
			}
		}
		else if (stage == -3) {
			if (delay > 0.f) {
				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("Next"), stage, data);
				if (GetWorld()) {
					GetWorld()->GetTimerManager().SetTimer(nextHandle, TimerDel, delay, false);
				}
			}
			else {
				Next(stage, data);
			}
		}
	}
	tempDelayFunction = false;
}

bool UInteractComponent::JumpStageGlobal(AActor* actor, FString stageString /*= ""*/, float delay, int stage /*= -4*/, bool cancelTimers)
{
	if (actor) {
		UInteractComponent* intInf = Cast<UInteractComponent>(actor->GetComponentByClass(UInteractComponent::StaticClass()));
		if (intInf) {
			int finalStage = -1;
			if (stage == -4) {
				if (stageString != "") {
					for (int i = 0; i < intInf->interactDataArray.Num(); i++) {
						if (intInf->interactDataArray[i].stageTitle == stageString) {
							finalStage = i;
						}
					}
				}
			}
			else {
				finalStage = stage;
			}
			if (finalStage != -4) {
				if (cancelTimers) {	
					FCancelTimer cancelStruct = { ETimerListCore::None, ETimerList::None, false, -1.f, -1.f };
					UInteractComponent::CancelTimer(actor, cancelStruct, intInf);
				}
				FJumpStage jumpStruct = { true, FString::FromInt(finalStage), FString::SanitizeFloat(delay) };
				intInf->tempType = EActionStageType::External;
				UInteractComponent::JumpStage(actor, jumpStruct, intInf);
				return true;
			}
			else {
				return false;
			}
		}
	}
	return false;
}

void UInteractComponent::UpdateStageAtributes()
{
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		m_priority = interactDataArray[m_currentStage].priorityStage;
		m_priorityRepeat = interactDataArray[m_currentStage].doOtherInteract;
		if (m_innerCapsule) {
			m_innerCapsule->SetCapsuleSize(interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleSize.X, interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleSize.Y);
			FTransform transform = FTransform();
			FRotator rotator = FRotator(interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleRotation.X, interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleRotation.Y, interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleRotation.Z);
			FQuat quat = FQuat(rotator);
			transform.SetLocation(interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleLocation);
			transform.SetRotation(quat);
			transform.SetScale3D(interactDataArray[m_currentStage].capsuleInteractStruct.m_CapsuleScale);
			m_innerCapsule->SetRelativeTransform(transform);
		}
		if (m_outsideCapsule) {
			m_outsideCapsule->SetCapsuleSize(interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleSize.X, interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleSize.Y);
			FTransform transform2 = FTransform();
			FRotator rotator2 = FRotator(interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleRotation.X, interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleRotation.Y, interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleRotation.Z);
			FQuat quat2 = FQuat(rotator2);
			transform2.SetLocation(interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleLocation);
			transform2.SetRotation(quat2);
			transform2.SetScale3D(interactDataArray[m_currentStage].capsuleHighlightStruct.m_CapsuleScale);
			m_outsideCapsule->SetRelativeTransform(transform2);
		}
		if (m_triggerCapsule) {
			m_triggerCapsule->SetCapsuleSize(interactDataArray[m_currentStage].capsuleTriggerStruct.m_CapsuleSize.X, interactDataArray[m_currentStage].capsuleTriggerStruct.m_CapsuleSize.Y);
			FTransform transform2 = FTransform();
			FRotator rotator2 = FRotator(interactDataArray[m_currentStage].capsuleTriggerStruct.m_CapsuleRotation.X, interactDataArray[m_currentStage].capsuleTriggerStruct.m_CapsuleRotation.Y, interactDataArray[m_currentStage].capsuleTriggerStruct.m_CapsuleRotation.Z);
			FQuat quat2 = FQuat(rotator2);
			transform2.SetLocation(interactDataArray[m_currentStage].capsuleTriggerStruct.m_CapsuleLocation);
			transform2.SetRotation(quat2);
			transform2.SetScale3D(interactDataArray[m_currentStage].capsuleTriggerStruct.m_CapsuleScale);
			m_triggerCapsule->SetRelativeTransform(transform2);
		}
	}
}


bool UInteractComponent::DebugCondition(AActor* actor, FDebugCondition& data, UInteractComponent* interactComponent)
{
	return data.retValue;
}

void UInteractComponent::DecrementIncrementTimer(AActor* actor, FDecrementIncrement& data)
{
	if (GetWorld() && GetOwner() && m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::decrementIncrement) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		int idDec = -1;
		//Si ya habia entrado anteriormente al trigger tendra esa variable distinta de nullptr con lo cual:
		for (int i = 0; i < m_variablesArray.Num(); i++) {
			//Comprobamos en que posicion del array est� nuestro Rotator
			if (m_variablesArray[i].Contains("Id=DecrementIncrement-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + ":")) {
				idDec = i;
			}
		}
		float max = 0.f;

		if (idDec != -1) {
			if (data.increment) {
				max = UInteractCore::GetValue(data.initNumber);
			}
			else {
				max = UInteractCore::GetValue(data.initNumber);
			}
			m_variablesArray[idDec] = "Id=DecrementIncrement-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + ":" + FString::SanitizeFloat(max);
		}
	}
}

bool UInteractComponent::DecrementIncrement(AActor* actor, FDecrementIncrement& data, UInteractComponent* interactComponent)
{
	data.savedState.tempCurrentStage = interactComponent->m_currentStage;
	data.savedState.tempFunctionIndex = interactComponent->functionIndex;
	data.savedState.tempStage = interactComponent->tempType;
	data.savedState.tempAction = interactComponent->tempAction;
	if (!interactComponent->m_registerTimerArray.Contains("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::decrementIncrement) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction))) {
		interactComponent->m_registerTimerArray.Add("Id=" + GETENUMSTRING(TEXT("ETimerListCore"), ETimerListCore::decrementIncrement) + "-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + "-" + FString::FromInt((int)data.savedState.tempStage) + "-" + FString::FromInt((int)data.savedState.tempAction));
	}

	if (data.resetTimer) {
		interactComponent->GetWorld()->GetTimerManager().ClearTimer(data.decrementIncrementTimer);
	}
	bool res = false;
	float max = 0.f;
	int idDec = -1;
	//Si ya habia entrado anteriormente al trigger tendra esa variable distinta de nullptr con lo cual:
	for (int i = 0; i < interactComponent->m_variablesArray.Num(); i++) {
		//Comprobamos en que posicion del array est� nuestro Rotator
		if (interactComponent->m_variablesArray[i].Contains("Id=DecrementIncrement-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + ":")) {
			idDec = i;
			FString nons;
			FString valu;
			interactComponent->m_variablesArray[i].Split("Id=DecrementIncrement-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + ":", &nons, &valu);
			max = FCString::Atof(*valu);
		}
	}
	if (idDec == -1) {
		if (data.increment) {
			max = UInteractCore::GetValue(data.initNumber);
		}
		else {
			max = UInteractCore::GetValue(data.initNumber);
		}
		idDec = interactComponent->m_variablesArray.Add("Id=DecrementIncrement-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + ":" + FString::SanitizeFloat(max));
		if (data.resetTimer) {
			FTimerDelegate decrementDel;
			decrementDel.BindUFunction(interactComponent, FName("DecrementIncrementTimer"), actor, data);
			interactComponent->GetWorld()->GetTimerManager().SetTimer(data.decrementIncrementTimer, decrementDel, data.timeToReset, false);
		}
	}
	else {
		if (data.increment) {
			if (max >= data.limit) {
				res = true;
				max = NaN;
				interactComponent->m_variablesArray.RemoveAt(idDec);
			}
			else {
				max += data.quantity;
				interactComponent->m_variablesArray[idDec] = "Id=DecrementIncrement-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + ":" + FString::SanitizeFloat(max);
				if (data.resetTimer) {
					FTimerDelegate decrementDel;
					decrementDel.BindUFunction(interactComponent, FName("DecrementIncrementTimer"), actor, data);
					interactComponent->GetWorld()->GetTimerManager().SetTimer(data.decrementIncrementTimer, decrementDel, data.timeToReset, false);
				}
			}
		}
		else if (!data.increment) {
			if (max <= data.limit) {
				res = true;
				max = NaN;
				interactComponent->m_variablesArray.RemoveAt(idDec);
			}
			else {
				max -= data.quantity;
				interactComponent->m_variablesArray[idDec] = "Id=DecrementIncrement-" + FString::FromInt(data.savedState.tempCurrentStage) + "-" + FString::FromInt(data.savedState.tempFunctionIndex) + ":" + FString::SanitizeFloat(max);
				if (data.resetTimer) {
					FTimerDelegate decrementDel;
					decrementDel.BindUFunction(interactComponent, FName("DecrementIncrementTimer"), actor, data);
					interactComponent->GetWorld()->GetTimerManager().SetTimer(data.decrementIncrementTimer, decrementDel, data.timeToReset, false);
				}
			}
		}
	}
	return res;
}

void UInteractComponent::IfFunction(AActor* actor, FIfFunction& data, UInteractComponent* interactComponent)
{
	if (data.condition.enumName != ECheckList::None || data.condition.enumNameCore != ECheckListCore::None) {
		bool val = false;

		int maxE = (int)GETENUMMAX(TEXT("ECheckListCore"));
		int value = ((int)data.condition.enumName) - 1;
		int valueCore = ((int)data.condition.enumNameCore) - 1;
		if (valueCore >= 0) {
			val = interactComponent->checkFunctionMap[valueCore].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType);
		}
		else if (value >= 0) {
			val = interactComponent->checkFunctionMap[value+maxE -1].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType);
		}

		if (data.negate && (data.function.enumName != EActionList::None || data.function.enumNameCore != EActionListCore::None)) {
			if (!val) {
				interactComponent->tempInElse = false;

				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int valueFunc = ((int)data.function.enumName) - 1;
				int valueFuncCore = ((int)data.function.enumNameCore) - 1;
				if (valueFuncCore >= 0) {
					interactComponent->interactFunctionMap[valueFuncCore].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, false);
				}
				else if (valueFunc >= 0) {
					interactComponent->interactFunctionMap[valueFunc + maxE2 - 1].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, false);
				}
			}
			else {
				interactComponent->tempInElse = true;

				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int valueFunc = ((int)data.elseFunction.enumName) - 1;
				int valueFuncCore = ((int)data.elseFunction.enumNameCore) - 1;
				if (valueFuncCore >= 0) {
					interactComponent->interactFunctionMap[valueFuncCore].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, true);
				}
				else if (valueFunc >= 0) {
					interactComponent->interactFunctionMap[valueFunc + maxE2 - 1].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, true);
				}
			}
		}
		else if(data.function.enumName != EActionList::None || data.function.enumNameCore != EActionListCore::None){
			if (val) {
				interactComponent->tempInElse = false;

				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int valueFunc = ((int)data.function.enumName) - 1;
				int valueFuncCore = ((int)data.function.enumNameCore) - 1;
				if (valueFuncCore >= 0) {
					interactComponent->interactFunctionMap[valueFuncCore].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, false);
				}
				else if (valueFunc >= 0) {
					interactComponent->interactFunctionMap[valueFunc + maxE2 - 1].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, false);
				}
			}
			else {
				interactComponent->tempInElse = true;

				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int valueFunc = ((int)data.elseFunction.enumName) - 1;
				int valueFuncCore = ((int)data.elseFunction.enumNameCore) - 1;
				if (valueFuncCore >= 0) {
					interactComponent->interactFunctionMap[valueFuncCore].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, true);
				}
				else if (valueFunc >= 0) {
					interactComponent->interactFunctionMap[valueFunc + maxE2 - 1].Execute(interactComponent, actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, NaN, -1, -1, true);
				}
			}
		}
	}
}

void UInteractComponent::ExecuteAction(AActor* actor, FExecuteAction& data, UInteractComponent* interactComponent)
{
	int val = (int)data.actor.enumName - 1;
	AActor* acto = nullptr;
	if (val >= 0) {
		acto = interactComponent->actorFunctionMap[val].Execute(interactComponent,actor, interactComponent->tempAction, interactComponent->tempTooltipIndex, interactComponent->tempType, -1, -1, -1, FExtraActorData(interactComponent->tempInElse, 0));
	}
	if (acto) {
		interactComponent->Interact(acto, data.actionButton);
	}
}

void UInteractComponent::JumpStage(AActor* actor, FJumpStage& data, UInteractComponent* interactComponent)
{
	if (data.stopOtherJumps) {
		interactComponent->stopStage = false;
		interactComponent->tempDelay = -1;
		interactComponent->GetWorld()->GetTimerManager().ClearTimer(interactComponent->nextHandle);
	}
	else {
		interactComponent->stopStage = true;
	}

	interactComponent->tempTooltip = UInteractCore::GetValue(data.nextStage, interactComponent->interactDataArray.Num() - 1);
	float dela = UInteractCore::GetValue(data.delayNextStage);
	if (interactComponent->tempTooltip == interactComponent->m_currentStage) {
		interactComponent->tempSameState = true;
	}
	if (dela > 0.f) {
		interactComponent->tempDelay = dela;
	}
	interactComponent->tempJumpStage = true;
	if (interactComponent->tempType != EActionStageType::Interact && interactComponent->tempType != EActionStageType::None) {
		interactComponent->NextStage(interactComponent->tempTooltip);
	}
}

void UInteractComponent::DebugText(AActor* actor, FDebugText& data, UInteractComponent* interactComponent)
{
	if (data.printAsLog) {
		ISM_ULOG_EDIT("%s", Log, *data.text)
	}
	else {
		ISM_PRINTLOG_EDIT(data.text, data.time, data.color)
	}
}

void UInteractComponent::InteractPressing(int tooltipIndex, AActor* actor, FActionButton actionButton, int newStage)
{
	if (GetWorld() && GetOwner() && interactDataArray.IsValidIndex(m_currentStage)) {
		GetWorld()->GetTimerManager().ClearTimer(functionPressingHandle);
		readyForNextStage = true;

		if (readyForNextStage) {
			if (tempTooltip != -2) {
				NextStage(tempTooltip, interactDataArray[m_currentStage].actions[tooltipIndex]);
			}
			else {
				NextStage(newStage, interactDataArray[m_currentStage].actions[tooltipIndex]);
			}
		}
	}
}

void UInteractComponent::FunctionsPressing(int tooltipIndex, AActor* actor, FActionButton actionButton, int newStage, float time)
{
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		tempDelayFunction = false;
		int stageCount = interactDataArray[m_currentStage].actions[tooltipIndex].stageFunction.Num();
		functionIndex = 0;
		tempActor = actor;
		while (functionIndex < stageCount)
		{
			if (GetWorld() && GetOwner() && interactDataArray.IsValidIndex(m_currentStage) && (interactDataArray[m_currentStage].actions[tooltipIndex].stageFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].actions[tooltipIndex].stageFunction[functionIndex].enumNameCore != EActionListCore::None)) {
				tempAction = actionButton.actionButton;
				//tempIndex = j;
				tempType = EActionStageType::Interact;

				int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
				int valueFunc = ((int)interactDataArray[m_currentStage].actions[tooltipIndex].stageFunction[functionIndex].enumName) - 1;
				int valueFuncCore = ((int)interactDataArray[m_currentStage].actions[tooltipIndex].stageFunction[functionIndex].enumNameCore) - 1;
				if (valueFuncCore >= 0) {
					interactFunctionMap[valueFuncCore].Execute(this, actor, actionButton.actionButton, tooltipIndex, EActionStageType::Interact, -1, -1, -1, false);
				}
				else if (valueFunc >= 0) {
					interactFunctionMap[valueFunc + maxE - 1].Execute(this, actor, actionButton.actionButton, tooltipIndex, EActionStageType::Interact, -1, -1, -1, false);
				}
				//functionIndex++;
				//readyForNextStage = true;
				if (stopStage || tempDelayFunction) {
					break;
				}
			}
			functionIndex++;
			//tempIndex++;
		}
		if (time != 0.f) {
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("FunctionsPressing"), tooltipIndex, actor, actionButton, newStage, time);
			GetWorld()->GetTimerManager().SetTimer(functionPressingHandle, TimerDel, time, false);
		}
	}
}

void UInteractComponent::NextStagePressing(int tooltipIndex)
{
	if (GetWorld() && GetOwner() && interactDataArray.IsValidIndex(m_currentStage)) {
		GetWorld()->GetTimerManager().ClearTimer(functionPressingHandle);
		GetWorld()->GetTimerManager().ClearTimer(pressingHandle);
		readyForNextStage = false;
		float newStage = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[tooltipIndex].m_nextFailStage, interactDataArray.Num() - 1);
		if (newStage != NaN) {
			int newStageint = static_cast<int>(newStage);
			if (tempTooltip != -2) {
				NextStage(tempTooltip, interactDataArray[m_currentStage].actions[tooltipIndex]);
			}
			else {
				NextStage(newStage, interactDataArray[m_currentStage].actions[tooltipIndex]);
			}
		}
	}
}

AActor* UInteractComponent::GetCustomActor(AActor* actor, FGetCustomActor& data, UInteractComponent* interactComponent)
{
	AActor* act = nullptr;
	if (data.tag.ToString() != "" && data.tag != NAME_None) {
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsWithTag(interactComponent->GetWorld(), data.tag, actors);
		for (int i = 0; i < actors.Num(); i++) {
			if ((actors[i]->GetClass() == data.actor) || !data.actor) {
				act = actors[i];
				break;
			}
		}
	}
	else {
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(interactComponent->GetWorld(), data.actor, actors);
		if (actors.Num() > 0) {
			act = actors[0];
		}
	}
	interactComponent->ReturnValue(&data.customReturn, act);
	CONFIRM_RETURN(interactComponent, data, FGetCustomActor)
	return act;
}

AActor* UInteractComponent::GetSpecificActor(AActor* actor, FGetSpecificActor& data, UInteractComponent* interactComponent)
{
	interactComponent->ReturnValue(&data.customReturn, data.actor);
	CONFIRM_RETURN(interactComponent, data, FGetSpecificActor)
	return data.actor;
}

AActor* UInteractComponent::GetThisActor(AActor* actor, FGetThisActor& data, UInteractComponent* interactComponent)
{
	if (data.getOwner) {
		interactComponent->ReturnValue(&data.customReturn, interactComponent->GetOwner());
		CONFIRM_RETURN(interactComponent, data, FGetThisActor)
		return interactComponent->GetOwner();
	}
	else {
		if (actor) {
			interactComponent->ReturnValue(&data.customReturn, actor);
			CONFIRM_RETURN(interactComponent, data, FGetThisActor)
			return actor;
		}
		else {
			interactComponent->ReturnValue(&data.customReturn, interactComponent->tempActor);
			CONFIRM_RETURN(interactComponent, data, FGetThisActor)
			return interactComponent->tempActor;
		}
	}
}

void UInteractComponent::Interact(AActor* actor, FActionButton actionButton)
{
	EAction button = actionButton.actionButton;
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		
		//tempIndex = 0;
		tempDelayFunction = false;
		tempTooltipIndex = 0;
		tempDoOnceDelay = false;
		ticksNum++;
		for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++)
		{
			if (interactDataArray[m_currentStage].actions[i].m_interactIfTooltipIsHidden || interactDataArray[m_currentStage].actions[i].m_showTooltip) {
				float newStage = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_nextStage, interactDataArray.Num() - 1);
				if (newStage != NaN) {
					int newStageint = static_cast<int>(newStage);
					if (interactDataArray[m_currentStage].actions[i].m_holdingDown) {
						if (interactDataArray[m_currentStage].actions[i].actionButton == button && actionButton.actionButtonType == EActionType::Pressed) {
							if (!readyForNextStage) {
								tempTooltipIndex = i;
								float functPressingTime = UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_updateTimePressing);
								if (functPressingTime != 0) {
									FTimerDelegate TimerDel;
									TimerDel.BindUFunction(this, FName("FunctionsPressing"), i, actor, actionButton, newStageint, functPressingTime);
									if (GetWorld()) {
										GetWorld()->GetTimerManager().SetTimer(functionPressingHandle, TimerDel, functPressingTime, false);
									}
								}
								else {
									FunctionsPressing(i, actor, actionButton, newStageint, functPressingTime);
								}

								FTimerDelegate TimerDel2;
								TimerDel2.BindUFunction(this, FName("InteractPressing"), i, actor, actionButton, newStageint);
								if (GetWorld()) {
									GetWorld()->GetTimerManager().SetTimer(pressingHandle, TimerDel2, UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_timePressing), false);
								}
							}
						}
						else if (interactDataArray[m_currentStage].actions[i].actionButton == button && actionButton.actionButtonType == EActionType::Released) {
							NextStagePressing(i);
						}
					}
					else {
						if (interactDataArray[m_currentStage].actions[i].actionButton == button && actionButton.actionButtonType == EActionType::Pressed) {
							int stageCount = interactDataArray[m_currentStage].actions[i].stageFunction.Num();
							tempActor = actor;
							tempAction = button;
							tempType = EActionStageType::Interact;

							FCancelJumpStage canlJump = {};
							UInteractComponent::CancelJumpStage(actor, canlJump, this);

							if (!readyForNextStage && stageCount == 0) {
								readyForNextStage = true;
								if (tempTooltip != -2) {
									NextStage(tempTooltip, interactDataArray[m_currentStage].actions[i]);
								}
								else {
									NextStage(newStageint, interactDataArray[m_currentStage].actions[i]);
								}
								break;
							}
							else if (!readyForNextStage) {
								functionIndex = 0;
								while (functionIndex < stageCount)
								{
									if (GetWorld() && GetOwner() && interactDataArray.IsValidIndex(m_currentStage) && (interactDataArray[m_currentStage].actions[i].stageFunction[functionIndex].enumName != EActionList::None || interactDataArray[m_currentStage].actions[i].stageFunction[functionIndex].enumNameCore != EActionListCore::None)) {
										tempAction = button;
										tempType = EActionStageType::Interact;
										tempTooltipIndex = i;
										readyForNextStage = true;

										int maxE = (int)GETENUMMAX(TEXT("EActionListCore"));
										int valueFunc = ((int)interactDataArray[m_currentStage].actions[i].stageFunction[functionIndex].enumName) - 1;
										int valueFuncCore = ((int)interactDataArray[m_currentStage].actions[i].stageFunction[functionIndex].enumNameCore) - 1;
										if (valueFuncCore >= 0) {
											interactFunctionMap[valueFuncCore].Execute(this, actor, button, i, EActionStageType::Interact, -1, -1, -1, false);
										}
										else if (valueFunc >= 0) {
											interactFunctionMap[valueFunc + maxE - 1].Execute(this, actor, button, i, EActionStageType::Interact, -1, -1, -1, false);
										}
									}
									if (stopStage || tempDelayFunction) {
										break;
									}
									functionIndex++;
								}

								if (readyForNextStage) {
									if (tempTooltip != -2) {
										NextStage(tempTooltip, interactDataArray[m_currentStage].actions[i]);
									}
									else {
										NextStage(newStageint, interactDataArray[m_currentStage].actions[i]);
									}
									break;
								}
							}
						}
					}
				}
				else {
					FString debugText = FString(TEXT("FUNCTION: Interact || ERROR: NextStage is not a number. || INTERACT OWNER ACTOR: "));
					FString ObjectName = (GetOwner() != NULL) ? GetOwner()->GetName() : FString(TEXT("None"));
					debugText.Append(ObjectName);
					FString debugText2 = FString(TEXT(" || CALLING ACTOR: "));
					debugText.Append(debugText2);
					FString ObjectName2 = (actor != NULL) ? actor->GetName() : FString(TEXT("None"));
					debugText.Append(ObjectName2);

					ISM_ULOG_EDIT("%s", Error, *debugText);
				}
			}
		}
	}
}

void UInteractComponent::UpdateInteractArrayData() 
{
	//CHANGE ATRIBUTES FOR ALL STAGES
	for (int j = 0; j < interactDataArray.Num(); j++) {
		for (int i = 0; i < interactDataArray[j].actions.Num(); i++)
		{
			if (!interactDataArray[j].actions[i].m_WidgetComponent)
			{
				FString str = "InteractWidgetComponent:%" + FString::FromInt(j) + "-" + FString::FromInt(i);
				FName name = FName(*str);
				UInteractWidgetComponent* widg = NewObject<UInteractWidgetComponent>(this, UInteractWidgetComponent::StaticClass(), name);
				widg->SetWidgetSpace(EWidgetSpace::Screen);
				widg->SetWidgetClass(StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/TooltipWidget.TooltipWidget_C'")));
				widg->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
				widg->SetFlags(EObjectFlags::RF_Public);
				widgetComps.Push(widg);
			}

			interactDataArray[j].actions[i].m_button = UInteractCore::GetActionButtonImage(interactDataArray[j].actions[i].actionButton);
			if (interactDataArray[j].actions[i].m_circle == nullptr) {
				interactDataArray[j].actions[i].m_circle = circleTexture;
			}
		}
	}

	//CHANGE ATRIBUTES ONLY FOR CURRENT STAGE
	UpdateStageAtributes();


}

void UInteractComponent::UpdateEverything(bool doExport)
{
	if (doExport) {
		ImportInteractDataInternal();
	}
	for (int j = 0; j < interactDataArray.Num(); j++) {
		for (int i = 0; i < interactDataArray[j].actions.Num(); i++) {
			if (!interactDataArray[j].actions[i].m_WidgetComponent)
			{
				widgetComps.Empty();
				break;
			}
		}
	}

	UpdateInteractArrayData();

	if (doExport) {
		ExportInteractDataInternal();
	}
}

void UInteractComponent::CalculateTicks(float time)
{
	if (ticksNum > m_ticksNum) {
		readyForNextStage = false;
		ShowTooltip();
	}
	ticksNum = 0;
}

void UInteractComponent::StopQTEStage()
{
	int selInd = -1;
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++)
		{
			if (IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
				UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());

				if (interactDataArray[m_currentStage].actions[i].m_quickTimeEvent && (m_TooltipWidget && (interactDataArray[m_currentStage].actions[i].m_interactIfTooltipIsHidden || interactDataArray[m_currentStage].actions[i].m_showTooltip || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f) && tempAction == interactDataArray[m_currentStage].actions[i].actionButton)) {
					selInd = i;
					break;
				}
				else if (!interactDataArray[m_currentStage].actions[i].m_quickTimeEvent && (m_TooltipWidget && (interactDataArray[m_currentStage].actions[i].m_interactIfTooltipIsHidden || interactDataArray[m_currentStage].actions[i].m_showTooltip || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f) && tempAction == interactDataArray[m_currentStage].actions[i].actionButton)) {
					selInd = i;
					break;
				}
			}
		}
	}
	for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++)
	{
		if (i != selInd) {
			if (IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
				UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());

				if (m_TooltipWidget && (interactDataArray[m_currentStage].actions[i].m_interactIfTooltipIsHidden || interactDataArray[m_currentStage].actions[i].m_showTooltip || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f)) {
					m_TooltipWidget->StopFunctions(true, UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_tooltipAnimSpeed));
				}
			}
		}
	}
}

bool UInteractComponent::IsQTEAvailable()
{
	bool res = false;
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++)
		{
			if (IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
				UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());

				if (interactDataArray[m_currentStage].actions[i].m_quickTimeEvent && (m_TooltipWidget && (interactDataArray[m_currentStage].actions[i].m_interactIfTooltipIsHidden || interactDataArray[m_currentStage].actions[i].m_showTooltip || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f) && tempAction == interactDataArray[m_currentStage].actions[i].actionButton)) {
					res = m_TooltipWidget->IsAvailableQTE(UInteractCore::GetValue(interactDataArray[m_currentStage].actions[i].m_tooltipAnimSpeed));
					break;
				}
				else if (!interactDataArray[m_currentStage].actions[i].m_quickTimeEvent && (m_TooltipWidget && (interactDataArray[m_currentStage].actions[i].m_interactIfTooltipIsHidden || interactDataArray[m_currentStage].actions[i].m_showTooltip || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f) && tempAction == interactDataArray[m_currentStage].actions[i].actionButton)) {
					res = true;
					break;
				}
			}
		}
	}

	return res;
}

bool UInteractComponent::HasAnyQTE()
{
	if (interactDataArray.IsValidIndex(m_currentStage)) {
		for (int i = 0; i < interactDataArray[m_currentStage].actions.Num(); i++)
		{
			if (IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent) && IsValid(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject())) {
				UTooltip* m_TooltipWidget = Cast<UTooltip>(interactDataArray[m_currentStage].actions[i].m_WidgetComponent->GetUserWidgetObject());
				if (interactDataArray[m_currentStage].actions[i].m_quickTimeEvent && (m_TooltipWidget && ((interactDataArray[m_currentStage].actions[i].m_interactIfTooltipIsHidden || interactDataArray[m_currentStage].actions[i].m_showTooltip) || m_TooltipWidget->GetButton()->ColorAndOpacity.A > 0.01f))) {
					return true;
				}
			}
		}
	}
	return false;
}

#if WITH_EDITOR
void UInteractComponent::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e)
{

	UpdateEverything();

	//CUSTOM EDITOR ONLY TRIGGERED IF CONDITION
	if (e.GetPropertyName().ToString() == "actions" ||
		e.GetPropertyName().ToString() == "conditions" ||
		e.GetPropertyName().ToString() == "functions" ||
		e.GetPropertyName().ToString() == "interactDataArray" ||
		e.GetPropertyName().ToString() == "preStageFunction" ||
		e.GetPropertyName().ToString() == "initFunction" ||
		e.GetPropertyName().ToString() == "stageFunction" ||
		e.GetPropertyName().ToString() == "exitFunction" ||
		e.GetPropertyName().ToString() == "m_holdingDown" ||
		e.GetPropertyName().ToString() == "enumName") {

		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModule.NotifyCustomizationModuleChanged();
	}
}
#endif

void UInteractComponent::OnPreWorldInitializationInteract(UWorld* World, const UWorld::InitializationValues IVS)
{
	UpdateEverything(true);
}