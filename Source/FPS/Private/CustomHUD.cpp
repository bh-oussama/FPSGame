// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ACustomHUD::ACustomHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WeaponInfoWidgetFinder(TEXT("/Game/Widgets/W_WeaponInfo"));
	if (WeaponInfoWidgetFinder.Succeeded())
	{
		WidgetsToCreate.Add(WeaponInfoWidgetFinder.Class);
	}
}

void ACustomHUD::BeginPlay()
{
	for (int i = 0; i < WidgetsToCreate.Num(); i++)
	{
		auto CreatedWidget = CreateWidget<UUserWidget>(GetGameInstance(), WidgetsToCreate[i]);
		if (CreatedWidget)
		{
			CreatedWidget->SetVisibility(ESlateVisibility::Visible);
			CreatedWidget->AddToViewport();
		}
	}
}


