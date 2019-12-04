#include "CustomGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "UObjectIterator.h"
#include "InteractCore.h"
#include "InteractComponent.h"
#include "ChPlayer.h"
#include <GameFramework/GameModeBase.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerInterface.h"

ACustomGameMode::ACustomGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Assets/Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACustomGameMode::StartPlay()
{
	for (TObjectIterator<AActor> acto; acto; ++acto)
	{
		if (ISM_ISVALID(acto) && ISM_ISVALID(acto->GetWorld())) {
			UInteractComponent* intComp = Cast<UInteractComponent>(acto->GetComponentByClass(UInteractComponent::StaticClass()));
			if (intComp) {
				if (!(intComp->interactDataArrayString != "" && intComp->interactDataArrayString != "NOT_VALUE" && intComp->interactDataArray.Num() == 0)) {
					intComp->ExportInteractDataInternal();
				}
			}
		}
	}
	Super::StartPlay();
	LoadGameISM();

}

void ACustomGameMode::LoadGameISM()
{
	for (TObjectIterator<AActor> acto2; acto2; ++acto2)
	{
		if (ISM_ISVALID(acto2) && ISM_ISVALID(acto2->GetWorld())) {
			IPlayerInterface* player = Cast<IPlayerInterface>(UGameplayStatics::GetPlayerCharacter(acto2->GetWorld(), 0));
			if (player) {
				player->ExecuteNearTriggers();
				break;
			}
		}
	}
}
