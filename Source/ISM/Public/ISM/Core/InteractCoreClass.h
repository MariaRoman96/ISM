
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <DelegateCombinations.h>
#include "LevelSequence.h"
#include "InteractCore.h"
#include "InteractCoreClass.generated.h"

class UInteractComponent;

DECLARE_DELEGATE_NineParams(FDelegate, UInteractComponent*, AActor*, EAction, int, EActionStageType, int, int, int, bool);
DECLARE_DELEGATE_RetVal_FiveParams(bool, FDelegateBool, UInteractComponent*, AActor*, EAction, int, EActionStageType);

////////////////////////////////
// ENUMS CORE //////////////////
////////////////////////////////

//TO ADD NEW SELECTABLE FUNCTION: ENUM, DISPLAY NAME AND STRUCT NAME MUST BE THE SAME
UENUM(BlueprintType)
enum class EActionListCore : uint8
{
	None 							UMETA(DisplayName = "None", customCategory = "Core"),
	destroyEntity					UMETA(DisplayName = "destroyEntity", customCategory = "Core"),
	ifFunction						UMETA(DisplayName = "ifFunction", customCategory = "Core"),
	executeAction					UMETA(DisplayName = "executeAction", customCategory = "Core"),
	jumpStage						UMETA(DisplayName = "jumpStage", customCategory = "Core"),
	debugText						UMETA(DisplayName = "debugText", customCategory = "Core"),
	externalInteract				UMETA(DisplayName = "externalInteract", customCategory = "Core"),
	jumpFunction					UMETA(DisplayName = "jumpFunction", customCategory = "Core"),
	cancelJumpStage					UMETA(DisplayName = "cancelJumpStage", customCategory = "Core"),
	breakFunction					UMETA(DisplayName = "breakFunction", customCategory = "Core"),
	delayFunction					UMETA(DisplayName = "delayFunction", customCategory = "Core"),
	cancelTimer						UMETA(DisplayName = "cancelTimer", customCategory = "Core"),
	createEditVariable				UMETA(DisplayName = "createEditVariable", customCategory = "Core"),
	openLevelFunction				UMETA(DisplayName = "openLevelFunction", customCategory = "Core"),
	changePriority					UMETA(DisplayName = "changePriority", customCategory = "Core"),
	executeTrigger					UMETA(DisplayName = "executeTrigger", customCategory = "Core"),
	getExternalVariable				UMETA(DisplayName = "getExternalVariable", customCategory = "Core"),
	getInternalVariable				UMETA(DisplayName = "getInternalVariable", customCategory = "Core"),
	execCommand						UMETA(DisplayName = "execCommand", customCategory = "Core"),
	setInternalVariable				UMETA(DisplayName = "setInternalVariable", customCategory = "Core"),
	executeInternalFunction			UMETA(DisplayName = "executeInternalFunction", customCategory = "Core"),
	whenFunction					UMETA(DisplayName = "whenFunction", customCategory = "Core"),
	callExternalDelegateFunction	UMETA(DisplayName = "callExternalDelegateFunction", customCategory = "Core"),
	loadStreamLevelInteract			UMETA(DisplayName = "loadStreamLevelInteract", customCategory = "Core"),
	unloadStreamLevelInteract		UMETA(DisplayName = "unloadStreamLevelInteract", customCategory = "Core"),

	rotateLookAtActor				UMETA(DisplayName = "rotateLookAtActor", customCategory = "Transform"),
	rotateToFunction				UMETA(DisplayName = "rotateToFunction", customCategory = "Transform"),
	teleportTo						UMETA(DisplayName = "teleportTo", customCategory = "Transform"),
	setTransformActor				UMETA(DisplayName = "setTransformActor", customCategory = "Transform"),
	moveToFunction					UMETA(DisplayName = "moveToFunction", customCategory = "Transform"),
	
	setVisibilityActor				UMETA(DisplayName = "setVisibilityActor", customCategory = "Common"),
	spawnerFunction					UMETA(DisplayName = "spawnerFunction", customCategory = "Common"),
	slowTimeFunction				UMETA(DisplayName = "slowTimeFunction", customCategory = "Common"),
	setCollisionSettings			UMETA(DisplayName = "setCollisionSettings", customCategory = "Common"),
	setPhysicsSettings				UMETA(DisplayName = "setPhysicsSettings", customCategory = "Common"),
	addTagFunction					UMETA(DisplayName = "addTagFunction", customCategory = "Common"),
	setMaterialMesh					UMETA(DisplayName = "setMaterialMesh", customCategory = "Common"),
	showHideOutlineShine			UMETA(DisplayName = "showHideOutlineShiCore", customCategory = "Common"),

	attachToObject					UMETA(DisplayName = "attachToObject", customCategory = "Character"),
	addImpulseFunction				UMETA(DisplayName = "addImpulseFunction", customCategory = "Character"),
	setRagdoll						UMETA(DisplayName = "setRagdoll", customCategory = "Character"),
	relocateFunction				UMETA(DisplayName = "relocateFunction", customCategory = "Character"),


	cameraEffect					UMETA(DisplayName = "cameraEffect", customCategory = "Camera"),

	playSoundFunction				UMETA(DisplayName = "playSoundFunction", customCategory = "Music"),
	stopSoundFunction				UMETA(DisplayName = "stopSoundFunction", customCategory = "Music"),
	modifySoundFunction				UMETA(DisplayName = "modifySoundFunction", customCategory = "Music"),

	addWidgetToViewport				UMETA(DisplayName = "addWidgetToViewport", customCategory = "UI"),
	removeWidgetFunction			UMETA(DisplayName = "removeWidgetFunction", customCategory = "UI"),
	showHideTooltip					UMETA(DisplayName = "showHideTooltip", customCategory = "UI"),
	setTooltipLocationSize			UMETA(DisplayName = "setTooltipLocationSize", customCategory = "UI"),
	playWidgetAnimation				UMETA(DisplayName = "playWidgetAnimation", customCategory = "UI"),

	playAnimCharacter				UMETA(DisplayName = "playAnimCharacter", customCategory = "Animation"),
	playSequence					UMETA(DisplayName = "playSequence", customCategory = "Animation"),
};

//TO ADD NEW SELECTABLE FUNCTION: ENUM, DISPLAY NAME AND STRUCT NAME MUST BE THE SAME
UENUM(BlueprintType)
enum class ECheckListCore : uint8
{
	None 						UMETA(DisplayName = "None", customCategory = "Core"),
	debugCondition				UMETA(DisplayName = "debugCondition", customCategory = "Core"),
	decrementIncrement			UMETA(DisplayName = "decrementIncrement", customCategory = "Core"),
	andFunction					UMETA(DisplayName = "andFunction", customCategory = "Core"),
	checkVariableValue			UMETA(DisplayName = "checkVariableValue", customCategory = "Core"),
	checkRaycast				UMETA(DisplayName = "checkRaycast", customCategory = "Core"),
	checkIfActorExists			UMETA(DisplayName = "checkIfActorExists", customCategory = "Core"),
	checkTooltipVisibility		UMETA(DisplayName = "checkTooltipVisibility", customCategory = "UI"),
	checkIfSoundIsPlaying		UMETA(DisplayName = "checkIfSoundIsPlaying", customCategory = "Music"),
	checkActorLocation			UMETA(DisplayName = "checkActorLocation", customCategory = "Transform"),
	checkActorRotation			UMETA(DisplayName = "checkActorRotation", customCategory = "Transform"),
};

UENUM(BlueprintType)
enum class EDelegateListCore : uint8
{
	None 						UMETA(DisplayName = "None", customCategory = "Core"),
	onDebugTest					UMETA(DisplayName = "onDebugTest", customCategory = "Core"),
	onAnimationNotification		UMETA(DisplayName = "onAnimationNotification", customCategory = "Character"),
	onAnimationTimer			UMETA(DisplayName = "onAnimationTimer", customCategory = "Character"),
};

