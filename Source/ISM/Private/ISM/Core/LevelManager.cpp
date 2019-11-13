// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "InteractComponent.h"
#include "InteractCore.h"

ALevelManager* ALevelManager::levelManager = nullptr;

float ALevelManager::fps = 0.f;

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("LevelManager");
	ALevelManager::levelManager = this;
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps = 1.f / DeltaTime;
}

float ALevelManager::GetFPS()
{
	return fps;
}