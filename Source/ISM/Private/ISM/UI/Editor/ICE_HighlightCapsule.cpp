// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_HighlightCapsule.h"

void UICE_HighlightCapsule::GetData()
{
	FString selectedColor = GETENUMSTRING(TEXT("EOutlineColor"), stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleColor).Replace(TEXT("E_"), TEXT(""));
	GetOutlineColor()->SetSelectedOption(selectedColor);
	SetSize(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleSize);
	SetLocation(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleLocation);
	SetRotation(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleRotation);
	SetScale(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleScale);
	//GetTimeShining()->SetText(FText::FromString(FString::SanitizeFloat(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_shiningIntroSpeed)));
	GetShining()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_isShining);
}

void UICE_HighlightCapsule::UpdateData()
{
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleColor = (EOutlineColor)GetOutlineColor()->GetSelectedIndex();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleSize = GetSize();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleLocation = GetLocation();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleRotation = GetRotation();
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_CapsuleScale = GetScale();
	//stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_shiningIntroSpeed = UInteractCore::GetValue(GetTimeShining()->Text.ToString());
	stageData->wrapper->interactComponent->interactDataArray[stageData->stage].capsuleHighlightStruct.m_isShining = GetShining()->IsChecked();
}
