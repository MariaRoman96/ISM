
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
	ALevelManager();

protected:
	virtual void BeginPlay() override;


public:	
	static float fps;

	static ALevelManager* levelManager;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		bool GetShowDebugMessages();

	UFUNCTION(BlueprintCallable, Category = "ISM - Core")
		static float GetFPS();
};
