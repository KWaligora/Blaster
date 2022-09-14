#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSTCasing.generated.h"

UCLASS()
class BLASTER_API ABSTCasing : public AActor
{
	GENERATED_BODY()

public:
	ABSTCasing();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;

	UPROPERTY(EditAnywhere)
	float ShellEjectionImpulse = 10.0f;

	UPROPERTY(EditAnywhere)
	class USoundCue* ShellSound;
};
