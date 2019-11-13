// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <ComboBoxString.h>
#include <ScrollBox.h>
#include "ICE_Search.h"
#include <HorizontalBox.h>
#include "ICE_Enum.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_Enum : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UICE_Search* GetICESearchBar();

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICENameContainer();

	UFUNCTION()
		FString GetName();

	UFUNCTION()
		void SetName(FString val, FString tooltip = "");

	UFUNCTION(BlueprintImplementableEvent)
		UComboBoxString* GetICEEnum();

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetEnumHolder();

};
