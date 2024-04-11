// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASWeapon.h"
#include "ASWeapon_PlayerPrimary.generated.h"

class AASBasicBullet;
class UNiagaraComponent;
class UNiagaraSystem;
class UNiagaraFunctionLibrary;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASWeapon|Primary", meta = (DisplayName = "Number of consecutive shots Level 1"))
	int32 m_NumOfCS1 = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASWeapon|Primary", meta = (DisplayName = "Number of consecutive shots Level 2"))
	int32 m_NumOfCS2 = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASWeapon|Primary", meta = (DisplayName = "Number of consecutive shots Level 3"))
	int32 m_NumOfCS3 = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASWeapon|Primary", meta = (DisplayName = "Time to reset consecutive shots count"))
	float m_TimeToResetCS = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ASWeapon|Primary")
	TSubclassOf<AASBasicBullet> m_BulletBP;

	UPROPERTY(EditDefaultsOnly, Category="ASWeapon|Primary")
	FVector m_ShootPoint;
	
	UPROPERTY()
	float m_CurrentTimeToResetCS;
	
	UPROPERTY()
	int32 m_CurrentNumberOfShotsInCS;

	void OnCS1Reached();
	void OnCS2Reached();
	void OnCS3Reached();

public:
	virtual void Fire(FVector FireOrigin, FVector FireDirection) override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SpawnBullet(FVector FireOrigin, FVector FireDirection);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnFire(FVector FireOrigin, FVector FireDestination);
	
	void OnResetCS();
};
