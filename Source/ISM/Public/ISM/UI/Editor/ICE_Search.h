
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <ScrollBox.h>
#include <ExpandableArea.h>
#include <TextBlock.h>
#include <ComboBoxString.h>
#include <EditableTextBox.h>
#include "ICE_Search.generated.h"

class UICE_BasicFunction;
class UICE_ActorFunction;

/**
 * 
 */
UCLASS()
class ISM_API UICE_Search : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetSearchText();

	UFUNCTION()
		void OnTextChanged(const FText& text);

	UPROPERTY()
		UComboBoxString* selector;

	UPROPERTY()
		bool hasImages = false;

	UPROPERTY()
		UICE_BasicFunction* basicFun = nullptr;

	UPROPERTY()
		UICE_ActorFunction* actorFun = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
		UScrollBox* GetHolderScroll();

	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetExpandableArea();

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetExpandableText();

	UFUNCTION()
		void UpdateValues();
};
