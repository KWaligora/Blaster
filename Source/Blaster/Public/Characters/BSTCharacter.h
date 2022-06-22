#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BSTCharacter.generated.h"

UCLASS()
class BLASTER_API ABSTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABSTCharacter();

protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;
	
	/*========================================================================
	 * *                         Components
	 *  ==========================================================================*/
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	/*========================================================================
	 * *                         Movement
	 *  ==========================================================================*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
};
