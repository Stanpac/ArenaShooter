// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaShooter/AI/Nodes/ASBTTask_RotateToFaceBBEntry.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Services/BTService_DefaultFocus.h"

UASBTTask_RotateToFaceBBEntry::UASBTTask_RotateToFaceBBEntry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Precision(10.f)
{
	NodeName = "AS_Rotate To Face BB Entry";
	INIT_TASK_NODE_NOTIFY_FLAGS();
	
	// accept only actors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UASBTTask_RotateToFaceBBEntry, BlackboardKey), AActor::StaticClass());
}

namespace
{
	FORCEINLINE_DEBUGGABLE FVector::FReal CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
	{
		return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
			? 1.f
			: VectorA.CosineAngle2D(VectorB);
	}
}

void UASBTTask_RotateToFaceBBEntry::PostInitProperties()
{
	Super::PostInitProperties();
	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

void UASBTTask_RotateToFaceBBEntry::PostLoad()
{
	Super::PostLoad();
	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

EBTNodeResult::Type UASBTTask_RotateToFaceBBEntry::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr || AIController->GetPawn() == nullptr) {
		return EBTNodeResult::Failed;
	}

	FBTFocusMemory* MyMemory = (FBTFocusMemory*)NodeMemory;
	check(MyMemory);
	MyMemory->Reset();

	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	const FVector PawnLocation = Pawn->GetActorLocation();
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	
	UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	AActor* ActorValue = Cast<AActor>(KeyValue);

	if (ActorValue != nullptr) {
		const FVector::FReal AngleDifference = CalculateAngleDifferenceDot(Pawn->GetActorForwardVector(), (ActorValue->GetActorLocation() - PawnLocation));
		
		if (AngleDifference >= PrecisionDot) {
			Result = EBTNodeResult::Succeeded;
			
		} else {
			AIController->SetFocus(ActorValue, EAIFocusPriority::Gameplay);
			MyMemory->FocusActorSet = ActorValue;
			MyMemory->bActorSet = true;
			Result = EBTNodeResult::InProgress;
		}
	}
	return Result;
}
