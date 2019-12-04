

#include "ICE_Color.h"

void UICE_Color::NativeConstruct()
{

}

FString UICE_Color::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_Color::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetColorHolder()->SetToolTipText(FText::FromString(tooltip));
}
