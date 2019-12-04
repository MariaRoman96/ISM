#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "InteractCore.h"
#include <GameFramework/Character.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Components/ShapeComponent.h>
#include <Engine/EngineTypes.h>
#include "Ch.generated.h"

class UInteractComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ISM_API ACh : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

public:

	// CONSTRUCTOR
	ACh();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* mRootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* mMesh;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void PostEditMove(bool bFinished);

protected:

	// BEGIN PLAY
	virtual void BeginPlay() override;

};