#include "ICE_Boolean.h"
#include "ICE_BasicFunction.h"

void UICE_Boolean::NativeConstruct()
{
	GetICEContainer()->OnCheckStateChanged.AddDynamic(this, &UICE_Boolean::OnChangeSelect);
}

void UICE_Boolean::OnChangeSelect(bool bIsChecked)
{
	basicFunc->UpdateData();
	basicFunc->GetData();
}

FString UICE_Boolean::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_Boolean::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetBooleanHolder()->SetToolTipText(FText::FromString(tooltip));
}