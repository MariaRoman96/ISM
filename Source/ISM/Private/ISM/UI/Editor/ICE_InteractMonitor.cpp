

#include "ICE_InteractMonitor.h"
#include "ICE_Button.h"
#include "ICE_TableRow.h"
#include <LevelUtils.h>
#include "ICE_Wrapper.h"

void UICE_InteractMonitor::NativeConstruct()
{
#if WITH_EDITOR

	BuildMonitor();
#endif
}

void UICE_InteractMonitor::BuildMonitor()
{
#if WITH_EDITOR

	if (GetEnum()) {
		FString nam = "Select Function";
		GetEnum()->GetICENameContainer()->SetText(FText::FromString(nam));

		if (GetEnum()->GetICEEnum()->GetOptionCount() == 0) {
			int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
			for (int i = 0; i < maxE2; i++) {
				GetEnum()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EActionListCore"), (EActionListCore)i));
			}
			int maxE = (int)GETENUMMAX(TEXT("EActionList"));
			for (int j = 1; j < maxE; j++) {
				GetEnum()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EActionList"), (EActionList)j));
			}
		}

		GetEnum()->GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EActionList"), EActionList::None));

		GetEnum()->GetICESearchBar()->selector = GetEnum()->GetICEEnum();
		GetEnum()->GetICESearchBar()->UpdateValues();
	}

	if (GetEnumCheck()) {
		FString nam29 = "Select CheckFunction";
		GetEnumCheck()->GetICENameContainer()->SetText(FText::FromString(nam29));
		if (GetEnumCheck()->GetICEEnum()->GetOptionCount() == 0) {
			int maxE2 = (int)GETENUMMAX(TEXT("ECheckListCore"));
			for (int i = 0; i < maxE2; i++) {
				GetEnumCheck()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("ECheckListCore"), (ECheckListCore)i));
			}
			int maxE = (int)GETENUMMAX(TEXT("ECheckList"));
			for (int j = 0; j < maxE; j++) {
				GetEnumCheck()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("ECheckList"), (ECheckList)j));
			}
		}

		GetEnumCheck()->GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("ECheckList"), ECheckList::None));

		GetEnumCheck()->GetICESearchBar()->selector = GetEnumCheck()->GetICEEnum();
		GetEnumCheck()->GetICESearchBar()->UpdateValues();
	}

	if (GetActorClass()) {
		FString name23 = "Actor Class";
		GetActorClass()->GetICEName()->SetText(FText::FromString(name23));
		//UObject* value = stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.actorClass;
		GetActorClass()->GetICEContent()->AddOption("None");
		GetActorClass()->elementArray = UICE_Wrapper::dataGetter.actorClasses;
		for (int i = 0; i < GetActorClass()->elementArray.Num(); i++) {
			if (ISM_ISVALID(GetActorClass()->elementArray[i])) {
				GetActorClass()->GetICEContent()->AddOption(GetActorClass()->elementArray[i]->GetName());
			}
		}
		GetActorClass()->elementArray.Insert(nullptr, 0);
		GetActorClass()->SetValue(nullptr);

		GetActorClass()->GetICESearchBar()->selector = GetActorClass()->GetICEContent();
		GetActorClass()->GetICESearchBar()->UpdateValues();
	}

	if (GetSearchButton()) {
		UICE_Button* searchButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		searchButton->GetButtonName()->SetText(FText::FromString("Search"));
		searchButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Search Actors"))));
		searchButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		searchButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/observe.observe'"), NULL, LOAD_None, NULL));
		searchButton->m_data = { ETargetInfo::E_Search, 0, this, "search" };
		GetSearchButton()->AddChild(searchButton);
	}

	if (GetSelectAllButton()) {
		UICE_Button* selectAllButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		selectAllButton->GetButtonName()->SetText(FText::FromString("Select All"));
		selectAllButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Select All Actors"))));
		selectAllButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		selectAllButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/select.select'"), NULL, LOAD_None, NULL));
		selectAllButton->m_data = { ETargetInfo::E_SelectAll, 0, this, "selectAll" };
		GetSelectAllButton()->AddChild(selectAllButton);
	}

	if (GetShowDebug()) {
		GetShowDebug()->SetIsChecked(UInteractCore::showDebugMessages);

		GetShowDebug()->OnCheckStateChanged.AddDynamic(this, &UICE_InteractMonitor::OnShowDebugChange);
	}

	if (GetOnlyInteract()) {
		GetOnlyInteract()->OnCheckStateChanged.AddDynamic(this, &UICE_InteractMonitor::OnShowInteractChange);
	}

	if (GetDisableWarnings()) {
		GetDisableWarnings()->OnCheckStateChanged.AddDynamic(this, &UICE_InteractMonitor::OnDisableWarningChange);
	}

	if (GetSearchResumeHolder()) {
		GetSearchResumeHolder()->SetVisibility(ESlateVisibility::Collapsed);
	}
#endif
}

void UICE_InteractMonitor::OnDisableWarningChange(bool IsChecked) {
	FString command = "";
	if (IsChecked) {
		command = "DISABLEALLSCREENMESSAGES";
	}
	else {
		command = "ENABLEALLSCREENMESSAGES";
	}
	GEngine->Exec(GetWorld(), *command);
}

