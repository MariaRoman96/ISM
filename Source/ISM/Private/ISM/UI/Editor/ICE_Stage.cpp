#include "ICE_Stage.h"
#include "ICE_Capsule.h"
#include "ICE_Button.h"
#include <HorizontalBox.h>
#include <WidgetSwitcher.h>
#include "ICE_Tooltip.h"
#include "InteractCore.h"
#include "ICE_BasicFunctionArray.h"
#include "ICE_TriggerCapsule.h"
#include "ICE_Enum.h"
#include "ICE_Boolean.h"
#include <SharedPointer.h>
#include <ReflectionMetadata.h>
#include "ICE_HighlightCapsule.h"
#include "ICE_InsideCapsule.h"

void UICE_Stage::NativeConstruct()
{
	UICE_Button* stageInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stageInsButton->GetButtonName()->SetText(FText::FromString(""));
	stageInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert Stage"))));
	stageInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stageInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
	stageInsButton->m_data = { ETargetInfo::E_Insert, stage, this, "stage" };
	GetStageOptions()->AddChild(stageInsButton);

	UICE_Button* stageCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stageCopyButton->GetButtonName()->SetText(FText::FromString(""));
	stageCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy Stage"))));
	stageCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stageCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
	stageCopyButton->m_data = { ETargetInfo::E_Copy, stage, this, "stage" };
	GetStageOptions()->AddChild(stageCopyButton);

	UICE_Button* stagePasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stagePasteButton->GetButtonName()->SetText(FText::FromString(""));
	stagePasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste Stage"))));
	stagePasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stagePasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
	stagePasteButton->m_data = { ETargetInfo::E_Paste, stage, this, "stage" };
	GetStageOptions()->AddChild(stagePasteButton);

	UICE_Button* stageDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	stageDelButton->GetButtonName()->SetText(FText::FromString(""));
	stageDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete Stage"))));
	stageDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	stageDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	stageDelButton->m_data = { ETargetInfo::E_Delete, stage, this, "stage" };
	GetStageOptions()->AddChild(stageDelButton);

	//FString namePrioRep = "Do other Interact";
	//GetPriorityRepeat()->GetICENameContainer()->SetText(FText::FromString(namePrioRep));

	UICE_BasicFunctionArray* basicFuncArray = CreateWidget<UICE_BasicFunctionArray>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunctionArray_BP.ICE_BasicFunctionArray_BP_C'")));
	basicFuncArray->stage = this;
	FString namePre = "Functions - PreStage";
	basicFuncArray->GetICEName()->SetText(FText::FromString(namePre));
	
	UICE_Button* functButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functButton->GetButtonName()->SetText(FText::FromString(""));
	functButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create Function"))));
	functButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
	functButton->m_data = { ETargetInfo::E_Create, 0, basicFuncArray, "preStage", EActionStageType::Pre, -1  };
	basicFuncArray->GetICEButtonOptions()->AddChild(functButton);
	preStageButton = functButton;

	UICE_Button* functArrCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functArrCopyButton->GetButtonName()->SetText(FText::FromString(""));
	functArrCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy functions"))));
	functArrCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functArrCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
	functArrCopyButton->m_data = { ETargetInfo::E_Copy, 0, basicFuncArray, "preStage", EActionStageType::Pre, -1 };
	basicFuncArray->GetICEButtonOptions()->AddChild(functArrCopyButton);

	UICE_Button* functArrPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functArrPasteButton->GetButtonName()->SetText(FText::FromString(""));
	functArrPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste functions"))));
	functArrPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functArrPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
	functArrPasteButton->m_data = { ETargetInfo::E_Paste, 0, basicFuncArray, "preStage", EActionStageType::Pre, -1 };
	basicFuncArray->GetICEButtonOptions()->AddChild(functArrPasteButton);

	UICE_Button* functArrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	functArrDelButton->GetButtonName()->SetText(FText::FromString(""));
	functArrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete functions"))));
	functArrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	functArrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	functArrDelButton->m_data = { ETargetInfo::E_Delete, 0, basicFuncArray, "preStage", EActionStageType::Pre, -1 };
	basicFuncArray->GetICEButtonOptions()->AddChild(functArrDelButton);
	
	for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].preStageFunction.Num(); i++) {
		UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
		basicFunc->currentIndex = i;
		basicFunc->stage = this;
		basicFunc->parentArr = basicFuncArray;
		basicFunc->currentStage = stage;
		basicFunc->currentState = { EStateFunctionList::PreStage, -1 };
		int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
		int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumName;
		int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumNameCore;
		if (enumValue > 0) {
			basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumName+ maxE2-1);
		}
		else {
			basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumNameCore);
		}
		basicFunc->GetData();
		if (enumValue > 0) {
			basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumName));
		}
		else {
			basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumNameCore));
		}
		//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

		basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
		basicFunc->GetICESearchBar()->basicFun = basicFunc;
		basicFunc->GetICESearchBar()->UpdateValues();

		UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functInsButton->GetButtonName()->SetText(FText::FromString(""));
		functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
		functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		functInsButton->m_data = { ETargetInfo::E_Insert, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

		UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functCopyButton->GetButtonName()->SetText(FText::FromString(""));
		functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
		functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
		functCopyButton->m_data = { ETargetInfo::E_Copy, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

		UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functPasteButton->GetButtonName()->SetText(FText::FromString(""));
		functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
		functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
		functPasteButton->m_data = { ETargetInfo::E_Paste, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

		UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functDelButton->GetButtonName()->SetText(FText::FromString(""));
		functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
		functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		functDelButton->m_data = { ETargetInfo::E_Delete, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functDelButton);

		basicFuncArray->basicFunctions.Add(basicFunc);
		basicFuncArray->GetICEContent()->AddChild(basicFunc);
	}
	GetPreStageFunctions()->AddChild(basicFuncArray);


	///////////////////////////////////////////////////////////////////////////////
	//CAPSULE CONSTRUCTION/////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	//TRIGGER CAPSULE
	UICE_Button* button2 = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	button2->GetButtonName()->SetText(FText::FromString(FString::Printf(TEXT("Trigger"))));
	button2->targetContent = GetCapsuleContent();
	button2->m_data = { ETargetInfo::E_Select, 0, this, "capsule" };
	GetCapsuleNavBar()->AddChild(button2);
	capsuleNavBarButtons.Add(button2);

	UICE_TriggerCapsule* capsule2 = CreateWidget<UICE_TriggerCapsule>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_TriggerCapsule_BP.ICE_TriggerCapsule_BP_C'")));
	capsule2->GetCapsuleTitle()->SetText(FText::FromString(FString::Printf(TEXT("Trigger"))));
	capsule2->RemoveOutlineColorOption();
	GetCapsuleContent()->AddChild(capsule2);
	capsule2->stage = this;
	capsule2->stageData = this;
	capsule2->capsuleIndex = 0;

	UICE_BasicFunctionArray* initFuncArray = CreateWidget<UICE_BasicFunctionArray>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunctionArray_BP.ICE_BasicFunctionArray_BP_C'")));
	initFuncArray->stage = this;
	FString nameInit = "Functions - Trigger enter";
	initFuncArray->GetICEName()->SetText(FText::FromString(nameInit));
	
	UICE_Button* initFunctButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	initFunctButton->GetButtonName()->SetText(FText::FromString(""));
	initFunctButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create function"))));
	initFunctButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	initFunctButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
	initFunctButton->m_data = { ETargetInfo::E_Create, 0, initFuncArray, "initTrigger", EActionStageType::Init, -1 };
	initFuncArray->GetICEButtonOptions()->AddChild(initFunctButton);
	capsule2->initButton = initFunctButton;

	UICE_Button* initArrFunctCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	initArrFunctCopyButton->GetButtonName()->SetText(FText::FromString(""));
	initArrFunctCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy functions"))));
	initArrFunctCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	initArrFunctCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
	initArrFunctCopyButton->m_data = { ETargetInfo::E_Copy, 0, initFuncArray, "initTrigger", EActionStageType::Init, -1 };
	initFuncArray->GetICEButtonOptions()->AddChild(initArrFunctCopyButton);

	UICE_Button* initArrFunctPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	initArrFunctPasteButton->GetButtonName()->SetText(FText::FromString(""));
	initArrFunctPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste functions"))));
	initArrFunctPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	initArrFunctPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
	initArrFunctPasteButton->m_data = { ETargetInfo::E_Paste, 0, initFuncArray, "initTrigger", EActionStageType::Init, -1 };
	initFuncArray->GetICEButtonOptions()->AddChild(initArrFunctPasteButton);

	UICE_Button* initArrFunctDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	initArrFunctDelButton->GetButtonName()->SetText(FText::FromString(""));
	initArrFunctDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete functions"))));
	initArrFunctDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	initArrFunctDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	initArrFunctDelButton->m_data = { ETargetInfo::E_Delete, 0, initFuncArray, "initTrigger", EActionStageType::Init, -1 };
	initFuncArray->GetICEButtonOptions()->AddChild(initArrFunctDelButton);
	
	for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction.Num(); i++) {
		UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
		basicFunc->currentIndex = i;
		basicFunc->stage = this;
		basicFunc->parentArr = initFuncArray;
		basicFunc->currentStage = stage;
		basicFunc->currentState = { EStateFunctionList::Init, -1 };
		int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
		int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumName;
		int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumNameCore;
		if (enumValue > 0) {
			basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumName+maxE2-1);
		}
		else {
			basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumNameCore);
		}
		basicFunc->GetData();
		if (enumValue > 0) {
			basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumName));
		}
		else {
			basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumNameCore));
		}
		//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

		basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
		basicFunc->GetICESearchBar()->basicFun = basicFunc;
		basicFunc->GetICESearchBar()->UpdateValues();

		UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functInsButton->GetButtonName()->SetText(FText::FromString(""));
		functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
		functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		functInsButton->m_data = { ETargetInfo::E_Insert, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

		UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functCopyButton->GetButtonName()->SetText(FText::FromString(""));
		functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
		functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
		functCopyButton->m_data = { ETargetInfo::E_Copy, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

		UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functPasteButton->GetButtonName()->SetText(FText::FromString(""));
		functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
		functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
		functPasteButton->m_data = { ETargetInfo::E_Paste, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

		UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functDelButton->GetButtonName()->SetText(FText::FromString(""));
		functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
		functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		functDelButton->m_data = { ETargetInfo::E_Delete, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functDelButton);

		initFuncArray->basicFunctions.Add(basicFunc);
		initFuncArray->GetICEContent()->AddChild(basicFunc);
	}
	capsule2->GetInitContent()->AddChild(initFuncArray);
	capsule2->initFunc = initFuncArray;

	UICE_BasicFunctionArray* exitFuncArray = CreateWidget<UICE_BasicFunctionArray>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunctionArray_BP.ICE_BasicFunctionArray_BP_C'")));
	exitFuncArray->stage = this;
	FString nameExit = "Functions - Trigger exit";
	exitFuncArray->GetICEName()->SetText(FText::FromString(nameExit));
	

	UICE_Button* exitFunctButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	exitFunctButton->GetButtonName()->SetText(FText::FromString(""));
	exitFunctButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create function"))));
	exitFunctButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	exitFunctButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
	exitFunctButton->m_data = { ETargetInfo::E_Create, 0, exitFuncArray, "exitTrigger", EActionStageType::Exit, -1 };
	exitFuncArray->GetICEButtonOptions()->AddChild(exitFunctButton);
	capsule2->exitButton = exitFunctButton;

	UICE_Button* exitArrFunctCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	exitArrFunctCopyButton->GetButtonName()->SetText(FText::FromString(""));
	exitArrFunctCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy functions"))));
	exitArrFunctCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	exitArrFunctCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
	exitArrFunctCopyButton->m_data = { ETargetInfo::E_Copy, 0, exitFuncArray, "exitTrigger", EActionStageType::Exit, -1 };
	exitFuncArray->GetICEButtonOptions()->AddChild(exitArrFunctCopyButton);

	UICE_Button* exitArrFunctPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	exitArrFunctPasteButton->GetButtonName()->SetText(FText::FromString(""));
	exitArrFunctPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste functions"))));
	exitArrFunctPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	exitArrFunctPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
	exitArrFunctPasteButton->m_data = { ETargetInfo::E_Paste, 0, exitFuncArray, "exitTrigger", EActionStageType::Exit, -1 };
	exitFuncArray->GetICEButtonOptions()->AddChild(exitArrFunctPasteButton);

	UICE_Button* exitArrFunctDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	exitArrFunctDelButton->GetButtonName()->SetText(FText::FromString(""));
	exitArrFunctDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete functions"))));
	exitArrFunctDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
	exitArrFunctDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
	exitArrFunctDelButton->m_data = { ETargetInfo::E_Delete, 0, exitFuncArray, "exitTrigger", EActionStageType::Exit, -1 };
	exitFuncArray->GetICEButtonOptions()->AddChild(exitArrFunctDelButton);

	for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction.Num(); i++) {
		UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
		basicFunc->currentIndex = i;
		basicFunc->stage = this;
		basicFunc->parentArr = exitFuncArray;
		basicFunc->currentStage = stage;
		basicFunc->currentState = { EStateFunctionList::Exit, -1 };
		int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
		int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumName;
		int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumNameCore;
		if (enumValue > 0) {
			basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumName+maxE2-1);
		}
		else {
			basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumNameCore);
		}
		basicFunc->GetData();
		if (enumValue > 0) {
			basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumName));
		}
		else {
			basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumNameCore));
		}
		//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

		basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
		basicFunc->GetICESearchBar()->basicFun = basicFunc;
		basicFunc->GetICESearchBar()->UpdateValues();

		UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functInsButton->GetButtonName()->SetText(FText::FromString(""));
		functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
		functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		functInsButton->m_data = { ETargetInfo::E_Insert, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

		UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functCopyButton->GetButtonName()->SetText(FText::FromString(""));
		functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
		functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
		functCopyButton->m_data = { ETargetInfo::E_Copy, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

		UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functPasteButton->GetButtonName()->SetText(FText::FromString(""));
		functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
		functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
		functPasteButton->m_data = { ETargetInfo::E_Paste, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

		UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		functDelButton->GetButtonName()->SetText(FText::FromString(""));
		functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
		functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		functDelButton->m_data = { ETargetInfo::E_Delete, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
		basicFunc->GetICEButtonOptions()->AddChild(functDelButton);

		exitFuncArray->basicFunctions.Add(basicFunc);
		exitFuncArray->GetICEContent()->AddChild(basicFunc);
	}
	capsule2->GetExitContent()->AddChild(exitFuncArray);
	capsule2->exitFunc = exitFuncArray;
	capsule2->GetData();

	//INSIDE CAPSULE
	UICE_Button* capsuleButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	capsuleButton->GetButtonName()->SetText(FText::FromString(FString::Printf(TEXT("Inside"))));
	capsuleButton->targetContent = GetCapsuleContent();
	capsuleButton->m_data = { ETargetInfo::E_Select, 1, this, "capsule" };
	GetCapsuleNavBar()->AddChild(capsuleButton);
	capsuleNavBarButtons.Add(capsuleButton);

	UICE_InsideCapsule* capsule = CreateWidget<UICE_InsideCapsule>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_InsideCapsule_BP.ICE_InsideCapsule_BP_C'")));
	capsule->GetCapsuleTitle()->SetText(FText::FromString(FString::Printf(TEXT("Inside"))));
	GetCapsuleContent()->AddChild(capsule);
	capsule->stageIndex = stage;
	capsule->stageData = this;
	capsule->capsuleIndex = 1;
	capsule->GetData();

	//OUTSIDE CAPSULE
	UICE_Button* button1 = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	button1->GetButtonName()->SetText(FText::FromString(FString::Printf(TEXT("Outside"))));
	button1->targetContent = GetCapsuleContent();
	button1->m_data = { ETargetInfo::E_Select, 2, this, "capsule" };
	GetCapsuleNavBar()->AddChild(button1);
	capsuleNavBarButtons.Add(button1);

	UICE_HighlightCapsule* capsule1 = CreateWidget<UICE_HighlightCapsule>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_HighlightCapsule_BP.ICE_HighlightCapsule_BP_C'")));
	capsule1->GetCapsuleTitle()->SetText(FText::FromString(FString::Printf(TEXT("Outside"))));
	GetCapsuleContent()->AddChild(capsule1);
	capsule1->stageIndex = stage;
	capsule1->stageData = this;
	capsule1->capsuleIndex = 2;
	capsule1->GetData();


	UpdateCapsuleNavBar(GetCapsuleContent()->GetActiveWidgetIndex());

	///////////////////////////////////////////////////////////////////////////////
	//TOOLTIP CONSTRUCTION/////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].actions.Num(); i++) {

		FText name = GETENUMDISPLAYNAME(TEXT("EAction"), wrapper->interactComponent->interactDataArray[stage].actions[i].actionButton);


		UICE_Button* buttonTooltip = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		buttonTooltip->GetButtonName()->SetText(name);
		buttonTooltip->targetContent = GetTooltipContent();
		buttonTooltip->m_data = { ETargetInfo::E_Select, i, this, "tooltip" };
		GetTooltipNavBar()->AddChild(buttonTooltip);
		tooltipNavBarButtons.Add(buttonTooltip);

		UICE_Tooltip* tooltip = CreateWidget<UICE_Tooltip>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Tooltip_BP.ICE_tooltip_BP_C'")));
		tooltip->button = buttonTooltip;
		tooltip->stageData = this;
		tooltip->stage = stage;
		tooltip->tooltipIndex = i;

		UICE_Button* tooltipCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		tooltipCopyButton->GetButtonName()->SetText(FText::FromString(""));
		tooltipCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy Tooltip"))));
		tooltipCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		tooltipCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
		tooltipCopyButton->m_data = { ETargetInfo::E_Copy, i, this, "tooltip" };
		tooltip->GetTooltipOptions()->AddChild(tooltipCopyButton);

		UICE_Button* tooltipPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		tooltipPasteButton->GetButtonName()->SetText(FText::FromString(""));
		tooltipPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste Tooltip"))));
		tooltipPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		tooltipPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
		tooltipPasteButton->m_data = { ETargetInfo::E_Paste, i, this, "tooltip" };
		tooltip->GetTooltipOptions()->AddChild(tooltipPasteButton);

		UICE_Button* tooltipDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		tooltipDelButton->GetButtonName()->SetText(FText::FromString(""));
		tooltipDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete Tooltip"))));
		tooltipDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		tooltipDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		tooltipDelButton->m_data = { ETargetInfo::E_Delete, i, this, "tooltip" };
		tooltip->GetTooltipOptions()->AddChild(tooltipDelButton);

		UICE_BasicFunctionArray* stageFuncArray = CreateWidget<UICE_BasicFunctionArray>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunctionArray_BP.ICE_BasicFunctionArray_BP_C'")));
		stageFuncArray->stage = this;
		FString nameStage = "Functions - Stage";
		stageFuncArray->GetICEName()->SetText(FText::FromString(nameStage));
		
		UICE_Button* stageFunctButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageFunctButton->GetButtonName()->SetText(FText::FromString(""));
		stageFunctButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create function"))));
		stageFunctButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageFunctButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		stageFunctButton->m_data = { ETargetInfo::E_Create, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageFunctButton);
		tooltip->stageButton = stageFunctButton;

		UICE_Button* stageArrFunctCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageArrFunctCopyButton->GetButtonName()->SetText(FText::FromString(""));
		stageArrFunctCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy function"))));
		stageArrFunctCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageArrFunctCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
		stageArrFunctCopyButton->m_data = { ETargetInfo::E_Copy, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctCopyButton);

		UICE_Button* stageArrFunctPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageArrFunctPasteButton->GetButtonName()->SetText(FText::FromString(""));
		stageArrFunctPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste function"))));
		stageArrFunctPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageArrFunctPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
		stageArrFunctPasteButton->m_data = { ETargetInfo::E_Paste, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctPasteButton);

		UICE_Button* stageArrFunctDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageArrFunctDelButton->GetButtonName()->SetText(FText::FromString(""));
		stageArrFunctDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete function"))));
		stageArrFunctDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageArrFunctDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		stageArrFunctDelButton->m_data = { ETargetInfo::E_Delete, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctDelButton);
		
		for (int j = 0; j < wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction.Num(); j++) {
			UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
			basicFunc->currentIndex = j;
			basicFunc->stage = this;
			basicFunc->parentArr = stageFuncArray;
			basicFunc->currentStage = stage;
			basicFunc->currentState = { EStateFunctionList::Action, i };
			int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
			int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumName;
			int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumNameCore;
			if (enumValue > 0) {
				basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumName+maxE2-1);
			}
			else {
				basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumNameCore);
			}
			basicFunc->GetData();
			//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));
			if (enumValue > 0) {
				basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumName));
			}
			else {
				basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumNameCore));
			}

			basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
			basicFunc->GetICESearchBar()->basicFun = basicFunc;
			basicFunc->GetICESearchBar()->UpdateValues();

			UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functInsButton->GetButtonName()->SetText(FText::FromString(""));
			functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
			functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			functInsButton->m_data = { ETargetInfo::E_Insert, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

			UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functCopyButton->GetButtonName()->SetText(FText::FromString(""));
			functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
			functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
			functCopyButton->m_data = { ETargetInfo::E_Copy, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

			UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functPasteButton->GetButtonName()->SetText(FText::FromString(""));
			functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
			functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
			functPasteButton->m_data = { ETargetInfo::E_Paste, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

			UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functDelButton->GetButtonName()->SetText(FText::FromString(""));
			functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
			functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			functDelButton->m_data = { ETargetInfo::E_Delete, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functDelButton);


			stageFuncArray->basicFunctions.Add(basicFunc);
			stageFuncArray->GetICEContent()->AddChild(basicFunc);
		}
		tooltip->GetStageFunction()->AddChild(stageFuncArray);

		GetTooltipContent()->AddChild(tooltip);
		tooltip->stageFunc = stageFuncArray;

		tooltip->UpdateImageSelector();
		tooltip->UpdateBackgroundSelector();
		tooltip->GetData();
	}

	UICE_Button* buttonNewTooltip = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
	buttonNewTooltip->ChangeOrange();
	buttonNewTooltip->GetButtonName()->SetText(FText::FromString("+"));
	buttonNewTooltip->SetToolTipText(FText::FromString("Add new tooltip"));
	buttonNewTooltip->m_data = { ETargetInfo::E_Create, -1, this, "tooltip" };
	GetTooltipNavBarAdd()->AddChild(buttonNewTooltip);

	UpdateTooltipNavBar(GetTooltipContent()->GetActiveWidgetIndex());

	GetStageTitle()->SetText(FText::FromString(wrapper->interactComponent->interactDataArray[stage].stageTitle));
	GetStageDescription()->SetText(FText::FromString(wrapper->interactComponent->interactDataArray[stage].stageDescription));

	GetStageTitle()->OnTextChanged.AddDynamic(this, &UICE_Stage::OnTextChanged);
	GetStageDescription()->OnTextChanged.AddDynamic(this, &UICE_Stage::OnTextChanged);

}

