// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_Rotator.h"

void UICE_Rotator::NativeConstruct()
{

}

FString UICE_Rotator::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_Rotator::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetRotatorHolder()->SetToolTipText(FText::FromString(tooltip));
}
