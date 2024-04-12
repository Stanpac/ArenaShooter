// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ASCharacter.generated.h"

class USpringArmComponent;
class UGravitySwitchComponent;
class UASSpeedComponent;
class UASDashComponent;
class UASGlobalWidget;
class UASHealthComponent;
class UASEventWorldSubSystem;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UASWeaponComponent;
class UASCloseCombatComponent;
class USkeletalMeshComponent;
class USoundCue;
class UAnimMontage;
struct FInputActionValue;

/**
 * Base Class for all Characters in the game (Player and AI)
 */

UCLASS()
class ARENASHOOTER_API AASCharacter : public ACharacter
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* m_Mesh1P;

	/** Spring Arm Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Camera", meta = (DisplayName = "Spring Arm Component"))
	USpringArmComponent* m_SpringArmComponent;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Camera", meta = (DisplayName = "First Person Camera Component"))
	UCameraComponent* m_FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Camera", meta = (DisplayName = "Play Shake On taking Damage"))
	bool bPlayShakeOntakingDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Camera", meta = (DisplayName = "Camera Shake Class"))
	TSubclassOf<UCameraShakeBase> m_ShakeClass;

	/** Health Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Health", meta = (DisplayName = "Health Component"))
	UASHealthComponent* m_HealthComponent;

	/** Weapon Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Weapon", meta = (DisplayName = "Weapon Component"))
	UASWeaponComponent* m_WeaponComponent;

public:
	/** Dash Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Speed", meta = (DisplayName = "DashComponent"))
	UASDashComponent* m_DashComponent;

	/** Gravity Switch Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Speed", meta = (DisplayName = "GravitySwitchComponent"))
	UGravitySwitchComponent* m_GravitySwitchComponent;

protected:
	/** Player Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Widget", meta = (DisplayName = "Player Widget"))
	UASGlobalWidget* M_PlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASCharacter|Widget", meta = (DisplayName = "Player Widget"))
	TSubclassOf<UASGlobalWidget> M_PlayerWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Widget", meta = (DisplayName = "Weapon"))
	bool m_IsPrimaryWeaponEquipped;

	/** Fire Animation */
	UPROPERTY(EditAnywhere, Category = "ASCharacter|Animation", meta = (DisplayName = "Fire Montage"))
	TObjectPtr<UAnimMontage> m_FireMontage;

	/** Dash Animation */
	UPROPERTY(EditAnywhere, Category = "ASCharacter|Animation", meta = (DisplayName = "Dash Montage"))
	TObjectPtr<UAnimMontage> m_DashMontage;
	
	/** Event World SubSystem */
	TObjectPtr<UASEventWorldSubSystem> m_EventWorldSubSystem;
	
	/* ----------------------------------- SOUND  -------------------------------*/

	UPROPERTY(EditAnywhere, Category = "ASCharacter|Sound", meta = (DisplayName = "Sound On Death"))
	USoundCue* m_SoundDeath;
	
	UPROPERTY(EditAnywhere, Category = "ASCharacter|Sound", meta = (DisplayName = "Sound On Hit"))
	USoundCue* m_SoundHit;

	UPROPERTY(EditAnywhere, Category = "ASCharacter|Sound", meta = (DisplayName = "Sound On Gravity Available"))
	USoundCue* m_SoundGravityAvailable;
	
	/* ---------------------- Input To move in component -------------------------------*/
	
	/** Default MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta=(DisplayName = "Default Mapping Context"))
	UInputMappingContext* m_DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta=(DisplayName = "Jump Action"))
	UInputAction* m_JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta=(DisplayName = "Move Action"))
	UInputAction* m_MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "Look Action"))
	UInputAction* m_LookAction;
	
	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "Shoot Action"))
	UInputAction* m_ShootAction;
	
	/** Close Combat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "Dash Action"))
	UInputAction* m_DashAction;
	
	/** Switch Gravity Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "Switch Gravity Action"))
	UInputAction* m_switchGravityAction;

	UPROPERTY()
	bool m_DisableMovements;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Widget", meta = (DisplayName = "Lock Widget Actor Class"))
	TSubclassOf<AActor> M_LockWidgetActorClass;

	UPROPERTY()
	AActor* M_LockWidgetActor;
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Exec)
	void DebugDamage(float amount);
	
	UFUNCTION(Exec)
	void DebugHealing(float amount);

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void SwitchGravity(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	
	UFUNCTION()
	virtual void OnStartDeath(AActor* OwningActor);

	UFUNCTION()
	virtual void OnEndDeath();

	UFUNCTION()
	virtual void OnFire();

	UFUNCTION()
	virtual void OnChangeGravity();

	UFUNCTION()
	virtual void OnAbilityCooldownEnd();

	UFUNCTION()
	virtual void OnGravityChargeRefill();

	UFUNCTION()
	virtual void OnHitTargetChange(AActor* HitTarget);

public:
	UFUNCTION()
	virtual void OnDashValidate();
	
	UFUNCTION()
	virtual void OnDashAbilityCooldownEnd();

	UFUNCTION()
	virtual void OnDashRechargeTick(float percent);
	
	UFUNCTION()
	void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer);

	void CheckPlayScreenShake();

	void GetAllSubsystem();
	
	void AddDefaultMappingContext();
	void RemoveDefaultMappingContext();

public:
	FORCEINLINE UASGlobalWidget* GetPlayerWidget() const { return M_PlayerWidget; }

	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return m_Mesh1P; }

	FORCEINLINE UCameraComponent* GetCameraComponent() const{ return m_FirstPersonCameraComponent; }

	FORCEINLINE	void SetMovementsDisabled(bool DisableMovements) { m_DisableMovements = DisableMovements; }
};
