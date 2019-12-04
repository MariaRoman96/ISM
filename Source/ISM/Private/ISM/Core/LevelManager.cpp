#include "LevelManager.h"
#include "InteractComponent.h"
#include "InteractCore.h"

ALevelManager* ALevelManager::levelManager = nullptr;

float ALevelManager::fps = 0.f;

ALevelManager::ALevelManager()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("LevelManager");
	ALevelManager::levelManager = this;
}

void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps = 1.f / DeltaTime;
}

float ALevelManager::GetFPS()
{
	return fps;
}