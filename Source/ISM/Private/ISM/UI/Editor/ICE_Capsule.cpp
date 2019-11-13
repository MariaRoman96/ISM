#include "ICE_Capsule.h"
#include "InteractCore.h"
#include <UnrealType.h>
#include "ICE_Stage.h"

void UICE_Capsule::NativeConstruct()
{
	int maxE = (int)GETENUMMAX(TEXT("EOutlineColor"));
	for (int fooInt = 0; fooInt != maxE; fooInt++)
	{
		EOutlineColor foo = static_cast<EOutlineColor>(fooInt);
		FString value = GETENUMSTRING(TEXT("EOutlineColor"), foo);
		GetOutlineColor()->AddOption(value.Replace(TEXT("E_"), TEXT("")));
	}

	GetOutlineColor()->SetSelectedOption("Blue");
}

void UICE_Capsule::RemoveOutlineColorOption()
{
	GetOutlineColorBlock()->RemoveFromParent();
}

void UICE_Capsule::GetData()
{
// 	FString selectedColor;
// 	if (capsuleIndex == 1)
// 	{
// 		selectedColor = GETENUMSTRING(TEXT("EOutlineColor"), stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleColor).Replace(TEXT("E_"), TEXT(""));
// 		GetOutlineColor()->SetSelectedOption(selectedColor);
// 		SetSize(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleSize);
// 		SetLocation(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleLocation);
// 		SetRotation(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleRotation);
// 		SetScale(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleScale);
// 	}
// 	else if (capsuleIndex == 2)
// 	{
// 		selectedColor = GETENUMSTRING(TEXT("EOutlineColor"), stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleColor).Replace(TEXT("E_"), TEXT(""));
// 		GetOutlineColor()->SetSelectedOption(selectedColor);
// 		SetSize(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleSize);
// 		SetLocation(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleLocation);
// 		SetRotation(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleRotation);
// 		SetScale(stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleScale);
// 	}

// 	else if (capsuleIndex == 0) {
// 		SetSize(interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleSize);
// 		SetLocation(interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleLocation);
// 		SetRotation(interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleRotation);
// 		SetScale(interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleScale);
// 	}
// 	GetOutlineColor()->SetSelectedOption(selectedColor);

}

void UICE_Capsule::UpdateData()
{
// 	if (capsuleIndex == 1)
// 	{
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleColor = (EOutlineColor)GetOutlineColor()->GetSelectedIndex();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleSize = GetSize();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleLocation = GetLocation();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleRotation = GetRotation();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleInteractStruct.m_CapsuleScale = GetScale();
// 	}
// 	else if (capsuleIndex == 2) {
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleColor = (EOutlineColor)GetOutlineColor()->GetSelectedIndex();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleSize = GetSize();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleLocation = GetLocation();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleRotation = GetRotation();
// 		stageData->wrapper->interactComponent->interactDataArray[stageIndex].capsuleHighlightStruct.m_CapsuleScale = GetScale();
// 	}

// 	else if (capsuleIndex == 0) {
// 		interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleSize = GetSize();
// 		interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleLocation = GetLocation();
// 		interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleRotation = GetRotation();
// 		interactComponent->interactDataArray[stage].capsuleTriggerStruct.m_CapsuleScale = GetScale();
// 	}
// 	interactComponent->UpdateInteractArrayData();
}