UENUM(BlueprintType)
enum class ETimerListCore : uint8
{
	None 						UMETA(DisplayName = "None", customCategory = "Core"),
	decrementIncrement			UMETA(DisplayName = "decrementIncrement", customCategory = "Core"),
	playAnimCharacter			UMETA(DisplayName = "playAnimCharacter", customCategory = "Animation"),
	moveToFunction				UMETA(DisplayName = "moveToFunction", customCategory = "Transform"),
	rotateToFunction			UMETA(DisplayName = "rotateToFunction", customCategory = "Transform"),
	playSoundFunction			UMETA(DisplayName = "playSoundFunction", customCategory = "Sound"),
	relocateFunction			UMETA(DisplayName = "relocateFunction", customCategory = "Character"),
};

////////////////////////////////
// ENUMS ///////////////////////
////////////////////////////////

UENUM(BlueprintType)
enum class ESetBoolValue : uint8
{
	E_None		UMETA(DisplayName = "None"),
	E_True		UMETA(DisplayName = "True"),
	E_False		UMETA(DisplayName = "False")
};

UENUM(BlueprintType)
enum class EVectorType : uint8
{
	Vector 	UMETA(DisplayName = "Vector"),
	Rotator	UMETA(DisplayName = "Rotator"),
	Color	UMETA(DisplayName = "Color")
};

UENUM(BlueprintType)
enum class ENumericComparison : uint8
{
	Equals				UMETA(DisplayName = "=="),
	LessThan			UMETA(DisplayName = "<"),
	GreaterThan			UMETA(DisplayName = ">"),
	LessEqualsThan		UMETA(DisplayName = "<="),
	GreaterEqualsThan	UMETA(DisplayName = ">="),
	Diferent			UMETA(DisplayName = "!="),
};

UENUM(BlueprintType)
enum class ECharacterStats : uint8
{
	None				UMETA(DisplayName = "None"),
	Health				UMETA(DisplayName = "Health"),
	Cold				UMETA(DisplayName = "Cold"),
	Freeze				UMETA(DisplayName = "Freeze"),
	Nutrition			UMETA(DisplayName = "Nutrition"),
	Hunger				UMETA(DisplayName = "Hunger"),
};

UENUM(BlueprintType)
enum class EWidgetAnimMode : uint8
{
	Forward				UMETA(DisplayName = "Forward"),
	Reverse				UMETA(DisplayName = "Reverse"),
	PingPong			UMETA(DisplayName = "PingPong"),
};

UENUM(BlueprintType)
enum class ECameraEffect : uint8
{
	None				UMETA(DisplayName = "None"),
	Fade				UMETA(DisplayName = "Fade"),
	DepthOfField		UMETA(DisplayName = "DepthOfField"),
	CancelDepthOfField  UMETA(DisplayName = "CancelDepthOfField"),
};

UENUM(BlueprintType)
enum class ESoundAttenuationFunction : uint8
{
	Linear				UMETA(DisplayName = "Linear"),
	Inverse				UMETA(DisplayName = "Inverse"),
	Logarithmic			UMETA(DisplayName = "Logarithmic"),
	LogReverse			UMETA(DisplayName = "LogReverse"),
	NaturalSound		UMETA(DisplayName = "NaturalSound"),
	CustomCurve			UMETA(DisplayName = "CustomCurve"),
};

UENUM(BlueprintType)
enum class ETriggerSelect : uint8
{
	Ignore				UMETA(DisplayName = "Ignore"),
	Enter				UMETA(DisplayName = "Enter"),
	Exit				UMETA(DisplayName = "Exit"),
};

UENUM(BlueprintType)
enum class ECurveCoordinates : uint8
{
	XY 				UMETA(DisplayName = "XY"),
	XZ				UMETA(DisplayName = "XZ"),
	YZ				UMETA(DisplayName = "YZ"),
};

UENUM(BlueprintType)
enum class EBone_Socket : uint8
{
	ROOT		UMETA(DisplayName = "Root"),
	PELVIS		UMETA(DisplayName = "Pelvis"),
	SPINE_01	UMETA(DisplayName = "Spine_01"),
	SPINE_02	UMETA(DisplayName = "Spine_02"),
	SPINE_03	UMETA(DisplayName = "Spine_03"),
	NECK		UMETA(DisplayName = "Neck"),
	HEAD		UMETA(DisplayName = "Head"),
	TOPHEAD		UMETA(DisplayName = "TopHead"),
	L_CLAVICLE	UMETA(DisplayName = "L_Clavicle"),
	L_UPPERARM	UMETA(DisplayName = "L_UpperArm"),
	L_LOWERARM	UMETA(DisplayName = "L_LowerArm"),
	L_HAND		UMETA(DisplayName = "L_Hand"),
	L_INDEX_01	UMETA(DisplayName = "L_Index_01"),
	L_INDEX_02	UMETA(DisplayName = "L_Index_02"),
	L_INDEX_03	UMETA(DisplayName = "L_Index_03"),
	L_INDEX_04	UMETA(DisplayName = "L_Index_04"),
	R_CLAVICLE	UMETA(DisplayName = "R_Clavicle"),
	R_UPPERARM	UMETA(DisplayName = "R_UpperArm"),
	R_LOWERARM	UMETA(DisplayName = "R_LowerArm"),
	R_HAND		UMETA(DisplayName = "R_Hand"),
	R_INDEX_01	UMETA(DisplayName = "R_Index_01"),
	R_INDEX_02	UMETA(DisplayName = "R_Index_02"),
	R_INDEX_03	UMETA(DisplayName = "R_Index_03"),
	R_INDEX_04	UMETA(DisplayName = "R_Index_04"),
	L_THIGH		UMETA(DisplayName = "L_Thigh"),
	L_CALF		UMETA(DisplayName = "L_Calf"),
	L_FOOT		UMETA(DisplayName = "L_Foot"),
	L_BALL		UMETA(DisplayName = "L_Ball"),
	L_TOECAP	UMETA(DisplayName = "L_Toecap"),
	R_THIGH		UMETA(DisplayName = "R_Thigh"),
	R_CALF		UMETA(DisplayName = "R_Calf"),
	R_FOOT		UMETA(DisplayName = "R_Foot"),
	R_BALL		UMETA(DisplayName = "R_Ball"),
	R_TOECAP	UMETA(DisplayName = "R_Toecap")
};

UENUM(BlueprintType)
enum class ECanCharacterStepUpOn : uint8
{
	NO 		UMETA(DisplayName = "No"),
	YES		UMETA(DisplayName = "Yes"),
	OWNER	UMETA(DisplayName = "(Owner)")
};

UENUM(BlueprintType)
enum class ECollisionEnabledType : uint8
{
	NOCOLLISION 		UMETA(DisplayName = "No Collision"),
	QUERYONLY			UMETA(DisplayName = "Query Only (No Physics Collision)"),
	PHYSICSONLY			UMETA(DisplayName = "Physics Only (No Query Collision)"),
	COLLISIONENABLED	UMETA(DisplayName = "Collision Enabled (Query and Physics)")
};

UENUM(BlueprintType)
enum class ECollisionObjectType : uint8
{
	VISIBILITY		UMETA(DisplayName = "Visibility"),
	CAMERA			UMETA(DisplayName = "Camera"),
	WORLDSTATIC		UMETA(DisplayName = "WorldStatic"),
	WORLDDYNAMIC	UMETA(DisplayName = "WorldDynamic"),
	PAWN			UMETA(DisplayName = "Pawn"),
	PHYSICSBODY		UMETA(DisplayName = "PhysicsBody"),
	VEHICLE			UMETA(DisplayName = "Vehicle"),
	DESTRUCTIBLE	UMETA(DisplayName = "Destructible")
};

