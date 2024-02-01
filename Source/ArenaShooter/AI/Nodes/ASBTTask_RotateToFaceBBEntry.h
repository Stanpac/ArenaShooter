// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ASBTTask_RotateToFaceBBEntry.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class ARENASHOOTER_API UASBTTask_RotateToFaceBBEntry : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	/* ---------------------------------- MEMBERS --------------------------------------*/
protected:
	/** Success condition precision in degrees */
	UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0"))
	float Precision;

private:
	/** cached Precision tangent value */
	float PrecisionDot;
	
	/* ---------------------------------- FUNCTIONS --------------------------------------*/
public:
	
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
