#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <ComboBoxString.h>
#include <HorizontalBox.h>
#include "InteractComponent.h"
#include "ICE_Capsule.generated.h"

class UICE_Stage;

/**
 * 
 */
UCLASS()
class ISM_API UICE_Capsule : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void RemoveOutlineColorOption();
	void GetData();
	void UpdateData();

	//VARIABLE ACCESS
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetCapsuleTitle();
	UFUNCTION(BlueprintImplementableEvent)
		UComboBoxString* GetOutlineColor();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetOutlineColorBlock();

	UFUNCTION(BlueprintImplementableEvent)
		FVector2D GetSize();
	UFUNCTION(BlueprintImplementableEvent)
		FVector GetLocation();
	UFUNCTION(BlueprintImplementableEvent)
		FVector GetRotation();
	UFUNCTION(BlueprintImplementableEvent)
		FVector GetScale();

	UFUNCTION(BlueprintImplementableEvent)
		bool SetSize(FVector2D vector);
	UFUNCTION(BlueprintImplementableEvent)
		bool SetLocation(FVector vector);
	UFUNCTION(BlueprintImplementableEvent)
		bool SetRotation(FVector vector);
	UFUNCTION(BlueprintImplementableEvent)
		bool SetScale(FVector vector);

	UPROPERTY()
		int stageIndex = 0;
	UPROPERTY()
		int capsuleIndex = 0;
	UPROPERTY()
		UICE_Stage* stageData;
// 	UPROPERTY()
// 		UInteractComponent* interactComponent;


};
