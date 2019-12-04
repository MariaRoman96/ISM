#include "ICE_BasicFunction.h"
#include "ICE_Float.h"
#include "ICE_Integer.h"
#include "ICE_BasicFunctionArray.h"
#include "ICE_String.h"
#include "ICE_Boolean.h"
#include "ICE_FName.h"
#include "ICE_Vector.h"
#include "ICE_Rotator.h"
#include "ICE_Struct.h"
#include "ICE_ActorFunction.h"
#include "ICE_Enum.h"
#include "InteractComponent.h"
#include "ICE_Array.h"
#include "ICE_ArrayElement.h"
#include <HorizontalBoxSlot.h>
#include "ICE_Object.h"
#include <UnrealType.h>
#include "ICE_Color.h"

using namespace std;

void UICE_BasicFunction::NativeConstruct()
{
	if (GetICEEnum()->GetOptionCount() == 0) {
		int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
		for (int i = 0; i < maxE2; i++) {
			GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EActionListCore"), (EActionListCore)i));
		}
		int maxE = (int)GETENUMMAX(TEXT("EActionList"));
		for (int j = 1; j < maxE; j++) {
			GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EActionList"), (EActionList)j));
		}
		GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EActionListCore"), (EActionList)0));
	}
	GetICEEnum()->OnSelectionChanged.AddDynamic(this, &UICE_BasicFunction::OnChangeSelect);
}

FString UICE_BasicFunction::GetName()
{
	return GetICEName()->GetText().ToString();
}

void UICE_BasicFunction::SetName(FString val)
{
	if (currentState.state == EStateFunctionList::PreStage) {
		GetICEName()->SetText(FText::FromString(FString::FromInt(currentIndex)+" - "+val));
	}
	else if (currentState.state == EStateFunctionList::Init) {
		GetICEName()->SetText(FText::FromString(FString::FromInt(currentIndex) + " - " + val));
	}
	else if (currentState.state == EStateFunctionList::Exit) {
		GetICEName()->SetText(FText::FromString(FString::FromInt(currentIndex) + " - " + val));
	}
	else if (currentState.state == EStateFunctionList::Action) {
		GetICEName()->SetText(FText::FromString(FString::FromInt(currentIndex) + " - " + val));
	}
}

void UICE_BasicFunction::OnChangeSelect(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::OnMouseClick) {
		UpdateData();
		GetData();
	}
}

FString UICE_BasicFunction::GetEnumString() {
	return GetICEEnum()->GetSelectedOption();
}

EActionList UICE_BasicFunction::GetEnum() {
	return (EActionList)GetICEEnum()->GetSelectedIndex();
}

void UICE_BasicFunction::SetEnum(int val) {
	int maxE = (int)GETENUMMAX(TEXT("EActionList"));
	int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
	if (GetICEEnum()->GetOptionCount() == 0) {
		for (int i = 0; i < maxE2; i++) {
			GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EActionListCore"), (EActionListCore)i));
		}
		for (int j = 1; j < maxE; j++) {
			GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EActionList"), (EActionList)j));
		}
	}
	int otherVal = val - maxE2 + 1;
	if (val < maxE2) {
		GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EActionListCore"), (EActionListCore)val));
	}
	else if(otherVal < maxE){
		GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EActionList"), (EActionList)(otherVal)));
	}
}

