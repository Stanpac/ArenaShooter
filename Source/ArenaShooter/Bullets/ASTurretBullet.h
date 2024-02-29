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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "HealthComponent"))
	UASHealthComponent* m_HealthComponent;

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
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MovementToCharacter(float DeltaTime);

	UFUNCTION()
	void OnDeath(AActor* actor);
	// Declare the overlap function
	//UFUNCTION()
	//void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
