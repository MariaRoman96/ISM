// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <HorizontalBox.h>
#include <TextBlock.h>
#include "ICE_FunctionProperties.h"
#include <ComboBoxString.h>
#include "InteractComponent.h"
#include "ICE_Search.h"
#include <VerticalBox.h>
#include "ICE_ActorFunction.generated.h"

class UICE_BasicFunction;
class UICE_ArrayElement;

/**
 * 
 */
UCLASS()
class ISM_API UICE_ActorFunction : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	FString realName = "";

	FString functionType = "";

	UPROPERTY()
		UICE_ArrayElement* arrElem = nullptr;

	UPROPERTY()
		UICE_BasicFunction* wrapper;

	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetICEExpandable();

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEButtonOptions();

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICEName();

	UFUNCTION(BlueprintImplementableEvent)
		UComboBoxString* GetICEEnum();

	UFUNCTION(BlueprintImplementableEvent)
		UICE_Search* GetICESearchBar();

	UFUNCTION()
		void OnChangeSelect(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		FString GetEnumString();

	UFUNCTION()
		int GetEnum();

	UFUNCTION()
		void SetEnum(int val);

	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetICEOtherProperties();

	UFUNCTION(BlueprintImplementableEvent)
		UICE_FunctionProperties* GetICEFunctionProperties();
};