UENUM(BlueprintType)
enum class ECollisionPresets : uint8
{
	NOCOLLISION				UMETA(DisplayName = "NoCollision"),
	BLOCKALL 				UMETA(DisplayName = "BlockAll"),
	OVERLAPALL 				UMETA(DisplayName = "OverlapAll"),
	BLOCKALLDYNAMIC			UMETA(DisplayName = "BlockAllDynamic"),
	OVERLAPALLDYNAMIC		UMETA(DisplayName = "OverlapAllDynamic"),
	IGNOREONLYPAWN 			UMETA(DisplayName = "IgnoreOnlyPawn"),
	OVERLAPONLYPAWN			UMETA(DisplayName = "OverlapOnlyPawn"),
	PAWN 					UMETA(DisplayName = "Pawn"),
	SPECTATOR 				UMETA(DisplayName = "Spectator"),
	CHARACTERMESH 			UMETA(DisplayName = "CharacterMesh"),
	PHYSICSACTOR 			UMETA(DisplayName = "PhysicsActor"),
	DESTRUCTIBLE 			UMETA(DisplayName = "Destructible"),
	INVISIBLEWALL 			UMETA(DisplayName = "InvisibleWall"),
	INVISIBLEWALLDYNAMIC	UMETA(DisplayName = "InvisibleWallDynamic"),
	TRIGGER 				UMETA(DisplayName = "Trigger"),
	RAGDOLL 				UMETA(DisplayName = "Ragdoll"),
	VEHICLE 				UMETA(DisplayName = "Vehicle"),
	UI						UMETA(DisplayName = "UI")
};

UENUM(BlueprintType)
enum class ECollisionType : uint8
{
	IGNORE 	UMETA(DisplayName = "Ignore"),
	OVERLAP	UMETA(DisplayName = "Overlap"),
	BLOCK	UMETA(DisplayName = "Block")
};

UENUM(BlueprintType)
enum class EImpulseType : uint8
{
	AddImpulseAtLocation 		UMETA(DisplayName = "AddImpulseAtLocation"),
	AddImpulse					UMETA(DisplayName = "AddImpulse"),
	AddAngularImpulseInDegrees 	UMETA(DisplayName = "AddAngularImpulseInDegrees"),
	AddAngularImpulseInRadians	UMETA(DisplayName = "AddAngularImpulseInRadians"),
	AddForceAtLocation			UMETA(DisplayName = "AddForceAtLocation"),
	AddForceAtLocationLocal 	UMETA(DisplayName = "AddForceAtLocationLocal"),
	AddForce					UMETA(DisplayName = "AddForce")
};

UENUM(BlueprintType)
enum class EInteractMovementState : uint8
{
	NONE		UMETA(DisplayName = "None"),
	GROUNDED	UMETA(DisplayName = "Grounded"),
	FALLING		UMETA(DisplayName = "Falling")
};

UENUM(BlueprintType)
enum class EInteractGaitMode : uint8
{
	NONE		UMETA(DisplayName = "None"),
	WALKING		UMETA(DisplayName = "Walking"),
	RUNNING		UMETA(DisplayName = "Running"),
	SPRINTING	UMETA(DisplayName = "Sprinting")
};

UENUM(BlueprintType)
enum class EInteractStanceMode : uint8
{
	NONE		UMETA(DisplayName = "None"),
	STANDING	UMETA(DisplayName = "Standing"),
	CROUCHING	UMETA(DisplayName = "Crouching")
};

UENUM(BlueprintType)
enum class EInteractLocationState : uint8
{
	NONE	UMETA(DisplayName = "None"),
	INDOOR	UMETA(DisplayName = "Indoor"),
	OUTDOOR	UMETA(DisplayName = "Outdoor")
};

UENUM(BlueprintType)
enum class EInteractCameraMode : uint8
{
	NONE		UMETA(DisplayName = "None"),
	FIRSTPERSON UMETA(DisplayName = "First Person"),
	THIRDPERSON UMETA(DisplayName = "Third Person")
};

UENUM(BlueprintType)
enum class ESaveSlot : uint8
{
	GAME_SLOT_01	UMETA(DisplayName = "GAME_SLOT_01"),
	ACTORS_SLOT_01	UMETA(DisplayName = "ACTORS_SLOT_01")
};

////////////////////////////////
// STRUCTS /////////////////////
////////////////////////////////

USTRUCT(BlueprintType)
struct FDebugTest : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FDebugTest() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
};

USTRUCT(BlueprintType)
struct FDebugCheckTest : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FDebugCheckTest() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "dataBool"))
		bool dataBool = true;
};

USTRUCT(BlueprintType)
struct FLoadStreamLevelInteract : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FLoadStreamLevelInteract() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "levelName"))
		FName levelName = NAME_None;

};

USTRUCT(BlueprintType)
struct FUnloadStreamLevelInteract : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FUnloadStreamLevelInteract() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "levelName"))
		FName levelName = NAME_None;
};

USTRUCT(BlueprintType)
struct FBaseCreateEditVariable {

	GENERATED_USTRUCT_BODY()
public:
	FBaseCreateEditVariable() {}

	FBaseCreateEditVariable(bool destroy1, FString variableName1, FString variableValue1)
	{
		destroy = destroy1;
		variableName = variableName1;
		variableValue = variableValue1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "destroy"))
		bool destroy = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableName"))
		FString variableName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableValue"))
		FString variableValue = "";

};

USTRUCT(BlueprintType)
struct FCreateEditVariable : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCreateEditVariable() {}

	FCreateEditVariable(bool doOnExternalActor1, FFunctionActorData actor1, TArray<FBaseCreateEditVariable> variables1)
	{
		doOnExternalActor = doOnExternalActor1;
		actor = actor1;
		variables = variables1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "doOnExternalActor"))
		bool doOnExternalActor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor", EditCondition = "doOnExternalActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variables"))
		TArray<FBaseCreateEditVariable> variables;

};

USTRUCT(BlueprintType)
struct FExecuteInternalFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FExecuteInternalFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "functionName"))
		bool selectActor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor", EditCondition = "selectActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificComponent", EditCondition = "selectActor"))
		TSubclassOf<UActorComponent> specificComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "componentTag", EditCondition = "selectActor"))
		FName componentTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificClass", EditCondition = "!selectActor"))
		UClass* specificClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "functionName"))
		FName functionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "parameters"))
		TArray<FString> parameters;

};

USTRUCT(BlueprintType)
struct FGetInternalVariable : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FGetInternalVariable() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "functionName"))
		bool selectActor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor", EditCondition = "selectActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificComponent", EditCondition = "selectActor"))
		TSubclassOf<UActorComponent> specificComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "componentTag", EditCondition = "selectActor"))
		FName componentTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificClass", EditCondition = "!selectActor"))
		UClass* specificClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableName"))
		FString variableName;

};

USTRUCT(BlueprintType)
struct FSetInternalVariable : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetInternalVariable() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificComponent"))
		TSubclassOf<UActorComponent> specificComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "componentTag"))
		FName componentTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableName"))
		FString variableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableValue"))
		FString variableValue;

};

USTRUCT(BlueprintType)
struct FExecCommand : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FExecCommand() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "command"))
		FString command;

};

USTRUCT(BlueprintType)
struct FCheckTooltipVisibility : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCheckTooltipVisibility() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificTooltip"))
		bool specificTooltip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "specificTooltip", DisplayName = "selectTooltip"))
		int selectTooltip = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "isVisible"))
		bool isVisible = false;

};

USTRUCT(BlueprintType)
struct FAddWidgetToViewport : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FAddWidgetToViewport() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificWidget"))
		TSubclassOf<UUserWidget> specificWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "zOrder"))
		int zOrder = 0;
};

USTRUCT(BlueprintType)
struct FPlayWidgetAnimation : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FPlayWidgetAnimation() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificWidget"))
		TSubclassOf<UUserWidget> specificWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "animationName"))
		FString animationName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "selectMode"))
		EWidgetAnimMode selectMode = EWidgetAnimMode::Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "speed"))
		float speed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "numLoops"))
		int numLoops = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "startTime"))
		float startTime = 0.f;
};

USTRUCT(BlueprintType)
struct FRemoveWidgetFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FRemoveWidgetFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificWidget"))
		TSubclassOf<UUserWidget> specificWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "doOnce"))
		bool doOnce = false;
};

USTRUCT(BlueprintType)
struct FGetExternalVariable : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FGetExternalVariable() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableNameExternal"))
		FString variableNameExternal = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableNameInternal"))
		FString variableNameInternal = "";

};

USTRUCT(BlueprintType)
struct FCheckVariableValue : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCheckVariableValue() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableName"))
		FString variableName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "numericComparison"))
		ENumericComparison numericComparison = ENumericComparison::Equals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "variableValue"))
		FString variableValue = "";

};

