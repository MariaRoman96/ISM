
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <Border.h>
#include <Image.h>
#include <EditableTextBox.h>
#include "ICE_TableRowLog.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_TableRowLog : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UBorder* GetRowHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetTimestamp();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetReadOnlyText();
	UFUNCTION(BlueprintImplementableEvent)
		UImage* GetIcon();

	UFUNCTION()
		void SetRowData(ELogHelpers logType, FString text, int rowID, FString file, FDateTime time);

};