void UICE_BasicFunction::GetDataInternal(UPanelWidget* panel, void * dir2, UProperty* It2, UStructProperty* parentStruct)
{
#if WITH_EDITOR
	if (It2->HasMetaData("EditCondition")) {
		FString editValue = It2->GetMetaData("EditCondition");
		if (editValue.StartsWith("!")) {
			FString noneS;
			FString resS;
			editValue.Split("!", &noneS, &resS);
			UProperty* editProp = FindField<UProperty>(parentStruct->Struct, *resS);
			if (editProp) {
				UBoolProperty* editBool = Cast<UBoolProperty>(editProp);
				if (editBool) {
					if (editBool->GetPropertyValue_InContainer(dir2, 0)) {
						return;
					}
				}
			}
		}
		else {
			UProperty* editProp = FindField<UProperty>(parentStruct->Struct, *editValue);
			if (editProp) {
				UBoolProperty* editBool = Cast<UBoolProperty>(editProp);
				if (editBool) {
					if (!editBool->GetPropertyValue_InContainer(dir2, 0)) {
						return;
					}
				}
			}
		}
	}
	
	if (It2->GetCPPType() == "EDelegateListCore" || It2->GetCPPType() == "EActionListCore" || It2->GetCPPType() == "ETimerListCore") {
		return;
	}

	if (It2->GetCPPType() == "EDelegateList" || It2->GetCPPType() == "EActionList" || It2->GetCPPType() == "ETimerList") {
		for (TFieldIterator<UProperty> It2Core(parentStruct->Struct); It2Core; ++It2Core) {
			if (It2Core->GetCPPType() == It2->GetCPPType()+"Core") {
				UICE_Enum* property = CreateWidget<UICE_Enum>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Enum_BP.ICE_Enum_BP_C'")));
				property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));

				UEnumProperty* It4Core = Cast<UEnumProperty>(*It2Core);
				int maxECore = 1;
				TArray<FString> enums = UInteractFunctionsClass::enumFunctions;
				for (int i = 0; i < enums.Num(); i++) {
					if (It2Core->GetCPPType() == enums[i]) {
						const TCHAR* enumNa = reinterpret_cast<const TCHAR*>(*enums[i]);
						maxECore = (int)GETENUMMAX(enumNa);
						for (int j = 0; j < maxECore; j++) {
							property->GetICEEnum()->AddOption(GETENUMSTRING(enumNa, j));
						}
						break;
					}
				}
				uint8 actorEnumCore = *It4Core->ContainerPtrToValuePtr<uint8>(dir2);

				UEnumProperty* It4 = Cast<UEnumProperty>(It2);
				for (int i = 0; i < enums.Num(); i++) {
					if (It2->GetCPPType() == enums[i]) {
						const TCHAR* enumNa = reinterpret_cast<const TCHAR*>(*enums[i]);
						int maxE = (int)GETENUMMAX(enumNa);
						for (int j = 1; j < maxE; j++) {
							property->GetICEEnum()->AddOption(GETENUMSTRING(enumNa, j));
						}
						break;
					}
				}
				uint8 actorEnum = *It4->ContainerPtrToValuePtr<uint8>(dir2);

				int offsetEnum = actorEnum + maxECore - 1;
				if (actorEnum > 0) {
					property->GetICEEnum()->SetSelectedIndex(offsetEnum);
				}
				else {
					property->GetICEEnum()->SetSelectedIndex(actorEnumCore);
				}

				property->GetICESearchBar()->selector = property->GetICEEnum();
				property->GetICESearchBar()->UpdateValues();
				panel->AddChild(property);
				return;
			}
		}
	}

	if (It2->GetCPPType() == "float") {
		UICE_Float* property = CreateWidget<UICE_Float>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Float_BP.ICE_Float_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		float val = Cast<UFloatProperty>(It2)->GetPropertyValue_InContainer(dir2, 0);
		property->SetValue(val);

		if (It2->HasMetaData("ClampMin")) {
			FString clampMinValue = It2->GetMetaData("ClampMin");
			property->GetICEContainer()->SetMinValue(UInteractCore::GetValue(clampMinValue));
			property->GetICEContainer()->SetMinSliderValue(UInteractCore::GetValue(clampMinValue));
		}
		if (It2->HasMetaData("ClampMax")) {
			FString clampMaxValue = It2->GetMetaData("ClampMax");
			property->GetICEContainer()->SetMaxValue(UInteractCore::GetValue(clampMaxValue));
			property->GetICEContainer()->SetMaxSliderValue(UInteractCore::GetValue(clampMaxValue));
		}

		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "int32") {
		UICE_Integer* property = CreateWidget<UICE_Integer>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Integer_BP.ICE_Integer_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		int val = Cast<UIntProperty>(It2)->GetPropertyValue_InContainer(dir2, 0);
		property->SetValue(val);

		if (It2->HasMetaData("ClampMin")) {
			FString clampMinValue = It2->GetMetaData("ClampMin");
			property->GetICEContainer()->SetMinValue(UInteractCore::GetValue(clampMinValue));
			property->GetICEContainer()->SetMinSliderValue(UInteractCore::GetValue(clampMinValue));
		}
		if (It2->HasMetaData("ClampMax")) {
			FString clampMaxValue = It2->GetMetaData("ClampMax");
			property->GetICEContainer()->SetMaxValue(UInteractCore::GetValue(clampMaxValue));
			property->GetICEContainer()->SetMaxSliderValue(UInteractCore::GetValue(clampMaxValue));
		}

		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "FString") {
		UICE_String* property = CreateWidget<UICE_String>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_String_BP.ICE_String_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		UStrProperty* cuProp = Cast<UStrProperty>(It2);
		FString val = "";
		val.Reserve(cuProp->GetPropertyValue_InContainer(dir2, 0).Len());
		val = cuProp->GetPropertyValue_InContainer(dir2, 0);
		property->SetValue(val);
		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "bool") {
		UICE_Boolean* property = CreateWidget<UICE_Boolean>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Boolean_BP.ICE_Boolean_BP_C'")));
		property->SetName(It2->GetName(),It2->GetMetaData("Tooltip"));
		property->basicFunc = this;
		bool val = Cast<UBoolProperty>(It2)->GetPropertyValue_InContainer(dir2, 0);
		property->SetValue(val);
		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "FName") {
		UICE_FName* property = CreateWidget<UICE_FName>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_FName_BP.ICE_FName_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		FName val = Cast<UNameProperty>(It2)->GetPropertyValue_InContainer(dir2, 0);
		property->SetValue(val);
		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "FVector") {
		UICE_Vector* property = CreateWidget<UICE_Vector>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Vector_BP.ICE_Vector_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		UStructProperty* It4 = Cast<UStructProperty>(It2);
		FVector vectorVal = { 0.f, 0.f, 0.f };
		void * dir3 = static_cast<uint8_t*>(dir2) + It4->GetOffset_ForInternal();
		It4->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
		property->SetValue(vectorVal);
		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "FRotator") {
		UICE_Rotator* property = CreateWidget<UICE_Rotator>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Rotator_BP.ICE_Rotator_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		UStructProperty* It4 = Cast<UStructProperty>(It2);
		FRotator vectorVal = { 0.f, 0.f, 0.f };
		void * dir3 = static_cast<uint8_t*>(dir2) + It4->GetOffset_ForInternal();
		It4->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
		property->SetValue(vectorVal);
		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "FColor") {
		UICE_Color* property = CreateWidget<UICE_Color>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Color_BP.ICE_Color_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		UStructProperty* It4 = Cast<UStructProperty>(It2);
		FColor vectorVal = { 0, 0, 0, 0 };
		void * dir3 = static_cast<uint8_t*>(dir2) + It4->GetOffset_ForInternal();
		It4->CopyValuesInternal(reinterpret_cast<void*>(&vectorVal), dir3, 1);
		property->SetValue(vectorVal);
		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "FFunctionActorData" || It2->GetCPPType() == "FFunctionCheckData" || It2->GetCPPType() == "FFunctionCheckDataBase"
		|| It2->GetCPPType() == "FFunctionDataWrapper" || It2->GetCPPType() == "FFunctionDataBase" || It2->GetCPPType() == "FConditionAnd") {
		UICE_ActorFunction* property = CreateWidget<UICE_ActorFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ActorFunction_BP.ICE_ActorFunction_BP_C'")));
		FString enumString = "";
		if (It2->GetCPPType() == "FFunctionActorData") {
			enumString = "EGetActorList";
		}
		else if (It2->GetCPPType() == "FFunctionCheckData") {
			enumString = "ECheckList";
		}
		else if (It2->GetCPPType() == "FFunctionCheckDataBase") {
			enumString = "ECheckList";
		}
		else if (It2->GetCPPType() == "FFunctionDataWrapper") {
			enumString = "EActionList";
		}
		else if (It2->GetCPPType() == "FFunctionDataBase") {
			enumString = "EActionList";
		}
		property->functionType = It2->GetCPPType();
		property->GetICEName()->SetText(It2->GetDisplayNameText());
		property->realName = It2->GetName();
		property->wrapper = this;
		UStructProperty* It4 = Cast<UStructProperty>(It2);
		void * dir3 = static_cast<uint8_t*>(dir2) + It4->GetOffset_ForInternal();
		int actorEnum = 0;
		UProperty* It9 = FindField<UProperty>(It4->Struct, "enumName");
		if (It9) {
			UEnumProperty* It6 = Cast<UEnumProperty>(It9);
			if (It6) {
				actorEnum = (int)*It6->ContainerPtrToValuePtr<uint8>(dir3);
			}
		}
		
		int actorEnumCore = 0;
		UProperty* It9Core = FindField<UProperty>(It4->Struct, "enumNameCore");
		if (It9Core) {
			UEnumProperty* It6 = Cast<UEnumProperty>(It9Core);
			if (It6) {
				actorEnumCore = (int)*It6->ContainerPtrToValuePtr<uint8>(dir3);
			}
		}

// 		int maxE = (int)GETENUMMAX(TEXT("EActionList"));
// 		int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
// 		int otherVal = val - maxE2 + 1;
// 		if (val < maxE2) {
// 			GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EActionListCore"), (EActionList)val));
// 		}
// 		else if (otherVal < maxE) {
// 			GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EActionList"), (EActionList)(otherVal)));
// 		}



		if (enumString == "EGetActorList") {
			property->SetEnum(actorEnum);
			UProperty* It10 = FindField<UProperty>(It4->Struct, *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
			if (It10) {
				UStructProperty* It7 = Cast<UStructProperty>(It10);
				if (It7) {
					void * dir4 = static_cast<uint8_t*>(dir3) + It7->GetOffset_ForInternal();
					for (TFieldIterator<UProperty> It8(It7->Struct); It8; ++It8) {
						GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir4, *It8, It7);
					}
				}
			}
		}
		else {
			FString enumStringCore = enumString + "Core";
			int maxE2 = (int)GETENUMMAX(reinterpret_cast<const TCHAR*>(*enumStringCore));
			int otherVal = actorEnum + maxE2 - 1;
			if (actorEnumCore > 0) {
				property->SetEnum((int)actorEnumCore);
				UProperty* It10 = FindField<UProperty>(It4->Struct, *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), actorEnumCore));
				if (It10) {
					UStructProperty* It7 = Cast<UStructProperty>(It10);
					if (It7) {
						void * dir4 = static_cast<uint8_t*>(dir3) + It7->GetOffset_ForInternal();
						for (TFieldIterator<UProperty> It8(It7->Struct); It8; ++It8) {
							GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir4, *It8, It7);
						}
					}
				}
			}
			else if (actorEnum > 0) {
				property->SetEnum((int)otherVal);
				UProperty* It10 = FindField<UProperty>(It4->Struct, *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
				if (It10) {
					UStructProperty* It7 = Cast<UStructProperty>(It10);
					if (It7) {
						void * dir4 = static_cast<uint8_t*>(dir3) + It7->GetOffset_ForInternal();
						for (TFieldIterator<UProperty> It8(It7->Struct); It8; ++It8) {
							GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir4, *It8, It7);
						}
					}
				}
			}
			else {
				property->SetEnum(0);
			}
		}

		property->GetICESearchBar()->selector = property->GetICEEnum();
		property->GetICESearchBar()->actorFun = property;
		property->GetICESearchBar()->UpdateValues();

		panel->AddChild(property);
		actorArr2.Add(property);
	}
	else if (It2->GetCPPType().StartsWith("F")) {
		UICE_Struct* property = CreateWidget<UICE_Struct>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Struct_BP.ICE_Struct_BP_C'")));
		property->GetICEName()->SetText(FText::FromString(It2->GetName()));
		UStructProperty* It4 = Cast<UStructProperty>(It2);
		void * dir3 = static_cast<uint8_t*>(dir2) + It4->GetOffset_ForInternal();
		for (TFieldIterator<UProperty> It5(It4->Struct); It5; ++It5) {
			GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir3, *It5, It4);
		}
		panel->AddChild(property);
	}
	else if (It2->GetCPPType().StartsWith("U") || It2->GetCPPType().StartsWith("A") || It2->GetCPPType().StartsWith("TSubclassOf")) {
		UICE_Object* property = CreateWidget<UICE_Object>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Object_BP.ICE_Object_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		UObjectProperty* It4 = Cast<UObjectProperty>(It2);
		UObject* value = It4->GetPropertyValue_InContainer(dir2, 0);
		property->GetICEContent()->AddOption("None");
		property->elementArray.Empty();
		//TODO HACER PARA TODAS LAS DEMAS LAS CLASES
		if (It2->GetCPPType() == "UClass*" || It2->GetCPPType().Contains("TSubclassOf<AActor>")) {
			property->elementArray = stage->wrapper->dataGetter.actorClasses;
		}
		else if (It2->GetCPPType() == "UCurveFloat*") {
			property->elementArray = stage->wrapper->dataGetter.curveFloat;
		}
		else if (It2->GetCPPType() == "UAnimMontage*") {
			property->elementArray = stage->wrapper->dataGetter.animMontage;
		}
		else if (It2->GetCPPType() == "UParticleSystem*") {
			property->elementArray = stage->wrapper->dataGetter.particleSystem;
		}
		else if (It2->GetCPPType() == "ULevelSequence*") {
			property->elementArray = stage->wrapper->dataGetter.sequenceClasses;
		}
		else if (It2->GetCPPType() == "AActor*") {
			property->elementArray = stage->wrapper->dataGetter.actorsLevel;
		}
		else if (It2->GetCPPType() == "UClass*") {
			property->elementArray = stage->wrapper->dataGetter.classes;
		}
		else if (It2->GetCPPType() == "USoundBase*") {
			property->elementArray = stage->wrapper->dataGetter.sounds;
		}
		else if (It2->GetCPPType() == "UMaterialInterface*") {
			property->elementArray = stage->wrapper->dataGetter.materials;
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<USceneComponent>")) {
			property->elementArray = stage->wrapper->dataGetter.componentClasses;
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UActorComponent>")) {
			property->elementArray = stage->wrapper->dataGetter.actorComponentClasses;
		}
		else if (It2->GetCPPType().Contains("TSubclassOf<UUserWidget>")) {
			property->elementArray = stage->wrapper->dataGetter.widgets;
		}
		for (int i = 0; i < property->elementArray.Num(); i++) {
			if (ISM_ISVALID(property->elementArray[i])) {
				property->GetICEContent()->AddOption(property->elementArray[i]->GetName());
			}
		}
		property->elementArray.Insert(nullptr, 0);
		property->SetValue(value);

		property->GetICESearchBar()->selector = property->GetICEContent();
		property->GetICESearchBar()->UpdateValues();

		panel->AddChild(property);
	}
	else if (It2->GetCPPType().StartsWith("E")) {
		UICE_Enum* property = CreateWidget<UICE_Enum>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Enum_BP.ICE_Enum_BP_C'")));
		property->SetName(It2->GetName(), It2->GetMetaData("Tooltip"));
		UEnumProperty* It4 = Cast<UEnumProperty>(It2);
		TArray<FString> enums = UInteractFunctionsClass::enumFunctions;
		for (int i = 0; i < enums.Num(); i++) {
			if (It2->GetCPPType() == enums[i]) {
				const TCHAR* enumNa = reinterpret_cast<const TCHAR*>(*enums[i]);
				int maxE = (int)GETENUMMAX(enumNa);
				for (int j = 0; j < maxE; j++) {
					property->GetICEEnum()->AddOption(GETENUMSTRING(enumNa, j));
				}
				break;
			}
		}
		uint8 actorEnum = *It4->ContainerPtrToValuePtr<uint8>(dir2);
		property->GetICEEnum()->SetSelectedIndex(actorEnum);

		property->GetICESearchBar()->selector = property->GetICEEnum();
		property->GetICESearchBar()->UpdateValues();

		panel->AddChild(property);
	}
	else if (It2->GetCPPType() == "TArray") {
		if (It2->GetName() != "customReturn") {
			UICE_Array* property = CreateWidget<UICE_Array>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Array_BP.ICE_Array_BP_C'")));
			property->GetICEName()->SetText(FText::FromString(It2->GetName()));
			UArrayProperty* It4 = Cast<UArrayProperty>(It2);
			property->parent = dir2;
			property->It = It4;
			property->stage = stage;
			property->basicFunc = this;

			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Create element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Create, 0, property, "preStage", EActionStageType::Pre, -1 };
			property->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete elements"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, 0, property, "preStage", EActionStageType::Pre, -1 };
			property->GetICEButtonOptions()->AddChild(arrDelButton);

			GetDataInternalArray(property->GetICEContent(), dir2, It4->Inner, It4, property);
			panel->AddChild(property);
		}
	}
