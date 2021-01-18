// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BC_Character.h"
#include "FPS_Char.generated.h"


UCLASS()
class FPS_API AFPS_Char : public ABC_Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPS_Char();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	// finds all camera components in the class and activates the next one after deactivating the current.
	UFUNCTION(BlueprintCallable)
		void SwitchCamera();

	/** returns forward movement value which is set via input. */
	//UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual float GetForwardMovement() override;

	/** returns right movement value which is set via input. */
	//UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual float GetRightMovement() override;
	
	void OnCrouchPressed();
	void OnCrouchRelease();
	void Run();
	void RunRelease();
	void Walk();
	void WalkRelease();

public:	

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* ThirdPersonSpringArmComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* ThirdPersonCameraComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		bool bToggleCrouch = true;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		bool bToggleRun = true;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		bool bToggleWalk = false;

	/** Hand's weapon socket */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		FName HandGripSocketName = "GripPoint";


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	/** sets the equipped weapon. */
	//UFUNCTION(BlueprintCallable)
		virtual void SetEquippedWeapon(class ABC_Weapon* WeaponToEquip) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
