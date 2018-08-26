// Fill out your copyright notice in the Description page of Project Settings.

#include "BC_Weapon.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
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

// Sets default values
ABC_Weapon::ABC_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh"));
	Mesh->SetupAttachment(Root);

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

}



void ABC_Weapon::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Gun is firing."));
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			Mesh->GetSocketLocation(MuzzleName);
			
			const FVector SpawnLocation = Mesh->GetSocketLocation(MuzzleName);;

			const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			const FVector2D ViewportCenter = FVector2D(ViewportSize.X/2, ViewportSize.Y/2);

			

			FVector AimPoint, AimDirection;

			UGameplayStatics::DeprojectScreenToWorld(
				Cast<APlayerController>(OwnerPawn->Controller),
				ViewportCenter,
				AimPoint,
				AimDirection
			);

			FHitResult HitResult;
			FCollisionQueryParams params = FCollisionQueryParams(FName("Trace"), true);
			params.AddIgnoredActor(OwnerPawn);

			GetWorld()->LineTraceSingleByChannel(
				HitResult,
				AimPoint,
				AimPoint + AimDirection * FireRange,
				ECollisionChannel::ECC_Camera,
				params
			);

			/* incomment to draw debug of projectile line trace.
					DebugDrawer::DrawDebugLineTrace(
						GetWorld(),
						HitResult,
						AimPoint,
						AimPoint + AimDirection * FireRange,
						5
					);
			*/

			
			

			const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, HitResult.ImpactPoint);

			

			UE_LOG(LogTemp, Warning, TEXT("ViewportCenter: %s"), *ViewportCenter.ToString());
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// spawn the projectile at the muzzle
			auto ProjectileSpawned = World->SpawnActor<ABC_Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			ProjectileSpawned->SphereComponent->IgnoreActorWhenMoving(OwnerPawn, true);
		}
	}
	
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance3P = OwnerPawn->GetMesh()->GetAnimInstance();
		UAnimInstance* AnimInstance1P = Cast<AFPS_Char>(OwnerPawn)->Mesh1P->GetAnimInstance();
		if (AnimInstance3P != NULL)
		{
			AnimInstance3P->Montage_Play(FireAnimation, 1.f);
		}
		if (AnimInstance1P != NULL)
		{
			AnimInstance1P->Montage_Play(FireAnimation, 1.f);
		}
	}

}

void ABC_Weapon::AutoFire()
{
	Fire();
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ABC_Weapon::Fire, 1/(FireRate/60), true);
}

void ABC_Weapon::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void ABC_Weapon::SetOwnerPawn(ACharacter* const OwnerCharacter)
{
	OwnerPawn = OwnerCharacter;
}