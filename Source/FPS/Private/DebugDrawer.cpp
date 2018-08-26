// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugDrawer.h"
#include "Kismet/KismetSystemLibrary.h"


DebugDrawer::DebugDrawer()
{
}

DebugDrawer::~DebugDrawer()
{
}

void DebugDrawer::DrawDebugLineTrace(UObject* ContestWorld, const FHitResult& HitResult, FVector LineStart, const FVector LineEnd, float Duration)
{
	const FVector HitLocation = HitResult.ImpactPoint;
	
	UKismetSystemLibrary::DrawDebugLine(
		ContestWorld,
		LineStart,
		HitLocation,
		FLinearColor(148, 232, 12),
		Duration,
		0.5
	);

	UKismetSystemLibrary::DrawDebugSphere(
		ContestWorld,
		HitLocation,
		12,
		12,
		FLinearColor(0, 127, 232),
		Duration,
		0.5
	);

	UKismetSystemLibrary::DrawDebugLine(
		ContestWorld,
		HitLocation,
		LineEnd,
		FLinearColor(255, 47, 0),
		Duration,
		0.5
	);

}
