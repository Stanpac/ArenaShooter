// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NinjaCharacter.h"
#include "ArenaShooter/Components/WeaponComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ASCharacter.generated.h"

class UASGlobalWidget;
class UASHealthComponent;
class UASEventWorldSubSystem;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UASWeaponComponent;
class USkeletalMeshComponent;
struct FInputActionValue;

/**
 * Base Class for all Characters in the game (Player and AI)
 * TODO :  Move input In component ?
 * TODO :  Implement Sprint ? 
 * TODO :  Implement Health -> Component 
 * 
 */

UCLASS()
class ARENASHOOTER_API AASCharacter : public ANinjaCharacter
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:
	/** Skeletal Mesh */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* m_SkeletalMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Camera", meta = (DisplayName = "FirstPersonCameraComponent"))
	UCameraComponent* m_FirstPersonCameraComponent;

	/** Health Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Health", meta = (DisplayName = "HealthComponent"))
	UASHealthComponent* m_HealthComponent;

	/** Weapon Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Weapon", meta = (DisplayName = "WeaponComponent"))
	UASWeaponComponent* m_WeaponComponent;

	/** Player Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Widget", meta = (DisplayName = "PlayerWidget"))
	UASGlobalWidget* M_PlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASCharacter|Widget", meta = (DisplayName = "PlayerWidget"))
	TSubclassOf<UASGlobalWidget> M_PlayerWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASCharacter|Widget", meta = (DisplayName = "Weapon"))
	bool m_IsPrimaryWeaponEquipped;
	
	/** Event World SubSystem */
	UPROPERTY()
	TObjectPtr<UASEventWorldSubSystem> m_EventWorldSubSystem;

	
	/* ---------------------- Input To move in component -------------------------------*/
	
	/** Default MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta=(DisplayName = "DefaultMappingContext"))
	UInputMappingContext* m_DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta=(DisplayName = "JumpAction"))
	UInputAction* m_JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta=(DisplayName = "MoveAction"))
	UInputAction* m_MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "LookAction"))
	UInputAction* m_LookAction;
	
	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "ShootAction"))
	UInputAction* m_ShootAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "ReloadAction"))
	UInputAction* m_ReloadAction;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASCharacter|Input", meta = (DisplayName = "SwitchAction"))
	//UInputAction* m_SwitchAction;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASCharacter(const FObjectInitializer& ObjectInitializer);
	
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
	void Switch(const FInputActionValue& Value) const;
	
	UFUNCTION()
	virtual void OnStartDeath();

	UFUNCTION()
	virtual void OnEndDeath();

	void GetAllSubsystem();
	void AddDefaultMappingContext();
	void RemoveDefaultMappingContext();

public:
	UASGlobalWidget* GetPlayerWidget() const { return M_PlayerWidget; }
};
