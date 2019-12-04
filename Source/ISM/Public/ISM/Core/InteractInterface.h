
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <Engine/Texture2D.h>
#include <Components/MeshComponent.h>
#include "InteractCore.h"
#include "InteractComponent.h"
#include <Components/ShapeComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include "InteractInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISM_API IInteractInterface
{
	GENERATED_BODY()

public:
	
	UInteractComponent* InteractComponent;


	virtual void Interact(AActor *actor, FActionButton actionButton) { InteractComponent->Interact(actor, actionButton);}
	virtual void DestroyActor() { if (AActor* actor = Cast<AActor>(this)) { actor->Destroy(); } }
	virtual bool CanInteractWithActionType(FActionButton actionButton) { return false; }
	inline virtual TArray<UMeshComponent *> GetActorMesh() {
		//TODO tener en cuenta el light
		TArray<UMeshComponent*> meshes;
		if (AActor* actor = Cast<AActor>(this)) {
			TArray<UActorComponent*> ActorCArray = actor->GetComponentsByClass(UMeshComponent::StaticClass());
			for (int i = 0; i < ActorCArray.Num(); i++) {
				meshes.Add(Cast<UMeshComponent>(ActorCArray[i]));
			}
		}
		return meshes;
	}
	inline virtual TArray<UShapeComponent *> GetActorCollision() {
		TArray<UShapeComponent*> collisions;
		if (AActor* actor = Cast<AActor>(this)) {
			TArray<UActorComponent*> ActorBArray = actor->GetComponentsByTag(UBoxComponent::StaticClass(), "ModifiableShape");
			TArray<UActorComponent*> ActorSArray = actor->GetComponentsByTag(USphereComponent::StaticClass(), "ModifiableShape");
			TArray<UActorComponent*> ActorCArray = actor->GetComponentsByTag(UCapsuleComponent::StaticClass(), "ModifiableShape");

			if (ActorBArray.Num() != 0)
			{
				for (int i = 0; i < ActorBArray.Num(); i++) {
					collisions.Add(Cast<UShapeComponent>(ActorBArray[i]));
				}
			}
			if (ActorSArray.Num() != 0)
			{
				for (int i = 0; i < ActorSArray.Num(); i++) {
					collisions.Add(Cast<UShapeComponent>(ActorSArray[i]));
				}
			}
			if (ActorCArray.Num() != 0)
			{
				for (int i = 0; i < ActorCArray.Num(); i++) {
					collisions.Add(Cast<UShapeComponent>(ActorCArray[i]));
				}
			}
		}
		return collisions;
	}
};
