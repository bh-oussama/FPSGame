// Fill out your copyright notice in the Description page of Project Settings.

#include "GM.h"
#include "CustomHUD.h"
#include "UObject/ConstructorHelpers.h"


AGM::AGM()
{

	static ConstructorHelpers::FClassFinder<APawn> PawnFinder(TEXT("/Game/BP/Characters/BP_FPS_Char"));
	DefaultPawnClass = PawnFinder.Class;

	HUDClass = ACustomHUD::StaticClass();
}


