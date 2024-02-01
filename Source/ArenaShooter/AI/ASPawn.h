// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASPawn.generated.h"

class UCapsuleComponent;
class UASHealthComponent;
class UStaticMeshComponent;


UCLASS()
class ARENASHOOTER_API AASPawn : public APawn
{
	GENERATED_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "MeshComponent"))
	UStaticMeshComponent* m_MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "CapsuleComponent"))
	UCapsuleComponent* m_CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "HealthComponent"))
	UASHealthComponent* m_HealthComponent;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASPawn();
};
