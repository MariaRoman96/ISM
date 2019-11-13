#pragma once

#include "CoreMinimal.h"
#include <limits>
#include <NoExportTypes.h>
#include <Class.h>
#include <UObjectGlobals.h>
#include <ObjectMacros.h>
#include <Engine/Texture2D.h>
#include <GameFramework/Actor.h>
#include "InteractConfig.h"
#include <Engine/Engine.h>
#include <DelegateCombinations.h>
#if WITH_EDITOR
#include "ICE_InteractLog.h"
#endif
#include <NoExportTypes.h>
#include "InteractCore.generated.h"

using namespace std;

DECLARE_LOG_CATEGORY_EXTERN(ISM, Log, All);

class UInteractComponent;

#define NaN numeric_limits<float>::infinity()

UENUM(BlueprintType)
enum class EPriority : uint8
{
	None		UMETA(DisplayName = "None"),
	V_Low		UMETA(DisplayName = "Very low"),
	Low			UMETA(DisplayName = "Low"),
	Normal		UMETA(DisplayName = "Normal"),
	High		UMETA(DisplayName = "High"),
	V_High		UMETA(DisplayName = "Very high"),
	Max			UMETA(DisplayName = "Max"),
	Min			UMETA(DisplayName = "Min")
};

UENUM(BlueprintType)
enum class EOutlineColor : uint8
{
	None		UMETA(DisplayName = "None"),
	Blue		UMETA(DisplayName = "Blue"),
	Orange		UMETA(DisplayName = "Orange"),
	Red			UMETA(DisplayName = "Red"),
	Green		UMETA(DisplayName = "Green"),
};

USTRUCT(BlueprintType)
struct FActionButton {

	GENERATED_USTRUCT_BODY()
public:
	FActionButton() {

	}

	FActionButton(EAction actionB, EActionType actionButtonT) {
		actionButton = actionB;
		actionButtonType = actionButtonT;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAction actionButton = EAction::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EActionType actionButtonType = EActionType::Pressed;
};


UENUM(BlueprintType)
enum class EActionStageType : uint8
{
	None					UMETA(DisplayName = "None"),
	Pre						UMETA(DisplayName = "Pre stage"),
	Init					UMETA(DisplayName = "Stage init"),
	Interact				UMETA(DisplayName = "Stage interact"),
	Exit					UMETA(DisplayName = "Stage exit"),
	External				UMETA(DisplayName = "External"),
};

//TO ADD NEW SELECTABLE FUNCTION: ENUM, DISPLAY NAME AND STRUCT NAME MUST BE THE SAME
UENUM(BlueprintType)
enum class EGetActorList : uint8
{
	None 						UMETA(DisplayName = "None", customCategory = "Core"),
	getCustomActor				UMETA(DisplayName = "getCustomActor", customCategory = "Core"),
	getSpecificActor			UMETA(DisplayName = "getSpecificActor", customCategory = "Core"),
	getThisActor				UMETA(DisplayName = "getThisActor", customCategory = "Core"),
};

USTRUCT(BlueprintType)
struct FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FInteractStruct() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "customVariableSet", Tooltip = "Establece valores en las variables del struct (separados por ;)(\"$\" es variable loca, \"_\" es variable global). Ej: $var1 = 1; $var2 = _miVariable;"))
		FString customVariableSet = "";

	UPROPERTY(BlueprintReadWrite)
		TArray<FString> customReturn;

};

UENUM(BlueprintType)
enum class ERaycastType : uint8
{
	Line	 				UMETA(DisplayName = "Line"),
	Capsule 				UMETA(DisplayName = "Capsule"),
};

UENUM(BlueprintType)
enum class ECollisionRayCastType : uint8
{
	AllDynamic	 			UMETA(DisplayName = "AllDynamic"),
	AllStatic 				UMETA(DisplayName = "AllStatic"),
	Pawn	 				UMETA(DisplayName = "Pawn"),
	PhysicsBody				UMETA(DisplayName = "PhysicsBody"),
	Visibility				UMETA(DisplayName = "Visibility"),
	Camera					UMETA(DisplayName = "Camera"),
	Destructible			UMETA(DisplayName = "Destructible"),
};

USTRUCT(BlueprintType)
struct FExtraActorData{

	GENERATED_USTRUCT_BODY()
public:
	FExtraActorData() {}

	FExtraActorData(bool inElse1, int actorNum1)
	{
		inElse = inElse1;
		actorNum = actorNum1;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool inElse = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int actorNum = 0;
};

DECLARE_DELEGATE_RetVal_NineParams(AActor*, FDelegateActor, UInteractComponent*, AActor*, EAction, int, EActionStageType, int, int, int, FExtraActorData);

USTRUCT(BlueprintType)
struct FGetCustomActor : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FGetCustomActor() {}

