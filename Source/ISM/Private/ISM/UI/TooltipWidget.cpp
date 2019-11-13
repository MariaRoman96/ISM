#include "TooltipWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ChPlayer.h"
#include "Math/Vector2D.h"
#include <WidgetAnimation.h>
#include <UserWidget.h>
#include <CanvasPanel.h>
#include <WidgetBlueprintGeneratedClass.h>
#include <WidgetTree.h>
#include <UMGSequencePlayer.h>
#include <TimerManager.h>
#include "InteractCore.h"

void UTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	InitAnim();
}

// void UTooltip::MarkPendingKill() {
// 	if (!IsRooted()) {
// 		Super::MarkPendingKill();
// 	}
// }

void UTooltip::BeginDestroy() {
	//ISM_PRINTLOG("Destruye tooltip: "+GetName())
	//SetDesignerFlags(EWidgetDesignFlags::Designing);
	Super::BeginDestroy();
}

void UTooltip::InitAnim()
{

	//Init animations
	UProperty* prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (prop != nullptr)
	{
		// Only interested in object properties
		if (prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

			// Only want the properties that are widget animations
			if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* object = objectProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(object);

				if (widgetAnim != nullptr) {

					FString Name = widgetAnim->GetName();
					WidgetAnimation.Add(Name, widgetAnim);
				}
			}
		}

		prop = prop->PropertyLinkNext;
	}
}

void UTooltip::Animate(float speedAnim)
{
	if (WidgetAnimation.Num()<=0) {
		InitAnim();
	}
	if (!isTooltipVisible || (quickEventTime != 0.f && delayQuickEventTime != 0.f)) {
		stoppedAnimations = false;

		isTooltipVisible = true;
		//Cast<UCanvasPanel>(GetWidgetTreeOwningClass()->WidgetTree->RootWidget)->GetSlots()[0]
		if (speedAnim != 0) {
			float time = 1.f / speedAnim;
			PlayAnimation(*WidgetAnimation.Find(L"Animation_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward, time);
			if (quickEventTime != 0.f && delayQuickEventTime != 0.f) {
				isQuickTimeAvailable = false;
				OnAnimComplete();
			}
		}
	}
}

void UTooltip::StopAnimate(float speedAnim)
{
	if (WidgetAnimation.Num() <= 0) {
		InitAnim();
	}
	if (isTooltipVisible) {
		isTooltipVisible = false;
		if (speedAnim != 0) {
			float time = 1.f / speedAnim;
			if (quickEventTime == 0.f || delayQuickEventTime == 0.f) {
				PlayAnimation(*WidgetAnimation.Find(L"Animation_INST"), 0.0f, 1, EUMGSequencePlayMode::Reverse, time);
			}else{
				//StopFunctions();
				//IsAvailableQTE();
				//PlayAnimation(*WidgetAnimation.Find(L"HideAll_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward);
			}
		}
		else {
			PlayAnimation(*WidgetAnimation.Find(L"HideAll_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward);
		}
	}
}

void UTooltip::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (quickEventTime != 0.f && delayQuickEventTime != 0.f) {
// 		if (Animation->GetName() == (*WidgetAnimation.Find(L"Animation_INST"))->GetName()) {
// 			OnAnimComplete();
// 		}
		if (Animation->GetName() == (*WidgetAnimation.Find(L"QTEOutside_INST"))->GetName()) {
			OnQTEDelayComplete();
		}
		else if (Animation->GetName() == (*WidgetAnimation.Find(L"QTEInside_INST"))->GetName()) {
			OnQTEComplete();
		}
	}
}

void UTooltip::OnAnimComplete()
{
	if (WidgetAnimation.Num() <= 0) {
		InitAnim();
	}
	if (quickEventTime != 0.f && delayQuickEventTime != 0.f) {
		float time = 1.f / delayQuickEventTime;
		if (!stoppedAnimations) {
			PlayAnimation(*WidgetAnimation.Find(L"QTEOutside_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward, time);
		}
		else {
			OnQTEDelayComplete();
		}
// 		FTimerDelegate delayDel;
// 		FTimerHandle delayHand;
// 		delayDel.BindUFunction(this, FName("OnQTEDelayComplete"));
// 		GetWorld()->GetTimerManager().SetTimer(delayHand, delayDel, delayQuickEventTime, false);
//		ActiveSequencePlayers.Last()->OnSequenceFinishedPlaying().AddUObject(this, &UTooltip::OnQTEDelayComplete);
	}
}

void UTooltip::OnQTEDelayComplete()
{
	if (WidgetAnimation.Num() <= 0) {
		InitAnim();
	}
	if (quickEventTime != 0.f) {
		float time = 1.f / quickEventTime;
		isQuickTimeAvailable = true;
		if (!stoppedAnimations) {
			PlayAnimation(*WidgetAnimation.Find(L"QTEInside_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward, time);
		}
		else {
			OnQTEComplete();
		}
// 		FTimerDelegate delayDel;
// 		FTimerHandle delayHand;
// 		delayDel.BindUFunction(this, FName("OnQTEComplete"));
// 		GetWorld()->GetTimerManager().SetTimer(delayHand, delayDel, quickEventTime, false);
//		ActiveSequencePlayers.Last()->OnSequenceFinishedPlaying().AddUObject(this, &UTooltip::OnQTEComplete);
	}
}

void UTooltip::OnQTEComplete()
{
	if (WidgetAnimation.Num() <= 0) {
		InitAnim();
	}
	if (quickEventTime != 0.f) {
		isQuickTimeAvailable = false;
		if (!stoppedAnimations) {
			float time = 1.f;
			if (speedOtherAnims != 0.f) {
				time = 1.f / speedOtherAnims;
			}
			PlayAnimation(*WidgetAnimation.Find(L"QTEFail_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward, time);
		}
		stoppedAnimations = false;
	}
}

float UTooltip::IsAvailableQTE(float speed)
{
	if (WidgetAnimation.Num() <= 0) {
		InitAnim();
	}
	if (isQuickTimeAvailable) {
		StopFunctions(false);
		if (!stoppedAnimations) {
			float time = 1.f;
			if (speed != 0.f) {
				time = 1.f / speed;
			}
			PlayAnimation(*WidgetAnimation.Find(L"QTESuccess_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward, time);
		}
		stoppedAnimations = true;
		return true;
	}
	else {
		StopFunctions(false);
		if (!stoppedAnimations) {
			float time = 1.f;
			if (speed != 0.f) {
				time = 1.f / speed;
			}
			PlayAnimation(*WidgetAnimation.Find(L"QTEFail_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward, time);
		}
		stoppedAnimations = true;
		isQuickTimeAvailable = true;
		return false;
	}
}

void UTooltip::StopFunctions(bool stopAnims, float speed, bool tooltipVisibleLogic)
{
	if (WidgetAnimation.Num() <= 0) {
		InitAnim();
	}
	//GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	StopAnimationsAndLatentActions();
	if (stopAnims) {
		stoppedAnimations = true;
	}
	if (tooltipVisibleLogic) {
		isTooltipVisible = false;
	}
	//StopAllAnimations();
	float time = 1.f;
	if (speed != 0.f) {
		time = 1.f / speed;
	}
	PlayAnimation(*WidgetAnimation.Find(L"HideAll_INST"), 0.0f, 1, EUMGSequencePlayMode::Forward, time);
}
