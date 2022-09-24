// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BSTProjectile.h"
#include "BSTProjectileBullet.generated.h"

UCLASS()
class BLASTER_API ABSTProjectileBullet : public ABSTProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABSTProjectileBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult) override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
