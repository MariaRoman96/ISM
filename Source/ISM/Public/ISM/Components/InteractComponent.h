#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include <vector>
#include <functional>
#include <cmath>
#include <DelegateCombinations.h>
#include <limits>
#include <Engine/Engine.h>
//IF ISN'T .H INCLUDED YET
#include "TooltipWidget.h"
#include <LevelSequencePlayer.h>
#include <LevelSequenceActor.h>
#include <Engine/DataTable.h>
#include <Sound/SoundBase.h>
#include <UserWidget.h>
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "LevelManager.h"
#include "InteractFunctionsClass.h"
#include "InteractWidgetComponent.h"
#include "InteractComponent.generated.h"

//#define STRUCTLISTNAME(EnumName, StructClass) \
//	UPROPERTY(EditAnywhere, Category = EnumName) \
//	StructClass EnumName;

using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(InteractLog, All, All)

// #define BIND_DELEGATEFINAL \
// BIND_DELEGATESCORE \
// BIND_DELEGATES

USTRUCT(BlueprintType)
struct FTooltipData {

	GENERATED_USTRUCT_BODY()
public:

	FTooltipData() {
		m_WidgetComponent = nullptr;
		widgetLocation = FVector(0.0f, 0.0f, 0.0f);
		m_nextStage = "0";
		m_nextFailStage = "0";
		m_timePressing = "0";
		m_nextStageDelay = "0";
		actionButton = EAction::None;
		m_holdingDown = false;
		m_hideTooltip = true;
		m_hideOutline = false;
		m_showTooltip = true;
		m_icon = nullptr;
		m_button = nullptr;
		m_circle = nullptr;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FFunctionData> stageFunction;

	UPROPERTY()
		UInteractWidgetComponent* m_WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector widgetLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector widgetSize = FVector(1.0f, 1.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool widgetInScreen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString m_nextStage = "0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString m_nextFailStage = "0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString m_timePressing = "0.1";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString m_updateTimePressing = "0.05";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString m_nextStageDelay = "0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Action button"))
		EAction actionButton = EAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Holding Down", ToolTip = "Indicate if action will be holding down"))
		bool m_holdingDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Hide tooltip", ToolTip = "Hide Tooltip when button is press"))
		bool m_hideTooltip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Hide outline", ToolTip = "Hide Outline when button is press"))
		bool m_hideOutline = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Show tooltip", ToolTip = "Adds tooltip popup to show information"))
		bool m_showTooltip = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Interact If Tooltip Is Hidden", ToolTip = "Interact with Action if the tooltip is hidden"))
		bool m_interactIfTooltipIsHidden = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (EditCondition = "m_showTooltip", DisplayName = "Tooltip Anim Speed", ToolTip = "Animation speed of Tooltip"))
		FString m_tooltipAnimSpeed = "1.0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Quick Time Event", ToolTip = "Indicate if action will be QTE"))
		bool m_quickTimeEvent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (EditCondition = "m_quickTimeEvent", DisplayName = "Delay Quick Event Time", ToolTip = "Animation speed of Quick Time Event"))
		FString m_delayQuickEventTime = "2.0";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (EditCondition = "m_quickTimeEvent", DisplayName = "Quick Event Time", ToolTip = "Time when pressing the button will be success"))
		FString m_quickEventTime = "0.5";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Tooltip icon"))
		UTexture2D *m_icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Tooltip button image"))
		UTexture2D *m_button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact|Tooltip", meta = (DisplayName = "Tooltip background image"))
		UTexture2D *m_circle;

};

USTRUCT(BlueprintType)
struct FCapsuleStruct {

	GENERATED_USTRUCT_BODY()

public:
	FCapsuleStruct() {}

	//CAPSULE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capsule", meta = (DisplayName = "Capsule"))
		FVector2D m_CapsuleSize = FVector2D(100.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_CapsuleLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_CapsuleRotation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_CapsuleScale = FVector(1.0f, 1.0f, 1.0f);
};

USTRUCT(BlueprintType)
struct FCapsuleRequired : public FCapsuleStruct
{