#endif

}

void UICE_BasicFunction::GetDataInternalArray(UPanelWidget* panel, void * dir2, UProperty* It2, UArrayProperty* arrProp, UICE_Array* arrayParent)
{
	arrayParent->typeArray = It2->GetCPPType();
	if (It2->GetCPPType() == "float") {
		TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_Float* property = CreateWidget<UICE_Float>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Float_BP.ICE_Float_BP_C'")));
			property->SetValue((*ArrayOfValues)[i]);
			property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}


			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "int32") {
		TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_Integer* property = CreateWidget<UICE_Integer>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Integer_BP.ICE_Integer_BP_C'")));
			property->SetValue((*ArrayOfValues)[i]);
			property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}


			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "FString") {
		TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_String* property = CreateWidget<UICE_String>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_String_BP.ICE_String_BP_C'")));
			property->SetValue((*ArrayOfValues)[i]);
			property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}

			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "bool") {
		TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_Boolean* property = CreateWidget<UICE_Boolean>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Boolean_BP.ICE_Boolean_BP_C'")));
			property->SetValue((*ArrayOfValues)[i]);
			property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);
			property->basicFunc = this;
			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}

			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "FName") {
		TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_FName* property = CreateWidget<UICE_FName>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_FName_BP.ICE_FName_BP_C'")));
			property->SetValue((*ArrayOfValues)[i]);
			property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}

			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "FVector") {
		TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_Vector* property = CreateWidget<UICE_Vector>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Vector_BP.ICE_Vector_BP_C'")));
			property->SetValue((*ArrayOfValues)[i]);
			property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}


			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "FRotator") {
		TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_Rotator* property = CreateWidget<UICE_Rotator>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Rotator_BP.ICE_Rotator_BP_C'")));
			property->SetValue((*ArrayOfValues)[i]);
			property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}


			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "FColor") {
	TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
	for (int i = 0; i < ArrayOfValues->Num(); i++) {
		UICE_Color* property = CreateWidget<UICE_Color>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Color_BP.ICE_Color_BP_C'")));
		property->SetValue((*ArrayOfValues)[i]);
		property->GetICENameContainer()->SetVisibility(ESlateVisibility::Collapsed);

		UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
		arrElem->parent = dir2;
		arrElem->It = arrProp;
		arrElem->index = i;
		arrElem->array = arrayParent;
		arrElem->stage = stage;
		arrElem->basicFunc = this;

		arrElem->GetICEContent()->AddChild(property);
		TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
		for (int j = 0; j < arrElemChild.Num(); j++) {
			UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
			if (horizontal) {
				FSlateChildSize siz = { ESlateSizeRule::Fill };
				horizontal->SetSize(siz);
				arrElemChild[j]->Slot = horizontal;
			}
		}


		UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		arrButton->GetButtonName()->SetText(FText::FromString(""));
		arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
		arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
		arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
		arrElem->GetICEButtonOptions()->AddChild(arrButton);

		UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
		arrDelButton->GetButtonName()->SetText(FText::FromString(""));
		arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
		arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
		arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
		arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
		arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

		panel->AddChild(arrElem);
	}
	}
	else if (It2->GetCPPType().StartsWith("U") || It2->GetCPPType().StartsWith("A") || It2->GetCPPType().StartsWith("TSubclassOf")) {
		TArray<UObject*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UObject*>>(dir2);
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
			UICE_Object* property = CreateWidget<UICE_Object>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Object_BP.ICE_Object_BP_C'")));
			UObject* value = (*ArrayOfValues)[i];
			property->GetICEContent()->AddOption("None");
			property->elementArray.Empty();
			//TODO HACER PARA TODAS LAS DEMAS LAS CLASES
			if (It2->GetCPPType() == "UClass*" || It2->GetCPPType().Contains("TSubclassOf<AActor>")) {
				property->elementArray = stage->wrapper->dataGetter.actorClasses;
			}
			else if (It2->GetCPPType() == "UCurveFloat*") {
				property->elementArray = stage->wrapper->dataGetter.curveFloat;
			}
			else if (It2->GetCPPType() == "UAnimMontage*") {
				property->elementArray = stage->wrapper->dataGetter.animMontage;
			}
			else if (It2->GetCPPType() == "UParticleSystem*") {
				property->elementArray = stage->wrapper->dataGetter.particleSystem;
			}
			else if (It2->GetCPPType() == "ULevelSequence*") {
				property->elementArray = stage->wrapper->dataGetter.sequenceClasses;
			}
			else if (It2->GetCPPType() == "AActor*") {
				property->elementArray = stage->wrapper->dataGetter.actorsLevel;
			}
			else if (It2->GetCPPType() == "UClass*") {
				property->elementArray = stage->wrapper->dataGetter.classes;
			}
			else if (It2->GetCPPType() == "USoundBase*") {
				property->elementArray = stage->wrapper->dataGetter.sounds;
			}
			else if (It2->GetCPPType() == "UMaterialInterface*") {
				property->elementArray = stage->wrapper->dataGetter.materials;
			}
			else if (It2->GetCPPType().Contains("TSubclassOf<USceneComponent>")) {
				property->elementArray = stage->wrapper->dataGetter.componentClasses;
			}
			else if (It2->GetCPPType().Contains("TSubclassOf<UActorComponent>")) {
				property->elementArray = stage->wrapper->dataGetter.actorComponentClasses;
			}
			else if (It2->GetCPPType().Contains("TSubclassOf<UUserWidget>")) {
				property->elementArray = stage->wrapper->dataGetter.widgets;
			}
			for (int j = 0; j < property->elementArray.Num(); j++) {
				if (ISM_ISVALID(property->elementArray[i])) {
					property->GetICEContent()->AddOption(property->elementArray[j]->GetName());
				}
			}
			property->elementArray.Insert(nullptr, 0);
			property->SetValue(value);

			property->GetICESearchBar()->selector = property->GetICEContent();
			property->GetICESearchBar()->UpdateValues();

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);

			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "FFunctionActorData" || It2->GetCPPType() == "FFunctionCheckData" || It2->GetCPPType() == "FFunctionCheckDataBase"
		|| It2->GetCPPType() == "FFunctionDataWrapper" || It2->GetCPPType() == "FFunctionDataBase" || It2->GetCPPType() == "FConditionAnd") {
		if (It2->GetCPPType() == "FFunctionActorData") {
			TArray<FFunctionActorData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionActorData>>(dir2);
			for (int i = 0; i < ArrayOfValues->Num(); i++) {
				UICE_ActorFunction* property = CreateWidget<UICE_ActorFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ActorFunction_BP.ICE_ActorFunction_BP_C'")));
				property->functionType = It2->GetCPPType();
				FString enumString = "EGetActorList";
				property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
				property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
				property->wrapper = this;
				property->SetEnum((int)(*ArrayOfValues)[i].enumName);

				property->GetICESearchBar()->selector = property->GetICEEnum();
				property->GetICESearchBar()->actorFun = property;
				property->GetICESearchBar()->UpdateValues();

				UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), property->GetEnum()));
				if (It14) {
					UStructProperty* It36 = Cast<UStructProperty>(It14);
					if (It36) {
						void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
						for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
							GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
						}
					}
				}

				UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
				arrElem->parent = dir2;
				arrElem->It = arrProp;
				arrElem->index = i;
				arrElem->array = arrayParent;
				arrElem->stage = stage;
				arrElem->basicFunc = this;

				arrElem->GetICEContent()->AddChild(property);
				actorArr.Add(arrElem);
				property->arrElem = arrElem;

				TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
				for (int j = 0; j < arrElemChild.Num(); j++) {
					UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
					if (horizontal) {
						FSlateChildSize siz = { ESlateSizeRule::Fill };
						horizontal->SetSize(siz);
						arrElemChild[j]->Slot = horizontal;
					}
				}

				UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrButton->GetButtonName()->SetText(FText::FromString(""));
				arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
				arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
				arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrButton);

				UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrDelButton->GetButtonName()->SetText(FText::FromString(""));
				arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
				arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
				arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

				panel->AddChild(arrElem);
			}
		}
		else if (It2->GetCPPType() == "FFunctionCheckData") {
			TArray<FFunctionCheckData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckData>>(dir2);
			for (int i = 0; i < ArrayOfValues->Num(); i++) {
				UICE_ActorFunction* property = CreateWidget<UICE_ActorFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ActorFunction_BP.ICE_ActorFunction_BP_C'")));
				property->functionType = It2->GetCPPType();
				FString enumString = "ECheckList";
// 				property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
// 				property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
				property->wrapper = this;

				int actorEnum = (int)(*ArrayOfValues)[i].enumName;

				int actorEnumCore = (int)(*ArrayOfValues)[i].enumNameCore;

				int maxE2 = (int)GETENUMMAX(TEXT("ECheckListCore"));
				int otherVal = actorEnum + maxE2 - 1;
				if (actorEnumCore > 0) {
					FString enumStringCore = enumString + "Core";
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore);
					property->SetEnum((int)actorEnumCore);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), actorEnumCore));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else if (actorEnum > 0) {
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
					property->SetEnum((int)otherVal);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else {
					property->SetEnum(0);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), 0));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}

				property->GetICESearchBar()->selector = property->GetICEEnum();
				property->GetICESearchBar()->actorFun = property;
				property->GetICESearchBar()->UpdateValues();

				UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
				arrElem->parent = dir2;
				arrElem->It = arrProp;
				arrElem->index = i;
				arrElem->array = arrayParent;
				arrElem->stage = stage;
				arrElem->basicFunc = this;

				arrElem->GetICEContent()->AddChild(property);
				actorArr.Add(arrElem);
				property->arrElem = arrElem;

				TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
				for (int j = 0; j < arrElemChild.Num(); j++) {
					UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
					if (horizontal) {
						FSlateChildSize siz = { ESlateSizeRule::Fill };
						horizontal->SetSize(siz);
						arrElemChild[j]->Slot = horizontal;
					}
				}

				UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrButton->GetButtonName()->SetText(FText::FromString(""));
				arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
				arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
				arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrButton);

				UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrDelButton->GetButtonName()->SetText(FText::FromString(""));
				arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
				arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
				arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

				panel->AddChild(arrElem);
			}
		}
		else if (It2->GetCPPType() == "FFunctionCheckDataBase") {
			TArray<FFunctionCheckDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckDataBase>>(dir2);
			for (int i = 0; i < ArrayOfValues->Num(); i++) {
				UICE_ActorFunction* property = CreateWidget<UICE_ActorFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ActorFunction_BP.ICE_ActorFunction_BP_C'")));
				property->functionType = It2->GetCPPType();
				FString enumString = "ECheckList";
// 				property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
// 				property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
				property->wrapper = this;
/*				property->SetEnum((int)(*ArrayOfValues)[i].enumName);*/

				int actorEnum = (int)(*ArrayOfValues)[i].enumName;

				int actorEnumCore = (int)(*ArrayOfValues)[i].enumNameCore;

				int maxE2 = (int)GETENUMMAX(TEXT("ECheckListCore"));
				int otherVal = actorEnum + maxE2 - 1;
				if (actorEnumCore > 0) {
					FString enumStringCore = enumString + "Core";
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore);
					property->SetEnum((int)actorEnumCore);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), actorEnumCore));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else if (actorEnum > 0) {
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
					property->SetEnum((int)otherVal);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else {
					property->SetEnum(0);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), 0));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}

				property->GetICESearchBar()->selector = property->GetICEEnum();
				property->GetICESearchBar()->actorFun = property;
				property->GetICESearchBar()->UpdateValues();

				UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
				arrElem->parent = dir2;
				arrElem->It = arrProp;
				arrElem->index = i;
				arrElem->array = arrayParent;
				arrElem->stage = stage;
				arrElem->basicFunc = this;

				arrElem->GetICEContent()->AddChild(property);
				actorArr.Add(arrElem);
				property->arrElem = arrElem;

				TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
				for (int j = 0; j < arrElemChild.Num(); j++) {
					UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
					if (horizontal) {
						FSlateChildSize siz = { ESlateSizeRule::Fill };
						horizontal->SetSize(siz);
						arrElemChild[j]->Slot = horizontal;
					}
				}

				UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrButton->GetButtonName()->SetText(FText::FromString(""));
				arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
				arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
				arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrButton);

				UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrDelButton->GetButtonName()->SetText(FText::FromString(""));
				arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
				arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
				arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

				panel->AddChild(arrElem);
			}
		}
		else if (It2->GetCPPType() == "FFunctionDataWrapper") {
			TArray<FFunctionDataWrapper>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataWrapper>>(dir2);
			for (int i = 0; i < ArrayOfValues->Num(); i++) {
				UICE_ActorFunction* property = CreateWidget<UICE_ActorFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ActorFunction_BP.ICE_ActorFunction_BP_C'")));
				property->functionType = It2->GetCPPType();
				FString enumString = "EActionList";
// 				property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
// 				property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
				property->wrapper = this;
/*				property->SetEnum((int)(*ArrayOfValues)[i].enumName);*/

				int actorEnum = (int)(*ArrayOfValues)[i].enumName;

				int actorEnumCore = (int)(*ArrayOfValues)[i].enumNameCore;

				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int otherVal = actorEnum + maxE2 - 1;
				if (actorEnumCore > 0) {
					FString enumStringCore = enumString + "Core";
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore);
					property->SetEnum((int)actorEnumCore);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), actorEnumCore));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else if (actorEnum > 0) {
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
					property->SetEnum((int)otherVal);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else {
					property->SetEnum(0);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), 0));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}

				property->GetICESearchBar()->selector = property->GetICEEnum();
				property->GetICESearchBar()->actorFun = property;
				property->GetICESearchBar()->UpdateValues();

				UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
				arrElem->parent = dir2;
				arrElem->It = arrProp;
				arrElem->index = i;
				arrElem->array = arrayParent;
				arrElem->stage = stage;
				arrElem->basicFunc = this;

				arrElem->GetICEContent()->AddChild(property);
				actorArr.Add(arrElem);
				property->arrElem = arrElem;

				TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
				for (int j = 0; j < arrElemChild.Num(); j++) {
					UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
					if (horizontal) {
						FSlateChildSize siz = { ESlateSizeRule::Fill };
						horizontal->SetSize(siz);
						arrElemChild[j]->Slot = horizontal;
					}
				}

				UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrButton->GetButtonName()->SetText(FText::FromString(""));
				arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
				arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
				arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrButton);

				UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrDelButton->GetButtonName()->SetText(FText::FromString(""));
				arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
				arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
				arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

				panel->AddChild(arrElem);
			}
		}
		else if (It2->GetCPPType() == "FFunctionDataBase") {
			TArray<FFunctionDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataBase>>(dir2);
			for (int i = 0; i < ArrayOfValues->Num(); i++) {
				UICE_ActorFunction* property = CreateWidget<UICE_ActorFunction>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ActorFunction_BP.ICE_ActorFunction_BP_C'")));
				property->functionType = It2->GetCPPType();
				FString enumString = "EActionList";
// 				property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
// 				property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
				property->wrapper = this;
/*				property->SetEnum((int)(*ArrayOfValues)[i].enumName);*/


				int actorEnum = (int)(*ArrayOfValues)[i].enumName;

				int actorEnumCore = (int)(*ArrayOfValues)[i].enumNameCore;

				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int otherVal = actorEnum + maxE2 - 1;
				if (actorEnumCore > 0) {
					FString enumStringCore = enumString + "Core";
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), (int)(*ArrayOfValues)[i].enumNameCore);
					property->SetEnum((int)actorEnumCore);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), actorEnumCore));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else if (actorEnum > 0) {
					property->GetICEName()->SetText(FText::FromString(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName)));
					property->realName = GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), (int)(*ArrayOfValues)[i].enumName);
					property->SetEnum((int)otherVal);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}
				else {
					property->SetEnum(0);
					UProperty* It14 = FindField<UProperty>((*ArrayOfValues)[i].StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), 0));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&(*ArrayOfValues)[i])) + It36->GetOffset_ForInternal();
							for (TFieldIterator<UProperty> It45(It36->Struct); It45; ++It45) {
								GetDataInternal(property->GetICEFunctionProperties()->GetICEContainer(), dir5, *It45, It36);
							}
						}
					}
				}

				property->GetICESearchBar()->selector = property->GetICEEnum();
				property->GetICESearchBar()->actorFun = property;
				property->GetICESearchBar()->UpdateValues();

				UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
				arrElem->parent = dir2;
				arrElem->It = arrProp;
				arrElem->index = i;
				arrElem->array = arrayParent;
				arrElem->stage = stage;
				arrElem->basicFunc = this;

				arrElem->GetICEContent()->AddChild(property);
				actorArr.Add(arrElem);
				property->arrElem = arrElem;

				TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
				for (int j = 0; j < arrElemChild.Num(); j++) {
					UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
					if (horizontal) {
						FSlateChildSize siz = { ESlateSizeRule::Fill };
						horizontal->SetSize(siz);
						arrElemChild[j]->Slot = horizontal;
					}
				}

				UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrButton->GetButtonName()->SetText(FText::FromString(""));
				arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
				arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
				arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrButton);

				UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
				arrDelButton->GetButtonName()->SetText(FText::FromString(""));
				arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
				arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
				arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
				arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
				arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

				panel->AddChild(arrElem);
			}
		}
	}
	else if (It2->GetCPPType() == "FBaseCreateEditVariable") {
		TArray<FBaseCreateEditVariable>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FBaseCreateEditVariable>>(dir2);
		UStructProperty* It2prop = Cast<UStructProperty>(It2);
		//int i = 0;
		for (int i = 0; i < ArrayOfValues->Num(); i++) {
		//for (TFieldIterator<UStructProperty> It7(It2prop->Struct); It7; ++It7) {
			UICE_Struct* property = CreateWidget<UICE_Struct>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Struct_BP.ICE_Struct_BP_C'")));

			property->GetICEName()->SetText(FText::FromString((*ArrayOfValues)[i].variableName));

			UICE_Boolean* property2 = CreateWidget<UICE_Boolean>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Boolean_BP.ICE_Boolean_BP_C'")));
			property2->SetName("destroy");
			property2->basicFunc = this;
			property2->SetValue((*ArrayOfValues)[i].destroy);
			property->GetICEFunctionProperties()->GetICEContainer()->AddChild(property2);

			UICE_String* property3 = CreateWidget<UICE_String>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_String_BP.ICE_String_BP_C'")));
			property3->SetName("Variable Name");
			property3->SetValue((*ArrayOfValues)[i].variableName);
			property->GetICEFunctionProperties()->GetICEContainer()->AddChild(property3);

			UICE_String* property4 = CreateWidget<UICE_String>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_String_BP.ICE_String_BP_C'")));
			property4->SetName("Variable Value");
			property4->SetValue((*ArrayOfValues)[i].variableValue);
			property->GetICEFunctionProperties()->GetICEContainer()->AddChild(property4);

			UICE_ArrayElement* arrElem = CreateWidget<UICE_ArrayElement>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_ArrayElement_BP.ICE_ArrayElement_BP_C'")));
			arrElem->parent = dir2;
			arrElem->It = arrProp;
			arrElem->index = i;
			arrElem->array = arrayParent;
			arrElem->stage = stage;
			arrElem->basicFunc = this;

			arrElem->GetICEContent()->AddChild(property);
			actorArr.Add(arrElem);

			TArray<UWidget*> arrElemChild = arrElem->GetICEContent()->GetAllChildren();
			for (int j = 0; j < arrElemChild.Num(); j++) {
				UHorizontalBoxSlot* horizontal = Cast<UHorizontalBoxSlot>(arrElemChild[j]->Slot);
				if (horizontal) {
					FSlateChildSize siz = { ESlateSizeRule::Fill };
					horizontal->SetSize(siz);
					arrElemChild[j]->Slot = horizontal;
				}
			}

			UICE_Button* arrButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrButton->GetButtonName()->SetText(FText::FromString(""));
			arrButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Insert element"))));
			arrButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/add.add'"), NULL, LOAD_None, NULL));
			arrButton->m_data = { ETargetInfo::E_Insert, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrButton);

			UICE_Button* arrDelButton = CreateWidget<UICE_Button>(this, StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/ISM/UI/ICE_Button_BP.ICE_Button_BP_C'")));
			arrDelButton->GetButtonName()->SetText(FText::FromString(""));
			arrDelButton->GetButton()->SetToolTipText(FText::FromString(FString::Printf(TEXT("Delete element"))));
			arrDelButton->GetIcon()->SetVisibility(ESlateVisibility::Visible);
			arrDelButton->GetIcon()->SetBrushFromTexture(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/ISM/Icons/bin.bin'"), NULL, LOAD_None, NULL));
			arrDelButton->m_data = { ETargetInfo::E_Delete, i, arrElem, "preStage", EActionStageType::Pre, -1 };
			arrElem->GetICEButtonOptions()->AddChild(arrDelButton);

			panel->AddChild(arrElem);
		}
	}
	else if (It2->GetCPPType() == "TArray") {
		//TODO ARRAY DE ARRAY
	}
}

void UICE_BasicFunction::GetData()
{
	if (GetICEFunctionProperties()->GetICEContainer()->HasAnyChildren()) {
		GetICEFunctionProperties()->GetICEContainer()->ClearChildren();
	}

	if(actorArr.Num()>0) {
		actorArr.Empty();
	}
	
	if (actorArr2.Num() > 0) {
		actorArr2.Empty();
	}


	int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
	int selectIndex = 0;
	FString action = "";
	if (stage->wrapper->interactComponent) {
		if (currentState.state == EStateFunctionList::PreStage) {
			int otherNormal = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex].enumName;
			int otherCore = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex].enumNameCore;
			if (otherNormal > 0) {
				selectIndex = otherNormal + maxE2 - 1;
				action = GETENUMSTRING(TEXT("EActionList"), otherNormal);
			}
			else {
				selectIndex = otherCore;
				action = GETENUMSTRING(TEXT("EActionListCore"), selectIndex);
			}
		}
		else if (currentState.state == EStateFunctionList::Init) {
			int otherNormal = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex].enumName;
			int otherCore = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex].enumNameCore;
			if (otherNormal > 0) {
				selectIndex = otherNormal + maxE2 - 1;
				action = GETENUMSTRING(TEXT("EActionList"), otherNormal);
			}
			else {
				selectIndex = otherCore;
				action = GETENUMSTRING(TEXT("EActionListCore"), selectIndex);
			}
		}
		else if (currentState.state == EStateFunctionList::Exit) {
			int otherNormal = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex].enumName;
			int otherCore = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex].enumNameCore;
			if (otherNormal > 0) {
				selectIndex = otherNormal + maxE2 - 1;
				action = GETENUMSTRING(TEXT("EActionList"), otherNormal);
			}
			else {
				selectIndex = otherCore;
				action = GETENUMSTRING(TEXT("EActionListCore"), selectIndex);
			}
		}
		else if (currentState.state == EStateFunctionList::Action) {
			int otherNormal = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex].enumName;
			int otherCore = (int)stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex].enumNameCore;
			if (otherNormal > 0) {
				selectIndex = otherNormal + maxE2 - 1;
				action = GETENUMSTRING(TEXT("EActionList"), otherNormal);
			}
			else {
				selectIndex = otherCore;
				action = GETENUMSTRING(TEXT("EActionListCore"), selectIndex);
			}
		}


		//EActionList action = (EActionList)GetICEEnum()->GetSelectedIndex();
		void * parent = nullptr;
		if (currentState.state == EStateFunctionList::PreStage) {
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex]);
			SetName(action);
		}
		else if (currentState.state == EStateFunctionList::Init) {
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex]);
			SetName(action);
		}
		else if (currentState.state == EStateFunctionList::Exit) {
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex]);
			SetName(action);
		}
		else if (currentState.state == EStateFunctionList::Action) {
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex]);
			SetName(action);
		}
		UProperty* It = FindField<UProperty>(FFunctionData::StaticStruct(), *action);
		if (It) {
			if (parent) {
				void * dir2 = static_cast<uint8_t*>(parent) + It->GetOffset_ForInternal();
				UStructProperty* ItStruct = Cast<UStructProperty>(It);
				if (ItStruct) {
					for (TFieldIterator<UProperty> It2(ItStruct->Struct); It2; ++It2) {
						GetDataInternal(GetICEFunctionProperties()->GetICEContainer(), dir2, *It2, ItStruct);
					}
				}
			}
		}
	}
}

