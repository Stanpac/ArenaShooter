// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASWeapon.h"
#include "ASWeapon_Turret.generated.h"


UCLASS()
class ARENASHOOTER_API AASWeapon_Turret : public AASWeapon
{
	GENERATED_BODY()

	
public:
	virtual void Fire(FVector fireOrigin, FVector fireDirection) override;
};
