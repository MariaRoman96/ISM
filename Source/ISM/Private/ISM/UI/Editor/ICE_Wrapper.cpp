#include "Editor/ICE_Wrapper.h"
#include "Editor/ICE_Button.h"
#include "Editor/ICE_Stage.h"
#include "InteractComponent.h"
#include <Class.h>
#include <PropertyPortFlags.h>
#include "InteractCore.h"
#include <LogMacros.h>
#include <Engine/Texture2D.h>
#include "ICE_ButtonImage.h"
#include "Styling/SlateBrush.h"
#include "ICE_Enum.h"

#if WITH_EDITOR
#include <Blutility/Public/IBlutilityModule.h>
#include <TimerManager.h>
#include <Analytics/Analytics/Public/AnalyticsEventAttribute.h>
#include <EngineAnalytics.h>
#include <NotificationManager.h>
#include <EditorStyleSet.h>
#include <Editor.h>
#include <SNotificationList.h>
#include <Engine/Engine.h>
#include <WindowsPlatformApplicationMisc.h>
void UICE_Wrapper::OnPreWorldInitializationInteract(UWorld* World, const UWorld::InitializationValues IVS)
{
	ResetWrapper(true);
}
#endif

FDataGetter UICE_Wrapper::dataGetter = FDataGetter();

void UICE_Wrapper::OnLoadStart()
{
#if WITH_EDITOR
	ModuleCompileStartTime = FPlatformTime::Seconds();

	if (CompileNotificationPtr.IsValid())
	{
		CompileNotificationPtr.Pin()->ExpireAndFadeout();
	}

	if (GEditor)
	{
		GEditor->PlayEditorSound(CompileStartSound);
	}

	FNotificationInfo Info(FText::FromString("Loading Interact Data"));
	//Info.Image = FEditorStyle::GetBrush(TEXT("LevelEditor.RecompileGameCode"));
	Info.FadeOutDuration = 0.5f;
	Info.ExpireDuration = 1.5f;
	Info.bUseThrobber = false;
	Info.bUseSuccessFailIcons = true;
	Info.bUseLargeFont = true;
	Info.bFireAndForget = false;
	Info.bAllowThrottleWhenFrameRateIsLow = false;

	// We can only show the cancel button on async builds
	//if (bIsAsyncCompile)
	//{
	//	Info.ButtonDetails.Add(FNotificationButtonInfo(LOCTEXT("CancelC++Compilation", "Cancel"), FText(), FSimpleDelegate::CreateRaw(this, &FMainFrameModule::OnCancelCodeCompilationClicked)));
	//}

	CompileNotificationPtr = FSlateNotificationManager::Get().AddNotification(Info);

	if (CompileNotificationPtr.IsValid())
	{
		CompileNotificationPtr.Pin()->SetCompletionState(SNotificationItem::CS_Pending);
	}
#endif
}

