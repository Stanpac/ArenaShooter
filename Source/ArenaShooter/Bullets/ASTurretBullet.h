#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASTurretBullet.generated.h"

class UASHealthComponent;

UCLASS()
class ARENASHOOTER_API AASTurretBullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AASTurretBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* m_Character;

	/** Bullet Speed in u/s */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	float m_Speed;

public:
	// Called every frame

	UPROPERTY()
	float m_bulletSpeed;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MovementToCharacter(float DeltaTime);

	UFUNCTION()
	void MovementForward(float DeltaTime);
	
};
