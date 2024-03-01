#include "ArenaShooter/Components/ASWeaponComponent.h"

#include "ArenaShooter/AI/ASPawn.h"
#include "ArenaShooter/Character/ASCharacter.h"
#include "ArenaShooter/Weapons/ASWeapon.h"

UASWeaponComponent::UASWeaponComponent()
{
}

void UASWeaponComponent::Fire(FVector fireOrigin, FVector fireDirection)
{
	if(IsValid(m_CurrentEquipedWeapon))
	{
		if(!m_CurrentEquipedWeapon->m_IsReloading && !m_CurrentEquipedWeapon->m_IsWaitingForFireDelay && (!IsValid(m_ASPawnOwner) || !m_ASPawnOwner->GetIsStunned()))
		{
			m_CurrentEquipedWeapon->Fire(fireOrigin, fireDirection);
			//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, TEXT("Fire"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO CURRENT WEAPON ON %s"), *GetOwner()->GetName());
	}
}


void UASWeaponComponent::Reload()
{
	if(!m_CurrentEquipedWeapon->m_IsReloading && m_CurrentEquipedWeapon->m_EnableReload)
	{
		m_CurrentEquipedWeapon->Reload();
	}
}

void UASWeaponComponent::SwitchWeapon()
{
	if(m_isPrimaryWeaponEquiped)
	{
		m_CurrentEquipedWeapon->Stach();
		m_CurrentEquipedWeapon = m_SecondaryWeapon;
		m_CurrentEquipedWeapon->Equip();
	}
	else
	{
		m_CurrentEquipedWeapon->Stach();
		m_CurrentEquipedWeapon = m_PrimaryWeapon;
		m_CurrentEquipedWeapon->Equip();
	}
}

void UASWeaponComponent::InitializeWeapon()
{
	Super::InitializeComponent();
	
	FActorSpawnParameters params;
	params.ObjectFlags |= RF_Transient;
	params.bNoFail = true;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	if(IsValid(m_primaryWeaponBlueprint))
	{
		m_PrimaryWeapon = GetWorld()->SpawnActor<AASWeapon>(m_primaryWeaponBlueprint, params);
		if(IsValid(m_PrimaryWeapon))
		{
			//m_PrimaryWeapon->SetHidden(true);
			if(GetOwner()->IsA<ACharacter>())
			{
				USceneComponent* parent = CastChecked<ACharacter>(GetOwner())->GetMesh();
				m_PrimaryWeapon->Owner = parent->GetAttachmentRootActor();
				m_PrimaryWeapon->AttachToComponent(parent, AttachmentRules, TEXT("WeaponSocket"));
			}
			else
			{
				m_PrimaryWeapon->Owner = GetOwner();
				m_PrimaryWeapon->AttachToActor(GetOwner(), AttachmentRules);
				m_PrimaryWeapon->SetActorScale3D(m_ScaleOffset);
				m_PrimaryWeapon->SetActorRotation(FRotator::MakeFromEuler(m_RotationOffset));
				m_PrimaryWeapon->SetActorLocation(GetOwner()->GetActorLocation() + m_PositionOffset);
			}
			m_CurrentEquipedWeapon = m_PrimaryWeapon;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Primary Weapon Spawned"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Blueprint for primary Weapon"));
		return;
	}

	/*if(IsValid(m_secondaryWeaponBlueprint))
	{
		m_SecondaryWeapon = GetWorld()->SpawnActor<AASWeapon>(m_secondaryWeaponBlueprint, params);
		if(IsValid(m_SecondaryWeapon))
		{
			m_SecondaryWeapon->SetHidden(true);
			m_SecondaryWeapon->Owner = parent->GetAttachmentRootActor();
			m_SecondaryWeapon->AttachToComponent(parent, AttachmentRules, TEXT("WeaponSocket"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Secondary Weapon Spawned"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Blueprint for secondary Weapon"));
		return;
	}
	
	m_CurrentEquipedWeapon = m_isPrimaryWeaponEquiped ? m_PrimaryWeapon :  m_SecondaryWeapon;
	m_CurrentEquipedWeapon->SetHidden(false);*/
}
