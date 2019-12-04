#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractComponent.h"
#include "ICE_Wrapper.h"
#include <EditableTextBox.h>
#include <SlateEnums.h>
#include <Text.h>
#include "ICE_Button.h"
#include "ICE_Stage.generated.h"

class UICE_Button;
class UICE_BasicFunction;
class UICE_Enum;
class UICE_Boolean;

UCLASS()
class ISM_API UICE_Stage : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY()
		int stage = 0;
	UPROPERTY()
		UICE_Wrapper* wrapper;
	UPROPERTY()
		UICE_Button* button;

	UPROPERTY()
		UICE_Button* preStageButton;
	UPROPERTY()
		TArray<UICE_BasicFunction*> preStageFunctions;

	void GetPreStageData();

	void GetInitData();

	void GetExitData();

	void GetStageData(int stIndex);

	void GetTooltipData();

	void GetAllData();

	void GetData();

	void UpdateData();

	UFUNCTION(BlueprintCallable)
		void OnTextChanged(const FText& text);


	UPROPERTY()
		TArray<UICE_Button*> capsuleNavBarButtons;
	UPROPERTY()
		TArray<UICE_Button*> tooltipNavBarButtons;
	UFUNCTION(BlueprintCallable)
		void UpdateCapsuleNavBar(int currentTab);
	UFUNCTION(BlueprintCallable)
		void UpdateTooltipNavBar(int currentTab);

	//VARIABLE ACCESS
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetCapsuleNavBar();
	UFUNCTION(BlueprintImplementableEvent)
		UWidgetSwitcher* GetCapsuleContent();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetTooltipNavBar();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetTooltipNavBarAdd();
	UFUNCTION(BlueprintImplementableEvent)
		UWidgetSwitcher* GetTooltipContent();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetStageTitle();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetStageDescription();
	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetPreStageFunctions();
	UFUNCTION(BlueprintImplementableEvent)
		UICE_Enum* GetPriority();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetPriorityRepeat();


	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetStageOptions();
};
