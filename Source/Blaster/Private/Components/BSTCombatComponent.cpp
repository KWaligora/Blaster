#include "Components/BSTCombatComponent.h"

#include "Characters/BSTCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BSTWeapon.h"

UBSTCombatComponent::UBSTCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBSTCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBSTCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UBSTCombatComponent, bAiming);
}

void UBSTCombatComponent::EquipWeapon(ABSTWeapon* WeaponToEquip)
{
	if (BSTCharacter == nullptr || WeaponToEquip == nullptr)
	{
		return;
	}
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = BSTCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(WeaponToEquip, BSTCharacter->GetMesh());
	}
	EquippedWeapon->SetOwner(BSTCharacter);
}

void UBSTCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBSTCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (!BSTCharacter->HasAuthority())
	{
		Server_SetAiming(bIsAiming);
	}
}

void UBSTCombatComponent::Server_SetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}
