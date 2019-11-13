// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractCoreClass.h"
#include "InteractComponent.h"


void UInteractCoreClass::BindAction(const FName& functionName)
{
	FDelegate del;
	del.BindUFunction(this, functionName);
	interactFunctionMap.Push(del);
}

void UInteractCoreClass::BindCheck(const FName& functionName)
{
	FDelegateBool del;
	del.BindUFunction(this, functionName);
	checkFunctionMap.Push(del);
}

void UInteractCoreClass::BindActor(const FName& functionName)
{
	FDelegateActor del;
	del.BindUFunction(this, functionName);
	actorFunctionMap.Push(del);
}


// Sets default values for this component's properties
UInteractCoreClass::UInteractCoreClass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInteractCoreClass::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractCoreClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

