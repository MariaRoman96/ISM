#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Button.h>
#include <DelegateCombinations.h>
#include <WidgetSwitcher.h>
#include "InteractComponent.h"
#include <Image.h>
#include "InteractCore.h"
#include "Engine/Level.h"
#include "ICE_Button.generated.h"

class UICE_BasicFunctionArray;
class UICE_BasicFunction;
class UICE_Tooltip;
class UICE_Stage;
class UICE_Wrapper;
class UICE_InteractMonitor;
class UICE_InteractLog;


UENUM(BlueprintType)
enum class ETargetInfo : uint8
{
	E_None 					UMETA(DisplayName = "None"),
	E_Select				UMETA(DisplayName = "Select"),
	E_Create				UMETA(DisplayName = "Create"),
	E_Delete				UMETA(DisplayName = "Delete"),
	E_Insert				UMETA(DisplayName = "Insert"),
	E_Copy					UMETA(DisplayName = "Copy"),
	E_Paste					UMETA(DisplayName = "Paste"),
	E_ResetToBP				UMETA(DisplayName = "ResetToBP"),
	E_SetBP					UMETA(DisplayName = "SetBP"),
	E_Search				UMETA(DisplayName = "Search"),
	E_Open					UMETA(DisplayName = "Open"),
	E_Close					UMETA(DisplayName = "Close"),
	E_Clear					UMETA(DisplayName = "Clear"),
	E_Filter				UMETA(DisplayName = "Filter"),
	E_Execute				UMETA(DisplayName = "Execute"),
	E_SelectAll				UMETA(DisplayName = "SelectAll"),
	E_Undo					UMETA(DisplayName = "Undo"),
	E_Redo					UMETA(DisplayName = "Redo"),
	E_AddInteract			UMETA(DisplayName = "AddInteract"),
};

USTRUCT()
struct FTargetData {
	GENERATED_USTRUCT_BODY()

public:
	FTargetData() { };
	FTargetData(ETargetInfo inf, int ind, UUserWidget* wrap, FString navBN)
	{ 
		info = inf; 
		index = ind; 
		wrapper = wrap; 
		navBarName = navBN;
	};
	FTargetData(ETargetInfo inf, int ind, UUserWidget* wrap, FString navBN, EActionStageType functionState1, int functionIndex1, int functionAddIndex1 = 0)
	{
		info = inf;
		index = ind;
		wrapper = wrap;
		navBarName = navBN;
		functionState = functionState1;
		functionIndex = functionIndex1;
		functionAddIndex = functionAddIndex1;
	};

	ETargetInfo info;
	int index;
	UUserWidget* wrapper;
	FString navBarName;
	EActionStageType functionState;
	int functionIndex = 0;
	int functionAddIndex;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateButton, FTargetData, data);

UCLASS()
class ISM_API UICE_Button : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FTimerHandle loadingHandle;

	UICE_BasicFunction* AddBasicFunctElement(UICE_Stage* stage, UICE_BasicFunctionArray* basicFuncArray, EActionStageType locationStage, int index, int interactIndex = -1, bool addEnd = true);

	UICE_Tooltip* AddTooltipElement(UICE_Stage* stage, int index, bool addEnd = true, EAction action = EAction::None, UICE_Button* buttonInsert = nullptr);

	UICE_Stage* AddStageElement(UICE_Wrapper* wrapper, int index, bool addEnd = true, UICE_Button* buttonInsert = nullptr);

	UFUNCTION(BlueprintCallable)
		void onClick();
	UFUNCTION()
		void ButtonAction(FTargetData data);

	static bool isCopiedFunctionData;
	static FFunctionData copiedFunctionData;
	static UICE_BasicFunction* copiedFunctionDataWidget;

	static bool isCopiedArrayFunctionData;
	static TArray<FFunctionData> copiedArrayFunctionData;
	static UICE_BasicFunctionArray* copiedArrayFunctionDataWidget;

	static bool isCopiedInteractData;
	static TArray<FInteractData> copiedInteractData;

	static TArray<FString> undoInteractData;
	static int undoInteractDataIndex;

	static bool isCopiedStageData;
	static FInteractData copiedStageData;
	static UICE_Stage* copiedStageDataWidget;

	static bool isCopiedTooltipData;
	static FTooltipData copiedTooltipData;
	static UICE_Tooltip* copiedTooltipDataWidget;

	UPROPERTY()
		FTargetData m_data;
	UPROPERTY()
		UWidgetSwitcher* targetContent = nullptr;
	UPROPERTY()
		FDelegateButton delegate;

	void CreateArrayElement(void * dir2, UArrayProperty* arrProp);

	void DeleteAllArrayElement(void * dir2, UArrayProperty* arrProp);

	void InsertArrayElement(void * dir2, UArrayProperty* arrProp, int index);

	void DeleteArrayElement(void * dir2, UArrayProperty* arrProp, int index);

	//VARIABLE ACCESS
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetButtonName();
	UFUNCTION(BlueprintImplementableEvent)
		UButton* GetButton();
	UFUNCTION(BlueprintImplementableEvent)
		UImage* GetIcon();

	UFUNCTION(BlueprintImplementableEvent)
		FFunctionData InitFunctionData();
	UFUNCTION(BlueprintImplementableEvent)
		FInteractData InitInteractData();
	UFUNCTION(BlueprintImplementableEvent)
		FTooltipData InitTooltipData();

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

	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeOrange();
	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeDarkGrey();
	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeMidGrey();
	UFUNCTION(BlueprintImplementableEvent)
		bool ChangeLightGrey();

};
