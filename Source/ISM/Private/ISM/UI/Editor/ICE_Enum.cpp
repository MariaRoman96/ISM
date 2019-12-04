

#include "ICE_Enum.h"

void UICE_Enum::NativeConstruct()
{

}

FString UICE_Enum::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_Enum::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetEnumHolder()->SetToolTipText(FText::FromString(tooltip));
}
