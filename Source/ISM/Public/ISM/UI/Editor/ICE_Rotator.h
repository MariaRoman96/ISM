// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <HorizontalBox.h>
#include <GridPanel.h>
#include "ICE_Rotator.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_Rotator : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICENameContainer();

	UFUNCTION()
		FString GetName();

	UFUNCTION()
		void SetName(FString val, FString tooltip = "");

	UFUNCTION(BlueprintImplementableEvent)
		UGridPanel* GetICEContainer();

	UFUNCTION(BlueprintImplementableEvent)
		FRotator GetValue();

	UFUNCTION(BlueprintImplementableEvent)
		void SetValue(FRotator val);

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetRotatorHolder();

};