USTRUCT(BlueprintType)
struct FDebugCondition : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FDebugCondition() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool retValue = false;
};

USTRUCT(BlueprintType)
struct FJumpStage : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FJumpStage() {}

	FJumpStage(bool stopOtherJumps1, FString nextStage1, FString delayNextStage1)
	{
		stopOtherJumps = stopOtherJumps1;
		nextStage = nextStage1;
		delayNextStage = delayNextStage1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stopOtherJumps", Tooltip = "If true, then it will stop other stage jumps (if true then put it first in the functions, the next functions will be executed; if false, it won't continue executing functions, then you can use canceljump if you want to cancel (it need to be first))"))
		bool stopOtherJumps = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "NextStage"))
		FString nextStage = "0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DelayNextStage"))
		FString delayNextStage = "0";

};

USTRUCT(BlueprintType)
struct FDebugText : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FDebugText() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Text"))
		FString text = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "printAsLog"))
		bool printAsLog = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "time"))
		float time = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "color"))
		FColor color = FColor(FColor::Red);

};

USTRUCT(BlueprintType)
struct FCameraEffectFade {

	GENERATED_USTRUCT_BODY()
public:
	FCameraEffectFade() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "fromAlpha"))
		float fromAlpha = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "toAlpha"))
		float toAlpha = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "duration"))
		float duration = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "color"))
		FColor color = FColor(FColor::Black);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "fadeAudio"))
		bool fadeAudio = false;

};

USTRUCT(BlueprintType)
struct FCameraEffectDepthOfField {

	GENERATED_USTRUCT_BODY()
public:
	FCameraEffectDepthOfField() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "focalDistance"))
		float FocalDistance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "focalRegion"))
		float FocalRegion = 150.0f;
};

USTRUCT(BlueprintType)
struct FCameraEffect : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCameraEffect() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "selectEffect"))
		ECameraEffect selectEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "fadeEffect"))
		FCameraEffectFade fadeEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "depthOfFieldEffect"))
		FCameraEffectDepthOfField depthOfFieldEffect;

};

USTRUCT(BlueprintType)
struct FSetMaterialMesh : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetMaterialMesh() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "material"))
		UMaterialInterface* material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "elementIndex"))
		int elementIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "meshIndex"))
		int meshIndex = 0;
};

USTRUCT(BlueprintType)
struct FOpenLevelFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FOpenLevelFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "restartLevel"))
		bool restartLevel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "levelName"))
		FName levelName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "options"))
		FString options = "";

};

USTRUCT(BlueprintType)
struct FDecrementIncrement : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FDecrementIncrement() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Number"))
		FString initNumber = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Limit"))
		float limit = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Quantity"))
		float quantity = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Increment"))
		bool increment = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "resetTimer"))
		bool resetTimer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "resetTimer", DisplayName = "timeToReset"))
		float timeToReset = 1.f;

	FTimerHandle decrementIncrementTimer;
	FSavedStateInteract savedState;

};

USTRUCT(BlueprintType)
struct FCancelJumpStage : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCancelJumpStage() {}

};

USTRUCT(BlueprintType)
struct FJumpFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FJumpFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Function"))
		FString function = "0";

};

USTRUCT(BlueprintType)
struct FBreakFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FBreakFunction() {}

};

USTRUCT(BlueprintType)
struct FSetTooltipLocationSize : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetTooltipLocationSize() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setLocation"))
		bool setLocation = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "location", EditCondition = "setLocation"))
		FVector location = { 0.f, 0.f, 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setSize"))
		bool setSize = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "size", EditCondition = "setSize"))
		FVector size = { 1.f, 1.f, 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificTooltip"))
		EAction specificTooltip = EAction::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificStage"))
		int specificStage = -1;

};

USTRUCT(BlueprintType)
struct FShowHideOutlineShine : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FShowHideOutlineShine() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setOutline"))
		bool setOutline = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "outlineColor", EditCondition = "setOutline"))
		EOutlineColor outlineColor = EOutlineColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "outlineValue", Tooltip = "It modify only the inner value (don't make outline visible)"))
		EOutlineColor outlineValue = EOutlineColor::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setShine"))
		bool setShine = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "showShine", EditCondition = "setShine"))
		bool showShine = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "shineValue", Tooltip = "It modify only the inner value (don't make shine visible)"))
		ESetBoolValue shineValue = ESetBoolValue::E_None;
};

USTRUCT(BlueprintType)
struct FShowHideTooltip : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FShowHideTooltip() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "showTooltip"))
		bool showTooltip = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setTooltipVisibilityValue", Tooltip = "Modify the inner visibility value"))
		bool setTooltipVisibilityValue = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "tooltipVisibilityValue", Tooltip = "If None then it will put the value of showTooltip to the inner visibility value", EditCondition = "setTooltipVisibilityValue"))
		ESetBoolValue tooltipVisibilityValue = ESetBoolValue::E_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setTooltipInteractValue"))
		bool setTooltipInteractValue = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "interactIfTooltipHidden", EditCondition = "setTooltipInteractValue"))
		bool interactIfTooltipHidden = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificTooltip"))
		EAction specificTooltip = EAction::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "tooltipAnimSpeed"))
		float tooltipAnimSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificStage"))
		int specificStage = -1;

};

USTRUCT(BlueprintType)
struct FChangePriority : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FChangePriority() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "priority"))
		EPriority priority = EPriority::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "doOtherInteracts"))
		bool doOtherInteracts = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stage"))
		int stage = 0;

};

USTRUCT(BlueprintType)
struct FPlaySoundFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FPlaySoundFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "sound"))
		USoundBase* sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "isLoop"))
		bool isLoop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "overrideValues"))
		bool overrideValues = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "atLocation"))
		bool atLocation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "attenuationType", EditCondition = "atLocation"))
		ESoundAttenuationFunction attenuationType = ESoundAttenuationFunction::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "falloutDistance", EditCondition = "atLocation"))
		float falloutDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor", EditCondition = "atLocation"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "attachToActor", EditCondition = "atLocation"))
		bool attachToActor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "attachedComponent", EditCondition = "attachToActor"))
		TSubclassOf<USceneComponent> attachedComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "attachedComponentTag", EditCondition = "attachToActor"))
		FName attachedComponentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "attachedPointName", EditCondition = "attachToActor"))
		FName attachedPointName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "offsetLocation", EditCondition = "atLocation"))
		FVector offsetLocation = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActorDirection", EditCondition = "atLocation"))
		EVectorDirection targetActorDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "distanceDirection", EditCondition = "atLocation"))
		float distanceDirection = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "volumeMultiplier", EditCondition = "overrideValues"))
		float volumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "pitchMultiplier", EditCondition = "overrideValues"))
		float pitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "startTime", EditCondition = "overrideValues"))
		float startTime = 0.0f;

	FTimerHandle soundTimer;
	FSavedStateInteract savedState;
	UAudioComponent* audioComponent = nullptr;
	bool tempOnce = false;

};

USTRUCT(BlueprintType)
struct FCheckIfSoundIsPlaying : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCheckIfSoundIsPlaying() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "sound"))
		USoundBase* sound;
};

USTRUCT(BlueprintType)
struct FStopSoundFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FStopSoundFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "sound"))
		USoundBase* sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "destroyAll"))
		bool destroyAll = false;
};

USTRUCT(BlueprintType)
struct FModifySoundFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FModifySoundFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "sound"))
		USoundBase* sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setVolumeMultiplier"))
		bool setVolumeMultiplier = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "volumeMultiplier", EditCondition = "setVolumeMultiplier"))
		float volumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setPitchMultiplier"))
		bool setPitchMultiplier = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "pitchMultiplier", EditCondition = "setPitchMultiplier"))
		float pitchMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "doOnAll"))
		bool doOnAll = false;

};

USTRUCT(BlueprintType)
struct FExecuteAction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FExecuteAction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActionButton actionButton;
};

USTRUCT(BlueprintType)
struct FExecuteStage {

	GENERATED_USTRUCT_BODY()
public:
	FExecuteStage() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stageName"))
		FString stageName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stageNumber"))
		int stageNumber = -4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "delay"))
		float delay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "cancelTimers"))
		bool cancelTimers = true;
};

