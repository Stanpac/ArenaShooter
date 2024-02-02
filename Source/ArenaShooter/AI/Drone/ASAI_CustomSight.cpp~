// Fill out your copyright notice in the Description page of Project Settings.


#include "ASAI_CustomSight.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

class AAIController;
// Sets default values for this component's properties
UASAI_CustomSight::UASAI_CustomSight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UASAI_CustomSight::BeginPlay()
{
	Super::BeginPlay();
	AAIController* AIController = Cast<AAIController>(GetOwner());
	check(AIController);
	m_BlackboardComponent =  AIController->GetBlackboardComponent();
	m_BlackboardComponent->SetValueAsObject(TEXT("Character"), UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}


// Called every frame
void UASAI_CustomSight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Detect();
	IsTargetInRange();
}

void UASAI_CustomSight::Detect()
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = m_Character->GetActorLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor( GetOwner());
	QueryParams.AddIgnoredActor( m_Character);
	QueryParams.bTraceComplex = true;
	FHitResult OutHitResult;
	
	// Perform the raycast
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	// Optionally, draw the raycast result for debugging
#if WITH_EDITOR
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, bHit ? FColor::Red : FColor::Green, false, 5.0f, 0, 1.0f);
#endif
	
	if (bHit)
	{
		m_BlackboardComponent->SetValueAsBool(TEXT("IsTargetInSight"), false);
		DrawDebugSphere(GetWorld(), OutHitResult.Location, 10.0f, 12, FColor::Red, false, 5.0f);
		m_IsInSight = false;
	}
	else
	{
		m_BlackboardComponent->SetValueAsBool(TEXT("IsTargetInSight"), true);
		m_IsInSight = true;
	}
}

void UASAI_CustomSight::IsTargetInRange()
{
	const float Distance = FVector::Distance(GetOwner()->GetActorLocation(),  m_Character->GetActorLocation());
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), m_DetectionRange, 12, FColor::Yellow,false, .01f);
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), m_LoseSightRange, 12, FColor::Emerald,false,  .01f);
	if(m_IsInRange && Distance > m_LoseSightRange)
	{
		m_IsInRange = false;
		m_BlackboardComponent->SetValueAsBool(TEXT("IsTargetInRange"), false);
	}
	else if(!m_IsInRange && Distance < m_DetectionRange)
	{
		m_IsInRange = true;
		m_BlackboardComponent->SetValueAsBool(TEXT("IsTargetInRange"), true);
	}
}

