// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BC_Weapon.h"
#include "BC_ShotgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ABC_ShotgunWeapon : public ABC_Weapon
{
	GENERATED_BODY()
	
protected:
	// right definition to spawn more projectiles
	virtual void SpawnProjectiles(FVector TargetLocation) override;
	
	
	
};
