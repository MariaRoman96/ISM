
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <CheckBox.h>
#include <HorizontalBox.h>
#include "ICE_Boolean.generated.h"

class UICE_BasicFunction;

/**
 * 
 */
UCLASS()
class ISM_API UICE_Boolean : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY()
		UICE_BasicFunction* basicFunc;

	UFUNCTION()
		void OnChangeSelect(bool bIsChecked);

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICENameContainer();

	UFUNCTION()
		FString GetName();

	UFUNCTION()
		void SetName(FString val, FString tooltip = "");

	UFUNCTION(BlueprintImplementableEvent)
		UCheckBox* GetICEContainer();

	UFUNCTION(BlueprintImplementableEvent)
		bool GetValue();

	UFUNCTION(BlueprintImplementableEvent)
		void SetValue(bool val);

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetBooleanHolder();

};
