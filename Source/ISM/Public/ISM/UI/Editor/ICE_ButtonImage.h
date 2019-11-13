// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <Image.h>
#include "ICE_Tooltip.h"
#include <Button.h>
#include <DelegateCombinations.h>
#include "ICE_ButtonImage.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EButtonTypeInfo : uint8
{
	E_None 					UMETA(DisplayName = "None"),
	E_Texture				UMETA(DisplayName = "Texture"),
	E_Select				UMETA(DisplayName = "Select"),
	E_Background			UMETA(DisplayName = "Background"),
};

UENUM(BlueprintType)
enum class EFunctionButtonInfo : uint8
{
	E_None 					UMETA(DisplayName = "None"),
	E_Basic					UMETA(DisplayName = "Basic"),
	E_Complex				UMETA(DisplayName = "Complex"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FDelegateButtonImage, EButtonTypeInfo, data, EFunctionButtonInfo, dataOther, UICE_BasicFunction*, basicFunc, UICE_ActorFunction*, actorFunc, int, ind);

UCLASS()
class ISM_API UICE_ButtonImage : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FDelegateButtonImage delegate;

	UPROPERTY()
		EButtonTypeInfo m_data;

	UPROPERTY()
		EFunctionButtonInfo m_dataOther = EFunctionButtonInfo::E_None;

	UPROPERTY()
		UICE_BasicFunction* basicFun = nullptr;

	UPROPERTY()
		UICE_ActorFunction* actorFun = nullptr;

	UFUNCTION()
		void ButtonAction(EButtonTypeInfo data, EFunctionButtonInfo dataOther, UICE_BasicFunction* basicFunc, UICE_ActorFunction* actorFunc, int ind);

	UPROPERTY()
		UICE_Tooltip* tooltip;

	UPROPERTY()
		UComboBoxString* selector;

	UPROPERTY()
		UExpandableArea* expandable;

	UPROPERTY()
		UTextBlock* expandableName;

	UPROPERTY()
		UEditableTextBox* searchText;

	UPROPERTY()
		int index = 0;

	virtual void NativeConstruct() override;
	UFUNCTION()
		void onClick();

	UFUNCTION(BlueprintImplementableEvent)
		UButton* GetButton();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetTextBlock();
	UFUNCTION(BlueprintImplementableEvent)
		UImage* GetImage();
};
