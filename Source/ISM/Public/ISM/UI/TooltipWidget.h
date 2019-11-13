#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Image.h>
#include "TooltipWidget.generated.h"

/**
*
*/
UCLASS()
class ISM_API UTooltip : public UUserWidget
{
	GENERATED_BODY()

	public:

		virtual void NativeConstruct() override;
		
		virtual void BeginDestroy() override;
		//virtual void Tick(FGeometry MyGeometry, float InDeltaTime) override;

		//On begin play get all animations
		UFUNCTION(BlueprintCallable)
			void InitAnim();
		UFUNCTION(BlueprintCallable)
			void Animate(float speedAnim);
		UFUNCTION(BlueprintCallable)
			void StopAnimate(float speedAnim);

		//Tooltip BG
		UFUNCTION(BlueprintImplementableEvent)
		UImage* GetCircle();

		//Tooltip Icon
		UFUNCTION(BlueprintImplementableEvent)
		UImage* GetIcon();

		//Tooltip ActionButton
		UFUNCTION(BlueprintImplementableEvent)
		UImage* GetButton();

		//Tooltip
		UFUNCTION(BlueprintImplementableEvent)
			void SetTooltipLocation(FVector location);

		//Tooltip
		UFUNCTION(BlueprintImplementableEvent)
			void SetTooltipSize(FVector location);

		//Tooltip BG
		UFUNCTION(BlueprintImplementableEvent)
			void SetCircleTexture(UTexture2D* texture);

		//Tooltip Icon
		UFUNCTION(BlueprintImplementableEvent)
			void SetIconTexture(UTexture2D* texture);

		//Tooltip ActionButton
		UFUNCTION(BlueprintImplementableEvent)
			void SetButtonTexture(UTexture2D* texture);

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* circleTexture;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* iconTexture;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* buttonTexture;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isTooltipVisible = false;


		virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

		UFUNCTION()
		void OnAnimComplete();
		UFUNCTION()
		void OnQTEDelayComplete();
		UFUNCTION()
		void OnQTEComplete();

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool stoppedAnimations = false;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool isQuickTimeAvailable = true;
		//Tooltip ActionButton
		UFUNCTION(BlueprintCallable)
			float IsAvailableQTE(float speed=1.f);

		UFUNCTION(BlueprintCallable)
			void StopFunctions(bool stopAnims = true, float speed = 1.f, bool tooltipVisibleLogic = false);

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float delayQuickEventTime = 0.f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float quickEventTime = 0.f;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			float speedOtherAnims = 0.f;


// 		void MarkPendingKill() override {
// 			if (!IsRooted()) {
// 				Super::MarkPendingKill();
// 			}
// 		}

	private:
		TMap<FString, class UWidgetAnimation*> WidgetAnimation;
};