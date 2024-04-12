// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASWeapon.h"
#include "ASWeapon_Turret.generated.h"


class AASCharacter;

UCLASS()
class ARENASHOOTER_API AASWeapon_Turret : public AASWeapon
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly, Category="Gun Play")
	float m_LaserRadius;

	UPROPERTY()
	float m_Deltatime;

	UPROPERTY()
	AASCharacter* m_Character;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
public:
	virtual void Fire(FVector fireOrigin, FVector fireDirection) override;
};
