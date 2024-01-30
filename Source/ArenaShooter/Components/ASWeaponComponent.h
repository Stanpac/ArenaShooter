// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/Character/ASCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "ASWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType
{
	PrimaryWeapon			UMETA(DisplayName="PrimaryWeapon"),
	SecondaryWeapon         UMETA(DisplayName="SecondaryWeapon"),
	CloseCombat				UMETA(DisplayName="CloseCombat"),
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UASWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:


	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* m_FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* m_FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector m_MuzzleOffset;

	/** Sets default values for this component's properties */
	UASWeaponComponent();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire(FVector direction, FVector startingPos);

protected:

private:
};


