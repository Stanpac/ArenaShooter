#include "ASDronePawn.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "ArenaShooter/Weapons/ASWeapon.h"
#include "GameFramework/Character.h"
// Sets default values
AASDronePawn::AASDronePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	m_HealthComponent = CreateDefaultSubobject<UASHealthComponent>(TEXT("HealthComponent"));

	m_WeaponComponent = CreateDefaultSubobject<UASWeaponComponent>(TEXT("WeaponComponent"));
}

// Called when the game starts or when spawned
void AASDronePawn::BeginPlay()
{
	Super::BeginPlay();
	m_Player = GetWorld()->GetFirstPlayerController()->GetCharacter();
	check(m_Player);

	m_WeaponComponent->InitializeWeapon();
}

void AASDronePawn::LookAtPlayer(float DeltaTime)
{
	const FVector LookAtLocation = m_Player->GetActorLocation();
	const FVector PawnLocation = GetActorLocation();
	const FRotator LookAtRotation = (LookAtLocation - PawnLocation).Rotation();
	SetActorRotation(LookAtRotation);
}

// Called every frame
void AASDronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAtPlayer(DeltaTime);
}

// Called to bind functionality to input
void AASDronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

