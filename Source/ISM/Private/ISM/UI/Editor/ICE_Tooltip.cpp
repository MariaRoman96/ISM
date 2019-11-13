#include "ICE_Tooltip.h"
#include "InteractCore.h"
#include <UnrealType.h>
#include <FileManagerGeneric.h>
#include "ICE_ButtonImage.h"
#include <Image.h>
#include <Engine/Texture2D.h>
#include <SlateBrush.h>

void UICE_Tooltip::NativeConstruct()
{
	int maxE = (int)GETENUMMAX(TEXT("EAction"));
	for (int fooInt = 0; fooInt != maxE; fooInt++)
	{
		EAction foo = static_cast<EAction>(fooInt);
		FText value = GETENUMDISPLAYNAME(TEXT("EAction"), foo);
		GetActionButton()->AddOption(value.ToString());
	}

	GetActionButton()->SetSelectedIndex((int32)stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].actionButton);
	GetActionButton()->OnSelectionChanged.AddDynamic(this, &UICE_Tooltip::OnSelectionChanged);
	GetHoldDownButtonHolder()->OnExpansionChanged.AddDynamic(this, &UICE_Tooltip::OnExpansionChanged);
	GetQTEButtonHolder()->OnExpansionChanged.AddDynamic(this, &UICE_Tooltip::OnExpansionChangedQTE);

	GetSearchText()->OnTextChanged.AddDynamic(this, &UICE_Tooltip::OnTextChanged);
	GetSearchBackgroundText()->OnTextChanged.AddDynamic(this, &UICE_Tooltip::OnTextBackgroundChanged);
}

void UICE_Tooltip::OnSelectionChanged(FString selectedItem, ESelectInfo::Type selectedType)
{
	FText name = GETENUMDISPLAYNAME(TEXT("EAction"), GetActionButton()->GetSelectedIndex());
	button->GetButtonName()->SetText(name);
	GetPreviewButton()->SetBrushFromTexture(UInteractCore::GetActionButtonImage((EAction)GetActionButton()->GetSelectedIndex()));
}

