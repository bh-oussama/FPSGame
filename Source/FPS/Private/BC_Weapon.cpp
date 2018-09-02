// Fill out your copyright notice in the Description page of Project Settings.

#include "BC_Weapon.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "BC_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "FPS_Char.h"
#include "Kismet/GameplayStatics.h"
#include "DebugDrawer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABC_Weapon::ABC_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh"));
	Mesh->SetupAttachment(Root);

	// sets the default fire mode and if there is no one set by default logs it.
	if (FireModes.Num() > 0)
	{
		CurrentFireMode = FireModes[0];
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The Weapon %s's FireModes array is empty."), *GetName());
	}

	CurrentAccuracy = Accuracy;
	CurrentAmmoInClip = ClipMaxSize;
	CurrentAmmoInPocket = MaxAmmoInPocket;
}

// Called when the game starts or when spawned
void ABC_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABC_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// clear BurstTimer 
	if (BulletCount >= 3)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		BulletCount = 0;
		bIsFiring = false;
	}

	if (!bIsFiring)
	{
		CurrentAccuracy = FMath::FInterpTo(CurrentAccuracy, Accuracy, DeltaTime, Control *4);
		BulletCount = 0;
	}

}

void ABC_Weapon::OnFire(FVector TargetLocation)
{
	if (Camera == NULL && TargetLocation.Equals(FVector(0,0,-100000)))
	{
		UE_LOG(LogTemp, Error, TEXT("The Weapon %s's Camera is not set."), *GetName());
		return;
	}
	if (!bCanFire || bIsReloading) { return; }
	switch (CurrentFireMode)
	{
	case EWeaponFireMode::FIRE_AUTO:
	{
		AutoFire(TargetLocation);
		break;
	}
	case EWeaponFireMode::FIRE_BURST:
	{
		BurstFire(TargetLocation);
		break;
	}
	case EWeaponFireMode::FIRE_MANUAL:
	{
		Fire(TargetLocation);
		break;
	}
	}
}

void ABC_Weapon::OnReload()
{
	if (
		// clip not full
		(CurrentAmmoInClip <= ClipMaxSize) ||
		// if there is a bullet in bolt and clip is a -1 bullet
		(bHasBulletInBolt && CurrentAmmoInClip == ClipMaxSize)
		)
	{
		if (CurrentAmmoInPocket > 0)
		{
			Reload();
		}
		else
		{
			bHasBulletInBolt = false;
			// no bullets are left 
			if (CurrentAmmoInClip == 0)
			{
				if (EmptyClipSound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, EmptyClipSound, GetActorLocation());
				}
			}
			return;
		}
	}
	
}

void ABC_Weapon::Reload()
{
	bCanFire = false;
	bIsReloading = true;

	ABC_Character* Pawn = Cast<ABC_Character>(OwnerPawn);
	UAnimMontage* ReloadAnimationToPlay;
	if (Pawn->GetWeaponPose() == EWeaponPose::EWP_Ironsight || Pawn->GetWeaponPose() == EWeaponPose::EWP_Ready)
	{
		ReloadAnimationToPlay = ReloadAnimation_Ironsight;
	}
	else
	{
		ReloadAnimationToPlay = ReloadAnimation_Relaxed;
	}
	{
		// try and play a firing animation if specified
		if (ReloadAnimationToPlay != NULL)
		{
			// Get the animation object for the arms mesh
			if (OwnerPawn->GetMesh())
			{
				UAnimInstance* AnimInstance3P = OwnerPawn->GetMesh()->GetAnimInstance();
				if (AnimInstance3P != NULL)
				{
					AnimInstance3P->Montage_Play(ReloadAnimationToPlay, Pawn->ReloadMultiplier);
				}

			}
			if (Cast<AFPS_Char>(OwnerPawn)->Mesh1P)
			{
				UAnimInstance* AnimInstance1P = Cast<AFPS_Char>(OwnerPawn)->Mesh1P->GetAnimInstance();
				if (AnimInstance1P != NULL)
				{
					AnimInstance1P->Montage_Play(ReloadAnimationToPlay, Pawn->ReloadMultiplier);
				}
			}
		}
	}
	
	// wait for ReloadAnimationToPlay in Ammo time, update ammo info and end reloading
	UKismetSystemLibrary::Delay(GetWorld(), ReloadTimeInAnimation, FLatentActionInfo());
	
	CurrentAmmoInPocket += CurrentAmmoInClip;
	CurrentAmmoInClip = FMath::Min<float>(CurrentAmmoInPocket, ClipMaxSize + ((bHasBulletInBolt) ? 1 : 0) );
	CurrentAmmoInPocket -= CurrentAmmoInClip;
	if (ReloadAnimationToPlay != NULL)
	{
		UKismetSystemLibrary::Delay(GetWorld(), ReloadAnimationToPlay->GetPlayLength() - ReloadTimeInAnimation, FLatentActionInfo());
	}

	bIsReloading = false;
	bHasBulletInBolt = false;
	bCanFire = true;
	
	// reload if the clip is empty.
	if (CurrentAmmoInClip == 0) OnReload();

}

