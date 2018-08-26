// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
class FPS_API DebugDrawer
{
public:
	DebugDrawer();
	~DebugDrawer();

	static void DrawDebugLineTrace(UObject* ContestWorld, const FHitResult& HitResult, FVector LineStart, const FVector LineEnd, float Duration = 2);
};
