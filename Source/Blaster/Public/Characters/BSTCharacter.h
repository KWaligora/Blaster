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
	void AimOffset(float DeltaTime);
	/*===================================================================*/
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

public:
	void SetOverlappingWeapon(ABSTWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	ABSTWeapon* GetEqquipedWeapon();
	FORCEINLINE float GetAO_YAW() const {return AO_Yaw;}
	FORCEINLINE float GetAO_Pitch() const {return AO_Pitch;}
	
private:
	UFUNCTION()
	void OnRep_OverlappingWeapon(ABSTWeapon* LastWeapon);

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
};
