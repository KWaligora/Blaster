#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSTProjectile.generated.h"

UCLASS()
class BLASTER_API ABSTProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABSTProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
};
