

#include "ICE_Integer.h"

void UICE_Integer::NativeConstruct()
{

}

FString UICE_Integer::GetName()
{
	return GetICENameContainer()->Text.ToString();
}

void UICE_Integer::SetName(FString val, FString tooltip)
{
	GetICENameContainer()->SetText(FText::FromString(val));
	GetIntegerHolder()->SetToolTipText(FText::FromString(tooltip));
}
