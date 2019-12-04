

#include "ICE_TableRow.h"

void UICE_TableRow::NativeConstruct()
{

}

void UICE_TableRow::SetRowData(FString rowID, FString rowActor, FString rowWorld, FString rowLevel, UICE_Button* rowButton, AActor* actor, FString procedence, ULevel* level)
{
#if WITH_EDITOR

	int number = FCString::Atoi(*rowID);

	m_actor = actor;
	m_procedence = procedence;
	m_level = level;
	m_rowActor = rowActor;

	GetRowID()->SetText(FText::FromString(FString::FromInt(number+1)));
	GetRowActor()->SetText(FText::FromString(rowActor));
	GetRowWorld()->SetText(FText::FromString(rowWorld));
	GetRowLevel()->SetText(FText::FromString(rowLevel));
	GetRowButton()->AddChild(rowButton);

	GetRowHolder()->SetBrushColor((number+1) % 2 ? FLinearColor(0.26f, 0.26f, 0.26f, 1.0f) : FLinearColor(0.13f, 0.13f, 0.13f, 1.0f));
#endif
}
