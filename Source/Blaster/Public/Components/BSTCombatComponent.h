#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);
	
	UFUNCTION(Server, Reliable)
	void Server_SetAiming(bool bIsAiming);

private:
	UPROPERTY()
	ABSTCharacter* BSTCharacter;
	
	UPROPERTY(Replicated)
	class ABSTWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;
};
