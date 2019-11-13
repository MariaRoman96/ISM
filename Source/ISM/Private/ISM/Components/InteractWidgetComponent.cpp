// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidgetComponent.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineGlobals.h"
#include "MaterialShared.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Engine.h"
#include "Widgets/SWindow.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Input/HittestGrid.h"
#include "SceneManagement.h"
#include "DynamicMeshBuilder.h"
#include "PhysicsEngine/BoxElem.h"
#include "PhysicsEngine/BodySetup.h"
#include "Slate/SGameLayerManager.h"
#include "Slate/WidgetRenderer.h"
#include "Widgets/SViewport.h"
#include <WeakObjectPtrTemplates.h>
#include <SharedPointer.h>
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Engine/LocalPlayer.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "SWorldInteractScreenLayer.h"

class FWorldWidgetScreenLayerInteract : public IGameLayer
{
public:
	FWorldWidgetScreenLayerInteract(const FLocalPlayerContext& PlayerContext)
	{
		OwningPlayer = PlayerContext;
	}

	virtual ~FWorldWidgetScreenLayerInteract()
	{
		// empty virtual destructor to help clang warning
	}

	void AddComponent(UWidgetComponent* Component)
	{
		if (Component)
		{
			Components.AddUnique(Component);
			if (ScreenLayer.IsValid())
			{
				if (UUserWidget* UserWidget = Component->GetUserWidgetObject())
				{
					ScreenLayer.Pin()->AddComponent(Component, UserWidget->TakeWidget());
				}
				else if (Component->GetSlateWidget().IsValid())
				{
					ScreenLayer.Pin()->AddComponent(Component, Component->GetSlateWidget().ToSharedRef());
				}
			}
		}
	}

	void RemoveComponent(UWidgetComponent* Component)
	{
		if (Component)
		{
			Components.RemoveSwap(Component);

			if (ScreenLayer.IsValid())
			{
				ScreenLayer.Pin()->RemoveComponent(Component);
			}
		}
	}

	virtual TSharedRef<SWidget> AsWidget() override
	{
		if (ScreenLayer.IsValid())
		{
			return ScreenLayer.Pin().ToSharedRef();
		}
		TSharedRef<SWorldInteractScreenLayer> NewScreenLayer = SNew(SWorldInteractScreenLayer, OwningPlayer);
		ScreenLayer = NewScreenLayer;

		// Add all the pending user widgets to the surface
		for (TWeakObjectPtr<UWidgetComponent>& WeakComponent : Components)
		{
			if (UWidgetComponent* Component = WeakComponent.Get())
			{
				if (UUserWidget* UserWidget = Component->GetUserWidgetObject())
				{
					NewScreenLayer->AddComponent(Component, UserWidget->TakeWidget());
				}
				else if (Component->GetSlateWidget().IsValid())
				{
					NewScreenLayer->AddComponent(Component, Component->GetSlateWidget().ToSharedRef());
				}
			}
		}

		return NewScreenLayer;
	}

private:
	FLocalPlayerContext OwningPlayer;
 	TWeakPtr<SWorldInteractScreenLayer> ScreenLayer;
 	TArray<TWeakObjectPtr<UWidgetComponent>> Components;
};

void UInteractWidgetComponent::AddViewportInteract(UUserWidget* tooltip)
{
	ULocalPlayer* TargetPlayer = GetOwnerPlayer();
	APlayerController* PlayerController = TargetPlayer ? TargetPlayer->PlayerController : nullptr;
	if (TargetPlayer && PlayerController && IsVisible())
	{
		if (!bAddedToScreen)
		{
			if (UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
			{
				TSharedPtr<IGameLayerManager> LayerManager = ViewportClient->GetGameLayerManager();
				if (LayerManager.IsValid())
				{
					TSharedPtr<FWorldWidgetScreenLayerInteract> ScreenLayer;

					FLocalPlayerContext PlayerContext(TargetPlayer, GetWorld());

					TSharedPtr<IGameLayer> Layer = LayerManager->FindLayerForPlayer(TargetPlayer, SharedLayerName);
					if (!Layer.IsValid())
					{
						TSharedRef<FWorldWidgetScreenLayerInteract> NewScreenLayer = MakeShareable(new FWorldWidgetScreenLayerInteract(PlayerContext));
						LayerManager->AddLayerForPlayer(TargetPlayer, SharedLayerName, NewScreenLayer, LayerZOrder);
						ScreenLayer = NewScreenLayer;
					}
					else
					{
						ScreenLayer = StaticCastSharedPtr<FWorldWidgetScreenLayerInteract>(Layer);
					}

					bAddedToScreen = true;

					if (tooltip && tooltip->IsValidLowLevel()){
						tooltip->SetPlayerContext(PlayerContext);
					}
					else if (Widget && Widget->IsValidLowLevel())
					{
						Widget->SetPlayerContext(PlayerContext);
					}

					ScreenLayer->AddComponent(this);
				}
			}
		}
	}
}
