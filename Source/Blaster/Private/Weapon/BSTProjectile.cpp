#include "Weapon/BSTProjectile.h"

#include "Components/BoxComponent.h"

ABSTProjectile::ABSTProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

void ABSTProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABSTProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

