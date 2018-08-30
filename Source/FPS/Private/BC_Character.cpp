// Fill out your copyright notice in the Description page of Project Settings.

#include "BC_Character.h"
#include "BC_Weapon.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

ABC_Character::ABC_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABC_Character::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* const World = GetWorld();


	// Spawn & attach weapons
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	if (PrimaryWPNClass != NULL)
	{
		auto WPN = World->SpawnActor<ABC_Weapon>(PrimaryWPNClass);
		WPN->AttachToComponent(GetMesh(), rules, EquippedWPNSocketName);
		EquippedWPN = WPN;
		EquippedWPN->SetOwnerPawn(this);
	}

	if (SecondaryWPNClass != NULL)
	{
		auto WPN = World->SpawnActor<ABC_Weapon>(SecondaryWPNClass);
		WPN->AttachToComponent(GetMesh(), rules, SecondaryWPNSocketName);
	}
	
	if (MeleeWPNClass != NULL)
	{
		auto WPN = World->SpawnActor<ABC_Weapon>(MeleeWPNClass);
		WPN->AttachToComponent(GetMesh(), rules, MeleeWPNSocketName);
	}
}

void ABC_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABC_Character::Aim()
{
	bIsAiming = true;
}

void ABC_Character::StopAim()
{

	bIsAiming = false;
}

void ABC_Character::Fire()
{
	EquippedWPN->OnFire();
}

void ABC_Character::Reload()
{
	EquippedWPN->OnReload();
}

void ABC_Character::SwitchWeaponMode()
{
	EquippedWPN->SetToNextFireModePossible();
}

void ABC_Character::StopFire()
{
	EquippedWPN->StopFire();
}

float ABC_Character::GetForwardMovement()
{
	return FVector::DotProduct(GetActorForwardVector(), GetVelocity().GetSafeNormal());
}

float ABC_Character::GetRightMovement()
{
	return FVector::DotProduct(GetActorRightVector(), GetVelocity().GetSafeNormal());
}

ABC_Weapon * ABC_Character::GetEquippedWeapon()
{
	return EquippedWPN;
}

void ABC_Character::SetEquippedWeapon(ABC_Weapon * WeaponToEquip)
{
	EquippedWPN = WeaponToEquip;
}

bool ABC_Character::IsFiring()
{
	return EquippedWPN->IsFiring();
}

ECharacterMovementStatus ABC_Character::GetMovementStatus()
{
	return MovementStatus;
}

EWeaponPose ABC_Character::GetWeaponPose()
{
	return WeaponPose;
}

