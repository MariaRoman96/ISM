// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <EditableTextBox.h>
#include <TextBlock.h>
#include <HorizontalBox.h>
#include "ICE_String.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_String : public UUserWidget
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
		FString GetValue();

	UFUNCTION()
		void SetValue(FString val);

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetStringHolder();

};
