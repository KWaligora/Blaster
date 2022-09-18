#include "Components/BSTCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/BSTCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/BSTHUD.h"
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

	if (BSTCharacter != nullptr && BSTCharacter->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCorsshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshair(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

void UBSTCombatComponent::SetHUDCrosshair(float DeltaTime)
{
	if (BSTCharacter == nullptr || BSTCharacter->Controller == nullptr)
	{
		return;
	}

	BSTPlayerController = BSTPlayerController == nullptr ? Cast<ABSTPlayerController>(BSTCharacter->Controller) : BSTPlayerController;	
	if (BSTPlayerController != nullptr)
	{
		BSTHUD = BSTHUD == nullptr ? Cast<ABSTHUD>(BSTPlayerController->GetHUD()) : BSTHUD;
		if (BSTHUD != nullptr && EquippedWeapon != nullptr)
		{
			FHUDPackage HUDPackage;
			
			if (EquippedWeapon != nullptr)
			{
				HUDPackage.CrosshairCenter = EquippedWeapon->CrosshairCenter;
				HUDPackage.CrosshairLeft = EquippedWeapon->CrosshairLeft;
				HUDPackage.CrosshairRight = EquippedWeapon->CrosshairRight;
				HUDPackage.CrosshairBottom = EquippedWeapon->CrosshairBottom;
				HUDPackage.CrosshairTop = EquippedWeapon->CrosshairTop;
			}
			else
			{
				HUDPackage.CrosshairCenter = nullptr;
				HUDPackage.CrosshairLeft = nullptr;
				HUDPackage.CrosshairRight = nullptr;
				HUDPackage.CrosshairBottom = nullptr;
				HUDPackage.CrosshairTop = nullptr;
			}

			FVector2D WalkSpeedRange(0.0f, BSTCharacter->GetMovementComponent()->GetMaxSpeed());
			FVector2D VelocityMultiplayerRange(0.0f, 1.0f);
			FVector Velocity = BSTCharacter->GetVelocity();
			Velocity.Z = 0.0f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplayerRange, Velocity.Size());

			if(BSTCharacter->GetMovementComponent()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.0f, DeltaTime, 30.0f);
			}
			
			HUDPackage.CrosshairSpreed = CrosshairVelocityFactor + CrosshairInAirFactor;
			
			BSTHUD->HUDPackage = HUDPackage;
		}
	}
}

void UBSTCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon != nullptr)
	{
		if (bAiming)
		{
			CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomedFOV(), DeltaTime, EquippedWeapon->GetZoomedInterpSpeed());
		}
		else
		{
			CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
		}

		if (BSTCharacter != nullptr && BSTCharacter->GetFollowCamera())
		{
			BSTCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV);
		}
	}
}

void UBSTCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BSTCharacter != nullptr)
	{
		BSTCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if (BSTCharacter->GetFollowCamera() != nullptr)
		{
			CurrentFOV = DefaultFOV = BSTCharacter->GetFollowCamera()->FieldOfView;
		}
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
