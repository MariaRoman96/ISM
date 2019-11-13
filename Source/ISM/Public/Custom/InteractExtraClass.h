// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <vector>
#include <functional>
#include <cmath>
#include <DelegateCombinations.h>
#include <limits>
#include <Engine/Engine.h>
#include <LevelSequencePlayer.h>
#include <LevelSequenceActor.h>
#include <Engine/DataTable.h>
#include <Sound/SoundBase.h>
#include <UserWidget.h>
//IF ISN'T .H INCLUDED YET
#include "TooltipWidget.h"
#include "LevelManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "InteractCoreClass.h"
#include "InteractExtraClass.generated.h"

////////////////////////////////
// ENUMS CORE //////////////////
////////////////////////////////

//TO ADD NEW SELECTABLE FUNCTION: ENUM, DISPLAY NAME AND STRUCT NAME MUST BE THE SAME
UENUM(BlueprintType)
enum class EActionList : uint8
{
	None 							UMETA(DisplayName = "None", customCategory = "Core"),
	objectInspector					UMETA(DisplayName = "objectInspector", customCategory = "Camera"),
	curveLineTrace					UMETA(DisplayName = "curveLineTrace", customCategory = "Common"),
};

//TO ADD NEW SELECTABLE FUNCTION: ENUM, DISPLAY NAME AND STRUCT NAME MUST BE THE SAME
UENUM(BlueprintType)
enum class ECheckList : uint8
{
	None 						UMETA(DisplayName = "None", customCategory = "Core"),
	placeholderCheck			UMETA(DisplayName = "placeholderCheck", customCategory = "Core"),

};

UENUM(BlueprintType)
enum class EDelegateList : uint8
{
	None 						UMETA(DisplayName = "None", customCategory = "Core"),
};

UENUM(BlueprintType)
enum class ETimerList : uint8
{
	None 						UMETA(DisplayName = "None", customCategory = "Core"),
	objectInspector				UMETA(DisplayName = "objectInspector", customCategory = "Camera"),
	curveLineTrace				UMETA(DisplayName = "curveLineTrace", customCategory = "Common"),
};

////////////////////////////////
// STRUCTS /////////////////////
////////////////////////////////

USTRUCT(BlueprintType)
struct FObjectInspector : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FObjectInspector() {}
	//Visibles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Inspectionate"))
		bool canInspectionate = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Next stage clue found"))
		int nextStageClueFound = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Velocity Yaw"))
		float rotationVelocityLeftRight = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Velocity Pitch"))
		float rotationVelocityUpDown = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Velocity Roll"))
		float rotationVelocityRoll = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Clue time lapse"))
		float timePastUntilFoundClue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Clue time to disappear"))
		float timePastUntilClueDisappear = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Clue distance to camera"))
		float clueDistancePosition = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Clue distance to camera"))
		float sphereSize = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Debug Sphere"))
		bool debugSphereOn = false;
	//The higher the number, the fastest will execute
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timerSpeedRate = 1.0f;

	//No visibles
	float deltaTimeAlt;
	FRotator ObjectActualRotatio;
	FRotator objectRotator;
	FRotator makeRotatorOfAxis;

	bool foundingClue = false;
	int finishingCounter = 5;
	float contadorObservador;
	float contadorClue;
	bool doOnce;
	float DeltaTime = 0.01f;
	FSavedStateInteract savedState;
	FTimerHandle ObjInspectorTimer;
};

USTRUCT(BlueprintType)
struct FCurveLineTrace : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FCurveLineTrace() {}
	//Visibles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stopDrawing"))
		bool stopDrawing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CP1Vector"))
		FVector CP1Vector = { 0,0,320 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CP1Float"))
		float CP1Float = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CP2Vector"))
		FVector CP2Vector = { 0, 0, 320 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CP2Float"))
		float CP2Float = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CPEndVector"))
		FVector CPEndVector = { 0, 0, 2500 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CPEndFloat"))
		float CPEndFloat = 4000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "numPoint"))
		int numPoints = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "lineDiscontinua"))
		bool lineaDiscontinua = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "particles"))
		UParticleSystem* particles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DeltaTime"))
		float DeltaTime = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "distancedOffsetToStartDrawing"))
		FVector distancedOffsetToStartDrawing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "bucle"))
		int bucle = 1;

	int contador = 0;

	UParticleSystemComponent* beamComp;
	TArray <FVector> outPoints;
	TArray<UParticleSystemComponent*> m_BeamsArray;
	FSavedStateInteract savedState;
	FTimerHandle m_Timer;
};

USTRUCT(BlueprintType)
struct FPlaceholderCheck : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FPlaceholderCheck() {}
};

////////////////////////////////
// CORE STRUCTS ////////////////
////////////////////////////////

USTRUCT(BlueprintType)
struct FFunctionDataBase : public FFunctionDataBaseCore
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionDataBase() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionList enumName = EActionList::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "objectInspector")
		FObjectInspector objectInspector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "curveLineTrace")
		FCurveLineTrace curveLineTrace;
};

USTRUCT(BlueprintType)
struct FFunctionCheckDataBase : public FFunctionCheckDataBaseCore
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionCheckDataBase() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECheckList enumName = ECheckList::None;

	//ADD NEW ENUM NAME AND HIS STRUCT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "placeholderCheck")
		FPlaceholderCheck placeholderCheck;

};

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDACTIONS "ObjectInspector","CurveLineTrace"

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDCHECKS "PlaceholderCheck"

//BIND ENUM USED IN STRUCTS
#define BINDENUMS "ENumericComparison", "ECurveCoordinates", "EBone_Socket", "ERaycastType", "ECollisionRayCastType", "EVectorDirection", "ESpecificAxis", "ESpecificRotAxis", \
"EItemType", "EAction", "EActionType", "EHandToHold", "ETimerList", "EMovementState", "EGaitMode", "EStanceMode", "ELocationState", "EOutlineColor", \
"ERotationMode", "ECameraMode", "ECardinalDirection", "EFootstepType", "EIdleEntryState", "ELocomotionState", "EMovementDirection", \
"ECanCharacterStepUpOn", "ECollisionEnabledType", "ECollisionObjectType", "ECollisionPresets", "ECollisionType", "ESaveSlot", "ECheckPlayerInput", "ESoundAttenuationFunction", \
"ECameraEffect", "EAttachmentRule", "EDetachmentRule", "EPriority", "ETriggerSelect", "EInventoryPanel", "EImpulseType", "ECharacterStats", "EWidgetAnimMode", \
"EInteractMovementState", "EInteractGaitMode", "EInteractStanceMode", "EInteractLocationState", "EInteractCameraMode", "EDelegateListCore", "EDelegateList", "EActionList", "EActionListCore", \
"EActionStageType", "ETimerListCore", "ESetBoolValue"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISM_API UInteractExtraClass : public UInteractCoreClass
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractExtraClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