void UICE_BasicFunction::UpdateDataInternal(UWidget* child, void * dir2, UStruct* structVal)
{

	UICE_Enum* enumPropertyC = Cast<UICE_Enum>(child);
	if (enumPropertyC) {
		if (enumPropertyC->GetName() == "delegateName" || enumPropertyC->GetName() == "specificFunction" || enumPropertyC->GetName() == "specificTimer") {
			FString coreClassName = "";
			if (enumPropertyC->GetName() == "delegateName") {
				coreClassName = "EDelegateListCore";
			}
			else if (enumPropertyC->GetName() == "specificFunction") {
				coreClassName = "EActionListCore";
			}
			else if (enumPropertyC->GetName() == "specificTimer") {
				coreClassName = "ETimerListCore";
			}
			int maxECore = (int)GETENUMMAX(*coreClassName);
			UProperty* It2 = FindField<UProperty>(structVal, *enumPropertyC->GetName());
			if (It2) {
				UEnumProperty* intIt = Cast<UEnumProperty>(It2);
				if (intIt) {
					uint8* actorEnumDir = intIt->ContainerPtrToValuePtr<uint8>(dir2);
					int selectIndex = enumPropertyC->GetICEEnum()->GetSelectedIndex();
					if (selectIndex >= maxECore) {
						*actorEnumDir = selectIndex - maxECore + 1;
					}
					else {
						*actorEnumDir = 0;
					}
				}
			}
			FString enumPropertyCore = enumPropertyC->GetName() + "Core";
			UProperty* It2Core = FindField<UProperty>(structVal, *enumPropertyCore);
			if (It2Core) {
				UEnumProperty* intIt3 = Cast<UEnumProperty>(It2Core);
				if (intIt3) {
					uint8* actorEnumDir3 = intIt3->ContainerPtrToValuePtr<uint8>(dir2);
					int selectIndex3 = enumPropertyC->GetICEEnum()->GetSelectedIndex();
					if (selectIndex3 < maxECore) {
						*actorEnumDir3 = selectIndex3;
					}
					else {
						*actorEnumDir3 = 0;
					}
				}
			}
			return;
		}
	}


	UICE_Float* floatProperty = Cast<UICE_Float>(child);
	if (floatProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *floatProperty->GetName());
		if (It2) {
			UFloatProperty* floatIt = Cast<UFloatProperty>(It2);
			floatIt->SetPropertyValue_InContainer(dir2, floatProperty->GetValue(), 0);
			return;
		}
	}
	UICE_Integer* intProperty = Cast<UICE_Integer>(child);
	if (intProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *intProperty->GetName());
		if (It2) {
			UIntProperty* intIt = Cast<UIntProperty>(It2);
			intIt->SetPropertyValue_InContainer(dir2, intProperty->GetValue(), 0);
			return;
		}
	}
	UICE_String* strProperty = Cast<UICE_String>(child);
	if (strProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *strProperty->GetName());
		if (It2) {
			UStrProperty* intIt = Cast<UStrProperty>(It2);
			intIt->SetPropertyValue_InContainer(dir2, strProperty->GetValue(), 0);
			return;
		}
	}
	UICE_Boolean* boolProperty = Cast<UICE_Boolean>(child);
	if (boolProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *boolProperty->GetName());
		if (It2) {
			UBoolProperty* intIt = Cast<UBoolProperty>(It2);
			intIt->SetPropertyValue_InContainer(dir2, boolProperty->GetValue(), 0);
			return;
		}
	}
	UICE_FName* nameProperty = Cast<UICE_FName>(child);
	if (nameProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *nameProperty->GetName());
		if (It2) {
			UNameProperty* intIt = Cast<UNameProperty>(It2);
			intIt->SetPropertyValue_InContainer(dir2, nameProperty->GetValue(), 0);
			return;
		}
	}
	UICE_Vector* vectorProperty = Cast<UICE_Vector>(child);
	if (vectorProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *vectorProperty->GetName());
		if (It2) {
			UStructProperty* intIt = Cast<UStructProperty>(It2);
			FVector vectorVal = vectorProperty->GetValue();
			void * dir3 = static_cast<uint8_t*>(dir2) + intIt->GetOffset_ForInternal();
			intIt->CopyValuesInternal(dir3, reinterpret_cast<void*>(&vectorVal), 1);
			return;
		}
	}
	UICE_Rotator* rotatorProperty = Cast<UICE_Rotator>(child);
	if (rotatorProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *rotatorProperty->GetName());
		if (It2) {
			UStructProperty* intIt = Cast<UStructProperty>(It2);
			FRotator vectorVal = rotatorProperty->GetValue();
			void * dir3 = static_cast<uint8_t*>(dir2) + intIt->GetOffset_ForInternal();
			intIt->CopyValuesInternal(dir3, reinterpret_cast<void*>(&vectorVal), 1);
			return;
		}
	}
	UICE_Color* colorProperty = Cast<UICE_Color>(child);
	if (colorProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *colorProperty->GetName());
		if (It2) {
			UStructProperty* intIt = Cast<UStructProperty>(It2);
			FColor vectorVal = colorProperty->GetValue();
			void * dir3 = static_cast<uint8_t*>(dir2) + intIt->GetOffset_ForInternal();
			intIt->CopyValuesInternal(dir3, reinterpret_cast<void*>(&vectorVal), 1);
			return;
		}
	}
	UICE_ActorFunction* actorFunctionProperty = Cast<UICE_ActorFunction>(child);
	if (actorFunctionProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *actorFunctionProperty->realName);
		if (It2) {
			UStructProperty* intIt = Cast<UStructProperty>(It2);
			void * dir3 = static_cast<uint8_t*>(dir2) + intIt->GetOffset_ForInternal();
			TArray<UWidget*> child2 = actorFunctionProperty->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();

			FString enumString = "";
			if (actorFunctionProperty->functionType == "FFunctionActorData") {
				enumString = "EGetActorList";
			}
			else if (actorFunctionProperty->functionType == "FFunctionCheckData") {
				enumString = "ECheckList";
			}
			else if (actorFunctionProperty->functionType == "FFunctionCheckDataBase") {
				enumString = "ECheckList";
			}
			else if (actorFunctionProperty->functionType == "FFunctionDataWrapper") {
				enumString = "EActionList";
			}
			else if (actorFunctionProperty->functionType == "FFunctionDataBase") {
				enumString = "EActionList";
			}

			if (enumString == "EGetActorList") {
				int actorEnum = 0;

				UProperty* It3C = FindField<UProperty>(intIt->Struct, "enumName");
				if (It3C) {
					UEnumProperty* It4 = Cast<UEnumProperty>(It3C);
					if (It4) {
						uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
						actorEnum = (int)(*actorEnumDir);
					}
				}

				UProperty* It5 = FindField<UProperty>(intIt->Struct, *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
				if (It5) {
					UStructProperty* It6 = Cast<UStructProperty>(It5);
					if (It6) {
						void * dir4 = static_cast<uint8_t*>(dir3) + It6->GetOffset_ForInternal();
						for (int i = 0; i < child2.Num(); i++) {
							UpdateDataInternal(child2[i], dir4, It6->Struct);
						}
					}
				}
				actorEnum = actorFunctionProperty->GetEnum();
				UProperty* It3 = FindField<UProperty>(intIt->Struct, "enumName");
				if (It3) {
					UEnumProperty* It4 = Cast<UEnumProperty>(It3);
					if (It4) {
						uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
						*actorEnumDir = actorEnum;
					}
				}


			}
			else {
				FString enumStringCore = enumString + "Core";
				int maxE2 = (int)GETENUMMAX(reinterpret_cast<const TCHAR*>(*enumStringCore));

				int actorEnum = 0;

				UProperty* It3C = FindField<UProperty>(intIt->Struct, "enumName");
				if (It3C) {
					UEnumProperty* It4 = Cast<UEnumProperty>(It3C);
					if (It4) {
						uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
						actorEnum = (int)(*actorEnumDir);
					}
				}

				int actorEnumCore = 0;

				UProperty* It3C2 = FindField<UProperty>(intIt->Struct, "enumNameCore");
				if (It3C2) {
					UEnumProperty* It4 = Cast<UEnumProperty>(It3C2);
					if (It4) {
						uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
						actorEnumCore = (int)(*actorEnumDir);
					}
				}

				if (actorEnumCore > 0) {

					UProperty* It5 = FindField<UProperty>(intIt->Struct, *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), actorEnumCore));
					if (It5) {
						UStructProperty* It6 = Cast<UStructProperty>(It5);
						if (It6) {
							void * dir4 = static_cast<uint8_t*>(dir3) + It6->GetOffset_ForInternal();
							for (int i = 0; i < child2.Num(); i++) {
								UpdateDataInternal(child2[i], dir4, It6->Struct);
							}
						}
					}
				}
				else {
					UProperty* It5 = FindField<UProperty>(intIt->Struct, *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It5) {
						UStructProperty* It6 = Cast<UStructProperty>(It5);
						if (It6) {
							void * dir4 = static_cast<uint8_t*>(dir3) + It6->GetOffset_ForInternal();
							for (int i = 0; i < child2.Num(); i++) {
								UpdateDataInternal(child2[i], dir4, It6->Struct);
							}
						}
					}
				}
				actorEnum = actorFunctionProperty->GetEnum();
				int otherVal = actorEnum - maxE2 + 1;
				if (actorEnum < maxE2) {
					UProperty* It3Core = FindField<UProperty>(intIt->Struct, "enumNameCore");
					if (It3Core) {
						UEnumProperty* It4 = Cast<UEnumProperty>(It3Core);
						if (It4) {
							uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
							*actorEnumDir = actorEnum;
						}
					}
					UProperty* It3 = FindField<UProperty>(intIt->Struct, "enumName");
					if (It3) {
						UEnumProperty* It4 = Cast<UEnumProperty>(It3);
						if (It4) {
							uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
							*actorEnumDir = 0;
						}
					}
				}
				else {
					UProperty* It3Core = FindField<UProperty>(intIt->Struct, "enumNameCore");
					if (It3Core) {
						UEnumProperty* It4 = Cast<UEnumProperty>(It3Core);
						if (It4) {
							uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
							*actorEnumDir = 0;
						}
					}
					UProperty* It3 = FindField<UProperty>(intIt->Struct, "enumName");
					if (It3) {
						UEnumProperty* It4 = Cast<UEnumProperty>(It3);
						if (It4) {
							uint8* actorEnumDir = It4->ContainerPtrToValuePtr<uint8>(dir3);
							*actorEnumDir = otherVal;
						}
					}
				}
			}
			return;
		}
	}
	UICE_Struct* structProperty = Cast<UICE_Struct>(child);
	if (structProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *structProperty->GetICEName()->GetText().ToString());
		if (It2) {
			UStructProperty* intIt = Cast<UStructProperty>(It2);
			void * dir3 = static_cast<uint8_t*>(dir2) + intIt->GetOffset_ForInternal();
			TArray<UWidget*> child2 = structProperty->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
			for (int i = 0; i < child2.Num(); i++) {
				UpdateDataInternal(child2[i], dir3, intIt->Struct);
			}
			return;
		}
	}
	UICE_Object* objectProperty = Cast<UICE_Object>(child);
	if (objectProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *objectProperty->GetName());
		if (It2) {
			UObjectProperty* floatIt = Cast<UObjectProperty>(It2);
			floatIt->SetPropertyValue_InContainer(dir2, objectProperty->GetValue(), 0);
			return;
		}
	}
	UICE_Enum* enumProperty = Cast<UICE_Enum>(child);
	if (enumProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *enumProperty->GetName());
		if (It2) {
			UEnumProperty* intIt = Cast<UEnumProperty>(It2);
			if (intIt) {
				uint8* actorEnumDir = intIt->ContainerPtrToValuePtr<uint8>(dir2);
				*actorEnumDir = enumProperty->GetICEEnum()->GetSelectedIndex();
			}
			return;
		}
	}
	UICE_Array* arrayProperty = Cast<UICE_Array>(child);
	if (arrayProperty) {
		UProperty* It2 = FindField<UProperty>(structVal, *arrayProperty->GetICEName()->GetText().ToString());
		if (It2) {
			UArrayProperty* intIt = Cast<UArrayProperty>(It2);
			if (intIt) {
				UpdateDataInternalArray(arrayProperty->GetICEContent(), dir2, structVal, intIt->Inner, intIt);
			}
			return;
		}
	}
}

