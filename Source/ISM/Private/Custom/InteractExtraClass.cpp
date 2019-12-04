#include "InteractExtraClass.h"



UInteractExtraClass::UInteractExtraClass()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInteractExtraClass::BeginPlay()
{
	Super::BeginPlay();
	
}


void UInteractExtraClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

