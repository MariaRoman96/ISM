// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractComponent.h"
#include <Engine/Texture2D.h>
#include <Components/MeshComponent.h>
#include <ScriptInterface.h>
#include <Components/InputComponent.h>
#include <GameFramework/Actor.h>
#include "InteractInterface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UPlayerInterface : public UInteractInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISM_API IPlayerInterface : public IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	inline virtual void ExecuteNearTriggers() {

		AActor* MyActor = Cast<AActor>(this);
		TArray<UPrimitiveComponent*> InteractiveItems;
		MyActor->GetOverlappingComponents(InteractiveItems);

		for (UPrimitiveComponent* itemC : InteractiveItems)
		{
			if (itemC->GetOwner()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) && itemC->GetOwner()->GetName() != MyActor->GetName())
			{
				IInteractInterface* itemClass = Cast<IInteractInterface>(itemC->GetOwner());

				UActorComponent* actorComponent = itemC->GetOwner()->GetComponentByClass(UInteractComponent::StaticClass());
				UInteractComponent* interactComponent = Cast<UInteractComponent>(actorComponent);

				if (interactComponent && interactComponent->interactDataArray.Num() > 0 && interactComponent->interactDataArray.IsValidIndex(interactComponent->m_currentStage))
				{
					//FActorSelect actorSelect = { true, false, nullptr, NAME_None };
					FFunctionActorData actorSelect = { EGetActorList::getThisActor, {}, { true } };
					//FGetCustomActor getCustomActor = { AChPlayer::StaticClass(), NAME_None };
					FFunctionActorData actorData = { EGetActorList::getThisActor, {}, { false } };
					ETriggerSelect innerC = ETriggerSelect::Ignore;
					ETriggerSelect triggerC = ETriggerSelect::Ignore;
					ETriggerSelect highlightC = ETriggerSelect::Ignore;
					if (itemC->GetName().Contains("InnerCapsule")) {
						innerC = ETriggerSelect::Enter;
					}
					else if (itemC->GetName().Contains("OutsideCapsule")) {
						highlightC = ETriggerSelect::Enter;
					}
					else if (itemC->GetName().Contains("TriggerCapsule")) {
						triggerC = ETriggerSelect::Enter;
					}
					FExecuteTrigger executeData = { actorSelect, actorData, triggerC, highlightC, innerC };
					UInteractComponent::ExecuteTrigger(MyActor, executeData, interactComponent);
				}
			}
		}
	}
	inline virtual void InteractAction(FActionButton actionButton) {

		AActor* MyActor = Cast<AActor>(this);
		TArray<UPrimitiveComponent*> InteractiveItems;
		MyActor->GetOverlappingComponents(InteractiveItems);

		TArray<UInteractComponent*> highestPriorityOverlappingItem;
		UInteractComponent* minItem = nullptr;
		bool sameButton = false;
		EPriority bestPriority = EPriority::None;

		for (UPrimitiveComponent* itemC : InteractiveItems)
		{
			if (itemC->GetOwner()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) && itemC->ComponentHasTag("Interact") && itemC->GetOwner()->GetName() != MyActor->GetName())
			{
				sameButton = false;
				//IInteractInterface* itemClass = Cast<IInteractInterface>(itemC->GetOwner());

				UActorComponent* actorComponent = itemC->GetOwner()->GetComponentByClass(UInteractComponent::StaticClass());
				UInteractComponent* interactComponent = Cast<UInteractComponent>(actorComponent);

				if (interactComponent->interactDataArray.Num() > 0 && interactComponent->interactDataArray.IsValidIndex(interactComponent->m_currentStage)) {
					for (int i = 0; i < interactComponent->interactDataArray[interactComponent->m_currentStage].actions.Num(); i++) {
						if (interactComponent->interactDataArray[interactComponent->m_currentStage].actions[i].actionButton == actionButton.actionButton) {
							sameButton = true;
						}
					}
				}
				if (interactComponent) {
					if (interactComponent->GetPriority() == EPriority::None) {
						continue;
					}
					if (interactComponent->GetPriority() == EPriority::Min) {
						minItem = interactComponent;
						continue;
					}
					if (interactComponent->GetPriority() == EPriority::Max && sameButton) {
						highestPriorityOverlappingItem.Empty();
						highestPriorityOverlappingItem.Add(interactComponent);
						bestPriority = EPriority::Max;
						break;
					}
					if (sameButton && (highestPriorityOverlappingItem.Num() <= 0 || (highestPriorityOverlappingItem[0]->GetPriority() < interactComponent->GetPriority()) || (!highestPriorityOverlappingItem[0]->GetPriorityRepeat() && interactComponent->GetPriorityRepeat())))
					{
						highestPriorityOverlappingItem.Empty();
						highestPriorityOverlappingItem.Add(interactComponent);
					}
					else if (sameButton && highestPriorityOverlappingItem.Num() > 0 && highestPriorityOverlappingItem[0]->GetPriority() == interactComponent->GetPriority() && (highestPriorityOverlappingItem[0]->GetPriorityRepeat() && interactComponent->GetPriorityRepeat())) {
						highestPriorityOverlappingItem.Add(interactComponent);
					}
				}
			}
		}

		if (highestPriorityOverlappingItem.Num() > 0) {
			for (int i = 0; i < highestPriorityOverlappingItem.Num(); i++) {
				highestPriorityOverlappingItem[i]->Interact(MyActor, actionButton);
			}
		}
		else {
			if (minItem) {
				minItem->Interact(MyActor, actionButton);
			}
			// IF IS NOT OVERLAPING THEN MAKE PLAYER ACTION
			if (UInteractComponent* intInf = Cast<UInteractComponent>(MyActor->GetComponentByClass(UInteractComponent::StaticClass()))) {
				intInf->Interact(MyActor, actionButton);
			}
		}
	}
	inline virtual void Interact(AActor *actor, FActionButton actionButton) override {
		if (actionButton.actionButton == EAction::R) {
			// IMPLEMENT RELOAD
		}
		if (actionButton.actionButton == EAction::SHIFT) {

		}
		if (actionButton.actionButton == EAction::Q) {

		}
		if (actionButton.actionButton == EAction::F) {

		}
	}
	inline virtual void SetupInteractInput(class UInputComponent* PlayerInputComponent) {

		AActor* MyActor = Cast<AActor>(this);

		int maxE = (int)GETENUMMAX(TEXT("EAction"));
		for (int j = 0; j < maxE; j++) {
			FInputActionBinding* buttonPressed = new FInputActionBinding(FName(*GETENUMSTRING(TEXT("EAction"), j)), IE_Pressed);
			FInputActionHandlerSignature buttonPressedHandler;
			buttonPressedHandler.BindUFunction(MyActor, FName("InteractAction"), FActionButton((EAction)j, EActionType::Pressed));
			buttonPressed->ActionDelegate = buttonPressedHandler;
			PlayerInputComponent->AddActionBinding(*buttonPressed);

			FInputActionBinding* buttonReleased = new FInputActionBinding(FName(*GETENUMSTRING(TEXT("EAction"), j)), IE_Released);
			FInputActionHandlerSignature buttonReleasedHandler;
			buttonReleasedHandler.BindUFunction(MyActor, FName("InteractAction"), FActionButton((EAction)j, EActionType::Released));
			buttonReleased->ActionDelegate = buttonReleasedHandler;
			PlayerInputComponent->AddActionBinding(*buttonReleased);
		}
	}
};
