
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <HorizontalBox.h>
#include <TextBlock.h>
#include <VerticalBox.h>
#include "ICE_Stage.h"
#include "ICE_Array.generated.h"

class UICE_BasicFunction;

/**
 * 
 */
UCLASS()
class ISM_API UICE_Array : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UPROPERTY()
		FString typeArray;

	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEButtonOptions();

	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetICEName();

	UFUNCTION(BlueprintImplementableEvent)
		UVerticalBox* GetICEContent();

	UFUNCTION(BlueprintImplementableEvent)
		UExpandableArea* GetICEExpandable();

	UPROPERTY()
		UICE_Stage* stage;

	UPROPERTY()
		UICE_BasicFunction* basicFunc;

	void* parent = nullptr;

	UArrayProperty* It = nullptr;
};
