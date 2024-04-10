#include "ASDronePawn.h"

#include "ASDroneManager.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "ArenaShooter/Weapons/ASWeapon.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
// Sets default values

AASDronePawn::AASDronePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AASDronePawn::BeginPlay()
{
	Super::BeginPlay();
	m_Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	m_DroneToPlayerMult = 1;
	check(m_Player);
	m_CurrentBehaviour = EDroneBehaviour::CHASING;
	OnBehaviourStateEnter();
	m_DroneManager = GetWorld()->GetSubsystem<UASDroneManager>();
	m_DroneManager->AddDrone(this);
	m_DispersionFactor = FMath::Lerp(m_DispersionFactorMin, m_DispersionFactorMax, FMath::FRand());
	if(IsValid(m_SoundMove))
	{
		//UGameplayStatics::PlaySoundAtLocation( GetWorld(), m_SoundMove, GetActorLocation());
	}
}

void AASDronePawn::OnDeath(AActor* DeathDealer)
{
	Super::OnDeath(DeathDealer);
	if(IsValid(m_SoundMove))
	{
		m_SoundMove->VolumeMultiplier = 0;
	}
}

void AASDronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAtPlayer(DeltaTime);
	OnBehaviourStateTick(DeltaTime);
	DroneMovement(DeltaTime);
	m_AITickTimer -= DeltaTime;
	if(m_AITickTimer <= 0)
	{
		m_AITickTimer = m_AITickRate;
		OnBehaviourAIStateTick();
	}
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
			m_WeaponComponent->m_PrimaryWeapon->m_IsReloading = true;
			m_WeaponComponent->m_PrimaryWeapon->m_ReloadTimer = m_TimeToLockInTarget;
			if(IsValid(m_LockInSound))
			UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			m_LockInSound,
			GetOwner()->GetActorLocation());
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
			if(m_EnableShooting) m_WeaponComponent->Fire(GetActorLocation(), GetActorForwardVector());
			break;
		case EDroneBehaviour::TELEPORTING:
			m_TeleportationTimer -= DeltaTime;
			if(m_TeleportationTimer <= 0)
			{
				TeleportDrone();
				if(IsValid(m_SoundTeleportation))
				{
					UGameplayStatics::PlaySoundAtLocation( GetWorld(), m_SoundTeleportation, GetActorLocation());
				}
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
		{
			ChasingBehaviour();
			m_DispersionVector = DroneRelativeMoveDirection();
			break;
		}

		case EDroneBehaviour::SHOOTING:
		{
			m_DispersionVector = DroneRelativeMoveDirection();
			float distanceToObject;
			bool sightCondition;
			sightCondition = IsPlayerInSight(distanceToObject);
			bool distanceCondition;
			distanceCondition = distanceToObject < m_MinRangeToAttack || distanceToObject > m_MaxRangeToAttack;
			if(!sightCondition || distanceCondition)
			{
				ChangeBehaviour(EDroneBehaviour::CHASING);
			}
			break;
		}
		case EDroneBehaviour::TELEPORTING:
			break;
	}
}

void AASDronePawn::LookAtPlayer(float DeltaTime)
{
	const FVector LookAtLocation = m_Player->GetActorLocation();
	const FVector PawnLocation = GetActorLocation();
	const FRotator LookAtRotation = (LookAtLocation - PawnLocation).Rotation();
	SetActorRotation(LookAtRotation);
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
		m_TeleportationTimer = m_TeleportationDuration;
		ChangeBehaviour(EDroneBehaviour::TELEPORTING);
	}
	else
	{
		m_DroneToPlayerMult = 1;
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
 		if(OutHit.GetActor() == m_Player)
			return true;
		else
			return false;
	}
	
	return true;
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
		float TPDistance = (numberOfTeleportation * m_DistanceByTeleportationSphereCast) + m_DistanceToTeleport;
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

void AASDronePawn::DroneMovement(float DeltaTime)
{
	const FVector droneToPlayerVector = (m_Player->GetActorLocation() - GetActorLocation()).GetSafeNormal() * m_DroneToPlayerMult;

	const float DroneToFromPlayerSpeed = m_DroneToPlayerMult > 0 ? m_DroneToPlayerSpeed : m_DroneAwayFromPlayerSpeed;
	float dispersionEnableValue = 1;
	if(!m_EnableDispersionBehaviour) dispersionEnableValue = 0;

	float movementTowardsPlayerEnableValue = 1;
	if(!m_EnableMovementTowardsPlayer || m_CurrentBehaviour != EDroneBehaviour::CHASING) movementTowardsPlayerEnableValue = 0;

	float distanceMult = m_DistanceSpeedCurve->FloatCurve.Eval(FMath::Clamp(FVector::Distance(m_Player->GetActorLocation(), GetActorLocation()), 0, m_MaxDistanceEvaluated) / m_MaxDistanceEvaluated) * m_DistanceSpeedMult;
	if(m_CurrentBehaviour != EDroneBehaviour::CHASING || m_DroneToPlayerMult < 0)
	{
		distanceMult = 1;
	}
	
	FVector moveToPlayer = droneToPlayerVector * DroneToFromPlayerSpeed * movementTowardsPlayerEnableValue * distanceMult;
	FVector dispersion = m_DispersionVector * m_DroneDispersionSpeed * dispersionEnableValue * m_DispersionFactor;
	dispersion.Z = FMath::Clamp(dispersion.Z, 0, 1000);
	FVector moveVector = GetActorLocation() + ( dispersion + moveToPlayer ) * DeltaTime;

	if(IsValid(m_SoundMove) && IsValid(m_MoveSoundCurve))
	{
		m_SoundMove->VolumeMultiplier = m_MoveSoundCurve->FloatCurve.Eval(FMath::Max(moveToPlayer.Length() / m_MaxMoveSpeed, m_MaxMoveSpeed));
	}

	FHitResult OutHit;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (moveVector - StartLocation).GetSafeNormal() * m_DistanceToTeleport *.8f;
	ECollisionChannel CollisionChannel = ECC_Visibility;
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(SphereCast), true) ;
	CollisionQueryParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		StartLocation,
		EndLocation,
		CollisionChannel,
		CollisionQueryParams,
		FCollisionResponseParams::DefaultResponseParam
		);

	if(!m_IsStunned && !bHit)
	{
		SetActorLocation(moveVector, true);
	}
}

FVector AASDronePawn::DroneRelativeMoveDirection()
{
	TArray<AActor*> drones = m_DroneManager->GetDrones();
	FVector averageDronesLocation = FVector::Zero();
	
	for(auto drone : drones)
	{
		if(drone != this && IsValid(drone))
		{
			FVector direction = (drone->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			float evaluatedAmplitude = m_DroneDispersionCurve->GetFloatValue(FVector::Distance(GetActorLocation(), drone->GetActorLocation()) / 1000);
			averageDronesLocation += direction * evaluatedAmplitude;
		}
	}
	//if(averageDronesLocation .Length() > 1) averageDronesLocation  = averageDronesLocation .GetSafeNormal();
	return -averageDronesLocation;
}




