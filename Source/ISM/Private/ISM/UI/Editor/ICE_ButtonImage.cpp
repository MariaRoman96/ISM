// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_ButtonImage.h"
#include "InteractCore.h"


void UICE_ButtonImage::NativeConstruct()
{
	GetButton()->OnClicked.AddDynamic(this, &UICE_ButtonImage::onClick);
	delegate.AddDynamic(this, &UICE_ButtonImage::ButtonAction);
}

void UICE_ButtonImage::onClick()
{
	delegate.Broadcast(this->m_data, this->m_dataOther, this->basicFun, this->actorFun, this->index);
}

void UICE_ButtonImage::ButtonAction(EButtonTypeInfo data, EFunctionButtonInfo dataOther, UICE_BasicFunction* basicFunc, UICE_ActorFunction* actorFunc, int ind)
{
	if (data == EButtonTypeInfo::E_Texture) {
		tooltip->UpdateImage(ind);
	}
	else if (data == EButtonTypeInfo::E_Background) {
		tooltip->UpdateBackground(ind);
	}
	else if (data == EButtonTypeInfo::E_Select) {
		if (dataOther == EFunctionButtonInfo::E_Basic) {
			basicFunc->UpdateData();
			basicFunc->SetEnum(ind);
			basicFunc->GetICEFunctionProperties()->GetICEContainer()->ClearChildren();
			basicFunc->OnChangeSelect(selector->GetOptionAtIndex(ind), ESelectInfo::OnMouseClick);
		}
		else if (dataOther == EFunctionButtonInfo::E_Complex) {
			//actorFunc->GetICEOtherProperties()->ClearChildren();
			actorFunc->wrapper->UpdateData();
			actorFunc->SetEnum(ind);
			//actorFunc->wrapper->GetData();
			actorFunc->OnChangeSelect(selector->GetOptionAtIndex(ind), ESelectInfo::OnMouseClick);
		}
		else {
			selector->SetSelectedIndex(ind);
		}
		expandable->SetIsExpanded(false);
		expandableName->SetText(FText::FromString(selector->GetOptionAtIndex(ind)));
	}
	if (searchText) {
		searchText->SetText(FText::FromString(""));
	}
}