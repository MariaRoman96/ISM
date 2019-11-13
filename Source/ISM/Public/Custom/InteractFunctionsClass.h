// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include <functional>
#include <cmath>
#include <DelegateCombinations.h>
#include <limits>
#include <Engine/Engine.h>
#include <LevelSequencePlayer.h>
#include <LevelSequenceActor.h>
#include <Engine/DataTable.h>
#include <Sound/SoundBase.h>
#include <UserWidget.h>
#include "InteractExtraClass.h"
#include "TooltipWidget.h"
#include "LevelManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "InteractMacrosClass.h"
#include "InteractFunctionsClass.generated.h"

using namespace std;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ISM_API UInteractFunctionsClass : public UInteractMacrosClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UInteractFunctionsClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//If Function have one actor (also if have more)
	FFunctionActorData GetFunctionActorData(FFunctionData functionData);

	FFunctionActorData GetFunctionActorDataWrapper(FFunctionDataWrapper functionData);

	FFunctionActorData GetFunctionActorDataBase(FFunctionDataBase functionData);

	FFunctionActorData GetFunctionActorDataCheck(FFunctionCheckDataBase functionData);

	//If Function have other actor
	FFunctionActorData GetFunctionCurrentActorData(FFunctionData functionData);

	FFunctionActorData GetFunctionCurrentActorDataWrapper(FFunctionDataWrapper functionData);

	FFunctionActorData GetFunctionCurrentActorDataBase(FFunctionDataBase functionData);

	FFunctionActorData GetFunctionCurrentActorDataCheck(FFunctionCheckDataBase functionData);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CallDelegateFunction(EActionList tempEnumNameCore, AActor* actor, FFunctionDataBase function, UInteractComponent* interactComponent);


	UFUNCTION()
		bool PlaceholderCheck(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType);


	//DUPLICATE FUNCTION FOR EACH DESIRED ONES, THEY WILL BE IMPLEMENTED DUPLICATING A MACRO IN CPP 
	UFUNCTION()
		void ObjectInspector(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);
	UFUNCTION()
		void CurveLineTrace(UInteractComponent* interactComponent, AActor* actor, EAction actionType, int index, EActionStageType actionStageType, int wrapperIndex = -1, int functionInd = -1, int currentStag = -1, bool inElse = false);

};
