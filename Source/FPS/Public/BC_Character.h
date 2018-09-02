// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BC_Character.generated.h"

class ABC_Weapon;

UENUM(BlueprintType)
enum class ECharacterMovementStatus : uint8
{
	ECMS_Walking		= 0	UMETA(DisplayName="Walking"),
	ECMS_Jogging		= 1	UMETA(DisplayName = "Jogging"),
	ECMS_Running		= 2	UMETA(DisplayName = "Running"),
	ECMS_Crouching		= 3	UMETA(DisplayName = "Crouching"),
};

UENUM(BlueprintType)
enum class EWeaponPose : uint8
{
	EWP_Low				= 0	UMETA(DisplayName = "Low"),
	EWP_Ready			= 1	UMETA(DisplayName = "Ready"),
	EWP_Ironsight		= 2	UMETA(DisplayName = "Ironsight"),
};

UCLASS()
class FPS_API ABC_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ABC_Character();

	/** Aims*/
	virtual void Aim();

	/** Stops aim*/
	virtual void StopAim();

	/** Fires a projectile. */
	virtual void Fire();

	/** Reloads equipped weapon.*/
	virtual void Reload();

	/** Switches weapon fire mode. */
	virtual void SwitchWeaponMode();

	/** Stops Firing projectiles. */
	virtual void StopFire();

	/** returns forward movement value calculated from velocity. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual float GetForwardMovement();

	/** returns right movement value calculated from velocity. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual float GetRightMovement();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		float AnimationPlayRate = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float RunMultiplier = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float GeneralMovementMultiplier = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float ReloadMultiplier = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		TSubclassOf<ABC_Weapon> PrimaryWPNClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		FName PrimaryWPNSocketName = "Socket_PrimaryWPN";
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		FName EquippedWPNSocketName = "GripPoint";

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		TSubclassOf<ABC_Weapon> SecondaryWPNClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		FName SecondaryWPNSocketName = "Socket_SecondaryWPN";

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		TSubclassOf<ABC_Weapon> MeleeWPNClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		FName MeleeWPNSocketName = "Socket_MeleeWPN";

	/** returns equipped weapon. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		ABC_Weapon* GetEquippedWeapon();

	/** sets the equipped weapon. */
	UFUNCTION(BlueprintCallable)
		void SetEquippedWeapon(ABC_Weapon* WeaponToEquip);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsFiring();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ECharacterMovementStatus GetMovementStatus();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		EWeaponPose GetWeaponPose();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
		float MoveRightValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
		float MoveForwardValue;

	// true if the pawn is lowering weapon.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		ECharacterMovementStatus MovementStatus = ECharacterMovementStatus::ECMS_Jogging;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		EWeaponPose WeaponPose = EWeaponPose::EWP_Low;

private:
	ABC_Weapon * PrimaryWPN = nullptr;
	ABC_Weapon* SecondaryWPN = nullptr;
	ABC_Weapon* MeleeWPN = nullptr;
	ABC_Weapon* EquippedWPN = nullptr;

};
