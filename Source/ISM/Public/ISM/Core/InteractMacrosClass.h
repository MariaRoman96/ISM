// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractExtraClass.h"
#include "InteractCore.h"
#include "InteractMacrosClass.generated.h"

class UInteractComponent;

USTRUCT(BlueprintType)
struct FCallExternalDelegateFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCallExternalDelegateFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "targetActor", Tooltip = "The only one actor that will be executed its delegate"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "delegateNameCore"))
		EDelegateListCore delegateNameCore = EDelegateListCore::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "delegateName"))
		EDelegateList delegateName = EDelegateList::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificData"))
		FString specificData = "";
};

USTRUCT(BlueprintType)
struct FWhenFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FWhenFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "executeAlways", Tooltip = "It makes that currentActor will be disabled if true"))
		bool executeAlways = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "waiting Actor", Tooltip = "The actor that it will be waiting if it trigger an action"))
		FFunctionActorData actor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "executing Actor", Tooltip = "The actor that will be executed (the stage functions)"))
		FFunctionActorData actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "delegateNameCore"))
		EDelegateListCore delegateNameCore = EDelegateListCore::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "delegateName", Tooltip = "Specific delegate"))
		EDelegateList delegateName = EDelegateList::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificData", Tooltip = "Specific delegate data"))
		FString specificData = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specifyData", Tooltip = "It activates the filter"))
		bool specifyData = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificFunctionCore", EditCondition = "specifyData"))
		EActionListCore specificFunctionCore = EActionListCore::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specificFunction", EditCondition = "specifyData", Tooltip = "Specific function where it will be waiting for its delegate"))
		EActionList specificFunction = EActionList::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stageIndex", EditCondition = "specifyData", Tooltip = "Specific stage where this function is"))
		int stageIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "functionIndex", EditCondition = "specifyData", Tooltip = "Specific function index where function is"))
		int functionIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stageType", EditCondition = "specifyData", Tooltip = "Specific stage type where function is"))
		EActionStageType stageType = EActionStageType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "buttonType", EditCondition = "specifyData", Tooltip = "Specific button type where function is"))
		EAction buttonType = EAction::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "isPermanent", Tooltip = "If this bind won't be deleted after used"))
		bool isPermanent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stageFunction", Tooltip = "Stage function that will be executed"))
		int stageFunction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "specifyFunction", Tooltip = "It makes more editable the functions that will be executed"))
		bool specifyFunction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "startFunctionIndex", EditCondition = "specifyFunction", Tooltip = "Specific start function index that will be executed"))
		int startFunctionIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "endFunctionIndex", EditCondition = "specifyFunction", Tooltip = "Specific end function index that will be executed"))
		int endFunctionIndex = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "stageTypeFunction", EditCondition = "specifyFunction", Tooltip = "Specific stage type that will be executed"))
		EActionStageType stageTypeFunction = EActionStageType::Pre;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "buttonTypeFunction", EditCondition = "specifyFunction", Tooltip = "Specific button type that will be executed"))
		EAction buttonTypeFunction = EAction::None;
};


USTRUCT(BlueprintType)
struct FCancelTimer : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCancelTimer() {}

	FCancelTimer(ETimerListCore m_functionCore, ETimerList m_cancelFunction,
		bool m_doOnce,
		float m_specificStage,
		float m_specificFunctionIndex)
	{
		specificTimerCore = m_functionCore;
		specificTimer = m_cancelFunction;
		doOnce = m_doOnce;
		specificStage = m_specificStage;
		specificFunctionIndex = m_specificFunctionIndex;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "functionCore"))
		ETimerListCore specificTimerCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "function"))
		ETimerList specificTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "doOnce"))
		bool doOnce = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "doOnce", DisplayName = "specificStage"))
		float specificStage = -1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "doOnce", DisplayName = "specificFunctionIndex"))
		float specificFunctionIndex = -1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "doOnce", DisplayName = "specificStageType"))
		EActionStageType specificStageType = EActionStageType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "doOnce", DisplayName = "specificAction"))
		EAction specificAction = EAction::None;
};


