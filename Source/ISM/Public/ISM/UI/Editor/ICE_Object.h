
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <ComboBoxString.h>
#include "ICE_Search.h"
#include <HorizontalBox.h>
#include "ICE_Object.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_Object : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICEName();

	UFUNCTION()
		FString GetName();

	UFUNCTION()
		void SetName(FString val, FString tooltip = "");

	UFUNCTION(BlueprintImplementableEvent)
		UComboBoxString* GetICEContent();

	UFUNCTION(BlueprintImplementableEvent)
		UICE_Search* GetICESearchBar();

	UFUNCTION()
		void SetValue(UObject* value);

	UFUNCTION()
		UObject* GetValue();

	TArray<UObject*> elementArray;


	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetObjectHolder();

};
