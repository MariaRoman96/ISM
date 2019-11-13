// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <HorizontalBox.h>
#include <TextBlock.h>
#include "ICE_FunctionProperties.h"
#include "ICE_Struct.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_Struct : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEButtonOptions();

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICEName();

	UFUNCTION(BlueprintImplementableEvent)
		UICE_FunctionProperties* GetICEFunctionProperties();
};
