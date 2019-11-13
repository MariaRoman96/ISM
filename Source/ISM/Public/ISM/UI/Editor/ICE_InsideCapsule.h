// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ICE_Stage.h"
#include "ICE_Capsule.h"
#include <CheckBox.h>
#include "ICE_InsideCapsule.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_InsideCapsule : public UICE_Capsule
{
	GENERATED_BODY()
public:

	void GetData();
	void UpdateData();

// 	UPROPERTY()
// 		UICE_Stage* stage;

	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetShining();
};
