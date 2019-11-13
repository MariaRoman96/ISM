// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <SpinBox.h>
#include <TextBlock.h>
#include <HorizontalBox.h>
#include "ICE_Float.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_Float : public UUserWidget
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
	USpinBox* GetICEContainer();
	
	UFUNCTION(BlueprintImplementableEvent)
	float GetValue();

	UFUNCTION(BlueprintImplementableEvent)
	void SetValue(float val);

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetFloatHolder();

};
