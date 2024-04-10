#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASDashComponent.generated.h"


class UCameraComponent;
class UCharacterMovementComponent;
class AASCharacter;

UENUM(BlueprintType)
enum class EDashStates : uint8
{
	InDash UMETA(DisplayName = "In Dash"),
	NoGravity UMETA(DisplayName = "No Gravity"),
	InCooldown UMETA(DisplayName = "Dash Cooldown"),
	Neutral UMETA(DisplayName = "Neutral")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UASDashComponent : public UActorComponent
{
	GENERATED_BODY()


	
public:	
	UASDashComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "CombatProperties", DisplayName = "Amount of damage of the dash")
	float m_DashDamage = 15;
	
	UPROPERTY(EditAnywhere, Category = "CombatProperties", DisplayName = "Stun duration on ennemy of the dash")
	float m_StunDuration = 2;

	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "Dash distance")
	float m_DashDistance = 1700;
	
	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "Dash Speed Curve")
	UCurveFloat* m_DashSpeedCurve;
	
	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "Dash Cooldown")
	float m_DashCooldown = 3;

	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "How long to complete dash")
	float m_DashDuration = .3f;

	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "Dash offset")
	float m_DashOffset = 100;

	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "No Gravity after dash cooldown")
	float m_NoGravityCooldown = 1.5f;
	
	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "Refresh rate of the sphere cast to detect AASPawns")
	float m_TickRefreshRate = .1f;

	UPROPERTY(EditAnywhere, Category = "Dash Properties", DisplayName = "Speed of time after successful dash")
	float m_TimeSlowValue = .5f;
	
	UPROPERTY(EditAnywhere, Category = "Dash Detection Properties", DisplayName = "Range in which ASPawns are detected")
	float m_DashTargetDetectionRange = 2000;

	UPROPERTY(EditAnywhere, Category = "Dash Detection Properties", DisplayName = "Detect radius")
	float m_DashTargetDetectionRadius = 20;
	
	UPROPERTY(EditDefaultsOnly, Category = "Dash Detection Properties", DisplayName = "Current Dash State")
	EDashStates m_CurrentDashState = EDashStates::Neutral;

	UPROPERTY(EditDefaultsOnly, Category = "Signs and Feedbacks", DisplayName = "Dash sound")
	USoundCue* m_SoundDash;

	UPROPERTY(EditDefaultsOnly, Category = "Signs and Feedbacks", DisplayName = "FOV during dash")
	float m_MaxFieldOfView = 150;

	UPROPERTY(EditDefaultsOnly, Category = "Signs and Feedbacks", DisplayName = "FOV change curve during dash")
	UCurveFloat* m_FOVevolutionCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Signs and Feedbacks", DisplayName = "Dash Available sound")
	USoundCue* m_SoundDashAvailable;
	
	UPROPERTY()
	float m_BaseFieldOfView;
	
	UPROPERTY()
	float m_TickRefreshTimer = 0;

	UPROPERTY()
	float m_DashCooldownTimer = 0;

	UPROPERTY()
	float m_DashDurationTimer = 0;

	UPROPERTY()
	float m_NoGravityTimer = 0;
	
	UPROPERTY()
	AActor* m_HitTarget = nullptr;

	UPROPERTY()
	FVector m_DashStartLocation = FVector::Zero();
	
	UPROPERTY()
	FVector m_DashEndLocation = FVector::Zero();

	UPROPERTY()
	AASCharacter* m_Character = nullptr;

	UPROPERTY()
	UCameraComponent* m_Camera = nullptr;
	
	UPROPERTY()
	float m_StartGravity = 0;

	UPROPERTY()
	float m_StartGroundFriction = 0;


	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool OnDash();

	UFUNCTION()
	void SlowTick(float DeltaTime);

	UFUNCTION()
	void DashMovement(float DeltaTime);
	
	UFUNCTION()
	AActor* DetectDashTarget();
};
