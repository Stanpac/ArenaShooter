// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASWeapon.h"
#include "ASWeapon_PlayerPrimary.generated.h"

class UMaterialInterface;

UCLASS()
class ARENASHOOTER_API AASWeapon_PlayerPrimary : public AASWeapon
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASWeapon|Primary", meta = (DisplayName = "Decal Material"))
	TObjectPtr<UMaterialInterface> m_DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASWeapon|Primary", meta = (DisplayName = "Decal Size"))
	FVector m_DecalSize;
public:
	virtual void Fire(FVector FireOrigin, FVector FireDirection) override;
};