	GENERATED_USTRUCT_BODY()

public:
	FCapsuleRequired() {}

	//CAPSULE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Capsule", meta = (DisplayName = "Capsule color"))
		EOutlineColor m_CapsuleColor = EOutlineColor::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsShining"))
		bool m_isShining = false;
};

USTRUCT(BlueprintType)
struct FCapsuleHighlight : public FCapsuleRequired
{

	GENERATED_USTRUCT_BODY()

public:
	FCapsuleHighlight() {}

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ShiningIntroSpeed"))
// 		float m_shiningIntroSpeed = 1.f;
};

USTRUCT(BlueprintType)
struct FCapsuleTrigger : public FCapsuleStruct
{

	GENERATED_USTRUCT_BODY()

public:
	FCapsuleTrigger() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FFunctionData> initFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FFunctionData> exitFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* actorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName tag;
};

USTRUCT(BlueprintType)
struct FInteractData {

	GENERATED_USTRUCT_BODY()

public:
	FInteractData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString stageTitle = "Stage";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString stageDescription = "There is no description yet";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPriority priorityStage = EPriority::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool doOtherInteract = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FFunctionData> preStageFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCapsuleRequired capsuleInteractStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCapsuleHighlight capsuleHighlightStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCapsuleTrigger capsuleTriggerStruct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTooltipData> actions;
};

USTRUCT(BlueprintType)
struct FDelegateInteractData {

	GENERATED_USTRUCT_BODY()

public:
	FDelegateInteractData() {}

