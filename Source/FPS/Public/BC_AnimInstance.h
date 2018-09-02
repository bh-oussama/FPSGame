// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BC_Character.h"
#include "BC_Weapon.h"
#include "Animation/AnimInstance.h"
#include "BC_AnimInstance.generated.h"


UCLASS()
class FPS_API UBC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:	
	// Represents the Player Right Input.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	float MoveRight = 0;

	// Represents the Player forward Input.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float MoveForward = 0;

	// Pitch offset between pawn rotation and control rotation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float PitchOffset = 0;
	
	// Yaw offset between pawn rotation and control rotation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float YawOffset = 0;

	// true if the pawn is lowering weapon.
	UPROPERTY( BlueprintReadWrite, Category = Weapon)
		ECharacterMovementStatus MovementStatus = ECharacterMovementStatus::ECMS_Jogging;

	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		EWeaponPose WeaponPose = EWeaponPose::EWP_Low;

	// Is firing.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		bool bIsFiring = false;

	// Is Reloading.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		bool bIsReloading = false;

	// Weapon Type
	UPROPERTY(BlueprintReadWrite, Category = "Basic")
		EWeaponType EquippedWeaponType = EWeaponType::WPN_HANDGUN;

	// Weapon fire mode
	UPROPERTY(BlueprintReadWrite, Category = "Basic")
		EWeaponFireMode WeaponFireMode = EWeaponFireMode::FIRE_MANUAL;

	UPROPERTY(BlueprintReadWrite, Category = "Basic")
		float AnimationPlayRate = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Basic")
		float RunMultiplier = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Basic")
		float GeneralMovementMultiplier = 1;

	UPROPERTY(BlueprintReadWrite, Category = "Basic")
		float ReloadMultiplier = 1;




	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	ABC_Character * OwnerPawn;
};
