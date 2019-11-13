// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <VerticalBox.h>
#include "ICE_BasicFunction.h"
#include <ExpandableArea.h>
#include "ICE_BasicFunctionArray.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_BasicFunctionArray : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY()
		TArray<UICE_BasicFunction*> basicFunctions;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEButtonOptions();

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICEName();

	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetICEContent();

	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetICEExpandable();

	UPROPERTY()
		UICE_Stage* stage;
};
