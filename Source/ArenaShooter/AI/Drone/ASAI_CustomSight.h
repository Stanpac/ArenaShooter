// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "Components/ActorComponent.h"
#include "ASAI_CustomSight.generated.h"


class UBlackboardComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UASAI_CustomSight : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASAI_CustomSight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Detection")
	void Detect();

	UFUNCTION(BlueprintCallable, Category="Detection")
	void IsTargetInRange();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPerception|Sight", meta = (DisplayName = "BlackBoard"))
	UBlackboardComponent* m_BlackboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPerception|Sight", meta = (DisplayName = "DetectionRange"))
	float m_DetectionRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPerception|Sight", meta = (DisplayName = "DistanceToLoseSight"))
	float m_LoseSightRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPerception|Sight", meta = (DisplayName = "IsInRange"))
	bool m_IsInRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASPerception|Sight", meta = (DisplayName = "IsInSight"))
	bool m_IsInSight;

	UPROPERTY()
	ACharacter* m_Character;
};
