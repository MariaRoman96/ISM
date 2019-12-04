
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TooltipWidget.h"
#include "InteractWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ISM_API UInteractWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void AddViewportInteract(UUserWidget* tooltip = nullptr);
};
