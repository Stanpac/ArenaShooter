// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaCharacter.h"
#include "ArenaShooter/Components/ASWeaponComponent.h"
#include "InputActionValue.h"
#include "ASCharacter.generated.h"

class UASSpeedComponent;
class UASGlobalWidget;
class UASHealthComponent;
class UASEventWorldSubSystem;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UASWeaponComponent;
class UASCloseCombatComponent;
class USkeletalMeshComponent;
struct FInputActionValue;

/**
 * Base Class for all Characters in the game (Player and AI)
 */

UCLASS()
class ARENASHOOTER_API AASCharacter : public ANinjaCharacter
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* m_Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Camera", meta = (DisplayName = "First Person Camera Component"))
	UCameraComponent* m_FirstPersonCameraComponent;

	/** Health Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Health", meta = (DisplayName = "Health Component"))
	UASHealthComponent* m_HealthComponent;

	/** Weapon Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Weapon", meta = (DisplayName = "Weapon Component"))
	UASWeaponComponent* m_WeaponComponent;
	
	/** Close Combat Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Weapon", meta = (DisplayName = "Close Combat Component"))
	UASCloseCombatComponent* m_CloseCombatComponent;

	/** Speed Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Speed", meta = (DisplayName = "SpeedComponent"))
	UASSpeedComponent* m_SpeedComponent;

	/** Player Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Widget", meta = (DisplayName = "Player Widget"))
	UASGlobalWidget* M_PlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASCharacter|Widget", meta = (DisplayName = "Player Widget"))
	TSubclassOf<UASGlobalWidget> M_PlayerWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Widget", meta = (DisplayName = "Weapon"))
	bool m_IsPrimaryWeaponEquipped;
	
	/** Event World SubSystem */
	UPROPERTY()
	TObjectPtr<UASEventWorldSubSystem> m_EventWorldSubSystem;

	
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

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "Reload Action"))
	UInputAction* m_ReloadAction;
	
	/** Close Combat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "Close Combat Action"))
	UInputAction* m_CloseCombatAction;

	/** CheatSpeed bool */
	bool SpeedCheatAllowed;

	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(Exec)
	void DebugDamage(float amount);
	
	UFUNCTION(Exec)
	void DebugHealing(float amount);

	UFUNCTION(Exec)
	void CheatSpeed(bool Cheat);

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void Switch(const FInputActionValue& Value) const;
	void CloseCombat(const FInputActionValue& Value);
	
	UFUNCTION()
	virtual void OnStartDeath(AActor* OwningActor);

	UFUNCTION()
	virtual void OnEndDeath();

	UFUNCTION()
	void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer);

	UFUNCTION()
	void OnSpeedProfileChanged(int SpeedProfile);

	UFUNCTION()
	void OnSpeedChanged(float NewSpeed, float MaxSpeed);

	void GetAllSubsystem();
	void AddDefaultMappingContext();
	void RemoveDefaultMappingContext();

public:
	UASGlobalWidget* GetPlayerWidget() const { return M_PlayerWidget; }
};
