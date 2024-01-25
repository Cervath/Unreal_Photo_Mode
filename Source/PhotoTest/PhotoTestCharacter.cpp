// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhotoTestCharacter.h"
#include "PhotoTestProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

/* ----PHOTO MODE---- */
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "UI/PH_HUD.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// APhotoTestCharacter

APhotoTestCharacter::APhotoTestCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	/* ----PHOTO MODE---- */

	//Attatch with the camare we want to followand have the focus.
	sceneCapture_ = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Photo"));
	sceneCapture_->SetupAttachment(FirstPersonCameraComponent);


}

void APhotoTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	hud_ = Cast<APH_HUD>(UGameplayStatics::GetPlayerController(GetOwner(), 0)->GetHUD());

	hud_->cameraWidget_->baseMaterial_ = baseMaterial_;
}

//////////////////////////////////////////////////////////////////////////// Input

void APhotoTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APhotoTestCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhotoTestCharacter::Look);

		/* ----PHOTO MODE---- */

		//Take Picture
		EnhancedInputComponent->BindAction(TakePictureAction, ETriggerEvent::Completed, this, &APhotoTestCharacter::TakePicture);

		//Next Image
		EnhancedInputComponent->BindAction(NextImageAction, ETriggerEvent::Completed, this, &APhotoTestCharacter::NextImage);

	}
}



void APhotoTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APhotoTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/* ----PHOTO MODE---- */

void APhotoTestCharacter::TakePicture(const FInputActionValue& Value){

	
	// Texture dimensions
	int32 Width = 1920;
	int32 Height = 1080;

	// Create render target 2D
	UTextureRenderTarget2D* MyRenderTarget = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass());
	MyRenderTarget->InitAutoFormat(Width, Height);
	MyRenderTarget->UpdateResourceImmediate(true);

	// We will save the output of sceneCapture_ in this texture
	sceneCapture_->TextureTarget = MyRenderTarget;

	// Other settings you want to change on the SceneCaptureComponent2D
	
	
	// Capture the scene
	sceneCapture_->CaptureScene();

	// We add it to our texture arrayand pass it to the hud to handle the change.
	targets_.Add(MyRenderTarget);
	hud_->cameraWidget_->SetImageTexture(MyRenderTarget);

}

void APhotoTestCharacter::NextImage(const FInputActionValue& Value) {

	ite_ = (ite_ + 1) % targets_.Num();

	hud_->cameraWidget_->SetImageTexture(targets_[ite_]);

}

void APhotoTestCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool APhotoTestCharacter::GetHasRifle()
{
	return bHasRifle;
}