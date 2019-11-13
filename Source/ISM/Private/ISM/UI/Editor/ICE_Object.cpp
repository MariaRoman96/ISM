// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_Object.h"

void UICE_Object::NativeConstruct()
{
	//GetICEContent()->OnSelectionChanged.AddDynamic(this, &UICE_Object::OnChangeSelect);
}

FString UICE_Object::GetName()
{
	return GetICEName()->Text.ToString();
}

void UICE_Object::SetName(FString val, FString tooltip)
{
	GetICEName()->SetText(FText::FromString(val));
	GetObjectHolder()->SetToolTipText(FText::FromString(tooltip));
}

void UICE_Object::SetValue(UObject* value)
{
	if (value) {
		GetICEContent()->SetSelectedOption(value->GetName());
	}
	else {
		GetICEContent()->SetSelectedOption("None");
	}
}

UObject* UICE_Object::GetValue()
{
	if (elementArray.IsValidIndex(GetICEContent()->GetSelectedIndex())) {
		return elementArray[GetICEContent()->GetSelectedIndex()];
	}
	else {
		return nullptr;
	}
}

// void UICE_Object::OnChangeSelect(FString SelectedItem, ESelectInfo::Type SelectionType)
// {
// 
// }