USTRUCT(BlueprintType)
struct FExternalInteract : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FExternalInteract() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool jumpToStage = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "jumpToStage"))
		FExecuteStage goToStage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FActionButton actionButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool overlapping = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "overlapping"))
		bool doLikePlayer = false;
};

USTRUCT(BlueprintType)
struct FCheckIfActorExists : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FCheckIfActorExists() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

};

USTRUCT(BlueprintType)
struct FExecuteTrigger : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FExecuteTrigger() {};
	FExecuteTrigger(FFunctionActorData m_currentActor, FFunctionActorData m_actor, ETriggerSelect m_enterTrigger, ETriggerSelect m_enterHighlight, ETriggerSelect m_enterInside)
	{
		actor2 = m_currentActor;
		actor = m_actor;
		enterTrigger = m_enterTrigger;
		enterHighlight = m_enterHighlight;
		enterInside = m_enterInside;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ownerTriggerActor", Tooltip = "The actor who has the trigger"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "simulatedActor", Tooltip = "The actor that will be simulated that enter the trigger"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "enterTrigger"))
		ETriggerSelect enterTrigger = ETriggerSelect::Enter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "enterHighlight"))
		ETriggerSelect enterHighlight = ETriggerSelect::Ignore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "enterInside"))
		ETriggerSelect enterInside = ETriggerSelect::Enter;
};

USTRUCT(BlueprintType)
struct FCheckActorLocation : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FCheckActorLocation() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentActor"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentSpecificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentComponentTag"))
		FName componentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetComponentClass"))
		TSubclassOf<USceneComponent> targetComponentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetComponentTag"))
		FName targetComponentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "location"))
		FVector location = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "direction"))
		EVectorDirection direction = EVectorDirection::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "distanceDirection"))
		float distanceDirection = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "selectAxis"))
		ESpecificAxis selectAxis = ESpecificAxis::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "toleranceError"))
		float toleranceError = 1.f;
};

USTRUCT(BlueprintType)
struct FCheckActorRotation : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FCheckActorRotation() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentActor"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentSpecificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentComponentTag"))
		FName componentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetComponentClass"))
		TSubclassOf<USceneComponent> targetComponentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetComponentTag"))
		FName targetComponentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "location"))
		FRotator rotator = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "selectAxis"))
		ESpecificRotAxis selectRotAxis = ESpecificRotAxis::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "toleranceError"))
		float toleranceError = 1.0f;
};

USTRUCT(BlueprintType)
struct FCheckRaycast : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FCheckRaycast() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentSpecificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentComponentTag"))
		FName componentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActorClass"))
		TSubclassOf<AActor> targetActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActorTag"))
		FName targetActorTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetComponentClass"))
		TSubclassOf<USceneComponent> targetComponentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetComponentTag"))
		FName targetComponentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "direction"))
		EVectorDirection direction = EVectorDirection::Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "distance"))
		float distance = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "relativeInitOffset"))
		FVector relativeInitOffset = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "relativeFinalOffset"))
		FVector relativeFinalOffset = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "raycastType"))
		ERaycastType raycastType = ERaycastType::Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "raycastRadius"))
		float raycastRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "raycastLength"))
		float raycastLength = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "collisionChannel"))
		ECollisionRayCastType collisionChannel = ECollisionRayCastType::AllDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ignoreActors"))
		bool ignoreActors = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "selectedActors"))
		TArray<TSubclassOf<AActor>> selectedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ignoreComponents"))
		bool ignoreComponents = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "selectedComponents"))
		TArray<TSubclassOf<USceneComponent>> selectedComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "debug"))
		bool debug = false;
};

USTRUCT(BlueprintType)
struct FDestroyEntity : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FDestroyEntity() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
};

USTRUCT(BlueprintType)
struct FPlayAnimCharacter : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FPlayAnimCharacter() {}
	FPlayAnimCharacter(FFunctionActorData actor1, UAnimMontage* animation1, float playRate1, bool looping1) {
		actor = actor1;
		animation = animation1;
		playRate = playRate1;
		looping = looping1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "animation"))
		UAnimMontage* animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "playRate"))
		float playRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "looping"))
		bool looping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "notifyTime"))
		float notifyTime = -1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "notifyInfo"))
		FString notifyInfo = "";
	//Execution velocity of the rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timerSpeedRate = 1.0f;

	float deltaTimeAlt;
	float deltaTime = 0.01;
	FTimerHandle animationTimer;
	FSavedStateInteract savedState;
	FString characterBP = "";
	float duration = 0.f;
	float time = 0.f;
	bool notifyOnce = false;
	bool doOnce = false;
	TArray<FAnimNotifyEvent> notifyEvents;
};

USTRUCT(BlueprintType)
struct FCurveDataFunction {

	GENERATED_USTRUCT_BODY()
public:
	FCurveDataFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "curve"))
		UCurveFloat* curve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "updateTime"))
		float updateTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "updateAmount"))
		float updateAmount = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "curveCoordinates"))
		ECurveCoordinates curveCoordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "negateFirstCoordinate"))
		bool negateFirstCoordinate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "negateSecondCoordinate"))
		bool negateSecondCoordinate = false;

};

USTRUCT(BlueprintType)
struct FRelocateFunction : public FInteractStruct {
	GENERATED_USTRUCT_BODY()
public:
	FRelocateFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;
	//The blueprint with the interact must have and arrow with tag, the actor will go to that specific arrow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "tag"))
		FName arrowTag;
	//Margin error of the location for debug 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "errorMarginLocation"))
		float errorMarginLocation = 1.0f;
	//Interact will go to the next stage when the relocate is done, if -1 wont jump anywhere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "nextStageWhenFinish"))
		int nextStageRelocateDone = -1;
	//Make the actor move besides there is obstacles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "forceInput"))
		bool forceMovement = true;
	//This will block player movement and camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "blockWhenFinish"))
		bool blockCameraAndMovement = false;
	//This must be true if we want our actor to start rotating once the relocate is done
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "rotateWhenFinish"))
		bool rotateWhenFinish = false;
	//Margin error of the rotation for debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "errorMarginRotation"))
		float errorMarginRotation = 2.0f;
	//The velocity the actor will rotate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "rotationSpeed"))
		float rotationSpeed = 5.0f;
	//Our event tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "deltaTime"))
		float DeltaTime = 0.001f;

	//Execution velocity of the rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timerSpeedRate = 1.0f;

	float deltaTimeAlt;
	bool isRotating = false;
	FSavedStateInteract savedState;
	FTimerHandle relocateFunctionTimer;
};

USTRUCT(BlueprintType)
struct FSetRagdoll : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetRagdoll() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Target Actor", Tooltip = "This is the actor which will do the ragdoll"))
		FFunctionActorData actor;

	AActor* m_actor = nullptr;
};

USTRUCT(BlueprintType)
struct FAddImpulseFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FAddImpulseFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Impulse Type"))
		EImpulseType ImpulseType = EImpulseType::AddImpulse;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Base Directions", Tooltip = "If true, the impulse direction will be a generic direction, else the impulse direction will be the forward vector of the actor who makes the impulse"))
		bool BaseDirections = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "BaseDirections", DisplayName = "Direction"))
		EVectorDirection Direction = EVectorDirection::Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Distance Direction", ClampMin = "20000.0", ClampMax = "20000000.0"))
		float DistanceDirection = 50000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Target Actor", Tooltip = "This is the target actor which will recieve the impulse"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Impulsed Bone", Tooltip = "Choose the bone which will recieve the force of the hit"))
		EBone_Socket ImpulsedBone = EBone_Socket::PELVIS;

	AActor* m_actor = nullptr;
};

