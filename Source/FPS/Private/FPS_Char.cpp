// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_Char.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BC_Weapon.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// TODO: give fps_char his proper equipped weapon in a way that tps and fps equipped weapons exist in the same time and with the same reference if possible.

// Sets default values
AFPS_Char::AFPS_Char()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// hidding the 3rd person mesh
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->RelativeRotation = FRotator(0, -90, 0);

	// Create a SpringArmComponent for the 3P's camera
	ThirdPersonSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	ThirdPersonSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	ThirdPersonSpringArmComponent->TargetArmLength = 150;

	ThirdPersonSpringArmComponent->SocketOffset = FVector(0, 66, 20);
	ThirdPersonSpringArmComponent->bInheritYaw = true;
	ThirdPersonSpringArmComponent->bInheritPitch = true;
	ThirdPersonSpringArmComponent->bUsePawnControlRotation = true;

	// Create a CameraComponent for the third person mesh view 	
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	ThirdPersonCameraComponent->SetupAttachment(ThirdPersonSpringArmComponent);
	ThirdPersonCameraComponent->bAutoActivate = false;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

}

// Called when the game starts or when spawned
void AFPS_Char::BeginPlay()
{
	Super::BeginPlay();
	
	if (PrimaryWPNClass != NULL)
	{
		// equip primary weapon because of the extra hands of the 1P
		GetEquippedWeapon()->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		GetEquippedWeapon()->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), HandGripSocketName);
		GetEquippedWeapon()->SetCamera(FirstPersonCameraComponent);
		GetEquippedWeapon()->SetOwnerPawn(this);
		SetEquippedWeapon(GetEquippedWeapon());
	}

	MovementStatus = ECharacterMovementStatus::ECMS_Jogging;
	WeaponPose = EWeaponPose::EWP_Ready;
}

// Called every frame
void AFPS_Char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPS_Char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPS_Char::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPS_Char::StopFire);

	// Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPS_Char::Reload);

	// Bind switch fire mode event
	PlayerInputComponent->BindAction("SwitchWeaponMode", IE_Pressed, this, &AFPS_Char::SwitchWeaponMode);

	// Bind camera switching event
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AFPS_Char::SwitchCamera);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Char::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Char::MoveRight);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPS_Char::OnCrouchPressed);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFPS_Char::Run);
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AFPS_Char::Walk);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPS_Char::OnCrouchRelease);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AFPS_Char::RunRelease);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AFPS_Char::WalkRelease);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPS_Char::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPS_Char::LookUpAtRate);

}

void AFPS_Char::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
	MoveForwardValue = Value;
}

void AFPS_Char::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
	MoveRightValue = Value;
}

void AFPS_Char::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPS_Char::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPS_Char::SwitchCamera()
{
	
	// switching camera
	FirstPersonCameraComponent->ToggleActive();
	ThirdPersonCameraComponent->ToggleActive();

	// updating the mesh that the player see
	if (ThirdPersonCameraComponent->IsActive())
	{
		Mesh1P->SetOwnerNoSee(true);
		GetMesh()->SetOwnerNoSee(false);
		GetEquippedWeapon()->Mesh->SetOwnerNoSee(false);
		GetEquippedWeapon()->Mesh1P->SetOwnerNoSee(true);
		GetEquippedWeapon()->SetCamera(ThirdPersonCameraComponent);
	}
	else
	{
		GetEquippedWeapon()->Mesh->SetOwnerNoSee(true);
		GetEquippedWeapon()->Mesh1P->SetOwnerNoSee(false);
		Mesh1P->SetOwnerNoSee(false);
		GetMesh()->SetOwnerNoSee(true);
		GetEquippedWeapon()->SetCamera(FirstPersonCameraComponent);
	}

	// incomment this section if the character has more than 2 cameras to toggle between.
	//auto Cameras = GetComponentsByClass(UCameraComponent::StaticClass());
	//for (int i = 0; i < Cameras.Num(); i++)
	//{
	//	// finding activated camera component
	//	if (Cameras[i]->bIsActive == false) continue;

	//	// if the active camera is the last in the list activate the 1st one.
	//	if (i == Cameras.Num() - 1)
	//	{
	//		Cameras[i]->Deactivate();
	//		Cameras[0]->Activate();
	//	}
	//	else 
	//	{
	//		Cameras[i]->Deactivate();
	//		Cameras[i + 1]->Activate();
	//	}
	//	return;
	//}
}

float AFPS_Char::GetForwardMovement()
{
	return MoveForwardValue;
}

float AFPS_Char::GetRightMovement()
{
	return MoveRightValue;
}

void AFPS_Char::OnCrouchPressed()
{
	if (bToggleCrouch)
	{
		if (MovementStatus == ECharacterMovementStatus::ECMS_Crouching)
			MovementStatus = ECharacterMovementStatus::ECMS_Jogging;
		else
			MovementStatus = ECharacterMovementStatus::ECMS_Crouching;
	}
	else
	{
		MovementStatus = ECharacterMovementStatus::ECMS_Crouching;
	}
	if (MovementStatus == ECharacterMovementStatus::ECMS_Crouching)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AFPS_Char::OnCrouchRelease()
{
	if(!bToggleCrouch)
		MovementStatus = ECharacterMovementStatus::ECMS_Jogging;
}

void AFPS_Char::Run()
{
	if (bToggleRun)
	{
		if (MovementStatus == ECharacterMovementStatus::ECMS_Running)
			MovementStatus = ECharacterMovementStatus::ECMS_Jogging;
		else
			MovementStatus = ECharacterMovementStatus::ECMS_Running;
	}
	else
	{
		MovementStatus = ECharacterMovementStatus::ECMS_Running;
	}
	auto MovComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MovementStatus == ECharacterMovementStatus::ECMS_Running)
	{
		MovComp->MaxWalkSpeed = 1000;
	}
	else
	{
		MovComp->MaxWalkSpeed = 600;
	}
}

void AFPS_Char::RunRelease()
{
	if (!bToggleRun)
		MovementStatus = ECharacterMovementStatus::ECMS_Jogging;
}

void AFPS_Char::Walk()
{
	if (bToggleWalk)
	{
		if (MovementStatus == ECharacterMovementStatus::ECMS_Walking)
			MovementStatus = ECharacterMovementStatus::ECMS_Jogging;
		else
			MovementStatus = ECharacterMovementStatus::ECMS_Walking;
	}
	else
	{
		MovementStatus = ECharacterMovementStatus::ECMS_Walking;
	}
}

void AFPS_Char::WalkRelease()
{
	if (!bToggleWalk)
		MovementStatus = ECharacterMovementStatus::ECMS_Jogging;
}

void AFPS_Char::SetEquippedWeapon(ABC_Weapon * WeaponToEquip)
{
	Super::SetEquippedWeapon(WeaponToEquip);
	GetEquippedWeapon()->Mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), EquippedWPNSocketName);
	GetEquippedWeapon()->Mesh1P->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), HandGripSocketName);
	if (ThirdPersonCameraComponent->IsActive() == false)
	{
		GetEquippedWeapon()->Mesh1P->SetOwnerNoSee(false);
		GetEquippedWeapon()->SetCamera(FirstPersonCameraComponent);
	}
	else
	{
		GetEquippedWeapon()->SetCamera(ThirdPersonCameraComponent);
	}
}