// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScript.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LevelScript, Log, All);

/**
 * 
 */
UCLASS()
class ALevelScript : public ALevelScriptActor
{
	GENERATED_UCLASS_BODY()

		/** A Custom name for each Level, specified in Level BP Default Properties! */
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Level)
		FName LevelName;

	//===========
	// BP Nodes
	//===========
public:
	/** This triggers a save / update of all the Save Game level data. This should be called before the level is unloaded/unstreamed via blueprints.*/
	UFUNCTION(BlueprintCallable, Category = "Level Save System")
		void Level__UpdateLevelSaveData();


	//OVERRIDES
protected:

	ALevelScript(const class FPostConstructInitializeProperties& PCIP);
	virtual void ReceiveBeginPlay();

	//ON REMOVE FROM WORLD - USE INSTEAD OF BEGIN DESTROY
	/** Called when this actor is in a level which is being removed from the world (e.g. my level is getting UWorld::RemoveFromWorld called on it) */
	/*
	virtual void OnRemoveFromWorld() override;
	*/
	//THIS GOT REPLACED BY

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//TICK
protected:

	//Tick
	virtual void Tick(float DeltaSeconds) override;

};