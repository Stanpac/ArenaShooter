#include "ASWeapon_PlayerPrimary.h"

#include "ArenaShooter/AI/ASPawn.h"
#include "ArenaShooter/Components/ASHealthComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "ArenaShooter/Bullets/ASBasicBullet.h"
#include "Sound/SoundCue.h"


void AASWeapon_PlayerPrimary::Fire(FVector FireOrigin, FVector FireDirection)
{
	Super::Fire(FireOrigin, FireDirection);

	m_CurrentNumberOfShotsInCS ++;
	if(m_CurrentNumberOfShotsInCS == m_NumOfCS1)
		OnCS1Reached();
	else if(m_CurrentNumberOfShotsInCS == m_NumOfCS2)
		OnCS2Reached();
	else if(m_CurrentNumberOfShotsInCS == m_NumOfCS3)
		OnCS3Reached();

	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bIgnoreTouches = false;
	CollisionParams.AddIgnoredActor(GetOwner());
	if(m_Sound_ShotFired) {
		UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),
		m_Sound_ShotFired,
		GetOwner()->GetActorLocation());
	}

	SpawnBullet(FireOrigin, FireDirection);

	

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,FireOrigin,FireOrigin + FireDirection * 1000000,ECC_Visibility, CollisionParams)) {
		// Process the hit result
		if (AActor* HitActor = HitResult.GetActor())
			{
				if(UASHealthComponent* HealthComponent = UASHealthComponent::FindHealthComponent(HitActor))
				{
					HealthComponent->Damage(m_DamageByBullet, GetOwner(), m_OnHitStunDuration, HitResult.Location, FireDirection.Rotation());
				}
			else
			{
				UDecalComponent* lDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), m_DecalMaterial, m_DecalSize, FVector(HitResult.ImpactPoint), HitResult.ImpactNormal.Rotation().GetInverse(), 5.0f);
				lDecal->SetFadeScreenSize(0.001f);
			}
		}
		//OnFire(FireOrigin ,HitResult.Location);
		/*if(IsValid(m_bulletTrail))
		{
			m_bulletTrail->Deactivate();
			FNiagaraVariable StartPosVar(FNiagaraTypeDefinition::GetVec3Def(), TEXT("StartPosition"));
			FNiagaraVariable EndPosVar(FNiagaraTypeDefinition::GetVec3Def(), TEXT("EndPosition"));
			StartPosVar.SetValue<FVector>(FireOrigin);
			EndPosVar.SetValue<FVector>(HitResult.Location);
			m_bulletTrail->SetVariableVec3(StartPosVar.GetName(), StartPosVar.GetValue<FVector>());
			m_bulletTrail->SetVariableVec3(EndPosVar.GetName(), EndPosVar.GetValue<FVector>());
		
			m_bulletTrail->Activate();
		}*/
	}
	else
	{
		//OnFire(FireOrigin ,FireOrigin + FireDirection * 1000);
		/*if(IsValid(m_bulletTrail))
		{
			m_bulletTrail->Deactivate();
			FNiagaraVariable StartPosVar(FNiagaraTypeDefinition::GetVec3Def(), TEXT("StartPosition"));
			FNiagaraVariable EndPosVar(FNiagaraTypeDefinition::GetVec3Def(), TEXT("EndPosition"));
			StartPosVar.SetValue(FireOrigin);
			EndPosVar.SetValue(FireOrigin + FireDirection * 1000);
			m_bulletTrail->SetVariableVec3(StartPosVar.GetName(), StartPosVar.GetValue<FVector>());
			m_bulletTrail->SetVariableVec3(EndPosVar.GetName(), EndPosVar.GetValue<FVector>());
		
			m_bulletTrail->Activate();
		}*/
	}
}

void AASWeapon_PlayerPrimary::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_CurrentTimeToResetCS += DeltaSeconds;
	if(m_CurrentTimeToResetCS >= m_TimeToResetCS)
	{
		OnResetCS();
	}
}

void AASWeapon_PlayerPrimary::SpawnBullet(FVector FireOrigin, FVector FireDirection)
{
	FActorSpawnParameters params;
	params.ObjectFlags |= RF_Transient;
	params.bNoFail = true;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if(IsValid(m_BulletBP))
	{
		//const FRotator LookAtRotation = (LookAtLocation - FireOrigin).Rotation();
		const FRotator LookAtRotation = FireDirection.Rotation();
		AASBasicBullet* bullet = GetWorld()->SpawnActor<AASBasicBullet>(m_BulletBP, GetActorLocation() ,LookAtRotation, params);
		bullet->m_bulletSpeed = 200;
	}
}

void AASWeapon_PlayerPrimary::OnResetCS()
{
	m_CurrentTimeToResetCS = 0;
	m_CurrentNumberOfShotsInCS = 0;
}

void AASWeapon_PlayerPrimary::OnCS1Reached()
{
}

void AASWeapon_PlayerPrimary::OnCS2Reached()
{
}

void AASWeapon_PlayerPrimary::OnCS3Reached()
{
}
