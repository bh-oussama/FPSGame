// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPS_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UTPS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	// Represents the Player Right Input.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float MoveRight = 0;

	// Represents the Player forward Input.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float MoveForward = 0;

	// Pitch offset between pawn rotation and control rotation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float PitchOffset = 0;

	// Is aiming.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float bIsAiming = 0;

	// Is firing.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
		float bIsFiring= 0;

	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
private:
	//class ACharacter OwnerPawn;
};
