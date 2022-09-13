#include "Weapon/BSTCasing.h"


ABSTCasing::ABSTCasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
}

void ABSTCasing::BeginPlay()
{
	Super::BeginPlay();
	
}
