// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BC_Weapon.generated.h"

class USkeletalMeshComponent;
class ABC_Projectile;
class FTimerManager;
class ACharacter;

struct FTimerHandle;
//struct FTimerDelegate;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WPN_HANDGUN		= 0	UMETA(DisplayName = "Handgun"),
	WPN_RIFLE		= 1	UMETA(DisplayName = "Rifle"),
	WPN_SHOTGUN		= 2	UMETA(DisplayName = "Shotgun"),
	WPN_SNIPER		= 3	UMETA(DisplayName = "Sniper"),
	WPN_MELEE		= 4	UMETA(DisplayName = "Knife"),
};

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	FIRE_MANUAL		= 0	UMETA(DisplayName = "Manual"),
	FIRE_AUTO		= 1	UMETA(DisplayName = "Auto"),
	FIRE_BURST		= 2	UMETA(DisplayName = "Burst"),
};
// TODO delete this enum if is has no use.
UENUM(BlueprintType)
enum class EWeaponFireTarget : uint8 
{
	TARGET_BYCAMERA			= 0	UMETA(DisplayName = "By Camera"),
	TARGET_BYLOCATION		= 0	UMETA(DisplayName = "By Location"),
};


UCLASS()
class FPS_API ABC_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABC_Weapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent* Mesh = nullptr;

	/** List of fire modes supported by the weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		TArray<EWeaponFireMode> FireModes;

	/** Max Size of a clip. */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		int32 ClipMaxSize = 30;

	/** Max ammo in pocket. */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		int32 MaxAmmoInPocket = 120;

	/** The distance that the bullet can travel */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float FireRange = 10000;

	/** Max Dammage delt by the weapon when shooting */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float Damage = 25;

	/** The fire rate of the weapon in 1 minute */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float FireRate = 500;

	/** The accuracy of the weapon (how precise will be firing projectiles, the 1st fire is always 100% precise and accuracy falls down while shooting and with the weapon's control the accuracy is reset to default, don't set to 0 or 1 but to a value in between).*/
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float Accuracy = 0.35;

	/** how much this weapon can be controlled (don't set to 0 or 1 but to a value in between). */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float Control = 0.30;

	/** Projectile class */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		TSubclassOf<ABC_Projectile> ProjectileClass;

	/** The name of the socket on the mesh which presents where to spawn projectile when shooting. */
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		FName MuzzleName = "Muzzle";

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** Sound to play when the clip is empty. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* EmptyClipSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** AnimMontage to play when reloading */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation_Relaxed;
	
	/** AnimMontage to play when reloading */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation_Ironsight;

	/** time in ReloadAnimation where the weapon should update ammo info (manually set). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float ReloadTimeInAnimation = 0;

	/** This function is the one that should be called to fire (it fires relatively to the current fire mode of the weapon). */
	UFUNCTION(BlueprintCallable)
		void OnFire(FVector TargetLocation = FVector(0,0,-100000));

	/** Reload the gun if possible (call when want to reload). */
	UFUNCTION(BlueprintCallable)
		void OnReload();


	/** Reload the gun. */
	UFUNCTION(BlueprintCallable)
		void Reload();

	/** Fires once. */
	UFUNCTION(BlueprintCallable)
		void Fire(FVector TargetLocation);

	/** Auto Fires. */
	UFUNCTION(BlueprintCallable)
		void AutoFire(FVector TargetLocation);

	/** Burst Fires (3 times). */
	UFUNCTION(BlueprintCallable)
		void BurstFire(FVector TargetLocation);

	/** Stops Firing (used to clear timer which is set in AutoFire mode). */
	UFUNCTION(BlueprintCallable)
		void StopFire();

	/** Sets a reference in the weapon to it's owning character. this should be called each time the weapon is equipped with a player or ai. */
	UFUNCTION(BlueprintCallable)
		void SetOwnerPawn(ACharacter* const OwnerCharacter);

	/** Calling this function will set the weapon to the next fire mode supported. */
	UFUNCTION(BlueprintCallable)
		void SetToNextFireModePossible();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentAmmoInClip();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetCurrentAmmoInPocket();

	/** returns weapon's current fire mode. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		EWeaponFireMode GetCurrentFireMode();

	/** returns if the weapon is firing or not. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsFiring();

	/** returns if the weapon is reloading or not. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool IsReloading();

	/** sets the camera which is used to calculate firing target point. */
	UFUNCTION(BlueprintCallable)
		void SetCamera(class UCameraComponent* CameraToSet);

protected:
	// Spawns a projectile. 
	virtual void SpawnProjectiles(FVector TargetLocation);

private:
	ACharacter * OwnerPawn = nullptr;
	FTimerDelegate TimerDelegate, BurstTimerDelegate;
	FTimerHandle Timer;
	FTimerHandle BurstTimer;
	int BulletCount = 0;
	bool bCanFire = true;
	bool bIsFiring = false;
	bool bIsReloading = false;
	EWeaponFireMode CurrentFireMode;
	class UCameraComponent* Camera;
	float CurrentAccuracy;
	bool bHasBulletInBolt = false;
	int32 CurrentAmmoInClip, CurrentAmmoInPocket;
};
