#pragma once

#include "CoreMinimal.h"
#include <Engine/Texture2D.h>
#include "AssetRegistryModule.h"
#include "AssetData.h"
#include "UObjectIterator.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/ActorComponent.h"
#include "UserWidget.h"
#include "InteractConfig.generated.h"

class ACh;
class AItem;

UENUM(BlueprintType)
enum class EAction : uint8
{
	None			UMETA(DisplayName = "None"),
	Auto1			UMETA(DisplayName = "Auto1"),
	Auto2			UMETA(DisplayName = "Auto2"),
	Auto3			UMETA(DisplayName = "Auto3"),
	Auto4			UMETA(DisplayName = "Auto4"),
	Auto5			UMETA(DisplayName = "Auto5"),
	LMB				UMETA(DisplayName = "LMB"),
	RMB				UMETA(DisplayName = "RMB"),
	SHIFT			UMETA(DisplayName = "Shift"),
	TAB				UMETA(DisplayName = "Tab"),
	ESC				UMETA(DisplayName = "Escape"),
	BottomArrow		UMETA(DisplayName = "Down"),
	LeftArrow		UMETA(DisplayName = "Left"),
	TopArrow		UMETA(DisplayName = "Top"),
	RightArrow		UMETA(DisplayName = "Right"),
	Q				UMETA(DisplayName = "Q"),
	W				UMETA(DisplayName = "W"),
	E				UMETA(DisplayName = "E"),
	R				UMETA(DisplayName = "R"),
	T				UMETA(DisplayName = "T"),
	Y				UMETA(DisplayName = "Y"),
	U				UMETA(DisplayName = "U"),
	I				UMETA(DisplayName = "I"),
	O				UMETA(DisplayName = "O"),
	P				UMETA(DisplayName = "P"),
	A				UMETA(DisplayName = "A"),
	S				UMETA(DisplayName = "S"),
	D				UMETA(DisplayName = "D"),
	F				UMETA(DisplayName = "F"),
	G				UMETA(DisplayName = "G"),
	H				UMETA(DisplayName = "H"),
	J				UMETA(DisplayName = "J"),
	K				UMETA(DisplayName = "K"),
	L				UMETA(DisplayName = "L"),
	Z				UMETA(DisplayName = "Z"),
	X				UMETA(DisplayName = "X"),
	C				UMETA(DisplayName = "C"),
	V				UMETA(DisplayName = "V"),
	B				UMETA(DisplayName = "B"),
	N				UMETA(DisplayName = "N"),
	M				UMETA(DisplayName = "M"),
	NUM1			UMETA(DisplayName = "1"),
	NUM2			UMETA(DisplayName = "2"),
	NUM3			UMETA(DisplayName = "3"),
	NUM4			UMETA(DisplayName = "4"),
	NUM5			UMETA(DisplayName = "5"),
	NUM6			UMETA(DisplayName = "6"),
	NUM7			UMETA(DisplayName = "7"),
	NUM8			UMETA(DisplayName = "8"),
	NUM9			UMETA(DisplayName = "9"),
	NUM0			UMETA(DisplayName = "0"),
	F1				UMETA(DisplayName = "F1"),
	F2				UMETA(DisplayName = "F2"),
	F3				UMETA(DisplayName = "F3"),
	F4				UMETA(DisplayName = "F4"),
	F5				UMETA(DisplayName = "F5"),
	F6				UMETA(DisplayName = "F6"),
	F7				UMETA(DisplayName = "F7"),
	F8				UMETA(DisplayName = "F8"),
	F9				UMETA(DisplayName = "F9"),
	F10				UMETA(DisplayName = "F10"),
	F11				UMETA(DisplayName = "F11"),
	F12				UMETA(DisplayName = "F12"),
};

UENUM(BlueprintType)
enum class EPlatform : uint8
{
	None 					UMETA(DisplayName = "None"),
	PC						UMETA(DisplayName = "PC"),
	Play					UMETA(DisplayName = "Play"),
	Xbox					UMETA(DisplayName = "Xbox"),
	Switch					UMETA(DisplayName = "Switch"),
	Mac						UMETA(DisplayName = "Mac"),
	IOS						UMETA(DisplayName = "iOS"),
	Android					UMETA(DisplayName = "Android")
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Pressed			UMETA(DisplayName = "Pressed"),
	Released		UMETA(DisplayName = "Released")
};

USTRUCT(BlueprintType)
struct FDataGetter
{
	GENERATED_USTRUCT_BODY()
public:

	FDataGetter() {
#if WITH_EDITOR
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		AssetRegistryModule.Get().OnAssetAdded().AddRaw(this, &FDataGetter::OnAssetAdded);
#endif
	}

	~FDataGetter() {
#if WITH_EDITOR
		if (!GIsRequestingExit) {
			FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
			AssetRegistryModule.Get().OnAssetAdded().RemoveAll(this);
		}
#endif
	}

#if WITH_EDITOR
	void OnAssetAdded(const FAssetData& AssetData);

	void ReloadAssetData(UWorld* world);
#endif

	UPROPERTY()
		UWorld* actualWorld = nullptr;
	UPROPERTY()
		TArray<UObject*> textures2D;
	UPROPERTY()
		TArray<UObject*> curveFloat;
	UPROPERTY()
		TArray<UObject*> animMontage;
	UPROPERTY()
		TArray<UObject*> particleSystem;
	UPROPERTY()
		TArray<UObject*> sequenceClasses;
	UPROPERTY()
		TArray<UObject*> actorClasses;
	UPROPERTY()
		TArray<UObject*> actorsLevel;
	UPROPERTY()
		TArray<UObject*> componentClasses;
	UPROPERTY()
		TArray<UObject*> actorComponentClasses;
	UPROPERTY()
		TArray<UObject*> materials;
	UPROPERTY()
		TArray<UObject*> sounds;
	UPROPERTY()
		TArray<UObject*> widgets;
	UPROPERTY()
		TArray<UObject*> classes;
};

UCLASS()
class ISM_API UInteractConfig: public UObject
{
	GENERATED_BODY()

public:
	UInteractConfig() {};

	//It is the currentPlatform (it could change for example if you connect a gamepad)
	static EPlatform currentPlatform;

	//It is the defaultPlatform for the game
	static EPlatform defaultPlatform;

	//If a gamepad is connected to PC
	static EPlatform defaultGamepadPlatform;

	UFUNCTION(BlueprintCallable, Category = "ISM - Platform")
		static void checkCurrentPlatform();

	UFUNCTION(BlueprintCallable, Category = "ISM - Platform")
		static UTexture2D* GetActionButtonImage(EAction actionButton);
};