USTRUCT(BlueprintType)
struct FAndFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FAndFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "conditions"))
		TArray<FFunctionCheckDataBase> conditions;

	int conditionIndex = 0;

};

USTRUCT(BlueprintType)
struct FFunctionCheckData : public FFunctionCheckDataBase
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionCheckData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "andFunction")
		FAndFunction andFunction;
};

USTRUCT(BlueprintType)
struct FFunctionDataWrapper : public FFunctionDataBase
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionDataWrapper() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "cancelTimer")
		FCancelTimer cancelTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "callExternalDelegateFunction")
		FCallExternalDelegateFunction callExternalDelegateFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "whenFunction")
		FWhenFunction whenFunction;
};

USTRUCT(BlueprintType)
struct FIfFunction : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FIfFunction() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Condition"))
		FFunctionCheckData condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Negate"))
		bool negate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Function"))
		FFunctionDataWrapper function;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ElseFunction"))
		FFunctionDataWrapper elseFunction;

};


USTRUCT(BlueprintType)
struct FFunctionData : public FFunctionDataWrapper
{
	GENERATED_USTRUCT_BODY()
public:

	FFunctionData() {
		/*ifFunction = FIfFunction();*/
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "delayFunction")
		FDelayFunction delayFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ifFunction")
		FIfFunction ifFunction;
};


#define CREATE_FUNCTIONCORE(FunctionName, Class, StructName, EnumName) \
void UInteractMacrosClass::FunctionName(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex, int functionInd, int currentStag, bool inElse){ \
	int functionIn = functionInd ==-1? interactComponent->functionIndex: functionInd; \
	int currentSta = currentStag ==-1? interactComponent->m_currentStage: currentStag; \
	if (actionStageType == EActionStageType::Pre) { \
		if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::EnumName) { \
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
			Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].EnumName, interactComponent); \
		} else if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
			if(!inElse){ \
				if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
				}\
			} \
			else { \
				if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Init) { \
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::EnumName) { \
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
			Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].EnumName, interactComponent); \
		} else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
			if(!inElse){ \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
				} \
			} \
			else { \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Interact) { \
		if(interactComponent->interactDataArray[currentSta].actions.IsValidIndex(index)){ \
			if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].EnumName, interactComponent); \
			} else if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
				if(!inElse){ \
					if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
					} \
				} \
				else { \
					if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Exit) { \
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::EnumName) { \
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
			Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].EnumName, interactComponent); \
		} else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
			if(!inElse){ \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.enumNameCore == EActionListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
				} \
			} \
			else { \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.enumNameCore == EActionListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
				} \
			} \
		} \
	} \
	else { \
		return; \
	} \
}

//TODO HACER DE FORMA GENERICA LAS FUNCIONES
#define CREATE_FUNCTIONCHECKCORE(FunctionName, Class, StructName, EnumName) \
bool UInteractMacrosClass::FunctionName(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType){ \
	if (actionStageType == EActionStageType::Pre) { \
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				if(interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
					return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
				else{ \
					return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
			} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
				int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumNameCore == ECheckListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
						return !Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Init) { \
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
					return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
				else{ \
					return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
			} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
				int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumNameCore == ECheckListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
						return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Interact) { \
		if(interactComponent->interactDataArray[interactComponent->m_currentStage].actions.IsValidIndex(index)){ \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
						return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
					} \
				} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
					int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
					if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumNameCore == ECheckListCore::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						if(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
							return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
						} \
						else{ \
							return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
						} \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Exit) { \
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
					return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
				else{ \
					return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
			} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
				int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumNameCore == ECheckListCore::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
						return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	return false; \
}





#define CREATE_FUNCTION(FunctionName, Class, StructName, EnumName) \
void UInteractFunctionsClass::FunctionName(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex, int functionInd, int currentStag, bool inElse){ \
	int functionIn = functionInd ==-1? interactComponent->functionIndex: functionInd; \
	int currentSta = currentStag ==-1? interactComponent->m_currentStage: currentStag; \
	if (actionStageType == EActionStageType::Pre) { \
		if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumName == EActionList::EnumName) { \
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
			Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].EnumName, interactComponent); \
		} else if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
			if(!inElse){ \
				if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.enumName == EActionList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
				} \
			} \
			else { \
				if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.enumName == EActionList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Init) { \
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumName == EActionList::EnumName) { \
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
			Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].EnumName, interactComponent); \
		} else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
			if(!inElse){ \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.enumName == EActionList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
				} \
			} \
			else { \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.enumName == EActionList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Interact) { \
		if(interactComponent->interactDataArray[currentSta].actions.IsValidIndex(index)){ \
			if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumName == EActionList::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].EnumName, interactComponent); \
			} else if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
				if(!inElse){ \
					if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.enumName == EActionList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
					} \
				} \
				else { \
					if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.enumName == EActionList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Exit) { \
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumName == EActionList::EnumName) { \
			UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
			Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].EnumName, interactComponent); \
		} else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
			if(!inElse){ \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.enumName == EActionList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.EnumName, interactComponent); \
				} \
			} \
			else { \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.enumName == EActionList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.EnumName), interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					Class::FunctionName(actor, interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.EnumName, interactComponent); \
				} \
			} \
		} \
	} \
	else { \
		return; \
	} \
}

