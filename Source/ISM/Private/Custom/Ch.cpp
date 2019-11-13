// Fill out your copyright notice in the Description page of Project Settings.

#include "Ch.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include <WidgetComponent.h>
#include <UserWidget.h>
#include "ChPlayer.h"
#include <ConstructorHelpers.h>
#include <GameFramework/Character.h>
#include "Components/SceneComponent.h"
#include <GameFramework/Actor.h>
#include "InteractCore.h"
#include <Kismet/KismetMathLibrary.h>
#include "InteractComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Engine/Engine.h"
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TooltipWidget.h"

	/*******************************************************************_CONSTRUCTOR_***********************************************************************/

ACh::ACh() {

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->ComponentTags.Add(FName(TEXT("light")));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CHARACTER MOVEMENT iCONFIGURATION
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	GetCharacterMovement()->CrouchedHalfHeight = 60.f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

}

// void ACh::Interact(AActor *actor, FActionButton actionButton)
// {
// 	IInteractInterface::Interact(actor, actionButton);
// 
// 	InteractComponent->Interact(actor, actionButton);
// 
// }

// void ACh::DestroyActor()
// {
// 	Destroy(this);
// }

// bool ACh::CanInteractWithActionType(FActionButton actionButton)
// {
// 	return false;
// }

// TArray<UMeshComponent *> ACh::GetActorMesh() {
// 	TArray<UMeshComponent*> meshes;
// 	meshes.Add(Cast<UMeshComponent>(GetMesh()));
// 	TArray<UActorComponent*> ActorCArray = GetComponentsByClass(UMeshComponent::StaticClass());
// 	//TArray<UActorComponent*> ActorCArray = GetComponentsByTag(UMeshComponent::StaticClass(), "light");
// 	//TArray<UActorComponent*> ActorCArray2 = GetComponentsByTag(UMeshComponent::StaticClass(), "shine");
// 	for (int i = 0; i < ActorCArray.Num(); i++) {
// 		meshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
// 	}
// 	// 	for (int i = 0; i < ActorCArray2.Num(); i++) {
// 	// 		meshes.Add(Cast<UMeshComponent>(ActorCArray2[i]));
// 	// 	}
// 	return meshes;
// }

// TArray<UShapeComponent *> ACh::GetActorCollision()
// {
// 	TArray<UShapeComponent*> collisions;
// 	TArray<UActorComponent*> ActorBArray = GetComponentsByTag(UBoxComponent::StaticClass(), "ModifiableShape");
// 	TArray<UActorComponent*> ActorSArray = GetComponentsByTag(USphereComponent::StaticClass(), "ModifiableShape");
// 	TArray<UActorComponent*> ActorCArray = GetComponentsByTag(UCapsuleComponent::StaticClass(), "ModifiableShape");
// 
// 	if (ActorBArray.Num() != 0)
// 	{
// 		for (int i = 0; i < ActorBArray.Num(); i++) {
// 			collisions.Add(Cast<UShapeComponent>(ActorBArray[i]));
// 		}
// 	}
// 	if (ActorSArray.Num() != 0)
// 	{
// 		for (int i = 0; i < ActorSArray.Num(); i++) {
// 			collisions.Add(Cast<UShapeComponent>(ActorSArray[i]));
// 		}
// 	}
// 	if (ActorCArray.Num() != 0)
// 	{
// 		for (int i = 0; i < ActorCArray.Num(); i++) {
// 			collisions.Add(Cast<UShapeComponent>(ActorCArray[i]));
// 		}
// 	}
// 
// 	return collisions;
// }

// EPriority ACh::GetPriority()
// {
// 	return m_priority;
// }
// 
// bool ACh::GetPriorityRepeat()
// {
// 	return m_priorityRepeat;
// }

void ACh::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (EndPlayReason == EEndPlayReason::EndPlayInEditor) {
		UInteractComponent* intComp = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));
		if (intComp) {
			intComp->delegateAuxFunctions.Empty();
		}
	}
}

void ACh::PostEditMove(bool bFinished)
{
	if (bFinished) {
		UInteractComponent* intComp = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));
		if (intComp) {
			intComp->UpdateStageAtributes();
		}
	}
}

void ACh::BeginPlay()
{
	Super::BeginPlay();

	InteractComponent = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));

}
