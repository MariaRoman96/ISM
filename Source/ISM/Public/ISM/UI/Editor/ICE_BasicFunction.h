
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <ComboBoxString.h>
#include "InteractComponent.h"
#include "ICE_FunctionProperties.h"
#include "ICE_Stage.h"
#include <UnrealType.h>
#include <ExpandableArea.h>
#include "ICE_ActorFunction.h"
#include "ICE_Array.h"
#include "ICE_Search.h"
#include "ICE_BasicFunction.generated.h"

class UICE_BasicFunctionArray;

UENUM(BlueprintType)
enum class EStateFunctionList : uint8
{
	None 						UMETA(DisplayName = "None"),
	PreStage 					UMETA(DisplayName = "PreStage"),
	Init 						UMETA(DisplayName = "Init"),
	Exit 						UMETA(DisplayName = "Exit"),
	Action 						UMETA(DisplayName = "Action"),
};

USTRUCT(BlueprintType)
struct FCurrentState
{
	GENERATED_USTRUCT_BODY()
public:
	FCurrentState() {}

	FCurrentState(EStateFunctionList state1, int actionIndex1) {
		state = state1;
		actionIndex = actionIndex1;
	}

	EStateFunctionList state;

	int actionIndex = -1;

};

/**
 * 
 */
UCLASS()
class ISM_API UICE_BasicFunction : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
		TArray<UICE_ArrayElement*> actorArr;

	UPROPERTY()
		TArray<UICE_ActorFunction*> actorArr2;

	UPROPERTY()
		UICE_BasicFunctionArray* parentArr;

	UPROPERTY()
		UICE_Stage* stage;
	UPROPERTY()
		int currentStage;
	UPROPERTY()
		FCurrentState currentState;
	UPROPERTY()
		int currentIndex;

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEButtonOptions();

	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetICEExpandable();

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICEName();

	UFUNCTION()
		FString GetName();

	UFUNCTION()
		void SetName(FString val);

	UFUNCTION(BlueprintImplementableEvent)
		UComboBoxString* GetICEEnum();

	UFUNCTION(BlueprintImplementableEvent)
		UICE_Search* GetICESearchBar();

	UFUNCTION(BlueprintImplementableEvent)
		FFunctionActorData InitFunctionActorData();

	UFUNCTION(BlueprintImplementableEvent)
		FFunctionCheckData InitFunctionCheckData();

	UFUNCTION(BlueprintImplementableEvent)
		FFunctionCheckDataBase InitFunctionCheckDataBase();

	UFUNCTION(BlueprintImplementableEvent)
		FFunctionDataWrapper InitFunctionDataWrapper();

	UFUNCTION(BlueprintImplementableEvent)
		FFunctionDataBase InitFunctionDataBase();

	UFUNCTION(BlueprintImplementableEvent)
		FBaseCreateEditVariable InitBaseCreateEditVariable();

// 	UFUNCTION(BlueprintImplementableEvent)
// 		FConditionAnd InitConditionAnd();

	UFUNCTION()
		void OnChangeSelect(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		FString GetEnumString();

	UFUNCTION()
		EActionList GetEnum();

	UFUNCTION()
		void SetEnum(int val);
	
	UFUNCTION(BlueprintImplementableEvent)
		UICE_FunctionProperties* GetICEFunctionProperties();

	void GetDataInternal(UPanelWidget* panel, void * dir2, UProperty* It2, UStructProperty* parentStruct);

	void GetDataInternalArray(UPanelWidget* panel, void * dir2, UProperty* It2, UArrayProperty* arrProp, UICE_Array* arrayParent);

	void UpdateDataInternal(UWidget* child, void * dir2, UStruct* structVal);

	void UpdateDataInternalArray(UPanelWidget* child, void * dir2, UStruct* structVal, UProperty* It2, UArrayProperty* arrProp);

	UFUNCTION()
		void GetData();

	UFUNCTION()
		void UpdateData();
	
};