void UICE_Tooltip::OnExpansionChanged(UExpandableArea* Area, bool bIsExpanded)
{
	if (bIsExpanded) {
		GetHoldDownButton()->SetCheckedState(ECheckBoxState::Checked);
	}
	else {
		GetHoldDownButton()->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UICE_Tooltip::OnExpansionChangedQTE(UExpandableArea* Area, bool bIsExpanded)
{
	if (bIsExpanded) {
		GetQTEButton()->SetCheckedState(ECheckBoxState::Checked);
	}
	else {
		GetQTEButton()->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UICE_Tooltip::UpdateImage(int index)
{
	if (index >= 0) {
		if (!textures2D.IsValidIndex(index)) {
			textures2D = stageData->wrapper->GetDataGetter().textures2D;
		}
		if (textures2D.IsValidIndex(index)) {
			GetPreviewIcon()->SetBrushFromTexture(Cast<UTexture2D>(textures2D[index]));
			GetImageSelector()->SetIsExpanded(false);
		}
	}
}

void UICE_Tooltip::OnTextChanged(const FText& text)
{
	UpdateImageSelector();
}

void UICE_Tooltip::OnTextBackgroundChanged(const FText& text)
{
	UpdateBackgroundSelector();
}

void UICE_Tooltip::UpdateImageSelector() 
{
	GetImageHolderScroll()->ClearChildren();
	textures2D = stageData->wrapper->GetDataGetter().textures2D;
	for (int i = 0; i < textures2D.Num(); i++) {
		if (GetSearchText()->GetText().ToString() == "" || textures2D[i]->GetFName().ToString().Contains(GetSearchText()->GetText().ToString())) {
			UTexture2D* texture = Cast<UTexture2D>(textures2D[i]);
			UICE_ButtonImage* imageOption = CreateWidget<UICE_ButtonImage>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ButtonImage_BP.ICE_ButtonImage_BP_C'")));
			imageOption->m_data = EButtonTypeInfo::E_Texture;
			imageOption->index = i;
			imageOption->tooltip = this;
			imageOption->searchText = GetSearchText();
			imageOption->GetImage()->SetBrushFromTexture(texture);
			imageOption->GetTextBlock()->SetText(FText::FromString(texture->GetFName().ToString()));
			imageOption->SetToolTipText(FText::FromString(texture->GetPathName()));
			GetImageHolderScroll()->AddChild(imageOption);
		}
	}
}

void UICE_Tooltip::UpdateBackgroundSelector()
{
	GetBackgroundHolderScroll()->ClearChildren();
	textures2D = stageData->wrapper->GetDataGetter().textures2D;
	for (int i = 0; i < textures2D.Num(); i++) {
		if (GetSearchBackgroundText()->GetText().ToString() == "" || textures2D[i]->GetFName().ToString().Contains(GetSearchBackgroundText()->GetText().ToString())) {
			UTexture2D* texture = Cast<UTexture2D>(textures2D[i]);
			UICE_ButtonImage* imageOption = CreateWidget<UICE_ButtonImage>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ButtonImage_BP.ICE_ButtonImage_BP_C'")));
			imageOption->m_data = EButtonTypeInfo::E_Background;
			imageOption->index = i;
			imageOption->tooltip = this;
			imageOption->searchText = GetSearchBackgroundText();
			imageOption->GetImage()->SetBrushFromTexture(texture);
			imageOption->GetTextBlock()->SetText(FText::FromString(texture->GetFName().ToString()));
			imageOption->SetToolTipText(FText::FromString(texture->GetPathName()));
			GetBackgroundHolderScroll()->AddChild(imageOption);
		}
	}
}

void UICE_Tooltip::UpdateBackground(int index)
{
	if (index >= 0) {
		if (!textures2D.IsValidIndex(index)) {
			textures2D = stageData->wrapper->GetDataGetter().textures2D;
		}
		if (textures2D.IsValidIndex(index)) {
			GetPreviewBg()->SetBrushFromTexture(Cast<UTexture2D>(textures2D[index]));
			GetBackgroundSelector()->SetIsExpanded(false);
		}
	}
}

void UICE_Tooltip::GetAllData()
{
	UICE_BasicFunctionArray* stageFuncArray = nullptr;
	if (GetStageFunction()->GetAllChildren().Num() > 0) {
		stageFuncArray = Cast<UICE_BasicFunctionArray>(GetStageFunction()->GetAllChildren()[0]);
		if (stageFuncArray) {
			stageFuncArray->GetICEContent()->ClearChildren();
			stageFuncArray->basicFunctions.Empty();
			for (int j = 0; j < stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].stageFunction.Num(); j++) {
				UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(stageData, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
				basicFunc->currentIndex = j;
				basicFunc->stage = stageData;
				basicFunc->parentArr = stageFuncArray;
				basicFunc->currentStage = stage;
				basicFunc->currentState = { EStateFunctionList::Action, tooltipIndex };
				basicFunc->SetEnum((int)stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].stageFunction[j].enumName);
				basicFunc->GetData();
				basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

				basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
				basicFunc->GetICESearchBar()->basicFun = basicFunc;
				basicFunc->GetICESearchBar()->UpdateValues();

				UICE_Button* functInsButton = CreateWidget<UICE_Button>(stageData, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functInsButton->GetButtonName()->SetText(FText::FromString(""));
				functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
				functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
				functInsButton->m_data = { ETargetInfo::E_Insert, j, basicFunc, "stageInteract", EActionStageType::Interact, tooltipIndex };
				basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

				UICE_Button* functCopyButton = CreateWidget<UICE_Button>(stageData, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functCopyButton->GetButtonName()->SetText(FText::FromString(""));
				functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
				functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
				functCopyButton->m_data = { ETargetInfo::E_Copy, j, basicFunc, "stageInteract", EActionStageType::Interact, tooltipIndex };
				basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

				UICE_Button* functPasteButton = CreateWidget<UICE_Button>(stageData, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functPasteButton->GetButtonName()->SetText(FText::FromString(""));
				functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
				functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
				functPasteButton->m_data = { ETargetInfo::E_Paste, j, basicFunc, "stageInteract", EActionStageType::Interact, tooltipIndex };
				basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

				UICE_Button* functDelButton = CreateWidget<UICE_Button>(stageData, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functDelButton->GetButtonName()->SetText(FText::FromString(""));
				functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
				functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
				functDelButton->m_data = { ETargetInfo::E_Delete, j, basicFunc, "stageInteract", EActionStageType::Interact, tooltipIndex };
				basicFunc->GetICEButtonOptions()->AddChild(functDelButton);


				stageFuncArray->basicFunctions.Add(basicFunc);
				stageFuncArray->GetICEContent()->AddChild(basicFunc);
			}
		}
	}
	GetData();
}

void UICE_Tooltip::GetData()
{
	//stageTitle = interactComponent->interactDataArray[stage].stageTitle;

	//IMAGES
	GetPreviewIcon()->SetBrushFromTexture(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_icon);
	GetPreviewButton()->SetBrushFromTexture(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_button);
	GetPreviewBg()->SetBrushFromTexture(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_circle);

	//VECTOR3
	SetWidgetLocation(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].widgetLocation);
	SetWidgetSize(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].widgetSize);

	//BOOLEANS
	GetHoldDownButtonHolder()->SetIsExpanded(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_holdingDown);
	GetHoldDownButton()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_holdingDown);
	GetShowTooltip()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_showTooltip);
	GetHideTooltip()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_hideTooltip);
	GetHideOutline()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_hideOutline);
	GetWidgetInScreen()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].widgetInScreen);
	GetQTEButtonHolder()->SetIsExpanded(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_quickTimeEvent);
	GetQTEButton()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_quickTimeEvent);
	GetInteractIfHide()->SetIsChecked(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_interactIfTooltipIsHidden);
	
	//TEXT
	GetNextStage()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextStage));
	if (stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_quickTimeEvent) {
		GetNextFailStageQTE()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextFailStage));
	}
	else {
		GetNextFailStage()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextFailStage));
	}
	GetTimePressing()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_timePressing));
	GetUpdateTimePressing()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_updateTimePressing));
	GetNextStageDelay()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextStageDelay));
	GetTooltipSpeed()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_tooltipAnimSpeed));
	GetDelayQTE()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_delayQuickEventTime));
	GetQuickEventTime()->SetText(FText::FromString(stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_quickEventTime));

	for (int i = 0; i < stageFunc->basicFunctions.Num(); i++) {
		stageFunc->basicFunctions[i]->GetData();
	}
}

