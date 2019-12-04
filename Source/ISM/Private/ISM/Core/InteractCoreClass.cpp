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


UInteractCoreClass::UInteractCoreClass()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UInteractCoreClass::BeginPlay()
{
	Super::BeginPlay();
	
}


void UInteractCoreClass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