#define CREATE_FUNCTIONCHECK(FunctionName, Class, StructName, EnumName) \
bool UInteractFunctionsClass::FunctionName(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType){ \
	if (actionStageType == EActionStageType::Pre) { \
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.enumName == ECheckList::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				if(interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
					return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
				else{ \
					return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
			} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
				int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumName == ECheckList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
						return !Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].preStageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Init) { \
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.enumName == ECheckList::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
					return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
				else{ \
					return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
			} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
				int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumName == ECheckList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
						return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.initFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Interact) { \
		if(interactComponent->interactDataArray[interactComponent->m_currentStage].actions.IsValidIndex(index)){ \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.enumName == ECheckList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
						return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
					} \
				} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
					int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
					if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumName == ECheckList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						if(interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
							return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
						} \
						else{ \
							return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].actions[index].stageFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
						} \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Exit) { \
		if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].enumNameCore == EActionListCore::ifFunction) { \
			if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.enumName == ECheckList::EnumName) { \
				UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
				if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.negate){ \
					return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
				else{ \
					return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.EnumName, interactComponent); \
				} \
			} else if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
				int condIndex = interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditionIndex; \
				if (interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].enumName == ECheckList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName), interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					if(interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].negate){ \
						return !Class::FunctionName(actor,interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
					else{ \
						return Class::FunctionName(actor, interactComponent->interactDataArray[interactComponent->m_currentStage].capsuleTriggerStruct.exitFunction[interactComponent->functionIndex].ifFunction.condition.andFunction.conditions[condIndex].EnumName, interactComponent); \
					} \
				} \
			} \
		} \
	} \
	return false; \
}