	FGetCustomActor(UClass* act1, FName tag1)
	{
		actor = act1;
		tag = tag1;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName tag;
};

USTRUCT(BlueprintType)
struct FGetThisActor : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FGetThisActor() {}

	FGetThisActor(bool getOwner1)
	{
		getOwner = getOwner1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool getOwner = true;
};

USTRUCT(BlueprintType)
struct FGetSpecificActor : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FGetSpecificActor() {}

	FGetSpecificActor(AActor* actor1)
	{
		actor = actor1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* actor;
};

UENUM(BlueprintType)
enum class EVectorDirection : uint8
{
	None	 				UMETA(DisplayName = "None"),
	Forward 				UMETA(DisplayName = "Forward"),
	Backward	 			UMETA(DisplayName = "Backward"),
	Right					UMETA(DisplayName = "Right"),
	Left					UMETA(DisplayName = "Left"),
	Up						UMETA(DisplayName = "Up"),
	Down					UMETA(DisplayName = "Down"),
};

UENUM(BlueprintType)
enum class ESpecificAxis : uint8
{
	None	 				UMETA(DisplayName = "None"),
	X	 					UMETA(DisplayName = "X"),
	Y	 					UMETA(DisplayName = "Y"),
	Z	 					UMETA(DisplayName = "Z"),
};

UENUM(BlueprintType)
enum class ESpecificRotAxis : uint8
{
	None	 				UMETA(DisplayName = "None"),
	Pitch	 				UMETA(DisplayName = "Pitch"),
	Yaw	 					UMETA(DisplayName = "Yaw"),
	Roll	 				UMETA(DisplayName = "Roll"),
};

// USTRUCT(BlueprintType)
// struct FActorSelect {
// 	GENERATED_USTRUCT_BODY()
// public:
// 	FActorSelect() {};
// 	FActorSelect(bool m_doOnOwner, bool m_customActor, UClass* m_currentActor, FName m_currentTag)
// 	{
// 		doOnOwner = m_doOnOwner;
// 		customActor = m_customActor;
// 		currentActor = m_currentActor;
// 		currentTag = m_currentTag;
// 	};
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "doOnOwner"))
// 		bool doOnOwner = true;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "customActor"))
// 		bool customActor = false;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "currentActor"))
// 		UClass* currentActor;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "currentTag"))
// 		FName currentTag;
// };

//ADD NEW STRUCT IN FUNCTIONDATA FOR EACH FUNCTION
USTRUCT(BlueprintType)
struct FFunctionActorData
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionActorData() {}

	FFunctionActorData(EGetActorList enumName1, FGetCustomActor getCustomActor1, FGetThisActor getThisActor1)
	{
		enumName = enumName1;
		getCustomActor = getCustomActor1;
		getThisActor = getThisActor1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGetActorList enumName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "getCustomActor")
		FGetCustomActor getCustomActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "getSpecificActor")
		FGetSpecificActor getSpecificActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "getThisActor")
		FGetThisActor getThisActor;
};


USTRUCT(BlueprintType)
struct FSavedStateInteract {

	GENERATED_USTRUCT_BODY()
public:
	FSavedStateInteract() {}

	FSavedStateInteract(EAction tempAction1, int tempTooltipIndex1, EActionStageType tempStage1,
		int tempFunctionIndex1, int tempCurrentStage1, bool debugCheck1 = false, bool tempInElse1 = false)
	{
		tempAction = tempAction1;
		tempTooltipIndex = tempTooltipIndex1;
		tempStage = tempStage1;
		tempFunctionIndex = tempFunctionIndex1;
		tempCurrentStage = tempCurrentStage1;
		debugCheck = debugCheck1;
		tempInElse = tempInElse1;
	}

	EAction tempAction;
	int tempTooltipIndex;
	EActionStageType tempStage;
	int tempFunctionIndex;
	int tempCurrentStage;
	bool tempInElse;

	bool debugCheck = false;
};

