// Fill out your copyright notice in the Description page of Project Settings.

#include "BC_Projectile.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABC_Projectile::ABC_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionProfileName(FName("BlockAll"));
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetupAttachment(SphereComponent);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Trace = CreateDefaultSubobject<UParticleSystemComponent>(FName("Trace"));
	Trace->SetupAttachment(SphereComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("MovementComponent"));
	MovementComponent->InitialSpeed = Velocity;
	MovementComponent->ProjectileGravityScale = 0;

	
}

// Called when the game starts or when spawned
void ABC_Projectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(2);
	SphereComponent->OnComponentHit.AddDynamic(this, &ABC_Projectile::OnHit);
}

// Called every frame
void ABC_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABC_Projectile::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *GetName(), *OtherActor->GetName());
}

