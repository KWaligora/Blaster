#include "Components/BSTCombatComponent.h"

#include "Characters/BSTCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BSTWeapon.h"

UBSTCombatComponent::UBSTCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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

void UBSTCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
	if(bFireButtonPressed)
	{
		FHitResult HitResult;
		TraceUnderCorsshairs(HitResult);
		
		Server_Fire(HitResult.ImpactPoint);
	}	
}

void UBSTCombatComponent::TraceUnderCorsshairs(FHitResult& TraceHitResult)
{
	FVector2d ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2d CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
		);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * 8000.0f;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECC_Visibility
			);
	}
;}

void UBSTCombatComponent::Multicast_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(EquippedWeapon != nullptr)
	{
		if (BSTCharacter != nullptr)
		{
			BSTCharacter->PlayFireMontage(bAiming);
			EquippedWeapon->Fire(TraceHitTarget);
		}
	}	
}

void UBSTCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	Multicast_Fire(TraceHitTarget);
}

void UBSTCombatComponent::Server_SetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (BSTCharacter)
	{
		BSTCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}
