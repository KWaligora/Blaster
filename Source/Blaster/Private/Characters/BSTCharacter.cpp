#include "Characters/BSTCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/BSTWeapon.h"
#include "Components/BSTCombatComponent.h"

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
}

void ABSTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABSTCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ABSTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABSTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent != nullptr)
	{
		CombatComponent->BSTCharacter = this;
	}
}

void ABSTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABSTCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABSTCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABSTCharacter::CrouchButtonRelease);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABSTCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABSTCharacter::AimButtonRelease);
	
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
