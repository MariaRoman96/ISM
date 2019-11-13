// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ICE_FunctionProperties.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_FunctionProperties : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetICEContainer();
};
