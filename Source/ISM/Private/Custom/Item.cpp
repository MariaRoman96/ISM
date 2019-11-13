// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ChPlayer.h"
#include <WidgetComponent.h>
#include <UserWidget.h>
#include <ConstructorHelpers.h>
#include "InteractComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(mRootComp);
	mMesh->ComponentTags.Add(FName(TEXT("light")));

}

// void AItem::Interact(AActor *actor, FActionButton actionButton)
// {
// 	IInteractInterface::Interact(actor, actionButton);
// 
// 	InteractComponent->Interact(actor, actionButton);
// 
// }

// void AItem::DestroyActor()
// {
// 	Destroy(this);
// }

// bool AItem::CanInteractWithActionType(FActionButton actionButton)
// {
// 	return false;
// }

// EPriority AItem::GetPriority()
// {
// 	return m_priority;
// }
// 
// bool AItem::GetPriorityRepeat()
// {
// 	return m_priorityRepeat;
// }


// TArray<UMeshComponent *> AItem::GetActorMesh() {
// 	TArray<UMeshComponent*> meshes;
// 	TArray<UActorComponent*> ActorCArray = GetComponentsByClass(UStaticMeshComponent::StaticClass());
// 	//TArray<UActorComponent*> ActorCArray2 = GetComponentsByTag(UStaticMeshComponent::StaticClass(), "shine");
// 	if (mMesh) {
// 		mMesh->ComponentTags.Add("light");
// 		mMesh->ComponentTags.Add("shine");
// 		meshes.Add(Cast<UMeshComponent>(mMesh));
// 	}
// 	for (int i = 0; i < ActorCArray.Num(); i++) {
// 		meshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
// 	}
// // 	for (int i = 0; i < ActorCArray2.Num(); i++) {
// // 		meshes.Add(Cast<UMeshComponent>(ActorCArray2[i]));
// // 	}
// 	return meshes;
// }

// TArray<UShapeComponent *> AItem::GetActorCollision()
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

void AItem::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (EndPlayReason == EEndPlayReason::EndPlayInEditor) {
		UInteractComponent* intComp = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));
		if (intComp) {
			intComp->delegateAuxFunctions.Empty();
		}
	}
}

void AItem::PostEditMove(bool bFinished)
{
	if (bFinished) {
		UInteractComponent* intComp = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));
		if (intComp) {
			intComp->UpdateStageAtributes();
		}
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	InteractComponent = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));

}