USTRUCT(BlueprintType)
struct FCustomCollisionPreset : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCustomCollisionPreset() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Collision Enabled", Tooltip = "Type of Collision enabled"))
		ECollisionEnabledType CollisionEnabled = ECollisionEnabledType::NOCOLLISION;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Object Type", Tooltip = "Enum indicating what type of object this should be considered as when it moves"))
		ECollisionObjectType ObjectType = ECollisionObjectType::DESTRUCTIBLE;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "All Channels Response To", Tooltip = "If true, Collision Responses will determine which type of collision will have every collision channel"))
		bool AllChannelsResponseTo = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "AllChannelsResponseTo", DisplayName = "Collision Responses", Tooltip = "Collision Responses will determine which type of collision will have every collision channel"))
		ECollisionType CollisionResponses = ECollisionType::OVERLAP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "Visibility"))
		ECollisionType Visibility = ECollisionType::OVERLAP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "Camera"))
		ECollisionType Camera = ECollisionType::OVERLAP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "WorldStatic"))
		ECollisionType WorldStatic = ECollisionType::OVERLAP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "WorldDynamic"))
		ECollisionType WorldDynamic = ECollisionType::OVERLAP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "Pawn"))
		ECollisionType Pawn = ECollisionType::OVERLAP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "PhysicsBody"))
		ECollisionType PhysicsBody = ECollisionType::OVERLAP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "Vehicle"))
		ECollisionType Vehicle = ECollisionType::OVERLAP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!AllChannelsResponseTo", DisplayName = "Destructible"))
		ECollisionType Destructible = ECollisionType::OVERLAP;
};

USTRUCT(BlueprintType)
struct FMeshCollisionSettings : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FMeshCollisionSettings() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Simulation Generates Hit Events", Tooltip = "Should 'Hit' events fire when this object collides during physics simulation"))
		bool SimGenHitEvents = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Generate Overlap Events"))
		bool GenOverlapEvents = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Can Character Step Up On", Tooltip = "Determine whether a character can step up onto this component. This controls whether they can try to step up on it when they bump in to it, not whether they can walk on it after landing on it"))
		ECanCharacterStepUpOn CanCharacterStepUpOn = ECanCharacterStepUpOn::YES;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Custom Preset", Tooltip = "If true, you can set custom collision settings"))
		bool CustomPreset = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!CustomPreset", DisplayName = "Collision Presets", Tooltip = "You can set collision preset settings"))
		ECollisionPresets CollisionPresets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "CustomPreset", DisplayName = "Custom Collision Preset"))
		FCustomCollisionPreset CustomCollisionPreset;
};

USTRUCT(BlueprintType)
struct FShapeCollisionSettings : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FShapeCollisionSettings() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Simulation Generates Hit Events", Tooltip = "Should 'Hit' events fire when this object collides during physics simulation"))
		bool SimGenHitEvents = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Generate Overlap Events"))
		bool GenOverlapEvents = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Can Character Step Up On", Tooltip = "Determine whether a character can step up onto this component. This controls whether they can try to step up on it when they bump in to it, not whether they can walk on it after landing on it"))
		ECanCharacterStepUpOn CanCharacterStepUpOn = ECanCharacterStepUpOn::YES;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Custom Preset", Tooltip = "If true, you can set custom collision settings"))
		bool CustomPreset = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "!CustomPreset", DisplayName = "Collision Presets", Tooltip = "You can set collision preset settings"))
		ECollisionPresets CollisionPresets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "CustomPreset", DisplayName = "Custom Collision Preset"))
		FCustomCollisionPreset CustomCollisionPreset;
};

USTRUCT(BlueprintType)
struct FMeshPhysicsSettings : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FMeshPhysicsSettings() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Simulate Physics", Tooltip = "If true, mesh will simulate physics"))
		bool SimulatePhysics = true;
};

USTRUCT(BlueprintType)
struct FShapePhysicsSettings : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FShapePhysicsSettings() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Simulate Physics", Tooltip = "If true, shape will simulate physics"))
		bool SimulatePhysics = true;
};

USTRUCT(BlueprintType)
struct FSetCollisionSettings : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetCollisionSettings() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Target Actor", Tooltip = "This is the target actor which collisions will be changed"))
		FFunctionActorData actor;
	AActor* m_actor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Change Mesh Collision", Tooltip = "If true, you can change the mesh collision settings"))
		bool ChangeMeshCollision = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Change Other Collision", Tooltip = "If true, you can change the shape component collision settings (capsule component for example)"))
		bool ChangeShapeCollision = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ChangeMeshCollision", DisplayName = "Mesh Collision", Tooltip = "The mesh collision settings"))
		FMeshCollisionSettings MeshCollisionSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ChangeShapeCollision", DisplayName = "Shape Collision", Tooltip = "The shape collision settings (capsule component for example)"))
		FShapeCollisionSettings ShapeCollisionSettings;
};

USTRUCT(BlueprintType)
struct FSlowTimeFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSlowTimeFunction() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Target Actor", Tooltip = "This is the target actor which time dilation will be changed"))
		FFunctionActorData actor;
	AActor* m_actor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Change World Time", Tooltip = "If true, you can set the world time dilation"))
		bool ChangeWorldTime = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ChangeWorldTime", DisplayName = "World Time Dilation", Tooltip = "World Time Dilation value", ClampMin = "0.001", ClampMax = "50"))
		float WorldTimeDilation = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Change Actor Time", Tooltip = "If true, you can change the other component collision settings (capsule component for example)"))
		bool ChangeCharacterTime = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ChangeCharacterTime", DisplayName = "Actor Time Dilation", Tooltip = "Actor Time Dilation value", ClampMin = "0.001", ClampMax = "50"))
		float CharacterTimeDilation = 1.0f;
};

USTRUCT(BlueprintType)
struct FAttachToObject : public FInteractStruct
{
	GENERATED_USTRUCT_BODY()
public:

	FAttachToObject() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Do On Actor", Tooltip = "If true this function will attach or detach the target actor, else attach or detach the target component"))
		bool DoOnActor = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Attach", Tooltip = "If true this function will attach the target actor to the SocketToAttach, else will detach it"))
		bool Attach = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Target Actor", Tooltip = "This is the attachment actor"))
		FFunctionActorData actor2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Attached Actor", Tooltip = "This is the target actor which will be attached to the character"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!DoOnActor", DisplayName = "Target Component", Tooltip = "This is the target component in which will be added a tag"))
		TSubclassOf<USceneComponent> specificChild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!DoOnActor"))
		FName componentTag = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Attach", DisplayName = "Custom Socket", Tooltip = "If true you can choose a new custom socket that it isn´t added to the preset socket list, else you must choose a preset socket of the list"))
		bool CustomSocket = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "CustomSocket", DisplayName = "Custom Socket To Attach", Tooltip = "Here you must write the name of the socket where the target actor will be attached"))
		FName CustomSocketToAttach = FName(TEXT("None"));
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!CustomSocket", DisplayName = "Socket To Attach", Tooltip = "Choose the socket which will be the attachment of the Target Actor"))
		EBone_Socket SocketToAttach = EBone_Socket::L_HAND;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Attach", DisplayName = "Attach Location Rule", Tooltip = "How to handle translation when attaching"))
		EAttachmentRule AttachLocationRule = EAttachmentRule::SnapToTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Attach", DisplayName = "Attach Rotation Rule", Tooltip = "How to handle rotation when attaching"))
		EAttachmentRule AttachRotationRule = EAttachmentRule::SnapToTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Attach", DisplayName = "Attach Scale Rule", Tooltip = "How to handle scale when attaching"))
		EAttachmentRule AttachScaleRule = EAttachmentRule::SnapToTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "Attach", DisplayName = "Weld Simulated Bodies", Tooltip = "Wether to weld together simulated physisc bodies"))
		bool WeldSimulatedBodies = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!Attach", DisplayName = "Detach Location Rule", Tooltip = "How to handle translation when detaching"))
		EDetachmentRule DetachLocationRule = EDetachmentRule::KeepRelative;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!Attach", DisplayName = "Detach Rotation Rule", Tooltip = "How to handle rotation when detaching"))
		EDetachmentRule DetachRotationRule = EDetachmentRule::KeepRelative;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!Attach", DisplayName = "Detach Scale Rule", Tooltip = "How to handle scale when detaching"))
		EDetachmentRule DetachScaleRule = EDetachmentRule::KeepRelative;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!Attach", DisplayName = "Call Modify", Tooltip = "Whether to call Modify on the components concerned when detaching"))
		bool CallModify = true;

	AActor* m_actor = nullptr;
	AActor* m_currentActor = nullptr;
	USceneComponent* m_component;
};

