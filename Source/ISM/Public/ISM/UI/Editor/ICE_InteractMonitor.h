// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CheckBox.h"
#include "WidgetSwitcher.h"
#include "ICE_Enum.h"
#include "HorizontalBox.h"
#include "ICE_Object.h"
#include "ICE_InteractMonitor.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_InteractMonitor : public UUserWidget
{
	GENERATED_BODY()
public:


	int searchDataElements = 0;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void BuildMonitor();

	UFUNCTION()
		void ChangeActor(int index, AActor* actor);

	UFUNCTION()
		void OnShowDebugChange(bool IsChecked);

	UFUNCTION()
		void OnDisableWarningChange(bool IsChecked);

	UFUNCTION()
		void OnShowInteractChange(bool IsChecked);

	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetShowDebug();
	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetOnlyInteract();

	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetDisableWarnings();

// 	UFUNCTION(BlueprintImplementableEvent)
// 		UHorizontalBox* GetEnumButton();
	UFUNCTION(BlueprintImplementableEvent)
		UICE_Enum* GetEnum();
// 	UFUNCTION(BlueprintImplementableEvent)
// 		UHorizontalBox* GetEnumCheckButton();
	UFUNCTION(BlueprintImplementableEvent)
		UICE_Enum* GetEnumCheck();
	UFUNCTION(BlueprintImplementableEvent)
		UBorder* GetSearchResumeHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetActorsFound();
	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetTableContent();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetActorName();

	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetLevelName();

	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetActorTag();
// 	UFUNCTION(BlueprintImplementableEvent)
// 		UHorizontalBox* GetActorNameButton();
	UFUNCTION(BlueprintImplementableEvent)
		UICE_Object* GetActorClass();
// 	UFUNCTION(BlueprintImplementableEvent)
// 		UHorizontalBox* GetActorClassButton();

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetEnumHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetEnumCheckHolder();

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetSearchButton();

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetSelectAllButton();

	UFUNCTION()
		bool GetCheckedValue();

	UFUNCTION()
		UObject* GetActorValue();

	UFUNCTION()
		void ClearSearch();
	UFUNCTION()
		void AddSearchData(FString actorName, FString worldName, FString levelName, AActor* actor, FString procedence, ULevel* level);
};
