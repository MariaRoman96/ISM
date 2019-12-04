

#include "ICE_Float.h"

void UICE_Float::NativeConstruct()
{

}

FString UICE_Float::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_Float::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetFloatHolder()->SetToolTipText(FText::FromString(tooltip));
}
