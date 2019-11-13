// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "InteractCore.h"
#include "LevelManager.generated.h"

class UInteractComponent;

USTRUCT(BlueprintType)
struct FCustom : public FInteractStruct {

	GENERATED_USTRUCT_BODY()
public:
	FCustom() {}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Values"))
		TArray<FString> val;
};


UCLASS()
class ISM_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	static float fps;

	static ALevelManager* levelManager;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		bool GetShowDebugMessages();

	UFUNCTION(BlueprintCallable, Category = "ISM - Core")
		static float GetFPS();
};
