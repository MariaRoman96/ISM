#pragma once

#include "CoreMinimal.h"
#include "TooltipWidget.h"
#include "InteractInterface.h"
#include <Components/MeshComponent.h>
#include "Engine/Engine.h"
#include "Item.generated.h"

class UInteractComponent;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ISM_API AItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	AItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* mRootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mMesh;

	/*virtual void DestroyActor() override;*/
// 	virtual bool CanInteractWithActionType(FActionButton actionButton);
// 	virtual TArray<UMeshComponent *> GetActorMesh();
// 	virtual TArray<UShapeComponent *> GetActorCollision();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void PostEditMove(bool bFinished);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};