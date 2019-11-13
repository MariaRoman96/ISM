// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_String.h"

void UICE_String::NativeConstruct()
{

}

FString UICE_String::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_String::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetStringHolder()->SetToolTipText(FText::FromString(tooltip));
}

void UICE_String::SetValue(FString val)
{
	GetICEContainer()->SetText(FText::FromString(val));
}
