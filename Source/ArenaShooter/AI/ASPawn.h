// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASPawn.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UASHealthComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class UUserWidget;


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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASPawn|Components", meta = (DisplayName = "HealthVisibilitySphereComponent"))
	USphereComponent* m_HealthVisibilitySphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASPawn|Components", meta = (DisplayName = "HealthBarWidgetComponent"))
	UWidgetComponent* m_HealthBarWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ASPawn|Widget", meta = (DisplayName = "HealthBarWidgetClass"))
	TSubclassOf<UUserWidget> m_HealthBarWidgetClass;
	
	FTimerHandle m_HealthBarTimerHandle;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	AASPawn();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(float PreviousHealth, float CurrentHealth, float MaxHealth, AActor* DamageDealer);
	
	UFUNCTION()
	void OnDeath(AActor* DeathDealer);

	void SetWidgetVisibility(bool visible);

	void SetWidgetVisibilityfalse();

	void OnHealthVisibilitySphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnHealthVisibilitySphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
};