USTRUCT(BlueprintType)
struct FSetPhysicsSettings : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetPhysicsSettings() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Target Actor", Tooltip = "This is the target actor which collisions will be changed"))
		FFunctionActorData actor;
	AActor* m_actor = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Change Mesh Physics", Tooltip = "If true, you can change the mesh physics settings"))
		bool ChangeMeshPhysics = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Change Other Physics", Tooltip = "If true, you can change the shape component physics settings (capsule component for example)"))
		bool ChangeShapePhysics = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ChangeMeshPhysics", DisplayName = "Mesh Physics", Tooltip = "The mesh physics settings"))
		FMeshPhysicsSettings MeshPhysicsSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "ChangeShapePhysics", DisplayName = "Shape Physics", Tooltip = "The shape physics settings (capsule component for example)"))
		FShapePhysicsSettings ShapePhysicsSettings;
};

USTRUCT(BlueprintType)
struct FAddTagFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FAddTagFunction() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Target Actor", Tooltip = "This is the target actor in which will be added a tag"))
		FFunctionActorData actor;
	AActor* m_actor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Target Component", Tooltip = "This is the target component in which will be added a tag"))
		TSubclassOf<USceneComponent> specificChild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName componentTag = NAME_None;
	USceneComponent* m_component;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Actor Tags", Tooltip = "These are the tag that will be added to the 'Target Actor'"))
		TArray<FName> m_actorTag;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Component Tags", Tooltip = "These are the tag that will be added to the 'Target Component'"))
		TArray<FName> m_componentTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Target Component", Tooltip = "If is checked then adds tag, if not removes tag"))
		bool addTag = true;

};

USTRUCT(BlueprintType)
struct FPlaySequence : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FPlaySequence() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Sequence", Tooltip = "This is the sequence that will play"))
		ULevelSequence* levelSequence;

};

USTRUCT(BlueprintType)
struct FSpawnerFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSpawnerFunction() {}

	//ACTOR
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* singleSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> spawnedActorTags;

	//SPAWN LOCATION
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpawnAschild"))
		bool spawnAsChild = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TargetActor"))
		FFunctionActorData actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpecificChild"))
		TSubclassOf<USceneComponent> specificChild;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName componentTag = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector position = { 0.f,0.f,0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator rotation = { 0.f,0.f,0.f };
	AActor* spawned;

};

USTRUCT(BlueprintType)
struct FSetTransformActor : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetTransformActor() {}
	FSetTransformActor(FFunctionActorData actor1, TSubclassOf<USceneComponent> specificChild1, FName componentTag1,
		bool setRelative1, FCurveDataFunction curve1, bool setLocation1, FVector location1, bool setRotation1,
		FRotator rotation1, bool setScale1, FVector scale1, float minTimeVal1 = NaN, float maxTimeVal1 = NaN, float acumTime1 = NaN,
		AActor* actorDebug1 = nullptr)
	{
		actor = actor1;
		specificChild = specificChild1;
		componentTag = componentTag1;
		setRelative = setRelative1;
		curve = curve1;
		setLocation = setLocation1;
		location = location1;
		rotation = rotation1;
		setRotation = setRotation1;
		setScale = setScale1;
		scale = scale1;
		minTimeVal = minTimeVal1;
		maxTimeVal = maxTimeVal1;
		acumTime = acumTime1;
		actorDebug = actorDebug1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	//If this is active, then only this component will move (not actor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName componentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setRelative"))
		bool setRelative = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "setRelative", DisplayName = "curve"))
		FCurveDataFunction curve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setLocation"))
		bool setLocation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "setLocation", DisplayName = "location"))
		FVector location = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setRotation"))
		bool setRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "setRotation", DisplayName = "rotation"))
		FRotator rotation = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "setScale"))
		bool setScale = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "setScale", DisplayName = "scale"))
		FVector scale = { 1.f, 1.f, 1.f };

	float minTimeVal = NaN;

	float maxTimeVal = NaN;

	float acumTime = NaN;

	AActor* actorDebug = nullptr;
};

USTRUCT(BlueprintType)
struct FMoveToFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FMoveToFunction()
	{}

	FMoveToFunction(FFunctionActorData currentActor1, FFunctionActorData actor1, TSubclassOf<USceneComponent> specificChild1,
		FName componentTag1, FCurveDataFunction curve1, FVector location1, EVectorDirection direction1, float distanceDirection1,
		float tickTime1, float speedAmount1, float toleranceError1, bool movableTarget1,
		FRotator targetV1, AActor* actorDebug1, FTimerHandle moveHandle1, FSavedStateInteract savedState1) {
		actor2 = currentActor1;
		actor = actor1;
		specificChild = specificChild1;
		componentTag = componentTag1;
		curve = curve1;
		location = location1;
		direction = direction1;
		distanceDirection = distanceDirection1;
		tickTime = tickTime1;
		speedAmount = speedAmount1;
		toleranceError = toleranceError1;
		movableTarget = movableTarget1;
		targetV = targetV1;
		actorDebug = actorDebug1;
		moveHandle = moveHandle1;
		savedState = savedState1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "forceMove"))
		bool forceMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentActor"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	//If this is active, then only this component will move (not actor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName componentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "curve"))
		FCurveDataFunction curve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "setLocation", DisplayName = "location"))
		FVector location = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "direction"))
		EVectorDirection direction = EVectorDirection::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "distanceDirection"))
		float distanceDirection = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "tickTime"))
		float tickTime = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "speedAmount"))
		float speedAmount = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "toleranceError"))
		float toleranceError = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "movableTarget"))
		bool movableTarget = true;

	FRotator targetV = { 0.f, 0.f, 0.f };

	AActor* actorDebug = nullptr;

	FTimerHandle moveHandle;

	FSavedStateInteract savedState;
};

USTRUCT(BlueprintType)
struct FRotateToFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FRotateToFunction() {}

	FRotateToFunction(FFunctionActorData currentActor1, FFunctionActorData actor1, TSubclassOf<USceneComponent> specificChild1,
		FName componentTag1, FRotator rotation1, float tickTime1, float speedAmount1, float toleranceError1, bool movableTarget1,
		FVector targetV1, AActor* actorDebug1, FTimerHandle rotateHandle1, FSavedStateInteract savedState1) {
		actor2 = currentActor1;
		actor = actor1;
		specificChild = specificChild1;
		componentTag = componentTag1;
		rotation = rotation1;
		tickTime = tickTime1;
		speedAmount = speedAmount1;
		toleranceError = toleranceError1;
		movableTarget = movableTarget1;
		targetV = targetV1;
		actorDebug = actorDebug1;
		rotateHandle = rotateHandle1;
		savedState = savedState1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "forceRotate"))
		bool forceRotate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentActor"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	//If this is active, then only this component will move (not actor)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName componentTag = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "setRotation", DisplayName = "rotation"))
		FRotator rotation = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "tickTime"))
		float tickTime = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "speedAmount"))
		float speedAmount = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "toleranceError"))
		float toleranceError = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "movableTarget"))
		bool movableTarget = true;

	//Velocity of the rotation speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timerSpeedRate = 1.0f;

	float deltaTimeAlt;
	FVector targetV = { 0.f, 0.f, 0.f };

	AActor* actorDebug = nullptr;

	FTimerHandle rotateHandle;

	FSavedStateInteract savedState;
};

USTRUCT(BlueprintType)
struct FTeleportTo : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FTeleportTo() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentActor"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "location"))
		FVector location = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActorDirection"))
		EVectorDirection targetActorDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "distanceDirection"))
		float distanceDirection = 0.0f;
};

USTRUCT(BlueprintType)
struct FRotateLookAtActor : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FRotateLookAtActor() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "currentActor"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "componentTag"))
		FName componentTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "rotateOnlyOn"))
		ESpecificRotAxis rotateOnlyOn = ESpecificRotAxis::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "rotation"))
		FRotator rotation = { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActorDirection"))
		EVectorDirection targetActorDirection;

};

USTRUCT(BlueprintType)
struct FSetVisibilityActor : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FSetVisibilityActor() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Visible"))
		bool visible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "doOnActor"))
		bool doOnActor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "doOnActor", DisplayName = "PropagateChildren"))
		bool propagateChildren = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificChild"))
		TSubclassOf<USceneComponent> specificChild;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName componentTag;
};