void UICE_Stage::OnTextChanged(const FText& text)
{
	button->GetButtonName()->SetText(FText::FromString(GetStageTitle()->GetText().ToString() + " (" + FString::FromInt(stage) + ")"));
	button->GetButtonName()->SetToolTipText(FText::FromString(GetStageDescription()->GetText().ToString()));
}

void UICE_Stage::GetPreStageData()
{
	if (GetPreStageFunctions()->GetAllChildren().Num() > 0) {
		UICE_BasicFunctionArray* basicFuncArray = Cast<UICE_BasicFunctionArray>(GetPreStageFunctions()->GetAllChildren()[0]);
		if (basicFuncArray) {
			basicFuncArray->GetICEContent()->ClearChildren();
			basicFuncArray->basicFunctions.Empty();
			for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].preStageFunction.Num(); i++) {
				UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
				basicFunc->currentIndex = i;
				basicFunc->stage = this;
				basicFunc->parentArr = basicFuncArray;
				basicFunc->currentStage = stage;
				basicFunc->currentState = { EStateFunctionList::PreStage, -1 };
				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumName;
				int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumNameCore;
				if (enumValue > 0) {
					basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumName+maxE2-1);
				}
				else {
					basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumNameCore);
				}
				basicFunc->GetData();
				if (enumValue > 0) {
					basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumName));
				}
				else {
					basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].preStageFunction[i].enumNameCore));
				}
				//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

				basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
				basicFunc->GetICESearchBar()->basicFun = basicFunc;
				basicFunc->GetICESearchBar()->UpdateValues();

				UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functInsButton->GetButtonName()->SetText(FText::FromString(""));
				functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
				functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
				functInsButton->m_data = { ETargetInfo::E_Insert, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
				basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

				UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functCopyButton->GetButtonName()->SetText(FText::FromString(""));
				functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
				functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
				functCopyButton->m_data = { ETargetInfo::E_Copy, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
				basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

				UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functPasteButton->GetButtonName()->SetText(FText::FromString(""));
				functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
				functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
				functPasteButton->m_data = { ETargetInfo::E_Paste, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
				basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

				UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				functDelButton->GetButtonName()->SetText(FText::FromString(""));
				functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
				functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
				functDelButton->m_data = { ETargetInfo::E_Delete, i, basicFunc, "preStage", EActionStageType::Pre, -1 };
				basicFunc->GetICEButtonOptions()->AddChild(functDelButton);

				basicFuncArray->basicFunctions.Add(basicFunc);
				basicFuncArray->GetICEContent()->AddChild(basicFunc);
			}
		}
	}
}

