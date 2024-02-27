#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/SubSystem/ASEventWorldSubSystem.h"
#include "Components/ActorComponent.h"
#include "ASSpeedComponent.generated.h"

class USpeedProfilsData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpeedComponent_UpdateSpeedProfileEvent, int, SpeedProfile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpeedComponent_UpdateSpeedEvent, float, NewSpeed, float, MaxSpeed);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UASSpeedComponent : public UActorComponent
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(EditDefaultsOnly, Category = "ASSpeedComponent|Profile", meta = (DisplayName = "Speed Profile Data Asset"))
	TObjectPtr<USpeedProfilsData> m_speedProfilData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASSpeedComponent|Profile", meta = (DisplayName = "Current Speed Profile", ClampMin = 0, ClampMax = 3))
	int m_CurrentSpeedProfile;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ASSpeedComponent|Speed", meta = (DisplayName = "SpeedBarValue"))
	float m_SpeedBarValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ASSpeedComponent|Speed", meta = (DisplayName = "SpeedBarMaxValue"))
	float m_SpeedBarMaxValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ASSpeedComponent|Speed", meta = (DisplayName = "EnemyDeathSpeedBarValueIncrease"))
	float m_EnemyDeathSpeedBarValueIncrease = 30.0f;
	
	/** Speed Bar Amount of loss per Second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASSpeedComponent|Speed", meta = (DisplayName = "SpeedBarDecreasRate"))
	float m_SpeedBarDecreasRate = 10.0f;
	
	/** Event World SubSystem */
	UPROPERTY()
	UASEventWorldSubSystem* m_EventWorldSubSystem;

public :
	/** Event that will be broadcasted when the speed profile is updated */
	UPROPERTY(BlueprintAssignable)
	FSpeedComponent_UpdateSpeedProfileEvent OnUpdateSpeedProfile;

	/** Event that will be broadcasted when the speed is updated */
	UPROPERTY(BlueprintAssignable)
	FSpeedComponent_UpdateSpeedEvent OnUpdateSpeed;
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:	
	UASSpeedComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	void UpdateSpeedProfile(int SpeedProfile);

	UFUNCTION()
	void UpdateSpeedBarValue(float Value);

	void UpdateSpeedBarMaxValue(float Value);

	UFUNCTION()
	void OnEnemyDeath();
};
