

#include "ICE_TableRowLog.h"
#include "ICE_InteractLog.h"
#include <NoExportTypes.h>

void UICE_TableRowLog::NativeConstruct()
{

}

void UICE_TableRowLog::SetRowData(ELogHelpers logType, FString text, int rowID, FString file, FDateTime time)
{
#if WITH_EDITOR

	GetRowHolder()->SetToolTipText(FText::FromString(file));
	FString hourString = FString::FromInt(time.GetHour());
	FString minString = FString::FromInt(time.GetMinute());
	FString secString = FString::FromInt(time.GetSecond());
	FString milString = FString::FromInt(time.GetMillisecond());
	if (time.GetHour() < 10) {
		hourString = "0" + hourString;
	}
	if (time.GetMinute() < 10) {
		minString = "0" + minString;
	}
	if (time.GetSecond() < 10) {
		secString = "0" + secString;
	}
	if (time.GetMillisecond() < 10) {
		milString = "00" + milString;
	}
	else if (time.GetMillisecond() < 100) {
		milString = "0" + milString;
	}
	GetTimestamp()->SetText(FText::FromString("["+ hourString +":"+ minString +":"+ secString +":"+ milString +"]"));
	GetReadOnlyText()->SetText(FText::FromString(text));

	if (logType == ELogHelpers::Info) {
		UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/info.info'"), NULL, LOAD_None, NULL);
		GetIcon()->SetBrushFromTexture(texture);
	}
	else if (logType == ELogHelpers::Warning)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/warning.warning'"), NULL, LOAD_None, NULL);
		GetIcon()->SetBrushFromTexture(texture);
	}
	else if (logType == ELogHelpers::Error)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/danger.danger'"), NULL, LOAD_None, NULL);
		GetIcon()->SetBrushFromTexture(texture);
	}
	else if (logType == ELogHelpers::Spam)
	{
		UTexture2D* texture = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/spam.spam'"), NULL, LOAD_None, NULL);
		GetIcon()->SetBrushFromTexture(texture);
	}
	GetRowHolder()->SetBrushColor((rowID) % 2 ? FLinearColor(0.26f, 0.26f, 0.26f, 1.0f) : FLinearColor(0.13f, 0.13f, 0.13f, 1.0f));
#endif
}
