// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_Vector.h"

void UICE_Vector::NativeConstruct()
{

}

FString UICE_Vector::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_Vector::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetVectorHolder()->SetToolTipText(FText::FromString(tooltip));
}
