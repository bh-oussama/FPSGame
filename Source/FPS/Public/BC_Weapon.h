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
	
	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float FireRange = 10000;

	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float Damage = 25;

	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float FireRate = 500;

	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		TSubclassOf<ABC_Projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		FName MuzzleName = "Muzzle";

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void AutoFire();

	UFUNCTION(BlueprintCallable)
		void StopFire();

	UFUNCTION(BlueprintCallable)
		void SetOwnerPawn(ACharacter* const OwnerCharacter);

private:
	ACharacter * OwnerPawn = nullptr;
	FTimerHandle Timer;
	/*
	FTimerManager TimerManager = FTimerManager();
	FTimerHandle Timer;
	*/



	
};
