// Fill out your copyright notice in the Description page of Project Settings.

#include "BC_AnimInstance.h"
#include "BC_Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BC_RifleWeapon.h"
#include "BC_HandgunWeapon.h"
#include "BC_ShotgunWeapon.h"
#include "BC_SniperWeapon.h"
#include "BC_MeleeWeapon.h"


void UBC_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	OwnerPawn = Cast<ABC_Character>(TryGetPawnOwner());
	if (OwnerPawn == NULL) { return; }

	// getting animation play rate
	AnimationPlayRate = OwnerPawn->AnimationPlayRate;

	// getting movement related VARs
	MoveRight = OwnerPawn->GetRightMovement();
	MoveForward = OwnerPawn->GetForwardMovement();
	MovementStatus = OwnerPawn->GetMovementStatus();

	// getting Weapon related VARs
	bIsFiring = OwnerPawn->IsFiring();
	WeaponPose = OwnerPawn->GetWeaponPose();

	// getting the rotation offset between pawn rotation and control rotation
	FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(OwnerPawn->GetControlRotation(), OwnerPawn->GetActorRotation());
	PitchOffset = DeltaRotation.Pitch;
	YawOffset = DeltaRotation.Yaw;

	// getting the type of the equipped weapon
	if (UKismetMathLibrary::ClassIsChildOf(OwnerPawn->GetEquippedWeapon()->GetClass(), ABC_RifleWeapon::StaticClass()))
	{
		EquippedWeaponType = EWeaponType::WPN_RIFLE;
	}
	else if (UKismetMathLibrary::ClassIsChildOf(OwnerPawn->GetEquippedWeapon()->GetClass(), ABC_HandgunWeapon::StaticClass()))
	{
		EquippedWeaponType = EWeaponType::WPN_HANDGUN;
	}
	else if (UKismetMathLibrary::ClassIsChildOf(OwnerPawn->GetEquippedWeapon()->GetClass(), ABC_ShotgunWeapon::StaticClass()))
	{
		EquippedWeaponType = EWeaponType::WPN_SHOTGUN;
	}
	else if (UKismetMathLibrary::ClassIsChildOf(OwnerPawn->GetEquippedWeapon()->GetClass(), ABC_SniperWeapon::StaticClass()))
	{
		EquippedWeaponType = EWeaponType::WPN_SNIPER;
	}
	else if (UKismetMathLibrary::ClassIsChildOf(OwnerPawn->GetEquippedWeapon()->GetClass(), ABC_MeleeWeapon::StaticClass()))
	{
		EquippedWeaponType = EWeaponType::WPN_MELEE;
	}

}




