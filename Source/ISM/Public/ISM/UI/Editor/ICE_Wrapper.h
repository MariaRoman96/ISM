#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Text.h>
#include <TextBlock.h>
#include <HorizontalBox.h>
#include <WidgetSwitcher.h>
#include <vector>
#include <Array.h>
#include <AssetData.h>
#include <AssetRegistryModule.h>
#include <ModuleManager.h>
#if WITH_EDITOR
#include <ClassViewerModule.h>
#include "Ch.h"
#include "Item.h"
#include <GameFramework/Pawn.h>
#include <GameFramework/Character.h>
#include <GameFramework/Actor.h>
#endif
#include "InteractCore.h"
#include "Ch.h"
#include <GameFramework/Pawn.h>
#include "Item.h"
#include <GameFramework/Actor.h>
#include <GameFramework/Character.h>
#include <SpinBox.h>
#include "ICE_Wrapper.generated.h"

using namespace std;

class UICE_Button;
class UICE_Stage;
class UInteractComponent;

UCLASS()
class ISM_API UICE_Wrapper : public UUserWidget
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	void OnPreWorldInitializationInteract(UWorld* World, const UWorld::InitializationValues IVS);
#endif

	static FDataGetter dataGetter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* savedAct;

	UPROPERTY()
		TArray<UICE_Button*> navBarButtons;
	UPROPERTY()
		UInteractComponent* interactComponent;
	UPROPERTY()
		TArray<UICE_Stage*> stages;

	UPROPERTY()
		UInteractComponent* interactComponentBP;

	UPROPERTY()
		AActor* actorInteractBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_ticksTime = 1.f;
	FTimerHandle wrapperHandle;


	FTimerHandle loadingHandle;


	UPROPERTY()
		UInteractComponent* interactComponentExp = nullptr;
	UFUNCTION(BlueprintCallable)
		void CheckExportData();

	UFUNCTION(BlueprintCallable)
 		void ExportSave();
	UFUNCTION(BlueprintCallable)
		void ExportReLoad();


	// Weak pointer to the level editor's compile notification item.
	TWeakPtr<SNotificationItem> CompileNotificationPtr;

	// Sounds used for compilation.
	class USoundBase* CompileStartSound;
	class USoundBase* CompileSuccessSound;
	class USoundBase* CompileFailSound;

	// Absolute real time that we started compiling modules. Used for stats tracking.
	double ModuleCompileStartTime;

	UFUNCTION(BlueprintCallable)
		void OnLoadStart();


	UFUNCTION(BlueprintCallable)
		void OnLoadEnd();


	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void CheckIfAreChanges(TArray<AActor*> selectedActorsArray);
	UFUNCTION(BlueprintCallable)
		void GetStageData(int fromIndex = -1);
	UFUNCTION(BlueprintCallable)
		void GetAllData(bool cleanActor = false, int resetNavSelected = -1);
	UFUNCTION(BlueprintCallable)
		void GetData();
	UFUNCTION(BlueprintCallable)
		void UpdateData();
	UFUNCTION(BlueprintCallable)
		void BuildEditor();
	UFUNCTION(BlueprintCallable)
		void ChangeStageTitle();
	UFUNCTION(BlueprintCallable)
		void ResetWrapper(bool cleanActor = true);
	UFUNCTION(BlueprintCallable)
		void UpdateNavBar(int currentTab = 0);
	UFUNCTION(BlueprintCallable)
		FDataGetter GetDataGetter();

	//VARIABLE ACCESS
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetICEOptions();
	UFUNCTION(BlueprintImplementableEvent)
		AActor* GetSelectedActor();
	UFUNCTION(BlueprintImplementableEvent)
		void SetSelectedActor(AActor* actor = nullptr);
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetTitle();
	UFUNCTION(BlueprintImplementableEvent)
		UTextBlock* GetCurrentStageText();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetNavBar();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetNavBarAdd();
	UFUNCTION(BlueprintImplementableEvent)
		UHorizontalBox* GetWrapperStages();
	UFUNCTION(BlueprintImplementableEvent)
		UWidgetSwitcher* GetContent();
	UFUNCTION(BlueprintImplementableEvent)
		UEditableTextBox* GetInitStage();
	UFUNCTION(BlueprintImplementableEvent)
		USpinBox* GetTickNum();
	UFUNCTION(BlueprintImplementableEvent)
		USpinBox* GetTickTime();
};
