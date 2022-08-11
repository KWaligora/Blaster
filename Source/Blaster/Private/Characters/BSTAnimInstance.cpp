#include "Characters/BSTAnimInstance.h"

#include "Characters/BSTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/BSTWeapon.h"

void UBSTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BSTCharacter = Cast<ABSTCharacter>(TryGetPawnOwner());
	if (!IsValid(BSTCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("BSTCharacter is not valid"));
		return;
	}
	MovementComponent = BSTCharacter->GetCharacterMovement();
	ensure(MovementComponent != nullptr);
}

void UBSTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!IsValid(BSTCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("BSTCharacter is not valid"));
		return;
	}
	
	FVector Velocity = BSTCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	Speed = Velocity.Size();

	bFalling = MovementComponent->IsFalling();
	bIsAccelerating = MovementComponent->GetCurrentAcceleration().Size() > 0.0f;
	bWeaponEquipped = BSTCharacter->IsWeaponEquipped();
	EquippedWeapon = BSTCharacter->GetEqquipedWeapon();
	bIsCrouched = BSTCharacter->bIsCrouched;
	bAiming = BSTCharacter->IsAiming();

	FRotator AimRotation = BSTCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BSTCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 5.0f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BSTCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.0f);
	Lean = FMath::Clamp(Interp, -90.0f, 90.0f);

	AO_Yaw = BSTCharacter->GetAO_YAW();
	AO_Pitch = BSTCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon != nullptr && EquippedWeapon->GetWeaponMesh() != nullptr && BSTCharacter->GetMesh() != nullptr)
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
		FVector OutPosition;
		FRotator OutRotation; 
		BSTCharacter->GetMesh()->TransformToBoneSpace(FName("Hand_R"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
}