void UICE_Stage::GetInitData()
{
	if (GetCapsuleContent()->GetAllChildren().Num() > 0) {
		UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(GetCapsuleContent()->GetAllChildren()[0]);
		if (capsule2) {
			if (capsule2->GetInitContent()->GetAllChildren().Num() > 0) {
				UICE_BasicFunctionArray* initFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetInitContent()->GetAllChildren()[0]);
				if (initFuncArray) {
					initFuncArray->GetICEContent()->ClearChildren();
					initFuncArray->basicFunctions.Empty();
					for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction.Num(); i++) {
						UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
						basicFunc->currentIndex = i;
						basicFunc->stage = this;
						basicFunc->parentArr = initFuncArray;
						basicFunc->currentStage = stage;
						basicFunc->currentState = { EStateFunctionList::Init, -1 };
						int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
						int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumName;
						int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumNameCore;
						if (enumValue > 0) {
							basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumName+maxE2-1);
						}
						else {
							basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumNameCore);
						}
						basicFunc->GetData();
						if (enumValue > 0) {
							basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumName));
						}
						else {
							basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.initFunction[i].enumNameCore));
						}
						//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

						basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
						basicFunc->GetICESearchBar()->basicFun = basicFunc;
						basicFunc->GetICESearchBar()->UpdateValues();

						UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functInsButton->GetButtonName()->SetText(FText::FromString(""));
						functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
						functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
						functInsButton->m_data = { ETargetInfo::E_Insert, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

						UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functCopyButton->GetButtonName()->SetText(FText::FromString(""));
						functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
						functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
						functCopyButton->m_data = { ETargetInfo::E_Copy, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

						UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functPasteButton->GetButtonName()->SetText(FText::FromString(""));
						functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
						functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
						functPasteButton->m_data = { ETargetInfo::E_Paste, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

						UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functDelButton->GetButtonName()->SetText(FText::FromString(""));
						functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
						functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
						functDelButton->m_data = { ETargetInfo::E_Delete, i, basicFunc, "initTrigger", EActionStageType::Init, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functDelButton);

						initFuncArray->basicFunctions.Add(basicFunc);
						initFuncArray->GetICEContent()->AddChild(basicFunc);
					}
				}
			}
		}
	}
}

