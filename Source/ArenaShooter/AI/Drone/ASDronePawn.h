// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASDronePawn.generated.h"

class UASHealthComponent;
class UASWeaponComponent;

UCLASS()
class ARENASHOOTER_API AASDronePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AASDronePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Health Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Health", meta = (DisplayName = "HealthComponent"))
	UASHealthComponent* m_HealthComponent;

	/** Weapon Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Weapon", meta = (DisplayName = "WeaponComponent"))
	UASWeaponComponent* m_WeaponComponent;

	/** Player Reference */
	UPROPERTY()
	ACharacter* m_Player;

	UFUNCTION()
	virtual void LookAtPlayer(float DeltaTime);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