#define CREATE_FUNCTIONACTOR(FunctionName, Class, StructName, EnumName) \
AActor* UInteractMacrosClass::FunctionName(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex, int functionInd, int currentStag, FExtraActorData extraData) \
{ \
int functionIn = functionInd == -1 ? interactComponent->functionIndex : functionInd; \
int currentSta = currentStag == -1 ? interactComponent->m_currentStage : currentStag; \
if (interactComponent->interactDataArray.IsValidIndex(currentSta)) { \
	if (actionStageType == EActionStageType::Pre) { \
		if (interactComponent->interactDataArray[currentSta].preStageFunction.IsValidIndex(functionIn)) { \
			if (getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumName).IsValidId()) { \
				FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
				if (extraData.actorNum == 0) { \
					data = interactComponent->GetFunctionActorData(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn]); \
				} \
				else if (extraData.actorNum == 1) { \
					data = interactComponent->GetFunctionCurrentActorData(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn]); \
				} \
				if (data.enumName == EGetActorList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					return Class::FunctionName(actor, data.EnumName, interactComponent); \
				} \
			} \
			else if (getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore).IsValidId()) { \
				FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
				if (extraData.actorNum == 0) { \
					data = interactComponent->GetFunctionActorDataCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn]); \
				} \
				else if (extraData.actorNum == 1) { \
					data = interactComponent->GetFunctionCurrentActorDataCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn]); \
				} \
				if (data.enumName == EGetActorList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					return Class::FunctionName(actor, data.EnumName, interactComponent); \
				} \
			} \
			else if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
				FSetElementId idElem = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.enumName); \
				FSetElementId idElem2 = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.enumName); \
				FSetElementId idElem1 = getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.enumName); \
				FSetElementId idElemCore = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function.enumNameCore); \
				FSetElementId idElem2Core = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction.enumNameCore); \
				FSetElementId idElem1Core = getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.enumNameCore); \
				if (!extraData.inElse) { \
					if (idElem.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					else if (idElemCore.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.function); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
				} \
				else { \
					if (idElem2.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					else if (idElem2Core.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.elseFunction); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
				} \
				if (idElem1.IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				else if (idElem1Core.IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				if (interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
					for (int r = 0; r < interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.andFunction.conditions.Num(); r++) { \
						if (getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumName).IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
						else if (getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumNameCore).IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].preStageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Init) { \
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction.IsValidIndex(functionIn)) { \
			if (getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumName).IsValidId()) { \
				FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
				if (extraData.actorNum == 0) { \
					data = interactComponent->GetFunctionActorData(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn]); \
				} \
				else if (extraData.actorNum == 1) { \
					data = interactComponent->GetFunctionCurrentActorData(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn]); \
				} \
				if (data.enumName == EGetActorList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					return Class::FunctionName(actor, data.EnumName, interactComponent); \
				} \
			} \
			else if (getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore).IsValidId()) { \
				FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
				if (extraData.actorNum == 0) { \
					data = interactComponent->GetFunctionActorDataCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn]); \
				} \
				else if (extraData.actorNum == 1) { \
					data = interactComponent->GetFunctionCurrentActorDataCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn]); \
				} \
				if (data.enumName == EGetActorList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					return Class::FunctionName(actor, data.EnumName, interactComponent); \
				} \
			} \
			else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
				FSetElementId idElem = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.enumName); \
				FSetElementId idElem2 = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.enumName); \
				FSetElementId idElem1 = getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.enumName); \
				FSetElementId idElemCore = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function.enumNameCore); \
				FSetElementId idElem2Core = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction.enumNameCore); \
				FSetElementId idElem1Core = getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.enumNameCore); \
				if (!extraData.inElse) { \
					if (idElem.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					else if (idElemCore.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.function); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
				} \
				else { \
					if (idElem2.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					else if (idElem2Core.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.elseFunction); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
				} \
				if (idElem1.IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				else if (idElem1Core.IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
					for (int r = 0; r < interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.andFunction.conditions.Num(); r++) { \
						if (getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumName).IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
						else if (getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumNameCore).IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.initFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Interact) { \
		if (interactComponent->interactDataArray[currentSta].actions.IsValidIndex(index)) { \
			if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction.IsValidIndex(functionIn)) { \
				if (getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumName).IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorData(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn]); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorData(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn]); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				else if (getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore).IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorDataCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn]); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorDataCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn]); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				else if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
					FSetElementId idElem = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.enumName); \
					FSetElementId idElem2 = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.enumName); \
					FSetElementId idElem1 = getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.enumName); \
					FSetElementId idElemCore = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function.enumNameCore); \
					FSetElementId idElem2Core = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction.enumNameCore); \
					FSetElementId idElem1Core = getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.enumNameCore); \
					if (!extraData.inElse) { \
						if (idElem.IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
						else if (idElemCore.IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.function); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
					} \
					else { \
						if (idElem2.IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
						else if (idElem2Core.IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.elseFunction); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
					} \
					if (idElem1.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					if (idElem1Core.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					if (interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
						for (int r = 0; r < interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.andFunction.conditions.Num(); r++) { \
							if (getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumName).IsValidId()) { \
								FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
								if (extraData.actorNum == 0) { \
									data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
								} \
								else if (extraData.actorNum == 1) { \
									data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
								} \
								if (data.enumName == EGetActorList::EnumName) { \
									UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
									return Class::FunctionName(actor, data.EnumName, interactComponent); \
								} \
							} \
							else if (getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumNameCore).IsValidId()) { \
								FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
								if (extraData.actorNum == 0) { \
									data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
								} \
								else if (extraData.actorNum == 1) { \
									data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].actions[index].stageFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
								} \
								if (data.enumName == EGetActorList::EnumName) { \
									UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
									return Class::FunctionName(actor, data.EnumName, interactComponent); \
								} \
							} \
						} \
					} \
				} \
			} \
		} \
	} \
	else if (actionStageType == EActionStageType::Exit) { \
		if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction.IsValidIndex(functionIn)) { \
			if (getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumName).IsValidId()) { \
				FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
				if (extraData.actorNum == 0) { \
					data = interactComponent->GetFunctionActorData(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn]); \
				} \
				else if (extraData.actorNum == 1) { \
					data = interactComponent->GetFunctionCurrentActorData(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn]); \
				} \
				if (data.enumName == EGetActorList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					return Class::FunctionName(actor, data.EnumName, interactComponent); \
				} \
			} \
			else if (getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore).IsValidId()) { \
				FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
				if (extraData.actorNum == 0) { \
					data = interactComponent->GetFunctionActorDataCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn]); \
				} \
				else if (extraData.actorNum == 1) { \
					data = interactComponent->GetFunctionCurrentActorDataCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn]); \
				} \
				if (data.enumName == EGetActorList::EnumName) { \
					UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
					return Class::FunctionName(actor, data.EnumName, interactComponent); \
				} \
			} \
			else if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].enumNameCore == EActionListCore::ifFunction) { \
				FSetElementId idElem = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.enumName); \
				FSetElementId idElem2 = getActorFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.enumName); \
				FSetElementId idElem1 = getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.enumName); \
				FSetElementId idElemCore = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function.enumNameCore); \
				FSetElementId idElem2Core = getActorFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction.enumNameCore); \
				FSetElementId idElem1Core = getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.enumNameCore); \
				if (!extraData.inElse) { \
					if (idElem.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					else if (idElemCore.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.function); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
				} \
				else { \
					if (idElem2.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapper(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
					else if (idElem2Core.IsValidId()) { \
						FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
						if (extraData.actorNum == 0) { \
							data = interactComponent->GetFunctionActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction); \
						} \
						else if (extraData.actorNum == 1) { \
							data = interactComponent->GetFunctionCurrentActorDataWrapperCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.elseFunction); \
						} \
						if (data.enumName == EGetActorList::EnumName) { \
							UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
							return Class::FunctionName(actor, data.EnumName, interactComponent); \
						} \
					} \
				} \
				if (idElem1.IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				else if (idElem1Core.IsValidId()) { \
					FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
					if (extraData.actorNum == 0) { \
						data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition); \
					} \
					else if (extraData.actorNum == 1) { \
						data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition); \
					} \
					if (data.enumName == EGetActorList::EnumName) { \
						UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
						return Class::FunctionName(actor, data.EnumName, interactComponent); \
					} \
				} \
				if (interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.enumNameCore == ECheckListCore::andFunction) { \
					for (int r = 0; r < interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.andFunction.conditions.Num(); r++) { \
						if (getActorCheckFunctions.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumName).IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataCheck(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
						else if (getActorCheckFunctionsCore.FindId(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.andFunction.conditions[r].enumNameCore).IsValidId()) { \
							FFunctionActorData data = { EGetActorList::getThisActor, {}, { true } }; \
							if (extraData.actorNum == 0) { \
								data = interactComponent->GetFunctionActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							else if (extraData.actorNum == 1) { \
								data = interactComponent->GetFunctionCurrentActorDataCheckCore(interactComponent->interactDataArray[currentSta].capsuleTriggerStruct.exitFunction[functionIn].ifFunction.condition.andFunction.conditions[r]); \
							} \
							if (data.enumName == EGetActorList::EnumName) { \
								UInteractComponent::UpdateVariables(reinterpret_cast<void*>(&data.EnumName), data.EnumName.customVariableSet, StructName::StaticStruct(), interactComponent); \
								return Class::FunctionName(actor, data.EnumName, interactComponent); \
							} \
						} \
					} \
				} \
			} \
		} \
	} \
	else { \
		return nullptr; \
	} \
} \
return nullptr; \
} \

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDACTIONSFINALCORE BINDACTION(BINDACTIONSCORE)

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDCHECKSFINALCORE BINDCHECK(BINDCHECKSCORE)

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDACTIONSFINAL BINDACTION(BINDACTIONS)

