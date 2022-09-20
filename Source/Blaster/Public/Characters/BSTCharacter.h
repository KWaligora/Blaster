#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractWithCrosshair.h"
#include "Types/BlasterTypes.h"
#include "BSTCharacter.generated.h"

UCLASS()
class BLASTER_API ABSTCharacter : public ACharacter, public IInteractWithCrosshair
{
	GENERATED_BODY()

public:
	ABSTCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	
	/*========================================================================
	 * *                         Components
	 *  ==========================================================================*/
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class ABSTWeapon* OverlappingWeapon;
	
	UPROPERTY(VisibleAnywhere)
	class UBSTCombatComponent* CombatComponent;

	/*========================================================================
	 * *                         Movement
	 *  ==========================================================================*/
public:
	FORCEINLINE bool ShouldRotateRootBone() const {return bRotateRootBone;}
	
	virtual void OnRep_ReplicatedMovement() override;
	
protected:
	ETurningInPlace TurningInPlace;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void CrouchButtonRelease();
	void AimButtonPressed();
	void AimButtonRelease();
	void CalculateAO_Pitch();
	float CalculateSpeed();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	void TurnInPlace(float DeltaTime);
	virtual void Jump() override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	float CameraThreshold = 200.0f;
	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	
	void HideCameraIfCharacterClose();

	/*===================================================================*/
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	/*========================================================================
	 * *                         Weapon
	 *  ==========================================================================*/	
public:

	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* FireWeaponMontage;
	
	void SetOverlappingWeapon(ABSTWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	ABSTWeapon* GetEqquipedWeapon();
	void FireButtonPressed();
	void FireButtonReleased();
	void PlayFireMontage(bool bAiming);
	FVector GetHitTarget() const;

protected:
	void PlayHitReactMontage();
	/*========================================================================
	 * *                         Combat
	 *  ==========================================================================*/
public:
	UPROPERTY(EditAnywhere, Category="Combat")
	UAnimMontage* HitReactMontage;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Hit();

	/*===================================================================*/
	
	FORCEINLINE float GetAO_YAW() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return AO_Pitch;}
	FORCEINLINE ETurningInPlace GetTurningInPlace() const {return TurningInPlace;}
	FORCEINLINE UCameraComponent* GetFollowCamera() const {return FollowCamera;}
	
private:
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	UFUNCTION()
	void OnRep_OverlappingWeapon(ABSTWeapon* LastWeapon);
};



