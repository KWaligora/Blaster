#include "Components/BSTCombatComponent.h"

#include "Characters/BSTCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BSTWeapon.h"

UBSTCombatComponent::UBSTCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseWalkSpeed = 600.0f;
	AimWalkSpeed = 450.0f;
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
	BSTCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	BSTCharacter->bUseControllerRotationYaw = true;
}

void UBSTCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BSTCharacter)
	{
		BSTCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void UBSTCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	Server_SetAiming(bIsAiming);
	if (BSTCharacter)
	{
		BSTCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void UBSTCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && BSTCharacter)
	{
		BSTCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		BSTCharacter->bUseControllerRotationYaw = true;
	}
}

void UBSTCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
}

void UBSTCombatComponent::Server_SetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (BSTCharacter)
	{
		BSTCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}