void ABC_Weapon::Fire(FVector TargetLocation)
{
	if (CurrentAmmoInClip == 0)
	{
		Reload();
		return;
	}
	bIsFiring = true;
	bCanFire = false;
	BulletCount++;
	
	// try and fire a projectile
	
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		if (OwnerPawn->GetMesh())
		{
			UAnimInstance* AnimInstance3P = OwnerPawn->GetMesh()->GetAnimInstance();
			if (AnimInstance3P != NULL)
			{
				AnimInstance3P->Montage_Play(FireAnimation, 1.f);
			}
		}
			
		UAnimInstance* AnimInstance1P = Cast<AFPS_Char>(OwnerPawn)->Mesh1P->GetAnimInstance();
		if (AnimInstance1P != NULL)
		{
			AnimInstance1P->Montage_Play(FireAnimation, 1.f);
		}
	}
	
	bHasBulletInBolt = (--CurrentAmmoInClip > 0) ? true : false;
	
	// Add pitch rotation to the player because of lack of control.
	OwnerPawn->AddControllerPitchInput((1 - Control) * (-2));

	UKismetSystemLibrary::Delay(GetWorld(), 1 / (FireRate / 60), FLatentActionInfo());
	// post-fire update
	CurrentAccuracy = FMath::Clamp<float>(CurrentAccuracy - (1 - Control) * BulletCount, 0, 100);
	bCanFire = true;
}

void ABC_Weapon::SpawnProjectiles(FVector TargetLocation)
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			float InversedAccuracy = 1 - CurrentAccuracy;

			const FVector SpawnLocation = Mesh->GetSocketLocation(MuzzleName);

			FRotator SpawnRotation;
			if (TargetLocation.Equals(FVector(0, 0, -100000)))
			{
				// when the player is firing.

				FRotator ViewRotation = Camera->GetComponentRotation() + FRotator(
					0,
					UKismetMathLibrary::RandomFloatInRange(-InversedAccuracy * BulletCount / 2, InversedAccuracy * BulletCount / 2),
					UKismetMathLibrary::RandomFloatInRange(-InversedAccuracy * BulletCount / 2, InversedAccuracy * BulletCount / 2)
				);

				FHitResult HitResult;
				FCollisionQueryParams params = FCollisionQueryParams(FName("Trace"), true);
				params.AddIgnoredActor(OwnerPawn);

				GetWorld()->LineTraceSingleByChannel(
					HitResult,
					Camera->GetComponentLocation(),
					Camera->GetComponentLocation() + UKismetMathLibrary::GetForwardVector(ViewRotation) * FireRange,
					ECollisionChannel::ECC_Camera,
					params
				);
				SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, HitResult.ImpactPoint);
			}
			else
			{
				// usually when the ai is firing.
				FVector FiringDirection = (TargetLocation - SpawnLocation).GetSafeNormal();
				FRotator FiringRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, SpawnLocation + FiringDirection) + FRotator(
					0,
					UKismetMathLibrary::RandomFloatInRange(-InversedAccuracy * BulletCount / 2, InversedAccuracy * BulletCount / 2),
					UKismetMathLibrary::RandomFloatInRange(-InversedAccuracy * BulletCount / 2, InversedAccuracy * BulletCount / 2)
				);

				SpawnRotation = FiringRotation;
			}

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			auto ProjectileSpawned = World->SpawnActor<ABC_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (ProjectileSpawned)
			{
				ProjectileSpawned->SphereComponent->IgnoreActorWhenMoving(OwnerPawn, true);
			}

		}
		else
		{
			// end if GetWorld() returned NULL value.
			return;
		}
	}
	else
	{
		// end if projectile class is not set.
		return;
	}

}


void ABC_Weapon::AutoFire(FVector TargetLocation)
{
	Fire(TargetLocation);
	TimerDelegate = FTimerDelegate::CreateUObject(this, &ABC_Weapon::Fire, TargetLocation);

	GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, 1/(FireRate/60), true);
}

void ABC_Weapon::BurstFire(FVector TargetLocation)
{
	Fire(TargetLocation);
	BurstTimerDelegate = FTimerDelegate::CreateUObject(this, &ABC_Weapon::Fire, TargetLocation);

	GetWorld()->GetTimerManager().SetTimer(BurstTimer, BurstTimerDelegate, 1 / (FireRate / 60), true);
}

void ABC_Weapon::StopFire()
{
	
	if (CurrentFireMode == EWeaponFireMode::FIRE_AUTO)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		bIsFiring = false;
	}
		
}

void ABC_Weapon::SetOwnerPawn(ACharacter* const OwnerCharacter)
{
	OwnerPawn = OwnerCharacter;
}

void ABC_Weapon::SetToNextFireModePossible()
{
	if (FireModes.Num() == 1) { return; }
	for (int i = 0; i < FireModes.Num(); i++) 
	{
		if (FireModes[i] != CurrentFireMode) { continue; }
		CurrentFireMode = (i + 1 < FireModes.Num()) ? FireModes[i + 1] : FireModes[0];
		return;
	}
	CurrentFireMode = FireModes[0];
}

int32 ABC_Weapon::GetCurrentAmmoInClip()
{
	return CurrentAmmoInClip;
}

int32 ABC_Weapon::GetCurrentAmmoInPocket()
{
	return CurrentAmmoInPocket;
}

EWeaponFireMode ABC_Weapon::GetCurrentFireMode()
{
	return CurrentFireMode;
}

bool ABC_Weapon::IsFiring()
{
	return bIsFiring;
}

bool ABC_Weapon::IsReloading()
{
	return bIsReloading;
}

void ABC_Weapon::SetCamera(UCameraComponent* CameraToSet)
{
	Camera = CameraToSet;
}

