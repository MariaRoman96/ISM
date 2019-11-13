// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <ComboBoxString.h>
#include <HorizontalBox.h>
#include "InteractComponent.h"
#include "ICE_Stage.h"
#include <ScrollBox.h>
#include <ExpandableArea.h>
#include <CheckBox.h>
#include <Image.h>
#include <EditableTextBox.h>
#include "ICE_Button.h"
#include "ICE_BasicFunctionArray.h"
#include "ICE_Tooltip.generated.h"

//struct FSlateBrush;

UCLASS()
class ISM_API UICE_Tooltip : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void UpdateImageSelector();

	void UpdateBackgroundSelector();
	UFUNCTION()
		void UpdateBackground(int index);

	void GetAllData();
	void GetData();
	void UpdateData();

	UFUNCTION(BlueprintCallable)
		void OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectedType);
	UFUNCTION(BlueprintCallable)
		void OnExpansionChanged(UExpandableArea* Area, bool bIsExpanded);
	UFUNCTION(BlueprintCallable)
		void OnExpansionChangedQTE(UExpandableArea* Area, bool bIsExpanded);

	UFUNCTION()
		void UpdateImage(int index);

	//VARIABLE ACCESS
	UFUNCTION(BlueprintImplementableEvent)
		UScrollBox* GetBackgroundHolderScroll();
	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetBackgroundSelector();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetSearchBackgroundText();


	UFUNCTION(BlueprintImplementableEvent)
		UScrollBox* GetImageHolderScroll();
	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetImageSelector();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetSearchText();
	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetHoldDownButtonHolder();

	UFUNCTION()
		void OnTextChanged(const FText& text);

	UFUNCTION()
		void OnTextBackgroundChanged(const FText& text);

	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetStageFunction();
	UPROPERTY()
		UICE_Button* stageButton;
	UPROPERTY()
		UICE_BasicFunctionArray* stageFunc;

	UFUNCTION(BlueprintImplementableEvent)
		UImage* GetPreviewBg();
	UFUNCTION(BlueprintImplementableEvent)
		UImage* GetPreviewIcon();
	UFUNCTION(BlueprintImplementableEvent)
		UImage* GetPreviewButton();
	UFUNCTION(BlueprintImplementableEvent)
		UComboBoxString* GetActionButton();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetWidgetInScreen();
	UFUNCTION(BlueprintImplementableEvent)
		FVector GetWidgetLocation();
	UFUNCTION(BlueprintImplementableEvent)
		FVector GetWidgetSize();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetNextStage();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetNextStageDelay();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetHoldDownButton();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetNextFailStage();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetUpdateTimePressing();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetTimePressing();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetHideTooltip();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetHideOutline();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetShowTooltip();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetInteractIfHide();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetTooltipSpeed();
	
	//QTE
	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetQTEButtonHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetQTEButton();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetNextFailStageQTE();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetDelayQTE();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetQuickEventTime();


	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetTooltipOptions();

	UFUNCTION(BlueprintImplementableEvent)
		bool SetWidgetLocation(FVector vector);

	UFUNCTION(BlueprintImplementableEvent)
		bool SetWidgetSize(FVector vector);

	UPROPERTY()
		TArray<UObject*> textures2D;

	UPROPERTY()
		UICE_Button* button;
	UPROPERTY()
		UICE_Stage* stageData;
	UPROPERTY()
		int stage = 0;
	UPROPERTY()
		int tooltipIndex = 0;
// 	UPROPERTY()
// 		UInteractComponent* interactComponent;

};
