// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BC_Projectile.generated.h"

UCLASS()
class FPS_API ABC_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABC_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly)
		float Damage = 25;

	UPROPERTY(EditDefaultsOnly)
		float Velocity = 30000;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* MovementComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Trace = nullptr;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	
};
