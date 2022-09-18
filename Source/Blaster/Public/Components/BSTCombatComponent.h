#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerControllers/BSTPlayerController.h"
#include "BSTCombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBSTCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ABSTCharacter;
	
public:	
	UBSTCombatComponent();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void EquipWeapon(class ABSTWeapon* WeaponToEquip);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);
	
	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void Server_Fire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire(const FVector_NetQuantize& TraceHitTarget);
	
	void TraceUnderCorsshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshair(float DeltaTime);

private:
	UPROPERTY()
	ABSTCharacter* BSTCharacter;

	UPROPERTY()
	ABSTPlayerController* BSTPlayerController;

	UPROPERTY()
	class ABSTHUD* BSTHUD;
	
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	class ABSTWeapon* EquippedWeapon;
	
	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/*========================================================================
	 *                         HUD and Crosshair
	  ==========================================================================*/
private:
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;

	FVector HitTarget;

	/*========================================================================
	 * *                         Aiming and FOV
	 *  ==========================================================================*/

	float DefaultFOV;
	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed = 20.0f;

	void InterpFOV(float DeltaTime);
};
