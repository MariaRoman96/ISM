
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ICE_Stage.h"
#include <HorizontalBox.h>
#include "ICE_Array.h"
#include "ICE_ArrayElement.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_ArrayElement : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEButtonOptions();

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEContent();

	UPROPERTY()
		UICE_Stage* stage;

	UPROPERTY()
		UICE_BasicFunction* basicFunc;

	UPROPERTY()
		UICE_Array* array;

	void* parent = nullptr;

	UArrayProperty* It = nullptr;

	int index = -1;
};
