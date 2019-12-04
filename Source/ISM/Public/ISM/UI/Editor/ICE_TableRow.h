
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <TextBlock.h>
#include <HorizontalBox.h>
#include "ICE_Button.h"
#include <Border.h>
#include "ICE_TableRow.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UICE_TableRow : public UUserWidget
{
	GENERATED_BODY()

public:
	FString m_rowActor = "";
	AActor* m_actor = nullptr;
	FString m_procedence = "";
	ULevel* m_level = nullptr;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
		UBorder* GetRowHolder();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetRowID();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetRowActor();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetRowWorld();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetRowLevel();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetRowButton();

	UFUNCTION()
		void SetRowData(FString rowID, FString rowActor, FString rowWorld, FString rowLevel, UICE_Button* rowButton, AActor* actor, FString procedence, ULevel* level);

};
