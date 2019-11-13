// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelScript.h"

DEFINE_LOG_CATEGORY(LevelScript);

ALevelScript::ALevelScript(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	LevelName = NAME_None;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Level Begin Play
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Actor.h 		ReceiveBeginPlay()
void ALevelScript::ReceiveBeginPlay()
{
	Super::ReceiveBeginPlay();
	//~~~~~~~~~~

	//UE_LOG(LevelScript,Error,TEXT("STREAMING LEVEL BEGIN PLAY %s"), *GetName());
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Level End Play
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ALevelScript::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	
	//UE_LOG(LevelScript,Error,TEXT("STREAMING LEVEL UNSTREAMED (name should not be none) %s"), *GetName());
	//UE_LOG(LevelScript,Error,TEXT("Victory!!!"));
	//UE_LOG(LevelScript,Error,TEXT("RUN ALL UNSTREAMING LEVEL LOGIC HERE"));
	

	//Save Level Just Before Exiting
	Level__UpdateLevelSaveData();

	//~~~~~~~~~~
}

void ALevelScript::Level__UpdateLevelSaveData()
{
	//Save Level Data
}

void ALevelScript::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//~~~~~~~~~~~~~~

	//UE_LOG(LevelScript,Warning,TEXT("Level Name Is: %s"), LevelName);
}