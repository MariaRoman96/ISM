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

	//When the game is closed in editor is necessary to reset the delegates values
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	//When you move an entity in editor mode, it is necessary to update all interact attributes (capsule, position...)
	virtual void PostEditMove(bool bFinished);

protected:
	virtual void BeginPlay() override;

};