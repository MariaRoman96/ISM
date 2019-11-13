// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <HorizontalBox.h>
#include <VerticalBox.h>
#include <EditableTextBox.h>
#include <DelegateCombinations.h>
#include <ScrollBox.h>
#include "ICE_InteractLog.generated.h"

UENUM(BlueprintType)
enum class ELogHelpers : uint8
{
	Error 						UMETA(DisplayName = "Error"),
	Warning						UMETA(DisplayName = "Warning"),
	Info						UMETA(DisplayName = "Info"),
	Spam						UMETA(DisplayName = "Spam"),
	Permanent 					UMETA(DisplayName = "Permanent"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateLogHelpers);

/**
 * 
 */
UCLASS()
class ISM_API UICE_InteractLog : public UUserWidget
{
	GENERATED_BODY()

public:

	static FDelegateLogHelpers delegateLogHelpers;

	int numRows = 0;

	int numPermanentRows = 0;

	bool showErrors = true;
	bool showWarnings = true;
	bool showInfo = true;
	bool showSpam = true;

	bool showPermanent = true;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void BuildMonitor();


	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetPermanentHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetCommandText();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetExecuteCommand();


	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetClearHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetFilterHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetTableHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetSearch();
	UFUNCTION(BlueprintImplementableEvent)
		UScrollBox* GetScrollHolder();

	UFUNCTION()
		void OnSearchText(const FText& Text);

	UFUNCTION()
		void OnLogAdded();

	UFUNCTION()
		void ClearPermanentData();
	UFUNCTION()
		void GetPermanentData();

	UFUNCTION()
		void ClearData(bool onlyData = false);
	UFUNCTION()
		void GetData(const FText& Text);
	UFUNCTION()
		void AddData(ELogHelpers type, FString text, FString file, FDateTime time);

	UFUNCTION()
		void AddPermanentData(ELogHelpers type, FString text, FString file, FDateTime time);

};
