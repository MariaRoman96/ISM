// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_ActorFunction.h"
#include "ICE_BasicFunction.h"
#include "ICE_ArrayElement.h"

void UICE_ActorFunction::NativeConstruct()
{
// 	if (GetICEEnum()->GetOptionCount() == 0) {
// 		int maxE = (int)GETENUMMAX(TEXT("EGetActorList"));
// 		for (int i = 0; i < maxE; i++) {
// 			GetICEEnum()->AddOption(GETENUMSTRING(TEXT("EGetActorList"), (EGetActorList)i));
// 		}
// 		GetICEEnum()->SetSelectedOption(GETENUMSTRING(TEXT("EGetActorList"), (EGetActorList)0));
// 	}
	GetICEEnum()->OnSelectionChanged.AddDynamic(this, &UICE_ActorFunction::OnChangeSelect);
}

void UICE_ActorFunction::OnChangeSelect(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::OnMouseClick) {
		GetICEOtherProperties()->ClearChildren();
		wrapper->UpdateData();
		wrapper->GetData();
		wrapper->GetICEExpandable()->SetIsExpanded(true);
		for (int i = 0; i < wrapper->actorArr.Num(); i++) {
			if (wrapper->actorArr[i]) {
				UPanelWidget* userWid = Cast<UPanelWidget>(wrapper->actorArr[i]->GetICEContent());
				UICE_ActorFunction* actorFunctProv = nullptr;
				if (userWid) {
					for (int j = 0; j < userWid->GetAllChildren().Num(); j++) {
						actorFunctProv = Cast<UICE_ActorFunction>(userWid->GetAllChildren()[j]);
						break;
					}
				}
				if (actorFunctProv) {
					if (GetEnumString() == actorFunctProv->GetEnumString()) {
						UICE_Array* arr = actorFunctProv->arrElem->array;
						if (arr) {
							arr->GetICEExpandable()->SetIsExpanded(true);
						}
						actorFunctProv->GetICEExpandable()->SetIsExpanded(true);
					}
				}
			}
		}
		for (int i = 0; i < wrapper->actorArr2.Num(); i++) {
			if (wrapper->actorArr2[i]) {
				if (wrapper->actorArr2[i]->realName == realName) {
					wrapper->actorArr2[i]->GetICEExpandable()->SetIsExpanded(true);
				}
			}
		}
	}
}

FString UICE_ActorFunction::GetEnumString() {
	return GetICEEnum()->GetSelectedOption();
}

int UICE_ActorFunction::GetEnum() {
	return GetICEEnum()->GetSelectedIndex();
}

void UICE_ActorFunction::SetEnum(int val) {
	FString enumString = "";
	if (functionType == "FFunctionActorData") {
		enumString = "EGetActorList";
	}
	else if (functionType == "FFunctionCheckData") {
		enumString = "ECheckList";
	}
	else if (functionType == "FFunctionCheckDataBase") {
		enumString = "ECheckList";
	}
	else if (functionType == "FFunctionDataWrapper") {
		enumString = "EActionList";
	}
	else if (functionType == "FFunctionDataBase") {
		enumString = "EActionList";
	}
	else if (functionType == "FConditionAnd") {
		enumString = "ECheckList";
	}
	if (enumString != "") {
		if (enumString == "EGetActorList") {
			if (GetICEEnum()->GetOptionCount() == 0) {
				int maxE = (int)GETENUMMAX(reinterpret_cast<const TCHAR*>(*enumString));
				for (int i = 0; i < maxE; i++) {
					GetICEEnum()->AddOption(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), i));
				}
			}
			GetICEEnum()->SetSelectedOption(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), val));
		}
		else {
			FString enumStringCore = enumString + "Core";
			int maxE2 = (int)GETENUMMAX(reinterpret_cast<const TCHAR*>(*enumStringCore));
			int maxE = (int)GETENUMMAX(reinterpret_cast<const TCHAR*>(*enumString));
			if (GetICEEnum()->GetOptionCount() == 0) {
				for (int i = 0; i < maxE2; i++) {
					GetICEEnum()->AddOption(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), i));
				}
				for (int j = 1; j< maxE; j++) {
					GetICEEnum()->AddOption(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), j));
				}
			}
			int otherVal = val - maxE2 + 1;
			if (val < maxE2) {
				GetICEEnum()->SetSelectedOption(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumStringCore), val));
			}
			else  if (otherVal < maxE) {
				GetICEEnum()->SetSelectedOption(GETENUMSTRING(reinterpret_cast<const TCHAR*>(*enumString), otherVal));
			}

		}
	}
}