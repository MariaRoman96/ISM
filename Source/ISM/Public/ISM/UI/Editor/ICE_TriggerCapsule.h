// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Editor/ICE_Capsule.h"
#include "ICE_BasicFunctionArray.h"
#include "ICE_Button.h"
#include "ICE_Object.h"
#include "ICE_FName.h"
#include "ICE_TriggerCapsule.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_TriggerCapsule : public UICE_Capsule
{
	GENERATED_BODY()
public:

	void GetData();
	void UpdateData();

// 	UPROPERTY()
// 		TArray<UObject*> actorClasses;

	UPROPERTY()
		UICE_Stage* stage;

	UPROPERTY()
		UICE_BasicFunctionArray* initFunc;

	UPROPERTY()
		UICE_BasicFunctionArray* exitFunc;

	UPROPERTY()
		UICE_Button* initButton;
	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetInitContent();
	UPROPERTY()
		UICE_Button* exitButton;
	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetExitContent();
	UFUNCTION(BlueprintImplementableEvent)
		UICE_Object* GetActorClass();
	UFUNCTION(BlueprintImplementableEvent)
		UICE_FName* GetActorTag();
};
