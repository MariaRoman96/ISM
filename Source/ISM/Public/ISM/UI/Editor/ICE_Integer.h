
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <EditableTextBox.h>
#include <SpinBox.h>
#include <HorizontalBox.h>
#include "ICE_Integer.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_Integer : public UUserWidget
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
		USpinBox* GetICEContainer();

	UFUNCTION(BlueprintImplementableEvent)
		int GetValue();

	UFUNCTION(BlueprintImplementableEvent)
		void SetValue(int val);

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetIntegerHolder();

};

