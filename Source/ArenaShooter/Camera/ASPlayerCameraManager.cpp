// Fill out your copyright notice in the Description page of Project Settings.


#include "ASPlayerCameraManager.h"

#include "Camera/CameraModifier.h"

DECLARE_CYCLE_STAT(TEXT("Camera ProcessViewRotation"), STAT_Camera_ProcessViewRotation, STATGROUP_Game);
void AASPlayerCameraManager::ProcessViewRotation(float DeltaTime, FRotator& OutViewRotation, FRotator& OutDeltaRot)
{
	SCOPE_CYCLE_COUNTER(STAT_Camera_ProcessViewRotation);

	const FRotator OldViewRotation = OutViewRotation;

	// Apply view modifications from active camera modifiers
	for (int32 ModifierIdx = 0; ModifierIdx < ModifierList.Num(); ++ModifierIdx)
	{
		if (ModifierList[ModifierIdx] != nullptr && !ModifierList[ModifierIdx]->IsDisabled())
		{
			if (ModifierList[ModifierIdx]->ProcessViewRotation(ViewTarget.Target, DeltaTime, OutViewRotation, OutDeltaRot))
			{
				break;
			}
		}
	}

	const APawn* Pawn = GetViewTargetPawn();
	const FVector ViewPlaneZ = (Pawn == nullptr) ? FVector::ZeroVector : Pawn->GetActorQuat().GetAxisZ();

	if (!OutDeltaRot.IsZero()) {

		// Obtain current view orthonormal axes
		FVector ViewRotationX, ViewRotationY, ViewRotationZ;
		FRotationMatrix(OutViewRotation).GetUnitAxes(ViewRotationX, ViewRotationY, ViewRotationZ);
		
		if (!ViewPlaneZ.IsZero()) {
			// Yaw rotation should happen taking into account a determined plane to avoid weird orbits
			ViewRotationZ = ViewPlaneZ;
		}
		
		// Add delta rotation
		FQuat ViewRotation = OutViewRotation.Quaternion();
		if (OutDeltaRot.Pitch != 0.0f)
		{
			ViewRotation = FQuat(ViewRotationY, FMath::DegreesToRadians(-OutDeltaRot.Pitch)) * ViewRotation;
		}
		
		if (OutDeltaRot.Yaw != 0.0f)
		{
			ViewRotation = FQuat(ViewRotationZ, FMath::DegreesToRadians(OutDeltaRot.Yaw)) * ViewRotation;
		}
		
		if (OutDeltaRot.Roll != 0.0f)
		{
			ViewRotation = FQuat(ViewRotationX, FMath::DegreesToRadians(OutDeltaRot.Yaw)) * ViewRotation;
		}
		OutViewRotation = ViewRotation.Rotator();

		// Consume delta rotation
		OutDeltaRot = FRotator::ZeroRotator;
	}
	
	if (OutViewRotation != OldViewRotation)
	{
		if (!ViewPlaneZ.IsZero())
		{
			// Limit the player's view pitch only
			// Obtain current view orthonormal axes
			FVector ViewRotationX, ViewRotationY, ViewRotationZ;
			FRotationMatrix(OutViewRotation).GetUnitAxes(ViewRotationX, ViewRotationY, ViewRotationZ);

			// Obtain angle (with sign) between current view Z vector and plane normal
			float PitchAngle = FMath::RadiansToDegrees(FMath::Acos(ViewRotationZ | ViewPlaneZ));
			
			if ((ViewRotationX | ViewPlaneZ) < 0.0f)
			{
				PitchAngle *= -1.0f;
			}

			if (PitchAngle > ViewPitchMax)
			{
				// Make quaternion from zero pitch
				FQuat ViewRotation(FRotationMatrix::MakeFromZY(ViewPlaneZ, ViewRotationY));

				// Rotate 'up' with maximum pitch
				ViewRotation = FQuat(ViewRotationY, FMath::DegreesToRadians(-ViewPitchMax)) * ViewRotation;

				OutViewRotation = ViewRotation.Rotator();
			}
			else if (PitchAngle < ViewPitchMin)
			{
				// Make quaternion from zero pitch
				FQuat ViewRotation(FRotationMatrix::MakeFromZY(ViewPlaneZ, ViewRotationY));

				// Rotate 'down' with minimum pitch
				ViewRotation = FQuat(ViewRotationY, FMath::DegreesToRadians(-ViewPitchMin)) * ViewRotation;

				OutViewRotation = ViewRotation.Rotator();
			}
			
		} else
		{
			// Limit player view axes
			LimitViewPitch(OutViewRotation, ViewPitchMin, ViewPitchMax);
			LimitViewYaw(OutViewRotation, ViewYawMin, ViewYawMax);
			LimitViewRoll(OutViewRotation, ViewRollMin, ViewRollMax);
		}
	}
}