USTRUCT(BlueprintType)
struct FDelayFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FDelayFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DelayAmount"))
		FString delayAmount = "0.1";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "isAffectedBySlowTime"))
		bool isAffectedBySlowTime = false;

	TArray<FDelegate> functions;

	FTimerHandle delayHandle;

	bool ready = false;

};

////////////////////////////////
// CORE STRUCTS ////////////////
////////////////////////////////

USTRUCT(BlueprintType)
struct FFunctionDataBaseCore
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionDataBaseCore() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionListCore enumNameCore = EActionListCore::None;

	//AQUI SE AÑADIRAN LAS FUNCIONES CORES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "debugTest")
		FDebugTest debugTest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "destroyEntity")
		FDestroyEntity destroyEntity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "playSequence")
		FPlaySequence playSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "executeAction")
		FExecuteAction executeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "jumpStage")
		FJumpStage jumpStage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "debugText")
		FDebugText debugText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "externalInteract")
		FExternalInteract externalInteract;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "jumpFunction")
		FJumpFunction jumpFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cancelJumpStage")
		FCancelJumpStage cancelJumpStage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "breakFunction")
		FBreakFunction breakFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "teleportTo")
		FTeleportTo teleportTo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "rotateLookAtActor")
		FRotateLookAtActor rotateLookAtActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "playAnimCharacter")
		FPlayAnimCharacter playAnimCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setVisibilityActor")
		FSetVisibilityActor setVisibilityActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setTransformActor")
		FSetTransformActor setTransformActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "moveToFunction")
		FMoveToFunction moveToFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "rotateToFunction")
		FRotateToFunction rotateToFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "createEditVariable")
		FCreateEditVariable createEditVariable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "spawnerFunction")
		FSpawnerFunction spawnerFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setRagdoll")
		FSetRagdoll setRagdoll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "showHideTooltip")
		FShowHideTooltip showHideTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "addImpulseFunction")
		FAddImpulseFunction addImpulseFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setCollisionSettings")
		FSetCollisionSettings setCollisionSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "slowTimeFunction")
		FSlowTimeFunction slowTimeFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "attachToObject")
		FAttachToObject attachToObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "playSoundFunction")
		FPlaySoundFunction playSoundFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stopSoundFunction")
		FStopSoundFunction stopSoundFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "modifySoundFunction")
		FModifySoundFunction modifySoundFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cameraEffect")
		FCameraEffect cameraEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "openLevelFunction")
		FOpenLevelFunction openLevelFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "changePriority")
		FChangePriority changePriority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "executeTrigger")
		FExecuteTrigger executeTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "addWidgetToViewport")
		FAddWidgetToViewport addWidgetToViewport;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "removeWidgetFunction")
		FRemoveWidgetFunction removeWidgetFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setPhysicsSettings")
		FSetPhysicsSettings setPhysicsSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "addTagFunction")
		FAddTagFunction addTagFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setMaterialMesh")
		FSetMaterialMesh setMaterialMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "getExternalVariable")
		FGetExternalVariable getExternalVariable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setTooltipLocationSize")
		FSetTooltipLocationSize setTooltipLocationSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "relocateFunction")
		FRelocateFunction relocateFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "playWidgetAnimation")
		FPlayWidgetAnimation playWidgetAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "showHideOutlineShine")
		FShowHideOutlineShine showHideOutlineShine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "getInternalVariable")
		FGetInternalVariable getInternalVariable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "execCommand")
		FExecCommand execCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setInternalVariable")
		FSetInternalVariable setInternalVariable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "executeInternalFunction")
		FExecuteInternalFunction executeInternalFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "loadStreamLevelInteract")
		FLoadStreamLevelInteract loadStreamLevelInteract;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "unloadStreamLevelInteract")
		FUnloadStreamLevelInteract unloadStreamLevelInteract;
};

USTRUCT(BlueprintType)
struct FFunctionCheckDataBaseCore
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionCheckDataBaseCore() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECheckListCore enumNameCore = ECheckListCore::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "negate"))
		bool negate = false;

	//AQUI SE AÑADIRAN LAS FUNCIONES CHECK CORES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "debugCheckTest")
		FDebugCheckTest debugCheckTest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "debugCondition")
		FDebugCondition debugCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "decrementIncrement")
		FDecrementIncrement decrementIncrement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "checkActorLocation")
		FCheckActorLocation checkActorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "checkActorRotation")
		FCheckActorRotation checkActorRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "checkVariableValue")
		FCheckVariableValue checkVariableValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "checkRaycast")
		FCheckRaycast checkRaycast;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "checkTooltipVisibility")
		FCheckTooltipVisibility checkTooltipVisibility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "checkIfActorExists")
		FCheckIfActorExists checkIfActorExists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "checkIfSoundIsPlaying")
		FCheckIfSoundIsPlaying checkIfSoundIsPlaying;
};


//BIND ACTIONS BY STRING
#define BINDACTION(...) \
const char * a[] = { __VA_ARGS__ }; \
int size = (sizeof(a) / sizeof(*a)); \
for (int i = 0; i < size; i++) { \
	BindAction(a[i]); \
}

//BIND ACTIONS BY STRING
#define BINDCHECK(...) \
const char * b[] = { __VA_ARGS__ }; \
int sizeb = (sizeof(b) / sizeof(*b)); \
for (int i = 0; i < sizeb; i++) { \
	BindCheck(b[i]); \
}

//BIND ACTIONS BY STRING
#define BINDACTOR(...) \
const char * c[] = { __VA_ARGS__ }; \
int sizec = (sizeof(c) / sizeof(*c)); \
for (int i = 0; i < sizec; i++) { \
	BindActor(c[i]); \
}

//BIND ENUM USED
#define BINDENUM(...) \
const char * d[] = { __VA_ARGS__ }; \
int sized = (sizeof(d) / sizeof(*d)); \
if(enumFunctions.Num() <= 0){ \
	for (int i = 0; i < sized; i++) { \
		enumFunctions.Add(d[i]); \
	} \
}

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDACTIONSCORE "DestroyEntity", "IfFunction" , "ExecuteAction", "JumpStage", "DebugText", "ExternalInteract", \
"JumpFunction", "CancelJumpStage", "BreakFunction", "DelayFunction", "CancelTimer", "CreateEditVariable", "OpenLevelFunction", \
"ChangePriority","ExecuteTrigger","GetExternalVariable","GetInternalVariable","ExecCommand","SetInternalVariable","ExecuteInternalFunction",\
"WhenFunction","CallExternalDelegateFunction","LoadStreamLevelInteract","UnloadStreamLevelInteract","RotateLookAtActor", \
"RotateToFunction", "TeleportTo", "SetTransformActor","MoveToFunction", "SetVisibilityActor", "SpawnerFunction", "SlowTimeFunction", \
"SetCollisionSettings","SetPhysicsSettings", "AddTagFunction", "SetMaterialMesh","ShowHideOutlineShine", "AttachToObject", \
"AddImpulseFunction", "SetRagdoll","RelocateFunction","CameraEffect","PlaySoundFunction","StopSoundFunction","ModifySoundFunction",\
"AddWidgetToViewport","RemoveWidgetFunction","ShowHideTooltip","SetTooltipLocationSize","PlayWidgetAnimation","PlayAnimCharacter", "PlaySequence"

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDCHECKSCORE "DebugCondition", "DecrementIncrement", "AndFunction", "CheckActorLocation", "CheckActorRotation","CheckVariableValue",  "CheckRaycast", "CheckTooltipVisibility", "CheckIfActorExists", "CheckIfSoundIsPlaying"

//ADD BIND ACTORS
#define BINDACTORS "GetCustomActor", "GetSpecificActor", "GetThisActor"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISM_API UInteractCoreClass : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInteractCoreClass();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<FDelegate> interactFunctionMap;
	TArray<FDelegateBool> checkFunctionMap;
	TArray<FDelegateActor> actorFunctionMap;

	void BindAction(const FName& functionName);
	void BindCheck(const FName& functionName);
	void BindActor(const FName& functionName);

};
