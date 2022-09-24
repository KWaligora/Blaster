// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BSTProjectileBullet.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABSTProjectileBullet::ABSTProjectileBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABSTProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSTProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& HitResult)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AController* OwnerController = OwnerCharacter->Controller;
		if (OwnerController)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
		}
	}
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, HitResult);
}

// Called every frame
void ABSTProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

