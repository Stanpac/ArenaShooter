// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASWeapon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UNiagaraFunctionLibrary;
class UStaticMeshComponent;
class USoundBase;


UCLASS()
class ARENASHOOTER_API AASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASWeapon();

	//TODO Introduce a curve variable for how the damage multiplier by distance is Evaluated

	/** Damage by bullet **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Play", meta=(DisplayName = "Damage By Bullet"))
	float m_DamageByBullet;

	/** How much time in seconds it takes to shoot another bullet **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Fire Delay Duration"))
	float m_FireDelayDuration;

	/** How much time is left to be able to fire another bullet **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Current Fire Delay Timer"))
	float m_FireDelayTimer;
	
	/** Is the weapon waiting to shoot another bullet **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Is Reloading ?"))
	bool m_IsWaitingForFireDelay;
	
	/** Damage Multiplier by distance **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Play", meta=(DisplayName = "Damage Multiplier By Distance;"))
	float m_DamageMultiplierByDistance;
	
	/** Can you maintain the fire input to keep firing **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Play", meta=(DisplayName = "Need New InputPress"))
	bool m_NeedNewInputPress;

	/** Are ammos necessary for the gun **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Is there a reload Feature ?"))
	bool m_EnableReload;
	
	/** Number of ammos in a magazine after reloading **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Magazine Size"))
	int32 m_MagazineSize;

	/** Current number of ammos in the magazine **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Current Ammos"))
	int32 m_CurrentAmmos;
	
	/** How much time in seconds it takes to reload **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Reload Duration"))
	float m_ReloadDuration;

	/** How much time is left to complete reload **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Current Reload Timer"))
	float m_ReloadTimer;
	
	/** Is the weapon reloading **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reload", meta=(DisplayName = "Is Reloading ?"))
	bool m_IsReloading;

	/** How long will targets be stunned when hit **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stun", meta=(DisplayName = "Ennemy Stun Duration On Hit"))
	float m_OnHitStunDuration = 0;
	
	/** Sound when shot is fired **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback", meta=(DisplayName = "Fire Sound"))
	USoundBase* m_Sound_ShotFired;

	/** Determines if the weapon will be set hidden at start **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback", meta=(DisplayName = "Is the Weapon Shown"))
	bool m_ShowWeapon;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* m_FireMuzzleComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Feedback", meta=(DisplayName = "Fire Shoot Pos"))
	USceneComponent* m_FireShootPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback", meta=(DisplayName = "Fire Shoot Niagara Component"))
	UNiagaraComponent* m_Niagara_ShotFiredComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feedback", meta=(DisplayName = "Weapon Mesh"))
	UStaticMeshComponent* M_WeaponMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ReloadTick(float DeltaTime);
	
	UFUNCTION()
	virtual void FireDelayProc();

	UFUNCTION()
	virtual void FireDelayTick(float DeltaTime);
	
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	virtual void Fire(FVector fireOrigin, FVector fireDirection);

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	virtual void Reload();
};


