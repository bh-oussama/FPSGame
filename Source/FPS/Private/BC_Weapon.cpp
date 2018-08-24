// Fill out your copyright notice in the Description page of Project Settings.

#include "BC_Weapon.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

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