void UICE_Stage::GetExitData()
{
	if (GetCapsuleContent()->GetAllChildren().Num() > 0) {
		UICE_TriggerCapsule* capsule2 = Cast<UICE_TriggerCapsule>(GetCapsuleContent()->GetAllChildren()[0]);
		if (capsule2) {
			if (capsule2->GetExitContent()->GetAllChildren().Num() > 0) {
				UICE_BasicFunctionArray* exitFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetExitContent()->GetAllChildren()[0]);
				if (exitFuncArray) {
					exitFuncArray->GetICEContent()->ClearChildren();
					exitFuncArray->basicFunctions.Empty();
					for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction.Num(); i++) {
						UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
						basicFunc->currentIndex = i;
						basicFunc->stage = this;
						basicFunc->parentArr = exitFuncArray;
						basicFunc->currentStage = stage;
						basicFunc->currentState = { EStateFunctionList::Exit, -1 };
						int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
						int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumName;
						int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumNameCore;
						if (enumValue > 0) {
							basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumName+maxE2-1);
						}
						else {
							basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumNameCore);
						}
						basicFunc->GetData();
						if (enumValue > 0) {
							basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumName));
						}
						else {
							basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].capsuleTriggerStruct.exitFunction[i].enumNameCore));
						}
						//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

						basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
						basicFunc->GetICESearchBar()->basicFun = basicFunc;
						basicFunc->GetICESearchBar()->UpdateValues();

						UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functInsButton->GetButtonName()->SetText(FText::FromString(""));
						functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
						functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
						functInsButton->m_data = { ETargetInfo::E_Insert, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

						UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functCopyButton->GetButtonName()->SetText(FText::FromString(""));
						functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
						functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
						functCopyButton->m_data = { ETargetInfo::E_Copy, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

						UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functPasteButton->GetButtonName()->SetText(FText::FromString(""));
						functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
						functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
						functPasteButton->m_data = { ETargetInfo::E_Paste, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

						UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functDelButton->GetButtonName()->SetText(FText::FromString(""));
						functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
						functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
						functDelButton->m_data = { ETargetInfo::E_Delete, i, basicFunc, "initTrigger", EActionStageType::Exit, -1 };
						basicFunc->GetICEButtonOptions()->AddChild(functDelButton);

						exitFuncArray->basicFunctions.Add(basicFunc);
						exitFuncArray->GetICEContent()->AddChild(basicFunc);
					}
				}
			}
		}
	}
}

