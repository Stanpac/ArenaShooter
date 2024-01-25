// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USkeletalMeshComponent;
struct FInputActionValue;

/**
 * Base Class for all Characters in the game (Player and AI)
 * TODO :  Bind Input Actions to Functions
 * TODO :  Implement jump, Sprint ? Crouch ?
 * TODO :  Implement Death
 * TODO :  Implement Health -> Component 
 * 
 */

UCLASS()
class ARENASHOOTER_API AASCharacter : public ACharacter
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
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASCharacter();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void AddDefaultMappingContext();

	void RemoveDefaultMappingContext();
};
