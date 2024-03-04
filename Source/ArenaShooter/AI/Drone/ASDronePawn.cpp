#include "ASDronePawn.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "GameFramework/Character.h"

AASDronePawn::AASDronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));

	m_WeaponComponent = CreateDefaultSubobject<UASWeaponComponent>(TEXT("WeaponComponent"));
}

void AASDronePawn::BeginPlay()
{
	Super::BeginPlay();
	m_Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	check(m_Player);
	m_CurrentBehaviour = EDroneBehaviour::CHASING;
	OnBehaviourStateEnter();
}

void AASDronePawn::LookAtPlayer(float DeltaTime)
{
	const FVector LookAtLocation = m_Player->GetActorLocation();
	const FVector PawnLocation = GetActorLocation();
	const FRotator LookAtRotation = (LookAtLocation - PawnLocation).Rotation();
	SetActorRotation(LookAtRotation);
}

void AASDronePawn::ChangeBehaviour(EDroneBehaviour newBehaviour)
{
	OnBehaviourStateExit();
	m_CurrentBehaviour = newBehaviour;
	OnBehaviourStateExit();
}

void AASDronePawn::OnBehaviourStateEnter()
{
	switch(m_CurrentBehaviour)
	{
		case EDroneBehaviour::CHASING:
			break;
		case EDroneBehaviour::SHOOTING:
			break;
		case EDroneBehaviour::TELEPORTING:
			m_TeleportationTimer = m_TeleportationDuration;
			break;
	}
}

void AASDronePawn::OnBehaviourStateExit()
{
	switch(m_CurrentBehaviour)
	{
		case EDroneBehaviour::CHASING:
			break;
		case EDroneBehaviour::SHOOTING:
			break;
		case EDroneBehaviour::TELEPORTING:
			break;
	}
}

void AASDronePawn::OnBehaviourStateTick(float DeltaTime)
{
	switch(m_CurrentBehaviour)
	{
		case EDroneBehaviour::CHASING:
			break;
		case EDroneBehaviour::SHOOTING:
			break;
		case EDroneBehaviour::TELEPORTING:
			m_TeleportationTimer -= DeltaTime;
			if(m_TeleportationTimer <= 0)
			{
				TeleportDrone();
			}
			break;
	}
}

void AASDronePawn::OnBehaviourAIStateTick()
{
	m_DroneToPlayerMult = 0;
	switch(m_CurrentBehaviour)
	{
		case EDroneBehaviour::CHASING:
			ChasingBehaviour();
			break;
		case EDroneBehaviour::SHOOTING:
			float distanceToObject;
			if(!IsPlayerInSight(distanceToObject) || distanceToObject < m_MaxRangeToAttack || distanceToObject > m_MinRangeToAttack)
			{
				ChangeBehaviour(EDroneBehaviour::CHASING);
			}
			else m_WeaponComponent->Fire(GetActorLocation(), GetActorForwardVector());
			break;
		case EDroneBehaviour::TELEPORTING:
			break;
	}
}

void AASDronePawn::ChasingBehaviour()
{
	float distanceToTarget;

	if(IsPlayerInSight(distanceToTarget))
	{
		if(distanceToTarget < m_MinRangeToAttack)
		{
			//Move away from player
			m_DroneToPlayerMult = -1;
		}
		else if(distanceToTarget > m_MaxRangeToAttack)
		{
			//Move towards player
			m_DroneToPlayerMult = 1;
		}
		else ChangeBehaviour(EDroneBehaviour::SHOOTING);
	}
	else if(distanceToTarget < m_DistanceToTeleport)
	{
		ChangeBehaviour(EDroneBehaviour::TELEPORTING);
	}
}

bool AASDronePawn::IsPlayerInSight(float& DistanceToTarget)
{
	FHitResult OutHit;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = m_Player->GetActorLocation();
	ECollisionChannel CollisionChannel = static_cast<ECollisionChannel>(StaticEnum<ECollisionChannel>()->GetValueByName(TEXT("Drone_Hit")));
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(SphereCast), true) ;
	CollisionQueryParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->SweepSingleByChannel(
	OutHit,
	StartLocation,
	EndLocation,
	FQuat::Identity,
	CollisionChannel,
	FCollisionShape::MakeSphere(m_SphereCastRadius),
	CollisionQueryParams);
	
	if(bHit)
	{
		DistanceToTarget = OutHit.Distance;
		if(OutHit.GetActor() == m_Player) return true;
	}
	return false;
}

void AASDronePawn::TeleportDrone()
{
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false; 
	QueryParams.bReturnPhysicalMaterial = false;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(m_Player);
	ECollisionChannel CollisionChannel = static_cast<ECollisionChannel>(StaticEnum<ECollisionChannel>()->GetValueByName(TEXT("Drone_Hit")));

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(m_SphereCastRadius);
	
	bool tpPointIsClear = false;
	int32 numberOfTeleportation = 0;
	FVector TeleportationLocation = GetActorLocation();
	while(tpPointIsClear == false)
	{
		numberOfTeleportation++;

		FVector Direction = (m_Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		float TPDistance = (numberOfTeleportation * m_DistanceByTeleportationSphereCast);
		FVector OverlapSphereLocation = GetActorLocation() +  Direction * TPDistance;

		// Perform the overlap check
		bool bHasOverlap = GetWorld()->OverlapBlockingTestByChannel(
			OverlapSphereLocation,
			FQuat::Identity,
			CollisionChannel, 
			CollisionShape,
			QueryParams
		);

		if(!bHasOverlap)
		{
			TeleportationLocation = OverlapSphereLocation;
			tpPointIsClear = true;
		}
		else if(numberOfTeleportation > m_NumberOfTPBeforeAbort)
		{
			UE_LOG(LogTemp, Warning, TEXT("CANNOT TELEPORT THROUGH WALL"));
			break;
		}
	}

	SetActorLocation(TeleportationLocation);
	ChangeBehaviour(EDroneBehaviour::CHASING);
}

void AASDronePawn::DroneMovement()
{
}

void AASDronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAtPlayer(DeltaTime);
	OnBehaviourStateTick(DeltaTime);

	m_AITickTimer -= DeltaTime;
	if(m_AITickTimer <= 0)
	{
		m_AITickTimer = m_AITickRate;
		OnBehaviourAIStateTick();
	}
}

void AASDronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