void UICE_Stage::GetStageData(int stIndex)
{
	if (GetTooltipContent()->GetAllChildren().IsValidIndex(stIndex)) {
		UICE_Tooltip* capsule2 = Cast<UICE_Tooltip>(GetTooltipContent()->GetAllChildren()[stIndex]);
		if (capsule2) {
			if (capsule2->GetStageFunction()->GetAllChildren().Num() > 0) {
				UICE_BasicFunctionArray* stageFuncArray = Cast<UICE_BasicFunctionArray>(capsule2->GetStageFunction()->GetAllChildren()[0]);
				if (stageFuncArray) {
					stageFuncArray->GetICEContent()->ClearChildren();
					stageFuncArray->basicFunctions.Empty();
					for (int j = 0; j < wrapper->interactComponent->interactDataArray[stage].actions[stIndex].stageFunction.Num(); j++) {
						UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
						basicFunc->currentIndex = j;
						basicFunc->stage = this;
						basicFunc->parentArr = stageFuncArray;
						basicFunc->currentStage = stage;
						basicFunc->currentState = { EStateFunctionList::Action,stIndex };
						int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
						int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].actions[stIndex].stageFunction[j].enumName;
						int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].actions[stIndex].stageFunction[j].enumNameCore;
						if (enumValue > 0) {
							basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].actions[stIndex].stageFunction[j].enumName+maxE2-1);
						}
						else {
							basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].actions[stIndex].stageFunction[j].enumNameCore);
						}
						basicFunc->GetData();
						if (enumValue > 0) {
							basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].actions[stIndex].stageFunction[j].enumName));
						}
						else {
							basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].actions[stIndex].stageFunction[j].enumNameCore));
						}
						//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

						basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
						basicFunc->GetICESearchBar()->basicFun = basicFunc;
						basicFunc->GetICESearchBar()->UpdateValues();

						UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functInsButton->GetButtonName()->SetText(FText::FromString(""));
						functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
						functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
						functInsButton->m_data = { ETargetInfo::E_Insert, j, basicFunc, "stageInteract", EActionStageType::Interact, stIndex };
						basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

						UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functCopyButton->GetButtonName()->SetText(FText::FromString(""));
						functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
						functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
						functCopyButton->m_data = { ETargetInfo::E_Copy, j, basicFunc, "stageInteract", EActionStageType::Interact, stIndex };
						basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

						UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functPasteButton->GetButtonName()->SetText(FText::FromString(""));
						functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
						functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
						functPasteButton->m_data = { ETargetInfo::E_Paste, j, basicFunc, "stageInteract", EActionStageType::Interact, stIndex };
						basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

						UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
						functDelButton->GetButtonName()->SetText(FText::FromString(""));
						functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
						functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
						functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
						functDelButton->m_data = { ETargetInfo::E_Delete, j, basicFunc, "stageInteract", EActionStageType::Interact, stIndex };
						basicFunc->GetICEButtonOptions()->AddChild(functDelButton);


						stageFuncArray->basicFunctions.Add(basicFunc);
						stageFuncArray->GetICEContent()->AddChild(basicFunc);
					}
				}
			}
		}
	}
}