void UICE_BasicFunction::UpdateDataInternalArray(UPanelWidget* child, void * dir2, UStruct* structVal, UProperty* It2, UArrayProperty* arrProp)
{
	if (It2->GetCPPType() == "float") {
		TArray<float>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<float>>(dir2);
		TArray<float> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_Float* property = Cast<UICE_Float>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "int32") {
		TArray<int>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<int>>(dir2);
		TArray<int> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_Integer* property = Cast<UICE_Integer>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "FString") {
		TArray<FString>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FString>>(dir2);
		TArray<FString> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_String* property = Cast<UICE_String>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "bool") {
		TArray<bool>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<bool>>(dir2);
		TArray<bool> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_Boolean* property = Cast<UICE_Boolean>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "FName") {
		TArray<FName>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FName>>(dir2);
		TArray<FName> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_FName* property = Cast<UICE_FName>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "FVector") {
		TArray<FVector>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FVector>>(dir2);
		TArray<FVector> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_Vector* property = Cast<UICE_Vector>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "FRotator") {
		TArray<FRotator>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FRotator>>(dir2);
		TArray<FRotator> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_Rotator* property = Cast<UICE_Rotator>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "FColor") {
		TArray<FColor>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FColor>>(dir2);
		TArray<FColor> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_Color* property = Cast<UICE_Color>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType().StartsWith("U") || It2->GetCPPType().StartsWith("A") || It2->GetCPPType().StartsWith("TSubclassOf")) {
		TArray<UObject*>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<UObject*>>(dir2);
		TArray<UObject*> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				UICE_Object* property = Cast<UICE_Object>(arrElem->GetICEContent()->GetAllChildren()[j]);
				NewValues.Add(property->GetValue());
				break;
			}
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "FFunctionActorData" || It2->GetCPPType() == "FFunctionCheckData" || It2->GetCPPType() == "FFunctionCheckDataBase"
	|| It2->GetCPPType() == "FFunctionDataWrapper" || It2->GetCPPType() == "FFunctionDataBase" || It2->GetCPPType() == "FConditionAnd") {
		if (It2->GetCPPType() == "FFunctionActorData") {
			TArray<FFunctionActorData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionActorData>>(dir2);
			TArray<FFunctionActorData> NewValues;
			for (int i = 0; i < child->GetAllChildren().Num(); i++) {
				UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
				UICE_ActorFunction* property = nullptr;
				for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
					property = Cast<UICE_ActorFunction>(arrElem->GetICEContent()->GetAllChildren()[j]);
					break;
				}

				FFunctionActorData funcStruct = InitFunctionActorData();
				funcStruct.enumName = (EGetActorList)property->GetEnum();
				FString enumString = "EGetActorList";
				TArray<UWidget*> child2 = property->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
				for (int x = 0; x < child2.Num(); x++) {
					UProperty* It14 = FindField<UProperty>(funcStruct.StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), property->GetEnum()));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&funcStruct)) + It36->GetOffset_ForInternal();
							UpdateDataInternal(child2[x], dir5, It36->Struct);
						}
					}
				}
				NewValues.Add(funcStruct);
			}
			ArrayOfValues->Empty();
			ArrayOfValues->Append(NewValues);
		}
		else if (It2->GetCPPType() == "FFunctionCheckData") {
			TArray<FFunctionCheckData>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckData>>(dir2);
			TArray<FFunctionCheckData> NewValues;
			for (int i = 0; i < child->GetAllChildren().Num(); i++) {
				UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
				UICE_ActorFunction* property = nullptr;
				for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
					property = Cast<UICE_ActorFunction>(arrElem->GetICEContent()->GetAllChildren()[j]);
					break;
				}

				FFunctionCheckData funcStruct = InitFunctionCheckData();

				FString enumString = "";
				int maxE2 = (int)GETENUMMAX(TEXT("ECheckListCore"));
				int actorEnum = property->GetEnum();

				if (actorEnum < maxE2) {
					enumString = "ECheckListCore";
					funcStruct.enumName = ECheckList::None;
					funcStruct.enumNameCore = (ECheckListCore)actorEnum;
				}
				else {
					actorEnum = actorEnum - maxE2 + 1;
					enumString = "ECheckList";
					funcStruct.enumName = (ECheckList)actorEnum;
					funcStruct.enumNameCore = ECheckListCore::None;
				}

				TArray<UWidget*> child2 = property->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
				for (int x = 0; x < child2.Num(); x++) {
					UProperty* It14 = FindField<UProperty>(funcStruct.StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&funcStruct)) + It36->GetOffset_ForInternal();
							UpdateDataInternal(child2[x], dir5, It36->Struct);
						}
					}
				}
				NewValues.Add(funcStruct);
			}
			ArrayOfValues->Empty();
			ArrayOfValues->Append(NewValues);
		}
		else if (It2->GetCPPType() == "FFunctionCheckDataBase") {
			TArray<FFunctionCheckDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionCheckDataBase>>(dir2);
			TArray<FFunctionCheckDataBase> NewValues;
			for (int i = 0; i < child->GetAllChildren().Num(); i++) {
				UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
				UICE_ActorFunction* property = nullptr;
				for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
					property = Cast<UICE_ActorFunction>(arrElem->GetICEContent()->GetAllChildren()[j]);
					break;
				}

				/*FString enumString = "";*/
				FFunctionCheckDataBase funcStruct = InitFunctionCheckData();
// 				funcStruct.enumName = (ECheckList)property->GetEnum();
// 				FString enumString = "ECheckList";
				FString enumString = "";
				int maxE2 = (int)GETENUMMAX(TEXT("ECheckListCore"));
				int actorEnum = property->GetEnum();

				if (actorEnum < maxE2) {
					enumString = "ECheckListCore";
					funcStruct.enumName = ECheckList::None;
					funcStruct.enumNameCore = (ECheckListCore)actorEnum;
				}
				else {
					actorEnum = actorEnum - maxE2 + 1;
					enumString = "ECheckList";
					funcStruct.enumName = (ECheckList)actorEnum;
					funcStruct.enumNameCore = ECheckListCore::None;
				}


				TArray<UWidget*> child2 = property->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
				for (int x = 0; x < child2.Num(); x++) {
					UProperty* It14 = FindField<UProperty>(funcStruct.StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&funcStruct)) + It36->GetOffset_ForInternal();
							UpdateDataInternal(child2[x], dir5, It36->Struct);
						}
					}
				}
				NewValues.Add(funcStruct);
			}
			ArrayOfValues->Empty();
			ArrayOfValues->Append(NewValues);
		}
		else if (It2->GetCPPType() == "FFunctionDataWrapper") {
			TArray<FFunctionDataWrapper>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataWrapper>>(dir2);
			TArray<FFunctionDataWrapper> NewValues;
			for (int i = 0; i < child->GetAllChildren().Num(); i++) {
				UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
				UICE_ActorFunction* property = nullptr;
				for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
					property = Cast<UICE_ActorFunction>(arrElem->GetICEContent()->GetAllChildren()[j]);
					break;
				}

				FFunctionDataWrapper funcStruct = InitFunctionDataWrapper();
				FString enumString = "";
				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int actorEnum = property->GetEnum();

				if (actorEnum < maxE2) {
					enumString = "EActionListCore";
					funcStruct.enumName = EActionList::None;
					funcStruct.enumNameCore = (EActionListCore)actorEnum;
				}
				else {
					actorEnum = actorEnum - maxE2 + 1;
					enumString = "EActionList";
					funcStruct.enumName = (EActionList)actorEnum;
					funcStruct.enumNameCore = EActionListCore::None;
				}


				TArray<UWidget*> child2 = property->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
				for (int x = 0; x < child2.Num(); x++) {
					UProperty* It14 = FindField<UProperty>(funcStruct.StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&funcStruct)) + It36->GetOffset_ForInternal();
								UpdateDataInternal(child2[x], dir5, It36->Struct);
						}
					}
				}
				NewValues.Add(funcStruct);
			}
			ArrayOfValues->Empty();
			ArrayOfValues->Append(NewValues);
		}
		else if (It2->GetCPPType() == "FFunctionDataBase") {
			TArray<FFunctionDataBase>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FFunctionDataBase>>(dir2);
			TArray<FFunctionDataBase> NewValues;
			for (int i = 0; i < child->GetAllChildren().Num(); i++) {
				UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
				UICE_ActorFunction* property = nullptr;
				for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
					property = Cast<UICE_ActorFunction>(arrElem->GetICEContent()->GetAllChildren()[j]);
					break;
				}

				/*FString enumString = "";*/
				FFunctionDataBase funcStructBase = InitFunctionDataBase();
// 				funcStructBase.enumName = (EActionList)property->GetEnum();
// 				FString enumString = "EActionList";
				FString enumString = "";
				int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
				int actorEnum = property->GetEnum();

				if (actorEnum < maxE2) {
					enumString = "EActionListCore";
					funcStructBase.enumName = EActionList::None;
					funcStructBase.enumNameCore = (EActionListCore)actorEnum;
				}
				else {
					actorEnum = actorEnum - maxE2 + 1;
					enumString = "EActionList";
					funcStructBase.enumName = (EActionList)actorEnum;
					funcStructBase.enumNameCore = EActionListCore::None;
				}

				TArray<UWidget*> child2 = property->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
				for (int x = 0; x < child2.Num(); x++) {
					UProperty* It14 = FindField<UProperty>(funcStructBase.StaticStruct(), *GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), actorEnum));
					if (It14) {
						UStructProperty* It36 = Cast<UStructProperty>(It14);
						if (It36) {
							void * dir5 = static_cast<uint8_t*>(reinterpret_cast<void*>(&funcStructBase)) + It36->GetOffset_ForInternal();
							UpdateDataInternal(child2[x], dir5, It36->Struct);
						}
					}
				}
				NewValues.Add(funcStructBase);
			}
			ArrayOfValues->Empty();
			ArrayOfValues->Append(NewValues);
		}
	}
	else if (It2->GetCPPType() == "FBaseCreateEditVariable") {
		TArray<FBaseCreateEditVariable>* ArrayOfValues = arrProp->ContainerPtrToValuePtr<TArray<FBaseCreateEditVariable>>(dir2);
		TArray<FBaseCreateEditVariable> NewValues;
		for (int i = 0; i < child->GetAllChildren().Num(); i++) {
			UICE_ArrayElement* arrElem = Cast<UICE_ArrayElement>(child->GetAllChildren()[i]);
			UICE_Struct* property = nullptr;
			for (int j = 0; j < arrElem->GetICEContent()->GetAllChildren().Num(); j++) {
				property = Cast<UICE_Struct>(arrElem->GetICEContent()->GetAllChildren()[j]);
				break;
			}
			UProperty* It5 = nullptr;
			FBaseCreateEditVariable funcStruct = InitBaseCreateEditVariable();

			TArray<UWidget*> child2 = property->GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
			for (int j = 0; j < child2.Num(); j++) {
				UICE_String* strProperty = Cast<UICE_String>(child2[j]);
				if (strProperty) {
					if (strProperty->GetICENameContainer()->GetText().ToString() == "Variable Name") {
						funcStruct.variableName = strProperty->GetValue();
					}
					else if (strProperty->GetICENameContainer()->GetText().ToString() == "Variable Value") {
						funcStruct.variableValue = strProperty->GetValue();
					}
				}
				UICE_Boolean* boolProperty = Cast<UICE_Boolean>(child2[j]);
				if (boolProperty) {
					if (boolProperty->GetICENameContainer()->GetText().ToString() == "Destroy") {
						funcStruct.destroy = boolProperty->GetValue();
					}
				}
			}
			NewValues.Add(funcStruct);
		}
		ArrayOfValues->Empty();
		ArrayOfValues->Append(NewValues);
	}
	else if (It2->GetCPPType() == "TArray") {
		//TODO ARRAY DE ARRAY
	}
}

