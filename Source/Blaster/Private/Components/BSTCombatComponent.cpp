#include "Components/BSTCombatComponent.h"

#include "Characters/BSTCharacter.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/BSTWeapon.h"

UBSTCombatComponent::UBSTCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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