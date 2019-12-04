
#include "ICE_InteractLog.h"
#include "ICE_Button.h"
#include "ICE_TableRowLog.h"
#include "InteractCore.h"

FDelegateLogHelpers UICE_InteractLog::delegateLogHelpers;

void UICE_InteractLog::NativeConstruct()
{
#if WITH_EDITOR

	UICE_Button* clearButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	clearButton->GetButtonName()->SetText(FText::FromString("Clear"));
	clearButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Clear Data"))));
	clearButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	clearButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	clearButton->m_data = { ETargetInfo::E_Clear, 0, this, "logs" };
	GetClearHolder()->AddChild(clearButton);

	UICE_Button* filterExecuteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	filterExecuteButton->GetButtonName()->SetText(FText::FromString("Execute"));
	filterExecuteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Execute"))));
	filterExecuteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	filterExecuteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/ue4_off.ue4_off'"), NULL, LOAD_None, NULL));
	filterExecuteButton->m_data = { ETargetInfo::E_Execute, 0, this, "Command" };
	GetExecuteCommand()->AddChild(filterExecuteButton);

	BuildMonitor();

	GetSearch()->OnTextChanged.AddDynamic(this, &UICE_InteractLog::OnSearchText);

	UICE_InteractLog::delegateLogHelpers.AddDynamic(this, &UICE_InteractLog::OnLogAdded);
#endif
}

void UICE_InteractLog::BuildMonitor()
{
#if WITH_EDITOR

	GetFilterHolder()->ClearChildren();
	ClearPermanentData();
	//GetExecuteCommand()->ClearChildren();

	UICE_Button* filterPermanentButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	filterPermanentButton->GetButtonName()->SetText(FText::FromString(""));
	filterPermanentButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Filter Permanent Data"))));
	filterPermanentButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	if (!showPermanent) {
		filterPermanentButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/ue4_off.ue4_off'"), NULL, LOAD_None, NULL));
	}
	else {
		filterPermanentButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/ue4.ue4'"), NULL, LOAD_None, NULL));
	}
	filterPermanentButton->m_data = { ETargetInfo::E_Filter, 2, this, "Permanent" };
	GetFilterHolder()->AddChild(filterPermanentButton);

	UICE_Button* filterErrorButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	filterErrorButton->GetButtonName()->SetText(FText::FromString(""));
	filterErrorButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Filter Errors"))));
	filterErrorButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	if (!showErrors) {
		filterErrorButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/danger_off.danger_off'"), NULL, LOAD_None, NULL));
	}
	else {
		filterErrorButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/danger.danger'"), NULL, LOAD_None, NULL));
	}
	filterErrorButton->m_data = { ETargetInfo::E_Filter, 0, this, "Errors" };
	GetFilterHolder()->AddChild(filterErrorButton);

	UICE_Button* filterWarningsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	filterWarningsButton->GetButtonName()->SetText(FText::FromString(""));
	filterWarningsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Filter Warnings"))));
	filterWarningsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	if (!showWarnings) {
		filterWarningsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/warning_off.warning_off'"), NULL, LOAD_None, NULL));
	}
	else {
		filterWarningsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/warning.warning'"), NULL, LOAD_None, NULL));
	}
	filterWarningsButton->m_data = { ETargetInfo::E_Filter, 1, this, "Warnings" };
	GetFilterHolder()->AddChild(filterWarningsButton);

	UICE_Button* filterInfoButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	filterInfoButton->GetButtonName()->SetText(FText::FromString(""));
	filterInfoButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Filter Info"))));
	filterInfoButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	if (!showInfo) {
		filterInfoButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/info_off.info_off'"), NULL, LOAD_None, NULL));
	}
	else {
		filterInfoButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/info.info'"), NULL, LOAD_None, NULL));
	}
	filterInfoButton->m_data = { ETargetInfo::E_Filter, 2, this, "Info" };
	GetFilterHolder()->AddChild(filterInfoButton);

	UICE_Button* filterSpamButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	filterSpamButton->GetButtonName()->SetText(FText::FromString(""));
	filterSpamButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Filter Spam"))));
	filterSpamButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	if (!showSpam) {
		filterSpamButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/spam_off.spam_off'"), NULL, LOAD_None, NULL));
	}
	else {
		filterSpamButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/spam.spam'"), NULL, LOAD_None, NULL));
	}
	filterSpamButton->m_data = { ETargetInfo::E_Filter, 3, this, "Spam" };
	GetFilterHolder()->AddChild(filterSpamButton);


// 	UICE_Button* filterExecuteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
// 	filterExecuteButton->GetButtonName()->SetText(FText::FromString("Execute"));
// 	filterExecuteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Execute"))));
// 	filterExecuteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
// 	filterExecuteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/ue4_off.ue4_off'"), NULL, LOAD_None, NULL));
// 	filterExecuteButton->m_data = { ETargetInfo::E_Execute, 0, this, "Command" };
// 	GetExecuteCommand()->AddChild(filterExecuteButton);

	GetPermanentData();
#endif
}

