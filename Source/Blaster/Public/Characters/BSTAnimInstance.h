#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BSTAnimInstance.generated.h"

/**
 * Base class for Anim Instance
 */
UCLASS()
class BLASTER_API UBSTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess = "true"))
	class ABSTCharacter* BSTCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	bool bFalling;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	
	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = "true"))
	bool bAiming;
};
