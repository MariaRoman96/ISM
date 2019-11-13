// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_InsideCapsule.h"

void UICE_InsideCapsule::GetData()
{
	FString selectedColor = GETENUMSTRING(TEXT("EOutlineColor"), stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleColor).Replace(TEXT("E_"), TEXT(""));
	GetOutlineColor()->SetSelectedOption(selectedColor);
	SetSize(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleSize);
	SetLocation(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleLocation);
	SetRotation(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleRotation);
	SetScale(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleScale);
	GetShining()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_isShining);
}

void UICE_InsideCapsule::UpdateData()
{
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleColor = (EOutlineColor)GetOutlineColor()->GetSelectedIndex();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleSize = GetSize();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleLocation = GetLocation();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleRotation = GetRotation();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_CapsuleScale = GetScale();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleInteractStruct.m_isShining = GetShining()->IsChecked();
}
