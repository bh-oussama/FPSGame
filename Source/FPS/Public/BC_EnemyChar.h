// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BC_EnemyChar.generated.h"

class ABC_Weapon;

UCLASS()
class FPS_API ABC_EnemyChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABC_EnemyChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		float Health = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Basic")
		TSubclassOf<ABC_Weapon> Weapon;
	
private:
	ABC_Weapon* PrimaryWPN;

	
};
