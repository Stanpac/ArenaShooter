// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASWeaponComponent.generated.h"

class AASPawn;
class AASWeapon;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENASHOOTER_API UASWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/** Sets default values for this component's properties */
	UASWeaponComponent();

	/** Blueprint reference to the primary weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	TSubclassOf<AASWeapon> m_primaryWeaponBlueprint;

	/** Blueprint reference to the secondary weapon */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	//TSubclassOf<AASWeapon> m_secondaryWeaponBlueprint;

	/** Weapon position offset (Especially useful when there is no socket) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	FVector m_PositionOffset;

	/** Weapon scale offset (Especially useful when there is no socket) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	FVector m_ScaleOffset;

	/** Weapon rotation offset (Especially useful when there is no socket) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	FVector m_RotationOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	bool m_isPrimaryWeaponEquiped;

	UPROPERTY()
	AASWeapon* m_PrimaryWeapon;

	UPROPERTY()
	AASWeapon* m_SecondaryWeapon;

	UPROPERTY()
	AASWeapon* m_CurrentEquipedWeapon;

	UPROPERTY()
	AASPawn* m_ASPawnOwner;
	
	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void Fire(FVector fireOrigin, FVector fireDirection);

	/** Make the weapon Recharge its munitions */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void Reload();
	
	/** Switch between primary and secondary weapons **/
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void SwitchWeapon();

	/** At Begin Play **/
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	virtual void InitializeWeapon();
};