//BIND ACTIONS BY STRING
#define ISM_ULOG(text,...) ISM_ULOG_EDIT(text, Warning, __VA_ARGS__)
#if WITH_EDITOR
#define ISM_ULOG_EDIT(text, type,...) {if(UInteractCore::showLogMessages){UE_LOG(ISM, type, TEXT(text), __VA_ARGS__); UInteractCore::logArrayType.Add(ELogVerbosity::type==ELogVerbosity::Fatal||ELogVerbosity::type==ELogVerbosity::Error?ELogHelpers::Error:ELogVerbosity::type==ELogVerbosity::Warning?ELogHelpers::Warning:ELogVerbosity::type==ELogVerbosity::Log?ELogHelpers::Info:ELogHelpers::Spam); UInteractCore::logArrayString.Add(FString::Printf(TEXT(text), __VA_ARGS__)); UInteractCore::dateTime.Add(FDateTime::Now()); UInteractCore::logArrayFile.Add(FString::Printf(TEXT("[%s] %s (%d)"), *FString(__FUNCTION__), *FString(__FILE__), __LINE__)); UICE_InteractLog::delegateLogHelpers.Broadcast();}}
#else
#define ISM_ULOG_EDIT(text, type,...) {if(UInteractCore::showLogMessages){UE_LOG(ISM, type, TEXT(text), __VA_ARGS__);}}
#endif
#define ISM_ISVALID(objectValue) (objectValue && objectValue->GetUniqueID() >= 0 && objectValue->IsValidLowLevel())
#define ISM_CLEARLOG() GEngine->ClearOnScreenDebugMessages();
#define ISM_PRINTLOG(text) ISM_PRINTLOG_EDIT(text, 5.f, FColor::Red)
#if WITH_EDITOR
#define ISM_PRINTLOG_EDIT(text, time, color) {if(UInteractCore::showDebugMessages){GEngine->AddOnScreenDebugMessage(-1, time, color, text); UInteractCore::logArrayType.Add(ELogHelpers::Warning); UInteractCore::logArrayString.Add(text); UInteractCore::dateTime.Add(FDateTime::Now()); UInteractCore::logArrayFile.Add(FString::Printf(TEXT("[%s] %s (%d)"), *FString(__FUNCTION__), *FString(__FILE__), __LINE__)); UICE_InteractLog::delegateLogHelpers.Broadcast();}}
#else
#define ISM_PRINTLOG_EDIT(text, time, color) {if(UInteractCore::showDebugMessages){GEngine->AddOnScreenDebugMessage(-1, time, color, text); }}
#endif
#define GETENUMVALUE(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, etype, true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, etype, true)->GetValueByNameString(evalue) : 0 )
#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, etype, true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, etype, true)->GetNameStringByIndex((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )
#define GETENUMMAX(etype) FindObject<UEnum>(ANY_PACKAGE, etype, true)->GetMaxEnumValue();
#define GETENUMDISPLAYNAME(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, etype, true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, etype, true)->GetDisplayNameTextByIndex((int32)evalue) : FText::FromString(FString("Invalid - are you sure enum uses UENUM() macro?")) )
#define GETENUMCUSTOMMETADATA(etype, evalue, emetadata) FindObject<UEnum>(ANY_PACKAGE, etype, true)->GetMetaData(emetadata), (int32)evalue);

UCLASS()
class ISM_API UInteractCore : public UInteractConfig
{
	GENERATED_BODY()

public:
	UInteractCore() {
		
	};

	//Core functions
	UFUNCTION(BlueprintCallable, Category = "ISM - Core")
		static void PrintLog(FString text, float time, FColor color);

	UFUNCTION(BlueprintCallable, Category = "ISM - Core")
		static bool JumpStageGlobal(AActor* actor, FString stageString = "", float delay = 0.1f, int stage = -4, bool cancelTimers = true);

	UFUNCTION(BlueprintCallable, Category = "ISM - Core")
		static AActor* MakeInteractActor(AActor* actor);

	//Math functions
	UFUNCTION(BlueprintCallable, Category = "ISM - Math Functions")
		static void BezierCurve(const FVector CPInicio, const FVector CP1, const FVector CP2, const FVector CPFin, int32 NumPoints, TArray <FVector> & OutPoints);

	static TArray<FString> logArrayString;
	static TArray<FString> logArrayFile;
#if WITH_EDITOR
	static TArray<ELogHelpers> logArrayType;
#endif
	static TArray<FDateTime> dateTime;

	static bool showDebugMessages;
	static bool showLogMessages;

	static AActor* FindActorByName(FString name, bool containsName = false);

	static void ReparentBlueprint(UBlueprint* BlueprintObj, UClass* ChosenClass, bool compile = true);

	static void AddComponentToBlueprint(UBlueprint* Blueprint, USceneComponent* Component);

	/*	static void ModifyComponentBlueprint(UBlueprint* Blueprint, USceneComponent* Component, FName componentName);*/

	static void CompileBlueprint(UBlueprint* Blueprint);

	static AActor* SpawnActorFromBlueprint(UBlueprint* Blueprint, AActor* actor, FVector location, FRotator rotation, UWorld* world);

	//static void EnsureBlueprintIsUpToDate(UBlueprint* BlueprintObj);

	//static TArray<UObject*> DynamicLoadContentFromPath(FString PathFromContentFolder = "Assets/Textures/Icons", UClass* AssetClass = nullptr, bool LoadFromSubfolders = false);

	//static float GetValueFromString(FString value, float max = NaN);
	//static UTexture2D* GetActionButtonImage(EAction actionButton);
	//static void checkCurrentPlatform();
	static float GetValue(FString value, float max = NaN);

	static void ISM_Destroy(UObject* ToDestroy);
};