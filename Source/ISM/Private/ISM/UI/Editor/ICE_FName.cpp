// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_FName.h"

void UICE_FName::NativeConstruct()
{

}

FString UICE_FName::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_FName::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetFNameHolder()->SetToolTipText(FText::FromString(tooltip));
}

void UICE_FName::SetValue(FName val)
{
	GetICEContainer()->SetText(FText::FromString(val.ToString()));
}
