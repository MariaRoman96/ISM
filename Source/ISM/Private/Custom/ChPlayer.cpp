
#include "ChPlayer.h"
#include "Item.h"
#include <UserWidget.h>
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Ch.h"
#include <Components/ArrowComponent.h>
#include "DrawDebugHelpers.h"
#include "InteractComponent.h"
#include <GameFramework/Character.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include <HeadMountedDisplayFunctionLibrary.h>

/**********************************************************************_CONSTRUCTOR_**************************************************************************/

AChPlayer::AChPlayer()
{
	// Set this Pawn to call Tick() every frame. You can turn this off to iimprove performance if you don´t need it.
	PrimaryActorTick.bCanEverTick = true;

	// AUTOPOSSES
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// ADD TAG
	Tags.Add(FName("player"));

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


}

/*************************************************************************_INPUT_*****************************************************************************/

void AChPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	SetupInteractInput(PlayerInputComponent);

	////////////////////
	//   MOVEMENT    ///
	////////////////////

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AChPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AChPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AChPlayer::LookUpAtRate);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AChPlayer::OnResetVR);
}

/*******************************************************************_BEGIN PLAY / TICK_***********************************************************************/

// void AChPlayer::ExecuteNearTriggers() {
// 	TArray<UPrimitiveComponent*> InteractiveItems;
// 	GetOverlappingComponents(InteractiveItems);
// 
// 	for (UPrimitiveComponent* itemC : InteractiveItems)
// 	{
// 		if (itemC->GetOwner()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()) && itemC->GetOwner()->GetName() != GetName())
// 		{
// 			IInteractInterface* itemClass = Cast<IInteractInterface>(itemC->GetOwner());
// 
// 			UActorComponent* actorComponent = itemC->GetOwner()->GetComponentByClass(UInteractComponent::StaticClass());
// 			UInteractComponent* interactComponent = Cast<UInteractComponent>(actorComponent);
// 
// 			if (interactComponent && interactComponent->interactDataArray.Num() > 0 && interactComponent->interactDataArray.IsValidIndex(interactComponent->m_currentStage))
// 			{
// 				FActorSelect actorSelect = { true, false, nullptr, NAME_None };
// 				//FGetCustomActor getCustomActor = { AChPlayer::StaticClass(), NAME_None };
// 				FFunctionActorData actorData = { EGetActorList::getThisActor, {}, { false } };
// 				ETriggerSelect innerC = ETriggerSelect::Ignore;
// 				ETriggerSelect triggerC = ETriggerSelect::Ignore;
// 				ETriggerSelect highlightC = ETriggerSelect::Ignore;
// 				if (itemC->GetName().Contains("InnerCapsule")) {
// 					innerC = ETriggerSelect::Enter;
// 				}
// 				else if (itemC->GetName().Contains("OutsideCapsule")) {
// 					highlightC = ETriggerSelect::Enter;
// 				}
// 				else if (itemC->GetName().Contains("TriggerCapsule")) {
// 					triggerC = ETriggerSelect::Enter;
// 				}
// 				FExecuteTrigger executeData = { actorSelect, actorData, triggerC, highlightC, innerC };
// 				UInteractComponent::ExecuteTrigger(this, executeData, interactComponent);
// 			}
// 		}
// 	}
// }

void AChPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
}

void AChPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**********************************************************************_INTERACTION_**********************************************************************/

void AChPlayer::InteractAction(FActionButton actionButton)
{
	IPlayerInterface::InteractAction(actionButton);

}

// void AChPlayer::Interact(AActor* actor, FActionButton actionButton)
// {
// 
// 	if (actionButton.actionButton == EAction::R) {
// 		// IMPLEMENT RELOAD
// 	}
// 	if (actionButton.actionButton == EAction::SHIFT) {
// 
// 	}
// 	if (actionButton.actionButton == EAction::Q) {
// 
// 	}
// 	if (actionButton.actionButton == EAction::F) {
// 
// 	}
// }

// void AChPlayer::DestroyActor()
// {
// 	// IMPLEMENT CUSTOM DESTROY HERE
// 
// 	Super::DestroyActor();
// }

void AChPlayer::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AChPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AChPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AChPlayer::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AChPlayer::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
