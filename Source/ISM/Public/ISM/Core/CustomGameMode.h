
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

UCLASS(minimalapi)
class ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACustomGameMode();

	void StartPlay() override;

	UFUNCTION(BlueprintCallable, Category = "ISM - Gameplay")
		static void LoadGameISM();
};