void UICE_Wrapper::OnLoadEnd()
{
#if WITH_EDITOR

	{
		const float ModuleCompileDuration = (float)(FPlatformTime::Seconds() - ModuleCompileStartTime);
		ISM_ULOG_EDIT("Interact load took %.3f seconds", Log, ModuleCompileDuration);
		//UE_LOG(LogMainFrame, Log, TEXT("MainFrame: Module compiling took %.3f seconds"), ModuleCompileDuration);

// 		if (FEngineAnalytics::IsAvailable())
// 		{
// 			TArray< FAnalyticsEventAttribute > CompileAttribs;
// 			CompileAttribs.Add(FAnalyticsEventAttribute(TEXT("Duration"), FString::Printf(TEXT("%.3f"), ModuleCompileDuration)));
// 			CompileAttribs.Add(FAnalyticsEventAttribute(TEXT("Result"), ECompilationResult::ToString(CompilationResult)));
// 			FEngineAnalytics::GetProvider().RecordEvent(TEXT("Editor.Modules.Recompile"), CompileAttribs);
// 		}
	}

	TSharedPtr<SNotificationItem> NotificationItem = CompileNotificationPtr.Pin();

	if (NotificationItem.IsValid())
	{
// 		if (!ECompilationResult::Failed(CompilationResult))
// 		{
		if (GEditor)
		{
			GEditor->PlayEditorSound(CompileSuccessSound);
		}

		NotificationItem->SetText(FText::FromString("Load Complete!"));
		NotificationItem->SetExpireDuration(5.0f);
		NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
// 		}
// 		else
// 		{
// 			struct Local
// 			{
// 				static void ShowCompileLog()
// 				{
// 					FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
// 					MessageLogModule.OpenMessageLog(FCompilerResultsLog::GetLogName());
// 				}
// 			};
// 
// 			if (GEditor)
// 			{
// 				GEditor->PlayEditorSound(CompileFailSound);
// 			}
// 
// 			if (CompilationResult == ECompilationResult::FailedDueToHeaderChange)
// 			{
// 				NotificationItem->SetText(NSLOCTEXT("MainFrame", "RecompileFailedDueToHeaderChange", "Compile failed due to the header changes. Close the editor and recompile project in IDE to apply changes."));
// 			}
// 			else if (CompilationResult == ECompilationResult::Canceled)
// 			{
// 				NotificationItem->SetText(NSLOCTEXT("MainFrame", "RecompileCanceled", "Compile Canceled!"));
// 			}
// 			else
// 			{
// 				NotificationItem->SetText(NSLOCTEXT("MainFrame", "RecompileFailed", "Compile Failed!"));
// 			}
// 
// 			NotificationItem->SetCompletionState(SNotificationItem::CS_Fail);
// 			NotificationItem->SetHyperlink(FSimpleDelegate::CreateStatic(&Local::ShowCompileLog));
// 			NotificationItem->SetExpireDuration(30.0f);
// 		}

		NotificationItem->ExpireAndFadeout();

		CompileNotificationPtr.Reset();
	}
#endif
}

