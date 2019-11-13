// Fill out your copyright notice in the Description page of Project Settings.


#include "ICE_TriggerCapsule.h"

void UICE_TriggerCapsule::GetData()
{
	SetSize(stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleSize);
	SetLocation(stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleLocation);
	SetRotation(stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleRotation);
	SetScale(stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleScale);
	for (int i = 0; i < initFunc->basicFunctions.Num(); i++) {
		initFunc->basicFunctions[i]->GetData();
	}
	for (int j = 0; j < exitFunc->basicFunctions.Num(); j++) {
		exitFunc->basicFunctions[j]->GetData();
	}

// 	if (actorClasses.Num() == 0) {
// 		for (TObjectIterator<UClass> It; It; ++It)
// 		{
// 			if (It->IsChildOf(ACh::StaticClass()) || It->IsChildOf(AItem::StaticClass()) || It->IsChildOf(AActor::StaticClass())
// 				|| It->IsChildOf(APawn::StaticClass()) || It->IsChildOf(ACharacter::StaticClass()))
// 			{
// 				actorClasses.Add(*It);
// 			}
// 		}
// 	}

	FString nam = "Actor Class";
	GetActorClass()->GetICEName()->SetText(FText::FromString(nam));
	UObject* value = stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.actorClass;
	GetActorClass()->GetICEContent()->AddOption("None");
	GetActorClass()->elementArray = stage->wrapper->dataGetter.actorClasses;
	for (int i = 0; i < GetActorClass()->elementArray.Num(); i++) {
		GetActorClass()->GetICEContent()->AddOption(GetActorClass()->elementArray[i]->GetName());
	}
	GetActorClass()->elementArray.Insert(nullptr, 0);
	GetActorClass()->SetValue(value);

	GetActorClass()->GetICESearchBar()->selector = GetActorClass()->GetICEContent();
	GetActorClass()->GetICESearchBar()->UpdateValues();

	FString nameTag = "Actor Tag";
	GetActorTag()->GetICENameContainer()->SetText(FText::FromString(nameTag));
	GetActorTag()->SetValue(stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.tag);
}

void UICE_TriggerCapsule::UpdateData()
{
	if (stage->wrapper->interactComponent->interactDataArray.IsValidIndex(stage->stage)) {
		stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleSize = GetSize();
		stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleLocation = GetLocation();
		stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleRotation = GetRotation();
		stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.m_CapsuleScale = GetScale();
		for (int i = 0; i < initFunc->basicFunctions.Num(); i++) {
			if (stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.initFunction.IsValidIndex(i)) {
				initFunc->basicFunctions[i]->UpdateData();
			}
		}
		for (int j = 0; j < exitFunc->basicFunctions.Num(); j++) {
			if (stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.exitFunction.IsValidIndex(j)) {
				exitFunc->basicFunctions[j]->UpdateData();
			}
		}

		stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.actorClass = Cast<UClass>(GetActorClass()->GetValue());
		stage->wrapper->interactComponent->interactDataArray[stage->stage].capsuleTriggerStruct.tag = GetActorTag()->GetValue();

		stage->wrapper->interactComponent->UpdateInteractArrayData();
	}
}
