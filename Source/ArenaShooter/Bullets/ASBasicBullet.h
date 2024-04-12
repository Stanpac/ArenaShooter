// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASBasicBullet.generated.h"

UCLASS()
class ARENASHOOTER_API AASBasicBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASBasicBullet();
	
	UPROPERTY(BlueprintReadWrite)
	float m_bulletSpeed;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void SetSpeed(float newSpeed)
	{
		m_bulletSpeed = newSpeed;
	}
};