void UICE_Tooltip::UpdateData()
{

	//ENUMS
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].actionButton = (EAction)GetActionButton()->GetSelectedIndex();

	//IMAGES
	UTexture2D* texture = Cast<UTexture2D>(GetPreviewIcon()->Brush.GetResourceObject());
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_icon = texture;
	UTexture2D* textureCircle = Cast<UTexture2D>(GetPreviewBg()->Brush.GetResourceObject());
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_circle = textureCircle;

	//VECTOR3
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].widgetLocation = GetWidgetLocation();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].widgetSize = GetWidgetSize();

	//BOOLEANS
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_holdingDown = GetHoldDownButton()->IsChecked();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_showTooltip = GetShowTooltip()->IsChecked();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_hideTooltip = GetHideTooltip()->IsChecked();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_hideOutline = GetHideOutline()->IsChecked();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].widgetInScreen = GetWidgetInScreen()->IsChecked();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_quickTimeEvent = GetQTEButton()->IsChecked();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_interactIfTooltipIsHidden = GetInteractIfHide()->IsChecked();

	//TEXT
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextStage = GetNextStage()->GetText().ToString();
	if (GetQTEButton()->IsChecked()) {
		stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextFailStage = GetNextFailStageQTE()->GetText().ToString();
	}else{
		stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextFailStage = GetNextFailStage()->GetText().ToString();
	}
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_timePressing = GetTimePressing()->GetText().ToString();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_updateTimePressing = GetUpdateTimePressing()->GetText().ToString();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_nextStageDelay = GetNextStageDelay()->GetText().ToString();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_tooltipAnimSpeed = GetTooltipSpeed()->GetText().ToString();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_delayQuickEventTime = GetDelayQTE()->GetText().ToString();
	stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].m_quickEventTime = GetQuickEventTime()->GetText().ToString();

	for (int i = 0; i < stageFunc->basicFunctions.Num(); i++) {
		if (stageData->wrapper->interactComponent->interactDataArray[stage].actions[tooltipIndex].stageFunction.IsValidIndex(i)) {
			stageFunc->basicFunctions[i]->UpdateData();
		}
		else {
			GetAllData();
		}
	}

	stageData->wrapper->interactComponent->UpdateInteractArrayData();
}