void UICE_Stage::GetTooltipData()
{
	tooltipNavBarButtons.Empty();
	GetTooltipNavBar()->ClearChildren();
	GetTooltipContent()->ClearChildren();
	for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].actions.Num(); i++) {

		FText name = GETENUMDISPLAYNAME(TEXT("EAction"), wrapper->interactComponent->interactDataArray[stage].actions[i].actionButton);


		UICE_Button* buttonTooltip = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		buttonTooltip->GetButtonName()->SetText(name);
		buttonTooltip->targetContent = GetTooltipContent();
		buttonTooltip->m_data = { ETargetInfo::E_Select, i, this, "tooltip" };
		GetTooltipNavBar()->AddChild(buttonTooltip);
		tooltipNavBarButtons.Add(buttonTooltip);

		UICE_Tooltip* tooltip = CreateWidget<UICE_Tooltip>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Tooltip_BP.ICE_tooltip_BP_C'")));
		tooltip->button = buttonTooltip;
		tooltip->stageData = this;
		tooltip->stage = stage;
		tooltip->tooltipIndex = i;

		UICE_Button* tooltipCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		tooltipCopyButton->GetButtonName()->SetText(FText::FromString(""));
		tooltipCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy Tooltip"))));
		tooltipCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		tooltipCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
		tooltipCopyButton->m_data = { ETargetInfo::E_Copy, i, this, "tooltip" };
		tooltip->GetTooltipOptions()->AddChild(tooltipCopyButton);

		UICE_Button* tooltipPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		tooltipPasteButton->GetButtonName()->SetText(FText::FromString(""));
		tooltipPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste Tooltip"))));
		tooltipPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		tooltipPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
		tooltipPasteButton->m_data = { ETargetInfo::E_Paste, i, this, "tooltip" };
		tooltip->GetTooltipOptions()->AddChild(tooltipPasteButton);

		UICE_Button* tooltipDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		tooltipDelButton->GetButtonName()->SetText(FText::FromString(""));
		tooltipDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete Tooltip"))));
		tooltipDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		tooltipDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		tooltipDelButton->m_data = { ETargetInfo::E_Delete, i, this, "tooltip" };
		tooltip->GetTooltipOptions()->AddChild(tooltipDelButton);

		UICE_BasicFunctionArray* stageFuncArray = CreateWidget<UICE_BasicFunctionArray>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunctionArray_BP.ICE_BasicFunctionArray_BP_C'")));
		stageFuncArray->stage = this;
		FString nameStage = "Functions - Stage";
		stageFuncArray->GetICEName()->SetText(FText::FromString(nameStage));

		UICE_Button* stageFunctButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageFunctButton->GetButtonName()->SetText(FText::FromString(""));
		stageFunctButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create function"))));
		stageFunctButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageFunctButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		stageFunctButton->m_data = { ETargetInfo::E_Create, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageFunctButton);
		tooltip->stageButton = stageFunctButton;

		UICE_Button* stageArrFunctCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageArrFunctCopyButton->GetButtonName()->SetText(FText::FromString(""));
		stageArrFunctCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy function"))));
		stageArrFunctCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageArrFunctCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
		stageArrFunctCopyButton->m_data = { ETargetInfo::E_Copy, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctCopyButton);

		UICE_Button* stageArrFunctPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageArrFunctPasteButton->GetButtonName()->SetText(FText::FromString(""));
		stageArrFunctPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste function"))));
		stageArrFunctPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageArrFunctPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
		stageArrFunctPasteButton->m_data = { ETargetInfo::E_Paste, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctPasteButton);

		UICE_Button* stageArrFunctDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		stageArrFunctDelButton->GetButtonName()->SetText(FText::FromString(""));
		stageArrFunctDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete function"))));
		stageArrFunctDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		stageArrFunctDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		stageArrFunctDelButton->m_data = { ETargetInfo::E_Delete, i, stageFuncArray, "stageInteract", EActionStageType::Interact, i };
		stageFuncArray->GetICEButtonOptions()->AddChild(stageArrFunctDelButton);

		for (int j = 0; j < wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction.Num(); j++) {
			UICE_BasicFunction * basicFunc = CreateWidget<UICE_BasicFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_BasicFunction_BP.ICE_BasicFunction_BP_C'")));
			basicFunc->currentIndex = j;
			basicFunc->stage = this;
			basicFunc->parentArr = stageFuncArray;
			basicFunc->currentStage = stage;
			basicFunc->currentState = { EStateFunctionList::Action, i };
			int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
			int enumValue = (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumName;
			int enumValueCore = (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumNameCore;
			if (enumValue > 0) {
				basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumName+maxE2-1);
			}
			else {
				basicFunc->SetEnum((int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumNameCore);
			}
			basicFunc->GetData();
			if (enumValue > 0) {
				basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumName));
			}
			else {
				basicFunc->SetName(GETENUMSTRING(TEXT("EActionListCore"), (int)wrapper->interactComponent->interactDataArray[stage].actions[i].stageFunction[j].enumNameCore));
			}
			//basicFunc->SetName(GETENUMSTRING(TEXT("EActionList"), basicFunc->GetEnum()));

			basicFunc->GetICESearchBar()->selector = basicFunc->GetICEEnum();
			basicFunc->GetICESearchBar()->basicFun = basicFunc;
			basicFunc->GetICESearchBar()->UpdateValues();

			UICE_Button* functInsButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functInsButton->GetButtonName()->SetText(FText::FromString(""));
			functInsButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert"))));
			functInsButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functInsButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			functInsButton->m_data = { ETargetInfo::E_Insert, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functInsButton);

			UICE_Button* functCopyButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functCopyButton->GetButtonName()->SetText(FText::FromString(""));
			functCopyButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Copy"))));
			functCopyButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functCopyButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/copy.copy'"), NULL, LOAD_None, NULL));
			functCopyButton->m_data = { ETargetInfo::E_Copy, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functCopyButton);

			UICE_Button* functPasteButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functPasteButton->GetButtonName()->SetText(FText::FromString(""));
			functPasteButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Paste"))));
			functPasteButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functPasteButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/paste.paste'"), NULL, LOAD_None, NULL));
			functPasteButton->m_data = { ETargetInfo::E_Paste, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functPasteButton);

			UICE_Button* functDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			functDelButton->GetButtonName()->SetText(FText::FromString(""));
			functDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete"))));
			functDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			functDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			functDelButton->m_data = { ETargetInfo::E_Delete, j, basicFunc, "stageInteract", EActionStageType::Interact, i };
			basicFunc->GetICEButtonOptions()->AddChild(functDelButton);


			stageFuncArray->basicFunctions.Add(basicFunc);
			stageFuncArray->GetICEContent()->AddChild(basicFunc);
		}
		tooltip->GetStageFunction()->AddChild(stageFuncArray);

		GetTooltipContent()->AddChild(tooltip);
		tooltip->stageFunc = stageFuncArray;

		tooltip->UpdateImageSelector();
		tooltip->UpdateBackgroundSelector();
		tooltip->GetData();
	}
}