//ADD BIND ACTION FOR EACH FUNCTION OF THE ENUM
#define BINDCHECKSFINAL BINDCHECK(BINDCHECKS)


#define BINDACTORSFINAL BINDACTOR(BINDACTORS)

//BIND ENUM USED IN STRUCTS
#define BINDENUMSFINAL BINDENUM(BINDENUMS)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISM_API UInteractMacrosClass : public UInteractExtraClass
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractMacrosClass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		AActor* GetCustomActor(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, FExtraActorData extraData = FExtraActorData( false, 0 ));
	UFUNCTION()
		AActor* GetSpecificActor(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, FExtraActorData extraData = FExtraActorData(false, 0));
	UFUNCTION()
		AActor* GetThisActor(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, FExtraActorData extraData = FExtraActorData(false, 0));

	UFUNCTION()
		void DestroyEntity(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void PlaySequence(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void IfFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ExecuteAction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void JumpStage(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void DebugText(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ExternalInteract(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void JumpFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void CancelJumpStage(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void BreakFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void DelayFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void TeleportTo(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void RotateLookAtActor(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void PlayAnimCharacter(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetVisibilityActor(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetTransformActor(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void MoveToFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void RotateToFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void CancelTimer(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void CreateEditVariable(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SpawnerFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetRagdoll(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ShowHideTooltip(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void AddImpulseFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetCollisionSettings(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SlowTimeFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void AttachToObject(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void PlaySoundFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void StopSoundFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ModifySoundFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void CameraEffect(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void OpenLevelFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ChangePriority(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ExecuteTrigger(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void AddWidgetToViewport(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void RemoveWidgetFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetPhysicsSettings(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void AddTagFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetMaterialMesh(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void GetExternalVariable(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetTooltipLocationSize(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void RelocateFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void PlayWidgetAnimation(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ShowHideOutlineShine(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void GetInternalVariable(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ExecCommand(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void SetInternalVariable(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void ExecuteInternalFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void WhenFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void CallExternalDelegateFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void LoadStreamLevelInteract(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void UnloadStreamLevelInteract(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);

	UFUNCTION()
		bool DebugCondition(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool DecrementIncrement(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool AndFunction(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool CheckActorLocation(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool CheckActorRotation(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool CheckVariableValue(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool CheckRaycast(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool CheckTooltipVisibility(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool CheckIfActorExists(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);
	UFUNCTION()
		bool CheckIfSoundIsPlaying(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);

	TSet<EActionListCore> getActorFunctionsCore;
	TSet<ECheckListCore> getActorCheckFunctionsCore;

	TSet<EActionList> getActorFunctions;
	TSet<ECheckList> getActorCheckFunctions;

	static TArray<FString> actorBaseFunctionName;
	static TArray<FString> actorCheckBaseFunctionName;
	static TArray<FString> enumFunctions;

	//If Function have one actor (also if have more)
	FFunctionActorData GetFunctionActorDataCore(FFunctionData functionData);

	FFunctionActorData GetFunctionActorDataWrapperCore(FFunctionDataWrapper functionData);

	FFunctionActorData GetFunctionActorDataBaseCore(FFunctionDataBase functionData);

	FFunctionActorData GetFunctionActorDataCheckCore(FFunctionCheckDataBase functionData);

	//If Function have other actor
	FFunctionActorData GetFunctionCurrentActorDataCore(FFunctionData functionData);

	FFunctionActorData GetFunctionCurrentActorDataWrapperCore(FFunctionDataWrapper functionData);

	FFunctionActorData GetFunctionCurrentActorDataBaseCore(FFunctionDataBase functionData);

	FFunctionActorData GetFunctionCurrentActorDataCheckCore(FFunctionCheckDataBase functionData);

	void CallDelegateCoreFunction(EActionListCore tempEnumNameCore, AActor* actor, FFunctionDataBase function, UInteractComponent* interactComponent);

};
