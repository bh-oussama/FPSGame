// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class FPS_API ACustomHUD : public AHUD
{
	GENERATED_BODY()

	ACustomHUD();
		
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UUserWidget>> WidgetsToCreate;
	
	virtual void BeginPlay() override;
	
};
