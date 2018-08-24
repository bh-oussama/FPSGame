// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BC_Weapon.generated.h"

class USkeletalMeshComponent;

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

	UPROPERTY(EditDefaultsOnly, Category = "Baisc")
		float Damage = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Baisc")
		USkeletalMeshComponent* Mesh = nullptr;
	
	
};