void UICE_Wrapper::NativeConstruct()
{
#if WITH_EDITOR
	FWorldDelegates::OnPreWorldInitialization.AddUObject(this, &UICE_Wrapper::OnPreWorldInitializationInteract);
#endif

	CompileStartSound = LoadObject<USoundBase>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileStart_Cue.CompileStart_Cue"));
	//CompileStartSound->AddToRoot();
	CompileSuccessSound = LoadObject<USoundBase>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
	//CompileSuccessSound->AddToRoot();
	CompileFailSound = LoadObject<USoundBase>(NULL, TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
	//CompileFailSound->AddToRoot();

	ModuleCompileStartTime = 0.0f;
}

void UICE_Wrapper::CheckExportData()
{
	//No se usa
	if (!ISM_ISVALID(interactComponentExp) || !ISM_ISVALID(interactComponentExp->GetOwner())) {
		interactComponentExp = interactComponent;
	}
	else {
		if (!ISM_ISVALID(interactComponent) || !ISM_ISVALID(interactComponent->GetOwner()) || (interactComponentExp->GetOwner()->GetName() != interactComponent->GetOwner()->GetName())) {
			interactComponent->ExportInteractDataInternal();
// 			if (interactComponentBP) {
// 				interactComponentBP->ExportInteractDataInternal();
// 			}
		}
	}
}


void UICE_Wrapper::BuildEditor()
{
#if WITH_EDITOR
	if (savedAct) {
		UInteractComponent* interactComponent2 = Cast<UInteractComponent>(savedAct->GetComponentByClass(UInteractComponent::StaticClass()));

	// 	if (interactComponent2 && interactComponent2->GetOwner() && interactComponent && interactComponent->GetOwner() && interactComponent->GetOwner()->GetName() != interactComponent2->GetOwner()->GetName()) {
	// 		interactComponent->ExportInteractDataInternal();
	// 		if (interactComponentBP) {
	// 			interactComponentBP->ExportInteractDataInternal();
	// 		}
	// 	}

		if (interactComponent2) {
			interactComponent = interactComponent2;
			actorInteractBP = savedAct->GetClass()->GetDefaultObject<AActor>();
			if (actorInteractBP) {
				TArray<UObject*> usc; 
				savedAct->GetClass()->CollectDefaultSubobjects(usc, true);
				for (int h = 0; h < usc.Num(); h++) {
					UInteractComponent* intc = Cast<UInteractComponent>(usc[h]->GetOuter());
					if (intc) {
						interactComponentBP = intc;
						break;
					}
				}
			}

			dataGetter.ReloadAssetData(interactComponent->GetWorld());

	// 		dataGetter.actorClasses.Empty();
	// 		dataGetter.componentClasses.Empty();
	// 		dataGetter.widgets.Empty();
	// 		for (TObjectIterator<UClass> It; It; ++It)
	// 		{
	// 			//TODO aqui es necesario sino no encuentra los de esa clase
	// 			if (It->IsChildOf(ACh::StaticClass()) || It->IsChildOf(AItem::StaticClass()) || It->IsChildOf(AActor::StaticClass())
	// 				|| It->IsChildOf(APawn::StaticClass()) || It->IsChildOf(ACharacter::StaticClass()))
	// 			{
	// 				dataGetter.actorClasses.Add(*It);
	// 			}
	// 			else if (It->IsChildOf(USceneComponent::StaticClass()))
	// 			{
	// 				dataGetter.componentClasses.Add(*It);
	// 				dataGetter.actorComponentClasses.Add(*It);
	// 			}
	// 			else if (It->IsChildOf(UActorComponent::StaticClass()))
	// 			{
	// 				dataGetter.actorComponentClasses.Add(*It);
	// 			}
	// 			else if (It->IsChildOf(UUserWidget::StaticClass()))
	// 			{
	// 				dataGetter.widgets.Add(*It);
	// 			}
	// 			dataGetter.classes.Add(*It);
	// 		}
	// 
	// 		if (dataGetter.actualWorld != interactComponent->GetWorld() || interactComponent->GetWorld()->GetActorCount() != dataGetter.actorsLevel.Num()) {
	// 			dataGetter.actorsLevel.Empty();
	// 			dataGetter.actualWorld = interactComponent->GetWorld();
	// 			for (TObjectIterator<AActor> actor; actor; ++actor)
	// 			{
	// 				if (actor->GetWorld() == interactComponent->GetWorld()) {
	// 					dataGetter.actorsLevel.Add(*actor);
	// 				}
	// 			}
	// 		}


			UICE_Button* copyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			copyButton->GetButtonName()->SetText(FText::FromString(""));
			copyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy Data"))));
			copyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			copyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
			copyButton->m_data = { ETargetInfo::E_Copy, 0, this, "wrapper"};
			GetICEOptions()->AddChild(copyButton);

			UICE_Button* pasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			pasteButton->GetButtonName()->SetText(FText::FromString(""));
			pasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste Data"))));
			pasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			pasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
			pasteButton->m_data = { ETargetInfo::E_Paste, 0, this, "wrapper"};
			GetICEOptions()->AddChild(pasteButton);

			UICE_Button* openButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			openButton->GetButtonName()->SetText(FText::FromString(""));
			openButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Open BP"))));
			openButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			openButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/door.door'"), NULL, LOAD_None, NULL));
			openButton->m_data = { ETargetInfo::E_Open, 0, this, "wrapper" };
			GetICEOptions()->AddChild(openButton);

			UICE_Button* undoButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			undoButton->GetButtonName()->SetText(FText::FromString(""));
			undoButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Undo Changes"))));
			undoButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			undoButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/undo.undo'"), NULL, LOAD_None, NULL));
			undoButton->m_data = { ETargetInfo::E_Undo, 0, this, "wrapper" };
			GetICEOptions()->AddChild(undoButton);

			UICE_Button* redoButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			redoButton->GetButtonName()->SetText(FText::FromString(""));
			redoButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Redo Changes"))));
			redoButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			redoButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/redo.redo'"), NULL, LOAD_None, NULL));
			redoButton->m_data = { ETargetInfo::E_Redo, 0, this, "wrapper" };
			GetICEOptions()->AddChild(redoButton);

			UICE_Button* resetButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			resetButton->GetButtonName()->SetText(FText::FromString(""));
			resetButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Reset Data from BP"))));
			resetButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			resetButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/reset.reset'"), NULL, LOAD_None, NULL));
			resetButton->m_data = { ETargetInfo::E_ResetToBP, 0, this, "wrapper" };
			GetICEOptions()->AddChild(resetButton);

			UICE_Button* setButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			setButton->GetButtonName()->SetText(FText::FromString(""));
			setButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Set Data to BP"))));
			setButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			setButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/save.save'"), NULL, LOAD_None, NULL));
			setButton->m_data = { ETargetInfo::E_SetBP, 0, this, "wrapper" };
			GetICEOptions()->AddChild(setButton);

			UICE_Button* delButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			delButton->GetButtonName()->SetText(FText::FromString(""));
			delButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete Data"))));
			delButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			delButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			delButton->m_data = { ETargetInfo::E_Delete, 0, this, "wrapper"};
			GetICEOptions()->AddChild(delButton);


			ChangeStageTitle();
			GetInitStage()->SetText(FText::FromString(interactComponent->m_initStage));
			GetTickNum()->SetValue((float)interactComponent->m_ticksNum);
			GetTickTime()->SetValue(interactComponent->m_ticksTime);

			//CREATE STAGES
			for (int i = 0; i < interactComponent->interactDataArray.Num(); i++)
			{
				UICE_Button* button = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				button->GetButtonName()->SetText(FText::FromString(interactComponent->interactDataArray[i].stageTitle + " (" + FString::FromInt(i) + ")"));
				button->GetButtonName()->SetToolTipText(FText::FromString(interactComponent->interactDataArray[i].stageDescription));
				button->targetContent = GetContent();
				button->m_data = { ETargetInfo::E_Select, i, this, "stage" };
				GetNavBar()->AddChild(button);
				navBarButtons.Add(button);

				UICE_Stage* stage = CreateWidget<UICE_Stage>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Stage_BP.ICE_Stage_BP_C'")));
				stage->stage = i;
				stage->wrapper = this;
				stage->button = button;
				GetContent()->AddChild(stage);

				FString namePrio = "Priority";
				stage->GetPriority()->GetICENameContainer()->SetText(FText::FromString(namePrio));
				if (stage->GetPriority()->GetICEEnum()->GetOptionCount() == 0) {
					int maxE = (int)GETENUMMAX(TEXT("EPriority"));
					for (int j = 0; j < maxE; j++) {
						stage->GetPriority()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EPriority"), (EPriority)j));
					}
				}
				stage->GetPriority()->GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EPriority"), interactComponent->interactDataArray[i].priorityStage));
				//stage->GetPriority()->searchBar = CreateWidget<UICE_Search>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Search_BP.ICE_Search_BP_C'")));
				stage->GetPriority()->GetICESearchBar()->selector = stage->GetPriority()->GetICEEnum();
				stage->GetPriority()->GetICESearchBar()->UpdateValues();
				stage->GetPriorityRepeat()->SetIsChecked(interactComponent->interactDataArray[i].doOtherInteract);
				stages.Push(stage);
			}

			if (GetNavBarAdd()->GetAllChildren().Num() < 1) {
				UICE_Button* button = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				button->ChangeOrange();
				button->GetButtonName()->SetText(FText::FromString("+"));
				button->SetToolTipText(FText::FromString("Add new stage"));
				button->m_data = { ETargetInfo::E_Create, -1, this, "stage" };
				GetNavBarAdd()->AddChild(button);
			}

			if (interactComponent->interactDataArray.Num() > 0) {
				UpdateNavBar(GetContent()->GetActiveWidgetIndex());
			}
		}

	}
#endif
}

void UICE_Wrapper::ExportSave() {
	if (ISM_ISVALID(interactComponent)) {
		interactComponent->ExportInteractDataInternal();
		interactComponent->ImportInteractDataInternal();
	}
}

void UICE_Wrapper::ExportReLoad() {
	if (ISM_ISVALID(interactComponent)) {
		if (!(interactComponent->interactDataArrayString != "" && interactComponent->interactDataArrayString != "NOT_VALUE" && interactComponent->interactDataArray.Num() == 0)) {
			interactComponent->ExportInteractDataInternal();
		}
	}
}

void UICE_Wrapper::CheckIfAreChanges(TArray<AActor*> selectedActorsArray) 
{
#if WITH_EDITOR
	//OnLoadStart();

	if (ISM_ISVALID(interactComponent) && ISM_ISVALID(interactComponent->GetOwner()) && (selectedActorsArray.Num() == 0 || !ISM_ISVALID(selectedActorsArray[0]) || (ISM_ISVALID(selectedActorsArray[0]) && selectedActorsArray[0]->GetName() != interactComponent->GetOwner()->GetName()))) {
		interactComponent->ExportInteractDataInternal();
// 		if (interactComponentBP) {
// 			interactComponentBP->ExportInteractDataInternal();
// 		}
	}

	//ONLY ONE SELECTED
	if (selectedActorsArray.Num() == 1) {
		if (selectedActorsArray[0]) {
			//IF IS THE SAME ACTOR JUST UPDATE DATA IF CHANGED
			if (selectedActorsArray[0] == GetSelectedActor() && interactComponent) {
				UpdateData();
				ExportSave();
				savedAct = selectedActorsArray[0];
				if (UICE_Button::undoInteractData.IsValidIndex(UICE_Button::undoInteractDataIndex)) {
					UICE_Button::undoInteractData[UICE_Button::undoInteractDataIndex] = interactComponent->ExportInteractData();
				}
			}
			else if(selectedActorsArray[0])
			{
				ResetWrapper();

				if (ISM_ISVALID(interactComponent) && ISM_ISVALID(interactComponent->GetOwner()) && selectedActorsArray[0]->GetName() == interactComponent->GetOwner()->GetName()) {
					ExportReLoad();
				}
				savedAct = selectedActorsArray[0];
				interactComponent = Cast<UInteractComponent>(selectedActorsArray[0]->GetComponentByClass(UInteractComponent::StaticClass()));

				//IF IMPLEMENTS INTERACT COMPONENT THEN BUILD EDITOR
				if (interactComponent)
				{
					SetSelectedActor(selectedActorsArray[0]);
//					interactComponentBP = UInteractComponent::GetInteractBP(selectedActorsArray[0]->GetClass());
					interactComponent->ImportInteractDataInternal();
// 					if (interactComponentBP) {
// 						interactComponentBP->ImportInteractDataInternal();
// 					}
					BuildEditor();
					UICE_Button::undoInteractData.Empty();
					UICE_Button::undoInteractData.Add(interactComponent->ExportInteractData());
					UICE_Button::undoInteractDataIndex = 0;
				}
				else {
					FString text;
					savedAct->GetName(text);
					if (GetTitle()) {
						GetTitle()->SetText(FText::FromString(text));
					}
					UICE_Button* intButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
					intButton->GetButtonName()->SetText(FText::FromString(""));
					intButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Add Interact"))));
					intButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
					intButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
					intButton->m_data = { ETargetInfo::E_AddInteract, 0, this, "wrapper" };
					GetICEOptions()->AddChild(intButton);
					ISM_PRINTLOG("Doesn't have interact component");
				}
			}
			selectedActorsArray[0]->MarkPackageDirty();
		}
	//MULTIPLE SELECTED MUST PRINT ERROR
	}else if (selectedActorsArray.Num() > 1) {
		ISM_PRINTLOG("Error, multiple actor selected");
		ResetWrapper();
	}
	//LoadTextures2D();
	//OnLoadEnd();
#endif
}

void UICE_Wrapper::GetStageData(int fromIndex)
{
#if WITH_EDITOR

	int initIndex = 0;
	if (fromIndex != -1) {
		initIndex = fromIndex;
	}
	for (int i = initIndex; i < interactComponent->interactDataArray.Num(); i++) {
		if (GetContent()->GetAllChildren().IsValidIndex(i)) {
			stages.RemoveAt(initIndex);
			GetContent()->RemoveChildAt(initIndex);
		}
	}
	for (int i = initIndex; i < interactComponent->interactDataArray.Num(); i++) {
		if (GetNavBar()->GetAllChildren().IsValidIndex(i)) {
			navBarButtons.RemoveAt(initIndex);
			GetNavBar()->RemoveChildAt(initIndex);
		}
	}
	for (int i = initIndex; i < interactComponent->interactDataArray.Num(); i++)
	{
		UICE_Button* button = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		button->GetButtonName()->SetText(FText::FromString(interactComponent->interactDataArray[i].stageTitle + " (" + FString::FromInt(i) + ")"));
		button->GetButtonName()->SetToolTipText(FText::FromString(interactComponent->interactDataArray[i].stageDescription));
		button->targetContent = GetContent();
		button->m_data = { ETargetInfo::E_Select, i, this, "stage" };
		GetNavBar()->AddChild(button);
		navBarButtons.Add(button);

		UICE_Stage* stage = CreateWidget<UICE_Stage>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Stage_BP.ICE_Stage_BP_C'")));
		stage->stage = i;
		stage->wrapper = this;
		stage->button = button;
		FString namePrio = "Priority";
		stage->GetPriority()->GetICENameContainer()->SetText(FText::FromString(namePrio));
		if (stage->GetPriority()->GetICEEnum()->GetOptionCount() == 0) {
			int maxE = (int)GETENUMMAX(TEXT("EPriority"));
			for (int j = 0; j < maxE; j++) {
				stage->GetPriority()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EPriority"), (EPriority)j));
			}
		}
		stage->GetPriority()->GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EPriority"), interactComponent->interactDataArray[i].priorityStage));
		//stage->GetPriority()->searchBar = CreateWidget<UICE_Search>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Search_BP.ICE_Search_BP_C'")));
		stage->GetPriority()->GetICESearchBar()->selector = stage->GetPriority()->GetICEEnum();
		stage->GetPriority()->GetICESearchBar()->UpdateValues();
		stage->GetPriorityRepeat()->SetIsChecked(interactComponent->interactDataArray[i].doOtherInteract);
		GetContent()->AddChild(stage);
		stages.Push(stage);
		stage->GetAllData();
	}
#endif
}

void UICE_Wrapper::GetAllData(bool cleanActor, int resetNavSelected)
{
#if WITH_EDITOR
	//OnLoadStart();
	ResetWrapper(cleanActor);
	BuildEditor();
	if (resetNavSelected>-1) {
		UpdateNavBar(resetNavSelected);
		GetContent()->SetActiveWidgetIndex(resetNavSelected);
	}
	FTimerDelegate loadingDel;
	loadingDel.BindUFunction(this, FName("OnLoadEnd"), false);
	GetWorld()->GetTimerManager().SetTimer(loadingHandle, loadingDel, 0.15f, false);
	//OnLoadEnd();
#endif
}

void UICE_Wrapper::GetData()
{
#if WITH_EDITOR

	for (int i = 0; i < stages.Num(); i++) {
		stages[i]->GetData();
	}
	if (interactComponent) {
		GetInitStage()->SetText(FText::FromString(interactComponent->m_initStage));
		GetTickNum()->SetValue((float)interactComponent->m_ticksNum);
		GetTickTime()->SetValue(interactComponent->m_ticksTime);
	}
#endif
}

void UICE_Wrapper::ResetWrapper(bool cleanActor)
{
#if WITH_EDITOR

	if (cleanActor)
	{
		SetSelectedActor(nullptr);
		interactComponent = nullptr;
		interactComponentBP = nullptr;
		actorInteractBP = nullptr;
		savedAct = nullptr;
	}

	navBarButtons.Empty();
	stages.Empty();

	if (GetNavBarAdd()) {
		GetNavBarAdd()->ClearChildren();
	}
	if (GetNavBar()) {
		GetNavBar()->ClearChildren();
	}
	if (GetContent()) {
		GetContent()->ClearChildren();
	}
	if (GetICEOptions()) {
		GetICEOptions()->ClearChildren();
	}

	ChangeStageTitle();

	//GEngine->ForceGarbageCollection();
#endif
}

void UICE_Wrapper::UpdateData()
{
#if WITH_EDITOR

	ChangeStageTitle();

	if (GetContent()) {
		TArray<UWidget*> children = GetContent()->GetAllChildren();
		for (int i = 0; i < children.Num(); i++) {
			UICE_Stage* stage = Cast<UICE_Stage>(children[i]);
			//if (stage && stage->stage == interactComponent->m_currentStage) {
			if (stage && interactComponent && interactComponent->interactDataArray.IsValidIndex(i)) {
				stage->UpdateData();
			}
			else {
				GetAllData();
			}
		}
	}

	interactComponent->m_initStage = GetInitStage()->GetText().ToString();
	interactComponent->m_ticksNum = (int)GetTickNum()->GetValue();
	interactComponent->m_ticksTime = GetTickTime()->GetValue();

	GEngine->ForceGarbageCollection();
#endif
}
  
void UICE_Wrapper::ChangeStageTitle()
{
#if WITH_EDITOR

	if (savedAct) {
		FString text;
		savedAct->GetName(text);
		if (GetTitle()) {
			GetTitle()->SetText(FText::FromString(text));
		}
		if (GetCurrentStageText() &&  interactComponent) {
			GetCurrentStageText()->SetText(FText::FromString(FString::Printf(TEXT("Current stage: %d"), interactComponent->m_currentStage)));
		}
		GetWrapperStages()->SetVisibility(ESlateVisibility::Visible);

	}
	else {
		if (GetTitle()) {
			GetTitle()->SetText(FText::FromString("None"));
		}
		if (GetCurrentStageText()) {
			GetCurrentStageText()->SetText(FText::FromString(""));
		}
		GetWrapperStages()->SetVisibility(ESlateVisibility::Collapsed);
	}
#endif
}

void UICE_Wrapper::UpdateNavBar(int currentTab)
{
#if WITH_EDITOR

	for (int i = 0; i < navBarButtons.Num(); i++)
	{
		if (currentTab == i) 
		{
			navBarButtons[i]->ChangeMidGrey();
			if (interactComponent) {
				interactComponent->m_currentStage = i;

				UpdateData();
			}
		}
		else {
			navBarButtons[i]->ChangeDarkGrey();
		}
	}
#endif
}

FDataGetter UICE_Wrapper::GetDataGetter()
{
	//textures2D = UInteractCore::DynamicLoadContentFromPath("Assets/Textures/Icons", UTexture2D::StaticClass(),true);
	return dataGetter;
}

// void UICE_Wrapper::onClick()
// {
// 	TArray<FString> arr;
// 	for (int i = 0; i < interactComponent->interactDataArray.Num(); i++) {
// 		FInteractData dat = interactComponent->interactDataArray[0];
// 		UScriptStruct* Struct = dat.StaticStruct();
// 		FString Output = TEXT("");
// 		Struct->ExportText(Output, &dat, nullptr, this, (PPF_ExportsNotFullyQualified | PPF_Copy | PPF_Delimited | PPF_IncludeTransient), nullptr);
// 		arr.Push(Output);
// 	}
// 	FString text = FString::Join(arr, TEXT(","));
// 	text = "(" + text + ")";
// 	ISM_PRINTLOG(text);
// 	ISM_ULOG("%s", *text);
// 	//LoadTextures2D();
// 	//FWindowsPlatformApplicationMisc::ClipboardCopy(*Output);
// }
