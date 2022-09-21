#include "Characters/BSTCharacter.h"

#include "Blaster/Blaster.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BSTWeapon.h"
#include "Components/BSTCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

ABSTCharacter::ABSTCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;	

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FolowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UBSTCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 850.0f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void ABSTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABSTCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ABSTCharacter, CurrentHealth);
}

void ABSTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}

	HideCameraIfCharacterClose();
}

void ABSTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent != nullptr)
	{
		CombatComponent->BSTCharacter = this;
	}
}

void ABSTCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();

	SimProxiesTurn();	
	TimeSinceLastMovementReplication = 0.0f;
}

void ABSTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABSTCharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABSTCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABSTCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABSTCharacter::CrouchButtonRelease);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABSTCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABSTCharacter::AimButtonRelease);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABSTCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABSTCharacter::FireButtonReleased);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::LookUp);
}

void ABSTCharacter::MoveForward(float Value)
{
	if(Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ABSTCharacter::MoveRight(float Value)
{
	if(Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ABSTCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABSTCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABSTCharacter::EquipButtonPressed()
{
	if (CombatComponent != nullptr)
	{
		if (HasAuthority())
		{
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void ABSTCharacter::CrouchButtonPressed()
{
	Crouch();
}

void ABSTCharacter::CrouchButtonRelease()
{
	UnCrouch();
}

void ABSTCharacter::AimButtonPressed()
{
	if (CombatComponent != nullptr)
	{
		CombatComponent->SetAiming(true);
	}
}

void ABSTCharacter::AimButtonRelease()
{
	if (CombatComponent != nullptr)
	{
		CombatComponent->SetAiming(false);
	}
}

void ABSTCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.0f && !IsLocallyControlled())
	{
		FVector2D InRange(270.0f, 360.0f);
		FVector2D OutRange(-90.0f, 0.0f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

float ABSTCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0;
	return Velocity.Size();
}

void ABSTCharacter::AimOffset(float DeltaTime)
{
	if (CombatComponent != nullptr && CombatComponent->EquippedWeapon == nullptr)
	{
		return;
	}

	float Speed = CalculateSpeed();	
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.0f && !bIsInAir)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}

	if (Speed > 0.0f || bIsInAir)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0);
		AO_Yaw = 0.0f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAO_Pitch();
}

void ABSTCharacter::SimProxiesTurn()
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr)
	{
		return;
	}

	bRotateRootBone = false;
	
	float Speed = CalculateSpeed();
	if (Speed > 0.0f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if (FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if (ProxyYaw > TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshold)
		{
			TurningInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void ABSTCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.0f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.0f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	else
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.0f, DeltaTime, 4.0f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.0f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.0f, GetBaseAimRotation().Yaw, 0.0f);
		}
	}
}

void ABSTCharacter::Jump()
{

	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void ABSTCharacter::HideCameraIfCharacterClose()
{
	if(IsLocallyControlled())
	{
		// Check if camera is too close to character
		if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
		{
			GetMesh()->SetVisibility(false);
			if (CombatComponent != nullptr && CombatComponent->EquippedWeapon != nullptr)
			{
				CombatComponent->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
			}
		}
		else
		{
			GetMesh()->SetVisibility(true);
			if (CombatComponent != nullptr && CombatComponent->EquippedWeapon != nullptr)
			{
				CombatComponent->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
			}
		}
	}
}

void ABSTCharacter::FireButtonPressed()
{
	if (CombatComponent != nullptr)
	{
		CombatComponent->FireButtonPressed(true);
	}
}

void ABSTCharacter::FireButtonReleased()
{
	if (CombatComponent != nullptr)
	{
		CombatComponent->FireButtonPressed(false);
	}
}

void ABSTCharacter::PlayFireMontage(bool bAiming)
{
	if (CombatComponent == nullptr || CombatComponent->EquippedWeapon == nullptr)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABSTCharacter::PlayHitReactMontage()
{
	if (CombatComponent != nullptr && CombatComponent->EquippedWeapon != nullptr)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr && HitReactMontage)
		{
			AnimInstance->Montage_Play(HitReactMontage);
			AnimInstance->Montage_JumpToSection(FName("FromFront"));
		}
	}
}

void ABSTCharacter::OnRep_Health()
{
	
}

FVector ABSTCharacter::GetHitTarget() const
{
	if (CombatComponent == nullptr)
	{
		return FVector();
	}

	return CombatComponent->HitTarget;
}

void ABSTCharacter::Multicast_Hit_Implementation()
{
	PlayHitReactMontage();
}

void ABSTCharacter::ServerEquipButtonPressed_Implementation()
{
	if (CombatComponent != nullptr)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}

void ABSTCharacter::SetOverlappingWeapon(ABSTWeapon* Weapon)
{
	if (OverlappingWeapon != nullptr)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon != nullptr)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ABSTCharacter::OnRep_OverlappingWeapon(ABSTWeapon* LastWeapon)
{
	if (OverlappingWeapon != nullptr)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if (LastWeapon != nullptr)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

bool ABSTCharacter::IsWeaponEquipped()
{
	return (CombatComponent != nullptr && CombatComponent->EquippedWeapon != nullptr);
}

bool ABSTCharacter::IsAiming()
{
	return  (CombatComponent != nullptr && CombatComponent->bAiming);
}

ABSTWeapon* ABSTCharacter::GetEqquipedWeapon()
{
	if (CombatComponent != nullptr)
	{
		return CombatComponent->EquippedWeapon;
	}
	return nullptr;
}
