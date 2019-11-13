// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <EditableTextBox.h>
#include <HorizontalBox.h>
#include "ICE_FName.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_FName : public UUserWidget
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
		UEditableTextBox* GetICEContainer();

	UFUNCTION(BlueprintImplementableEvent)
		FName GetValue();

	UFUNCTION()
		void SetValue(FName val);

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetFNameHolder();

};
