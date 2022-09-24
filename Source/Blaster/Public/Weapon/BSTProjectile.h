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
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	UPROPERTY(EditAnywhere)
	float Damage = 20.0f;
	
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	UPROPERTY()
	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;
};
