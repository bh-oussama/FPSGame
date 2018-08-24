// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrollingGuard.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{


	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();

	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrollingPoints = PatrolRoute->GetPatrolPoints();

	if(PatrollingPoints.Num() == 0) 
	{ 
		auto PawnName = ControlledPawn->GetName();
		UE_LOG(LogTemp, Error, TEXT("Patrol %s: missing Patrol Points"), *PawnName);
		return EBTNodeResult::Failed;	
	}

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	if (firstWay)
	{
		Index++;
	}
	else 
	{
		Index--;
	}

	if (Index == PatrollingPoints.Num() - 1 || Index == -1)
		firstWay = !firstWay;

	Index = FMath::Clamp<int>(Index, 0, PatrollingPoints.Num()-1);
	// setting next waypoint.
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrollingPoints[Index]);
	
	// setting new index.
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);

	return EBTNodeResult::Succeeded;
}


