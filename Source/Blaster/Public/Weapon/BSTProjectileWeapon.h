#pragma once

#include "CoreMinimal.h"
#include "Weapon/BSTWeapon.h"
#include "BSTProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABSTProjectileWeapon : public ABSTWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& HitTarget) override;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABSTProjectile> ProjectileClass;
};