void UICE_Stage::GetAllData()
{
	GetPreStageData();
	GetInitData();
	GetExitData();
	GetTooltipData();
	for (int i = 0; i < wrapper->interactComponent->interactDataArray[stage].actions.Num(); i++) {
		GetStageData(i);
	}
	GetData();
}

void UICE_Stage::GetData()
{
	if (GetPreStageFunctions()) {
		TArray<UWidget*> children = GetPreStageFunctions()->GetAllChildren();
		for (int i = 0; i < children.Num(); i++) {
			UICE_BasicFunctionArray* funcArray = Cast<UICE_BasicFunctionArray>(children[i]);
			if (funcArray && funcArray->GetICEContent()) {
				TArray<UWidget*> funcChildren = funcArray->GetICEContent()->GetAllChildren();
				for (int j = 0; j < funcChildren.Num(); j++) {
					UICE_BasicFunction * basicFunc = Cast<UICE_BasicFunction>(funcChildren[j]);
					if (basicFunc) {
						basicFunc->GetData();
					}
				}
			}
		}
	}

	if (GetCapsuleContent()) {
		TArray<UWidget*> children = GetCapsuleContent()->GetAllChildren();
		for (int i = 0; i < children.Num(); i++) {
			UICE_TriggerCapsule* triggerCapsule = Cast<UICE_TriggerCapsule>(children[i]);
			if (triggerCapsule) {
				triggerCapsule->GetData();
				continue;
			}
			UICE_InsideCapsule* insideCapsule = Cast<UICE_InsideCapsule>(children[i]);
			if (insideCapsule) {
				insideCapsule->GetData();
				continue;
			}
			UICE_HighlightCapsule* highlightCapsule = Cast<UICE_HighlightCapsule>(children[i]);
			if (highlightCapsule) {
				highlightCapsule->GetData();
				continue;
			}
			UICE_Capsule* capsule = Cast<UICE_Capsule>(children[i]);
			if (capsule) {
				capsule->GetData();
			}
		}
	}

	if (GetTooltipContent()) {
		TArray<UWidget*> children = GetTooltipContent()->GetAllChildren();
		for (int i = 0; i < children.Num(); i++) {
			UICE_Tooltip* tooltip = Cast<UICE_Tooltip>(children[i]);
			if (tooltip) {
				tooltip->GetData();
			}
		}
	}

// 	FString namePrio = "Priority";
// 	GetPriority()->GetICENameContainer()->SetText(FText::FromString(namePrio));
// 	if (GetPriority()->GetICEEnum()->GetOptionCount() == 0) {
// 		int maxE = (int)GETENUMMAX(TEXT("EPriority"));
// 		for (int i = 0; i < maxE; i++) {
// 			GetPriority()->GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EPriority"), (EPriority)i));
// 		}
// 	}
	GetPriority()->GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EPriority"), wrapper->interactComponent->interactDataArray[stage].priorityStage));

	//GetPriority()->searchBar = CreateWidget<UICE_Search>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Search_BP.ICE_Search_BP_C'")));
	GetPriority()->GetICESearchBar()->selector = GetPriority()->GetICEEnum();
	GetPriority()->GetICESearchBar()->UpdateValues();


	GetPriorityRepeat()->SetIsChecked(wrapper->interactComponent->interactDataArray[stage].doOtherInteract);
}

