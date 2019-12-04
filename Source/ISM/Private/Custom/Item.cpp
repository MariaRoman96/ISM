#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ChPlayer.h"
#include <WidgetComponent.h>
#include <UserWidget.h>
#include <ConstructorHelpers.h>
#include "InteractComponent.h"
#include <Kismet/KismetMathLibrary.h>

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	mRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(mRootComp);
	mMesh->ComponentTags.Add(FName(TEXT("light")));

}

void AItem::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (EndPlayReason == EEndPlayReason::EndPlayInEditor) {
		UInteractComponent* intComp = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));
		if (intComp) {
			intComp->delegateAuxFunctions.Empty();
		}
	}
}

void AItem::PostEditMove(bool bFinished)
{
	if (bFinished) {
		UInteractComponent* intComp = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));
		if (intComp) {
			intComp->UpdateStageAtributes();
		}
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	InteractComponent = Cast<UInteractComponent>(GetComponentByClass(UInteractComponent::StaticClass()));

}
