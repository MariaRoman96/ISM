// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractExtraClass.h"



// Sets default values for this component's properties
UInteractExtraClass::UInteractExtraClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractExtraClass::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractExtraClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

