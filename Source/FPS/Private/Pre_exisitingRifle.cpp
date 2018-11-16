// Fill out your copyright notice in the Description page of Project Settings.

#include "Pre_exisitingRifle.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"


APre_exisitingRifle::APre_exisitingRifle()
	:Super()
{
	//Super::ABC_Weapon();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(TEXT("/Game/Assets/Weapons/FPWeapon/Mesh/SK_FPGun"));
	Mesh->SetSkeletalMesh(MeshFinder.Object);
	Mesh1P->SetSkeletalMesh(MeshFinder.Object);
	FireRange = 10000;
	Damage = 30;
	FireRate = 500;
	Control = 0.7;
	Accuracy = 0.45;
	MuzzleName = "Muzzle";

	FireModes = { EWeaponFireMode::FIRE_AUTO, EWeaponFireMode::FIRE_MANUAL, EWeaponFireMode::FIRE_BURST };
	SetToNextFireModePossible();

	static ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileClassFinder(TEXT("/Game/BP/Weapons/Projectiles/FP_GunProjectile"));
	ProjectileClass = (UClass*)ProjectileClassFinder.Object->GeneratedClass;
	
	static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundFinder(TEXT("/Game/Assets/Characters/UE4_Mannequin/FPS/Audio/FirstPersonTemplateWeaponFire02.FirstPersonTemplateWeaponFire02"));
	FireSound = FireSoundFinder.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimationFinder(TEXT("/Game/Assets/Characters/UE4_Mannequin/FPS/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage"));
	FireAnimation = FireAnimationFinder.Object;
	

}


