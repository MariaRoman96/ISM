
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <GridPanel.h>
#include <TextBlock.h>
#include <HorizontalBox.h>
#include "ICE_Color.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_Color : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICENameContainer();

	UFUNCTION()
		FString GetName();

	UFUNCTION()
		void SetName(FString val, FString tooltip = "");

	UFUNCTION(BlueprintImplementableEvent)
		UGridPanel* GetICEContainer();

	UFUNCTION(BlueprintImplementableEvent)
		FColor GetValue();

	UFUNCTION(BlueprintImplementableEvent)
		void SetValue(FColor val);

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetColorHolder();
};
