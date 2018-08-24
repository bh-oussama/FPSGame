// Fill out your copyright notice in the Description page of Project Settings.

#include "BC_EnemyChar.h"
#include "BC_Weapon.h"
#include "Engine/World.h"

// Sets default values
ABC_EnemyChar::ABC_EnemyChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
// Called when the game starts or when spawned
void ABC_EnemyChar::BeginPlay()
{
	Super::BeginPlay();
	
	//spawning the wpn.
	PrimaryWPN = GetWorld()->SpawnActor<ABC_Weapon>(Weapon);

	// Attaching wpn to the character.
	if (ensure(Weapon))
	{
		PrimaryWPN->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), FName("Socket_PrimaryWPN"));
	}
}


// Called every frame
void ABC_EnemyChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABC_EnemyChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