void UICE_InteractLog::OnSearchText(const FText& Text)
{
#if WITH_EDITOR
	ClearData(true);
	GetData(Text);
#endif
}

void UICE_InteractLog::OnLogAdded()
{
#if WITH_EDITOR

	if ((UInteractCore::logArrayType[UInteractCore::logArrayType.Num() - 1] == ELogHelpers::Error && showErrors) || (UInteractCore::logArrayType[UInteractCore::logArrayType.Num() - 1] == ELogHelpers::Warning && showWarnings) || (UInteractCore::logArrayType[UInteractCore::logArrayType.Num() - 1] == ELogHelpers::Info && showInfo) || (UInteractCore::logArrayType[UInteractCore::logArrayType.Num() - 1] == ELogHelpers::Spam && showSpam)) {
		AddData(UInteractCore::logArrayType[UInteractCore::logArrayType.Num() - 1], UInteractCore::logArrayString[UInteractCore::logArrayString.Num() - 1], UInteractCore::logArrayFile[UInteractCore::logArrayFile.Num() - 1], UInteractCore::dateTime[UInteractCore::dateTime.Num() - 1]);
		GetScrollHolder()->ScrollToEnd();
	}
#endif
}

void UICE_InteractLog::ClearPermanentData()
{
	numPermanentRows = 0;
	GetPermanentHolder()->ClearChildren();
}

void UICE_InteractLog::GetPermanentData()
{
	if (GetWorld()) {
		if (GetWorld()->bKismetScriptError) {
			FString textReflection = "BLUEPRINT COMPILE ERROR";
			AddPermanentData(ELogHelpers::Error, textReflection, "", FDateTime::Now());
		}

		if (GetWorld()->NumLightingUnbuiltObjects > 0) {
			FString textLight = "LIGHTING NEEDS TO BE REBUILT (" + FString::FromInt(GetWorld()->NumLightingUnbuiltObjects)+" lights)";
			AddPermanentData(ELogHelpers::Warning, textLight, "", FDateTime::Now());
		}

		if (GetWorld()->NumUnbuiltReflectionCaptures > 0) {
			FString textReflection = "REFLECTION CAPTURES NEED TO BE REBUILT (" + FString::FromInt(GetWorld()->NumUnbuiltReflectionCaptures) + " unbuilt)";
			AddPermanentData(ELogHelpers::Warning, textReflection, "", FDateTime::Now());
		}

	}
}

void UICE_InteractLog::AddPermanentData(ELogHelpers type, FString text, FString file, FDateTime time)
{
	UICE_TableRowLog* tableRow = CreateWidget<UICE_TableRowLog>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_TableRowLog_BP.ICE_TableRowLog_BP_C'")));
	tableRow->SetRowData(type, text, numPermanentRows, file, time);
	GetPermanentHolder()->AddChild(tableRow);
	numPermanentRows++;
}


void UICE_InteractLog::ClearData(bool onlyData)
{
#if WITH_EDITOR
	numRows = 0;
	GetTableHolder()->ClearChildren();
	if (!onlyData) {
		UInteractCore::logArrayString.Empty();
		UInteractCore::logArrayType.Empty();
	}
#endif
}

void UICE_InteractLog::GetData(const FText& Text)
{
#if WITH_EDITOR

	for (int i = 0; i < UInteractCore::logArrayString.Num(); i++) {
		if ((Text.ToString() == "" || UInteractCore::logArrayString[i].Contains(Text.ToString())) && UInteractCore::logArrayType.IsValidIndex(i) && UInteractCore::logArrayFile.IsValidIndex(i) && UInteractCore::dateTime.IsValidIndex(i)) {
			ELogHelpers logType = UInteractCore::logArrayType[i];
			if ((logType == ELogHelpers::Error && showErrors) || (logType == ELogHelpers::Warning && showWarnings) || (logType == ELogHelpers::Info && showInfo) || (logType == ELogHelpers::Spam && showSpam)) {
				AddData(logType, UInteractCore::logArrayString[i], UInteractCore::logArrayFile[i], UInteractCore::dateTime[i]);
			}
		}
	}
	GetScrollHolder()->ScrollToEnd();
#endif
}

void UICE_InteractLog::AddData(ELogHelpers type, FString text, FString file, FDateTime time)
{
#if WITH_EDITOR

	UICE_TableRowLog* tableRow = CreateWidget<UICE_TableRowLog>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_TableRowLog_BP.ICE_TableRowLog_BP_C'")));
	tableRow->SetRowData(type, text, numRows, file, time);
	GetTableHolder()->AddChild(tableRow);
	numRows++;
#endif
}