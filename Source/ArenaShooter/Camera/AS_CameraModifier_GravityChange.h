#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "AS_CameraModifier_GravityChange.generated.h"


UCLASS()
class ARENASHOOTER_API UAS_CameraModifier_GravityChange : public UCameraModifier
{
	GENERATED_BODY()

public:
	virtual bool ProcessViewRotation(AActor* ViewTarget, float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot) override;
};