void UICE_BasicFunction::UpdateData()
{
	FString enumString = "";
	int maxE2 = (int)GETENUMMAX(TEXT("EActionListCore"));
	int selectedIndex = GetICEEnum()->GetSelectedIndex();

	int otherVal = selectedIndex - maxE2 + 1;
	if (selectedIndex < maxE2) {
		enumString = *GETENUMSTRING(TEXT("EActionListCore"), selectedIndex);
	}
	else {
		enumString = *GETENUMSTRING(TEXT("EActionList"), otherVal);
	}

	//EActionList action = (EActionList)GetICEEnum()->GetSelectedIndex();
	void * parent = nullptr;
	if (stage->wrapper->interactComponent) {
		if (currentState.state == EStateFunctionList::PreStage) {
			if (selectedIndex < maxE2) {
				stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex].enumNameCore = (EActionListCore)selectedIndex;
				stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex].enumName = (EActionList)0;
			}
			else {
				stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex].enumNameCore = (EActionListCore)0;
				stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex].enumName = (EActionList)otherVal;
			}
			SetName(enumString);
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].preStageFunction[currentIndex]);

		}
		else if (currentState.state == EStateFunctionList::Init) {
			if (selectedIndex < maxE2) {
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex].enumNameCore = (EActionListCore)selectedIndex;
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex].enumName = (EActionList)0;
			}
			else {
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex].enumNameCore = (EActionListCore)0;
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex].enumName = (EActionList)otherVal;
			}
			SetName(enumString);
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.initFunction[currentIndex]);
		}
		else if (currentState.state == EStateFunctionList::Exit) {
			if (selectedIndex < maxE2) {
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex].enumNameCore = (EActionListCore)selectedIndex;
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex].enumName = (EActionList)0;
			}
			else {
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex].enumNameCore = (EActionListCore)0;
				stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex].enumName = (EActionList)otherVal;
			}
			SetName(enumString);
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].capsuleTriggerStruct.exitFunction[currentIndex]);
		}
		else if (currentState.state == EStateFunctionList::Action) {
			if (selectedIndex < maxE2) {
				stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex].enumNameCore = (EActionListCore)selectedIndex;
				stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex].enumName = (EActionList)0;
			}
			else {
				stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex].enumNameCore = (EActionListCore)0;
				stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex].enumName = (EActionList)otherVal;
			}
			SetName(enumString);
			parent = reinterpret_cast<void*>(&stage->wrapper->interactComponent->interactDataArray[currentStage].actions[currentState.actionIndex].stageFunction[currentIndex]);
		}
		UProperty* It = FindField<UProperty>(FFunctionData::StaticStruct(), *enumString);
		if (It) {
			if (parent != nullptr) {
				UStructProperty* ItStruct = Cast<UStructProperty>(It);
				if (ItStruct) {
					void * dir2 = static_cast<uint8_t*>(parent) + ItStruct->GetOffset_ForInternal();
					TArray<UWidget*> child = GetICEFunctionProperties()->GetICEContainer()->GetAllChildren();
					for (int i = 0; i < child.Num(); i++) {
						UpdateDataInternal(child[i], dir2, ItStruct->Struct);
					}
				}
			}
		}
		stage->wrapper->interactComponent->UpdateEverything();
	}
}