	FDelegateInteractData(EDelegateList delegateName1, EDelegateListCore delegateNameCore1, FString infoData1, int stageIndex1, int functionIndex1, EActionStageType stageType1,
		EAction actionButton1, EActionList specificFunction1, EActionListCore specificFunctionCore1, int stageFunction1, bool isPermanent1, int currentStageIndex1,
		int currentFunctionIndex1, EActionStageType currentStageType1, EAction currentActionButton1, int startFunctionIndex1, int endFunctionIndex1, EActionStageType stageTypeFunction1,
		EAction buttonTypeFunction1, FString interactName1, UClass* interactCompClass1, FString interactCompActorName1, FString currentInteractName1)
	{
		delegateName = delegateName1;
		delegateNameCore = delegateNameCore1;
		infoData = infoData1;
		stageIndex = stageIndex1;
		functionIndex = functionIndex1;
		stageType = stageType1;
		actionButton = actionButton1;
		specificFunction = specificFunction1;
		specificFunctionCore = specificFunctionCore1;
		stageFunction = stageFunction1;
		isPermanent = isPermanent1;
		currentStageIndex = currentStageIndex1;
		currentFunctionIndex = currentFunctionIndex1;
		currentStageType = currentStageType1;
		currentActionButton = currentActionButton1;
		startFunctionIndex = startFunctionIndex1;
		endFunctionIndex = endFunctionIndex1;
		stageTypeFunction = stageTypeFunction1;
		buttonTypeFunction = buttonTypeFunction1;
		interactName = interactName1;
		interactCompClass = interactCompClass1;
		interactCompActorName = interactCompActorName1;
		currentInteractName = currentInteractName1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDelegateList delegateName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDelegateListCore delegateNameCore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString infoData = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int stageIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int functionIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionStageType stageType = EActionStageType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAction actionButton = EAction::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionList specificFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionListCore specificFunctionCore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int stageFunction = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isPermanent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int currentStageIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int currentFunctionIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionStageType currentStageType = EActionStageType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAction currentActionButton = EAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int startFunctionIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int endFunctionIndex = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionStageType stageTypeFunction = EActionStageType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAction buttonTypeFunction = EAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString interactName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* interactCompClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString interactCompActorName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString currentInteractName = "";
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ISM_API UInteractComponent : public UInteractFunctionsClass
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UInteractComponent();

	UPROPERTY()
		TArray<UInteractWidgetComponent*> widgetComps;

	UPROPERTY()
		UTexture2D* circleTexture;

	//_____Variables de Luis________
	//Variables Object Inspector
	FVector initPosGlobal = { 0,0,0 };
	FRotator initRotGlobal = { 0,0,0 };
	bool doOnceInspector = false;
	UPROPERTY(BlueprintReadWrite)
		bool activateLigths;
	//Variables de draw line trace
	bool stopPlaneLineTraceTimer;

	//Timer Variables
	TArray<FString> m_variablesArray;
	TArray<FString> m_registerTimerArray;

	TArray<UObject*> m_variablesObject;

	TArray<FString> m_sharedVariablesArray;

	TArray<FString> m_actorOutsideEnterNames;
	TArray<FString> m_actorOutsideExitNames;

	TArray<FString> m_actorInsideEnterNames;
	TArray<FString> m_actorInsideExitNames;

	TArray<FString> m_actorTriggerEnterNames;
	TArray<FString> m_actorTriggerExitNames;


	TArray<UObject*> m_sharedFunctionObjects;

	static TArray<FString> functionType;
	static TArray<FString> propertyBaseFunctionName;
	static TArray<FString> propertyFunctionName;
	static TArray<FString> propertyWrapperFunctionName;
	static TArray<FString> checkFunctionName;
	static TArray<FString> checkFunctionNameAnd;
	static TArray<FString> actorFunctionName;

	static TArray<FDelegateInteractData> delegateAuxFunctions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "InitStage"))
		FString m_initStage = "0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CurrentStage (Only for Edit)"))
		int m_currentStage = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ticks Time"))
		float m_ticksTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Ticks Num"))
		int m_ticksNum = 12;

	EAction tempAction;
	EActionStageType tempType;
	int functionIndex = 0;
	AActor* tempActor = nullptr;

	bool readyForNextStage = false;
	int tempTooltipIndex = 0;
	int tempTooltip = -2;
	float tempDelay = -1;
	bool tempSameState = false;
	bool stopStage = false;
	float tempMax = NaN;
	bool tempHoldingButton = false;
	bool tempDelayFunction = false;
	bool tempDoOnceDelay = false;
	bool tempInteractAvailable = false;
	bool tempInElse = false;

	bool tempJumpStage = false;
	bool tempJumpFunction = false;

	int tempDelayWrapper = 0;
	int ticksNum = 0;
	FTimerHandle ticksHandle;

	UPROPERTY()
		bool tooltipLoaded = false;

	UPROPERTY()
		bool tooltipLoadedViewport = false;

	TArray<EAction> pressingIdFunctions;
	TArray<float> pressingTimeFunctions;
	TArray<FDelegate> pressingCallFunctions;

	float auxFunctionPressingTime = 0.f;

	FTimerHandle pressingHandle;
	FTimerHandle functionPressingHandle;
	FTimerHandle nextHandle;

	FTimerHandle tooltipHandle;
	bool firstTimeTooltip = false;

	static AActor* GetCustomActor(AActor* actor, FGetCustomActor& data, UInteractComponent* interactComponent);
	static AActor* GetSpecificActor(AActor* actor, FGetSpecificActor& data, UInteractComponent* interactComponent);
	static AActor* GetThisActor(AActor* actor, FGetThisActor& data, UInteractComponent* interactComponent);

	static bool DebugCondition(AActor* actor, FDebugCondition& data, UInteractComponent* interactComponent);
	static bool DecrementIncrement(AActor* actor, FDecrementIncrement& data, UInteractComponent* interactComponent);
	static bool AndFunction(AActor* actor, FAndFunction& data, UInteractComponent* interactComponent);
	static bool CheckActorLocation(AActor* actor, FCheckActorLocation& data, UInteractComponent* interactComponent);
	static bool CheckActorRotation(AActor* actor, FCheckActorRotation& data, UInteractComponent* interactComponent);
	static bool CheckVariableValue(AActor* actor, FCheckVariableValue& data, UInteractComponent* interactComponent);
	static bool CheckRaycast(AActor* actor, FCheckRaycast& data, UInteractComponent* interactComponent);
	static bool CheckTooltipVisibility(AActor* actor, FCheckTooltipVisibility& data, UInteractComponent* interactComponent);
	static bool CheckIfActorExists(AActor* actor, FCheckIfActorExists& data, UInteractComponent* interactComponent);
	static bool CheckIfSoundIsPlaying(AActor* actor, FCheckIfSoundIsPlaying& data, UInteractComponent* interactComponent);

	static bool PlaceholderCheck(AActor* actor, FPlaceholderCheck& data, UInteractComponent* interactComponent);

	static void IfFunction(AActor* actor, FIfFunction& data, UInteractComponent* interactComponent);
	static void ExecuteAction(AActor* actor, FExecuteAction& data, UInteractComponent* interactComponent);
	static void JumpStage(AActor* actor, FJumpStage& data, UInteractComponent* interactComponent);
	static void DebugText(AActor* actor, FDebugText& data, UInteractComponent* interactComponent);
	static void ExternalInteract(AActor* actor, FExternalInteract& data, UInteractComponent* interactComponent);
	static void JumpFunction(AActor* actor, FJumpFunction& data, UInteractComponent* interactComponent);
	static void CancelJumpStage(AActor* actor, FCancelJumpStage& data, UInteractComponent* interactComponent);
	static void BreakFunction(AActor* actor, FBreakFunction& data, UInteractComponent* interactComponent);
	static void DelayFunction(AActor* actor, FDelayFunction& data, UInteractComponent* interactComponent);
	static void PlayAnimCharacter(AActor* actor, FPlayAnimCharacter& data, UInteractComponent* interactComponent);
	static void TeleportTo(AActor* actor, FTeleportTo& data, UInteractComponent* interactComponent);
	static void RotateLookAtActor(AActor* actor, FRotateLookAtActor& data, UInteractComponent* interactComponent);
	static void SetVisibilityActor(AActor* actor, FSetVisibilityActor& data, UInteractComponent* interactComponent);
	static void SetTransformActor(AActor* actor, FSetTransformActor& data, UInteractComponent* interactComponent);
	static void DestroyEntity(AActor* actor, FDestroyEntity& data, UInteractComponent* interactComponent);
	static void MoveToFunction(AActor *actor, FMoveToFunction& data, UInteractComponent* interactComponent);
	static void RotateToFunction(AActor *actor, FRotateToFunction& data, UInteractComponent* interactComponent);
	static void CancelTimer(AActor *actor, FCancelTimer& data, UInteractComponent* interactComponent);
	static void CreateEditVariable(AActor *actor, FCreateEditVariable& data, UInteractComponent* interactComponent);
	static void SpawnerFunction(AActor *actor, FSpawnerFunction& data, UInteractComponent* interactComponent);
	static void SetRagdoll(AActor* actor, FSetRagdoll& data, UInteractComponent* interactComponent);
	static void ShowHideTooltip(AActor *actor, FShowHideTooltip& data, UInteractComponent* interactComponent);
	static void AddImpulseFunction(AActor* actor, FAddImpulseFunction& data, UInteractComponent* interactComponent);
	static void ObjectInspector(AActor *actor, FObjectInspector& data, UInteractComponent* interactComponent);
	static void SetCollisionSettings(AActor* actor, FSetCollisionSettings& data, UInteractComponent* interactComponent);
	static void SlowTimeFunction(AActor* actor, FSlowTimeFunction& data, UInteractComponent* interactComponent);
	static void AttachToObject(AActor* actor, FAttachToObject& data, UInteractComponent* interactComponent);
	static void PlaySoundFunction(AActor* actor, FPlaySoundFunction& data, UInteractComponent* interactComponent);
	static void StopSoundFunction(AActor* actor, FStopSoundFunction& data, UInteractComponent* interactComponent);
	static void ModifySoundFunction(AActor* actor, FModifySoundFunction& data, UInteractComponent* interactComponent);
	static void PlaySequence(AActor* actor, FPlaySequence& data, UInteractComponent* interactComponent);
	static void CameraEffect(AActor* actor, FCameraEffect& data, UInteractComponent* interactComponent);
	static void OpenLevelFunction(AActor* actor, FOpenLevelFunction& data, UInteractComponent* interactComponent);
	static void ChangePriority(AActor* actor, FChangePriority& data, UInteractComponent* interactComponent);
	static void ExecuteTrigger(AActor* actor, FExecuteTrigger& data, UInteractComponent* interactComponent);
	static void AddWidgetToViewport(AActor* actor, FAddWidgetToViewport& data, UInteractComponent* interactComponent);
	static void RemoveWidgetFunction(AActor* actor, FRemoveWidgetFunction& data, UInteractComponent* interactComponent);
	static void SetPhysicsSettings(AActor* actor, FSetPhysicsSettings& data, UInteractComponent* interactComponent);
	static void AddTagFunction(AActor* actor, FAddTagFunction& data, UInteractComponent* interactComponent);
	static void SetMaterialMesh(AActor* actor, FSetMaterialMesh& data, UInteractComponent* interactComponent);
	static void GetExternalVariable(AActor* actor, FGetExternalVariable& data, UInteractComponent* interactComponent);
	static void SetTooltipLocationSize(AActor* actor, FSetTooltipLocationSize& data, UInteractComponent* interactComponent);
	static void RelocateFunction(AActor *actor, FRelocateFunction& data, UInteractComponent* interactComponent);
	static void PlayWidgetAnimation(AActor *actor, FPlayWidgetAnimation& data, UInteractComponent* interactComponent);
	static void ShowHideOutlineShine(AActor *actor, FShowHideOutlineShine& data, UInteractComponent* interactComponent);
	static void CurveLineTrace(AActor *actor, FCurveLineTrace& data, UInteractComponent* interactComponent);
	static void GetInternalVariable(AActor *actor, FGetInternalVariable& data, UInteractComponent* interactComponent);
	static void ExecCommand(AActor *actor, FExecCommand& data, UInteractComponent* interactComponent);
	static void SetInternalVariable(AActor *actor, FSetInternalVariable& data, UInteractComponent* interactComponent);
	static void ExecuteInternalFunction(AActor *actor, FExecuteInternalFunction& data, UInteractComponent* interactComponent);
	static void WhenFunction(AActor *actor, FWhenFunction& data, UInteractComponent* interactComponent);
	static void CallExternalDelegateFunction(AActor *actor, FCallExternalDelegateFunction& data, UInteractComponent* interactComponent);
	static void LoadStreamLevelInteract(AActor *actor, FLoadStreamLevelInteract& data, UInteractComponent* interactComponent);
	static void UnloadStreamLevelInteract(AActor *actor, FUnloadStreamLevelInteract& data, UInteractComponent* interactComponent);

	UInteractComponent* interactBP;

	EPriority m_priority = EPriority::Normal;

	bool m_priorityRepeat = false;

	static TArray<TArray<FInteractData>> datasStreamLoad;
	static TArray<FString> actorsStreamLoad;

	static UInteractComponent* GetInteractBP(UClass* interactOwnerClass);

	bool doOnceStart = false;

	UFUNCTION(BlueprintCallable)
		void ResetBPInteract(); 

	UFUNCTION()
		void StartInteract();

	EPriority GetPriority() { return m_priority; }
	bool GetPriorityRepeat() { return m_priorityRepeat; }

	void ImportInteractData(FString text);
	FString ExportInteractData();

	UFUNCTION(BlueprintCallable)
		void ImportInteractDataInternal();
	UFUNCTION(BlueprintCallable)
		void ExportInteractDataInternal();


	UFUNCTION()
		void ObjectInspectorTimer(AActor *actor, FObjectInspector& data);//Corregido fps
	UFUNCTION()
		void PlayAnimCharacterTimer(ACharacter* actor, FPlayAnimCharacter& data); //Preguntar a Maria
	UFUNCTION()
		void MoveToFunctionTimer(AActor* actor, FMoveToFunction& data); //Preguntar a Maria
	UFUNCTION()
		void RotateToFunctionTimer(AActor* actor, FRotateToFunction& data); //Corregido fps
	UFUNCTION()
		void DecrementIncrementTimer(AActor* actor, FDecrementIncrement& data); //Preguntar a Maria
	UFUNCTION()
		void PlaySoundTimer(AActor *actor, FPlaySoundFunction& data); //Preguntar a Maria
	UFUNCTION()
		void RelocateFunctionTimer(AActor *actor, FRelocateFunction& data);//Corregido fps
	UFUNCTION()
		void CurveLineTraceTimer(AActor *actor, FCurveLineTrace& data); //Deprecated

	UPROPERTY()
		UCapsuleComponent* m_innerCapsule;
	UPROPERTY()
		UCapsuleComponent* m_outsideCapsule;
	UPROPERTY()
		UCapsuleComponent* m_triggerCapsule;
	UPROPERTY()
		UWidgetComponent* firstWidget;
	UPROPERTY()
		TArray<UMeshComponent *> m_actorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FInteractData> interactDataArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString interactDataArrayString = "NOT_VALUE";

	UFUNCTION()
		void InteractPressing(int tooltipIndex, AActor* actor, FActionButton actionButton, int newStage);
	UFUNCTION()
		void FunctionsPressing(int tooltipIndex, AActor* actor, FActionButton actionButton, int newStage, float time);
	UFUNCTION()
		void NextStagePressing(int tooltipIndex);
	
	//OVERLAPS
	UFUNCTION()
		void InsideCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void InsideCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OutsideCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OutsideCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void TriggerCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TriggerCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//INTERACT FUNCTIONS
	UFUNCTION()
		void ShowTooltip();
	UFUNCTION()
		void HideTooltip();
	UFUNCTION()
		void HideTooltipHard();
	UFUNCTION()
		void ShowInnerOutline();
	UFUNCTION()
		void ShowOutsideOutline();
	UFUNCTION()
		void HideOutline();

	bool isShining = false;
	float shiningAux = 0.f;
	FTimerHandle shiningHandle;

	UFUNCTION()
		void ShowShine();
	UFUNCTION()
		void HideShine();
	UFUNCTION()
		void InitializeTooltip();
	UFUNCTION()
		void Next(int stage, FTooltipData data = {});
	UFUNCTION()
		void DelayTimer(AActor* actor, FDelayFunction& data, int functIndex, bool tempDoOnceDel, int teTooltipIndex = -1);

	UFUNCTION()
		void NextStage(int stage, FTooltipData data = {});

	static bool JumpStageGlobal(AActor* actor, FString stageString = "", float delay = 0.0f, int stage = -4, bool cancelTimers = true);

	UFUNCTION()
		void UpdateStageAtributes();
	UFUNCTION()
		void Interact(AActor* actor, FActionButton actionButton);
	UFUNCTION(BlueprintCallable)
		void UpdateInteractArrayData();
	UFUNCTION(BlueprintCallable)
		void UpdateEverything(bool doExport = false);

	UFUNCTION()
	void CalculateTicks(float time);

	void StopQTEStage();
	bool IsQTEAvailable();
	bool HasAnyQTE();

	UFUNCTION(BlueprintCallable)
		void CallInteractDelegate(AActor* actor, EActionList function, EDelegateList delegateName, FString infoData, int stageIndex, int functionIndexV, EActionStageType stageType, EAction actionButton, FString intName = "", bool onlyThis = false, EActionListCore functionCore = EActionListCore::None, EDelegateListCore delegateNameCore = EDelegateListCore::None);

	void UpdateSharedVariable(FString name, FString value);
	UFUNCTION(BlueprintCallable)
	void UpdateVariables(UPARAM(ref) FCustom& parent, FString variables, FCustom structClass);
	static void UpdateVariables(void* parent, FString variables, UScriptStruct* structClass, UInteractComponent* interactComponent);
	FString FindVariable(FString name);
	FString FindLocalVariable(void* parent, UStruct* structClass, FString var);
	FString DivideString(FString str, FString* var1, FString* var2, bool equals = false);
	void SetVariableValue(void* parent, FString property, FString value, UStruct* structClass, void* structValue = nullptr);

	float CalculateNumber(FString value);
	bool CalculateString(FString value, FString* res, bool init);
	bool CalculateBoolean(FString value, FString* res);
	bool CalculateVector(FString value, FString* res, EVectorType type);
	bool CalculateArray(FString value, FString* res, bool init);
	bool CalculateObject(FString value, FString* res, bool init);
	FString CalculateValue(FString value, bool init = false);

	//Auxiliary functions

	static AActor* GetTargetActor(UInteractComponent* interactComponent, FFunctionActorData dataActor, AActor* actor, EAction action, int tooltipIndex, EActionStageType stageType, bool inElse, int wrapperIndex = -1, int functIndex = -1, int stageIndex = -1);
	static AActor* GetCurrentActor(UInteractComponent* interactComponent, FFunctionActorData dataActor, AActor* actor, EAction action, int tooltipIndex, EActionStageType stageType, bool inElse, int wrapperIndex = -1, int functIndex = -1, int stageIndex = -1);
	static USceneComponent* GetSceneComponent(UInteractComponent* interactComponent, AActor* dataActor, TSubclassOf<USceneComponent> specificChild, FName componentTag);
	static UActorComponent* GetActorComponent(UInteractComponent* interactComponent, AActor* dataActor, TSubclassOf<UActorComponent> specificChild, FName componentTag);
	static FVector GetDirectionInteract(EVectorDirection direction, float distance, FVector offsetLocation, AActor* dataActor = nullptr, USceneComponent* dataComponent = nullptr);
	static void RegisterTimer(UInteractComponent* interactComponent, ETimerList enumName, int currentStage, int currentFunction, EActionStageType currentStageType, EAction currentAction, ETimerListCore enumNameCore = ETimerListCore::None);
	static bool IsTimerRegister(UInteractComponent* interactComponent, ETimerList enumName, int currentStage, int currentFunction, EActionStageType currentStageType, EAction currentAction, ETimerListCore enumNameCore = ETimerListCore::None);

	UFUNCTION(BlueprintCallable)
		void ReturnValueFloat(UPARAM(ref) TArray<FString>& customReturn, float returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueInteger(UPARAM(ref) TArray<FString>& customReturn, int returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueString(UPARAM(ref) TArray<FString>& customReturn, FString returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueBool(UPARAM(ref) TArray<FString>& customReturn, bool returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueName(UPARAM(ref) TArray<FString>& customReturn, FName returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueVector(UPARAM(ref) TArray<FString>& customReturn, FVector returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueColor(UPARAM(ref) TArray<FString>& customReturn, FColor returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueRotator(UPARAM(ref) TArray<FString>& customReturn, FRotator returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueObject(UPARAM(ref) TArray<FString>& customReturn, UObject* returnValue);
	UFUNCTION(BlueprintCallable)
		void ReturnValueActor(UPARAM(ref) TArray<FString>& customReturn, AActor* returnValue);

	void ReturnValue(TArray<FString>* customReturn, float returnValue);
	void ReturnValue(TArray<FString>* customReturn, int returnValue);
	void ReturnValue(TArray<FString>* customReturn, FString returnValue, bool isString = true);
	void ReturnValue(TArray<FString>* customReturn, bool returnValue);
	void ReturnValue(TArray<FString>* customReturn, FName returnValue);
	void ReturnValue(TArray<FString>* customReturn, FVector returnValue);
	void ReturnValue(TArray<FString>* customReturn, FColor returnValue);
	void ReturnValue(TArray<FString>* customReturn, FRotator returnValue);
	void ReturnValue(TArray<FString>* customReturn, UObject* returnValue);
	void ReturnValue(TArray<FString>* customReturn, AActor* returnValue);
	#define CONFIRM_RETURN(interactComponent, data, dataStruct) UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data), data.customVariableSet, dataStruct::StaticStruct(), interactComponent); \
		data.customReturn.Empty();

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& e) override;
#endif
	void OnPreWorldInitializationInteract(UWorld* World, const UWorld::InitializationValues IVS);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	TArray<FInteractData> GetInteractDataArray() const { return interactDataArray; }
	void SetInteractDataArray(TArray<FInteractData> val) { interactDataArray = val; }
};