void UICE_Stage::UpdateData()
{
	if (GetPreStageFunctions()) {
		TArray<UWidget*> children = GetPreStageFunctions()->GetAllChildren();
		for (int i = 0; i < children.Num(); i++) {
			UICE_BasicFunctionArray* funcArray = Cast<UICE_BasicFunctionArray>(children[i]);
			if (funcArray && funcArray->GetICEContent()) {
				TArray<UWidget*> funcChildren = funcArray->GetICEContent()->GetAllChildren();
				for (int j = 0; j < funcChildren.Num(); j++) {
					UICE_BasicFunction * basicFunc = Cast<UICE_BasicFunction>(funcChildren[j]);
					if (basicFunc) {
						if (wrapper->interactComponent->interactDataArray[stage].preStageFunction.IsValidIndex(j)) {
							basicFunc->UpdateData();
						}
					}
				}
			}
		}
	}

	if (GetCapsuleContent()) {
		TArray<UWidget*> children = GetCapsuleContent()->GetAllChildren();
		for (int i = 0; i < children.Num(); i++) {
			UICE_TriggerCapsule* triggerCapsule = Cast<UICE_TriggerCapsule>(children[i]);
			if (triggerCapsule) {
				triggerCapsule->UpdateData();
			}
			UICE_InsideCapsule* insideCapsule = Cast<UICE_InsideCapsule>(children[i]);
			if (insideCapsule) {
				insideCapsule->UpdateData();
			}
			UICE_HighlightCapsule* highlightCapsule = Cast<UICE_HighlightCapsule>(children[i]);
			if (highlightCapsule) {
				highlightCapsule->UpdateData();
			}
			UICE_Capsule* capsule = Cast<UICE_Capsule>(children[i]);
			if (capsule) {
				capsule->UpdateData();
			}
		}
	}

	if (GetTooltipContent()) {
		TArray<UWidget*> children = GetTooltipContent()->GetAllChildren();
		for (int i = 0; i < children.Num(); i++) {
			UICE_Tooltip* tooltip = Cast<UICE_Tooltip>(children[i]);
			if (tooltip && wrapper->interactComponent->interactDataArray[stage].actions.IsValidIndex(i)) {
				tooltip->UpdateData();
			}
			else {
				GetAllData();
			}
		}
	}

	wrapper->interactComponent->interactDataArray[stage].stageTitle = GetStageTitle()->GetText().ToString();
	wrapper->interactComponent->interactDataArray[stage].stageDescription = GetStageDescription()->GetText().ToString();

	wrapper->interactComponent->interactDataArray[stage].priorityStage = (EPriority)GetPriority()->GetICEEnum()->GetSelectedIndex();

	wrapper->interactComponent->interactDataArray[stage].doOtherInteract = GetPriorityRepeat()->IsChecked();

}

void UICE_Stage::UpdateCapsuleNavBar(int currentTab)
{
	for (int i = 0; i < capsuleNavBarButtons.Num(); i++)
	{
		if (currentTab == i) {
			capsuleNavBarButtons[i]->ChangeLightGrey();
		}
		else {
			capsuleNavBarButtons[i]->ChangeMidGrey();
		}
	}
}
void UICE_Stage::UpdateTooltipNavBar(int currentTab)
{
	for (int i = 0; i < tooltipNavBarButtons.Num(); i++)
	{
		if (currentTab == i) {
			tooltipNavBarButtons[i]->ChangeLightGrey();
		}
		else {
			tooltipNavBarButtons[i]->ChangeMidGrey();
		}
	}
}

