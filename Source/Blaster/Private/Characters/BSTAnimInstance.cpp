#include "Characters/BSTAnimInstance.h"

#include "Characters/BSTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	bIsCrouched = BSTCharacter->bIsCrouched;
}