void UICE_InteractMonitor::OnShowDebugChange(bool IsChecked)
{
#if WITH_EDITOR

	UInteractCore::showDebugMessages = IsChecked;
#endif
}

void UICE_InteractMonitor::OnShowInteractChange(bool IsChecked)
{
#if WITH_EDITOR

	//UInteractCore::showDebugMessages = IsChecked;
	if (IsChecked) {
		GetEnumCheckHolder()->SetVisibility(ESlateVisibility::Visible);
		GetEnumHolder()->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		GetEnumCheckHolder()->SetVisibility(ESlateVisibility::Collapsed);
		GetEnumHolder()->SetVisibility(ESlateVisibility::Collapsed);
	}
#endif
}

bool UICE_InteractMonitor::GetCheckedValue()
{
	return GetOnlyInteract()->IsChecked();
}

UObject* UICE_InteractMonitor::GetActorValue()
{
	return GetActorClass()->GetValue();
}

void UICE_InteractMonitor::ClearSearch()
{
#if WITH_EDITOR

	GetSearchResumeHolder()->SetVisibility(ESlateVisibility::Visible);
	GetTableContent()->ClearChildren();
	searchDataElements = 0;
#endif
}


void UICE_InteractMonitor::ChangeActor(int index, AActor* actor) {
#if WITH_EDITOR

	UICE_TableRow* tablRow = Cast<UICE_TableRow>(GetTableContent()->GetAllChildren()[index]);
	if (tablRow) {
		tablRow->m_actor = actor;
	}
#endif
}


void UICE_InteractMonitor::AddSearchData(FString actorName, FString worldName, FString levelName, AActor* actor, FString procedence, ULevel* level)
{
#if WITH_EDITOR

	UICE_TableRow* row = CreateWidget<UICE_TableRow>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_TableRow_BP.ICE_TableRow_BP_C'")));

	if (level) {
		UICE_Button* levelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		levelButton->GetButtonName()->SetText(FText::FromString(""));
		levelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		// 	if (!level) {
		// 		level = actor->GetLevel();
		// 	}
		if (FLevelUtils::IsLevelLocked(level)) {
			levelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Unlock level"))));
			levelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/locked.locked'"), NULL, LOAD_None, NULL));
			levelButton->m_data = { ETargetInfo::E_Open, searchDataElements, this, "level" };
		}
		else {
			levelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Lock level"))));
			levelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/unlocked.unlocked'"), NULL, LOAD_None, NULL));
			levelButton->m_data = { ETargetInfo::E_Close, searchDataElements, this, "level" };
		}
		row->GetRowButton()->AddChild(levelButton);
	}

	UICE_Button* bpButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	bpButton->GetButtonName()->SetText(FText::FromString(""));
	bpButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Go to actor"))));
	bpButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	bpButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/observe.observe'"), NULL, LOAD_None, NULL));
	bpButton->m_data = { ETargetInfo::E_Search, searchDataElements, this, "tableRow" };

	row->SetRowData(FString::FromInt(searchDataElements), actorName, worldName, levelName, bpButton, actor, procedence, level);

	FString pathS = "Blueprint'" + actor->GetClass()->GetPathName().LeftChop(2) + "'";
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	FAssetData asstData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*pathS));
	UBlueprint* bp = Cast<UBlueprint>(asstData.GetAsset());
	if (bp) {
		UICE_Button* openButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		openButton->GetButtonName()->SetText(FText::FromString(""));
		openButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		openButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Open BP"))));
		openButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/door.door'"), NULL, LOAD_None, NULL));
		openButton->m_data = { ETargetInfo::E_Open, searchDataElements, this, "openActor" };
		row->GetRowButton()->AddChild(openButton);
	}
	else {
		UICE_Button* addBPButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		addBPButton->GetButtonName()->SetText(FText::FromString(""));
		addBPButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		addBPButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create BP"))));
		addBPButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		addBPButton->m_data = { ETargetInfo::E_Create, searchDataElements, this, "addBP" };
		row->GetRowButton()->AddChild(addBPButton);
	}
	
	UInteractComponent* intComp = Cast<UInteractComponent>(actor->GetComponentByClass(UInteractComponent::StaticClass()));
	if (intComp) {
		UICE_Button* resetButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		resetButton->GetButtonName()->SetText(FText::FromString(""));
		resetButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		resetButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Reset to BP"))));
		resetButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/reset.reset'"), NULL, LOAD_None, NULL));
		resetButton->m_data = { ETargetInfo::E_ResetToBP, searchDataElements, this, "reset" };
		row->GetRowButton()->AddChild(resetButton);
	}
	else {
		UICE_Button* addInteractButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		addInteractButton->GetButtonName()->SetText(FText::FromString(""));
		addInteractButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		addInteractButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create Interact"))));
		addInteractButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		addInteractButton->m_data = { ETargetInfo::E_Create, searchDataElements, this, "addInteract" };
		row->GetRowButton()->AddChild(addInteractButton);
	}

	GetTableContent()->AddChild(row);

	searchDataElements++;
#endif
}
