#include "Weapon/BSTCasing.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


ABSTCasing::ABSTCasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	CasingMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	
	SetRootComponent(CasingMesh);
}

void ABSTCasing::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CasingMesh->OnComponentHit.AddDynamic(this, &ABSTCasing::OnHit);
}

void ABSTCasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

void ABSTCasing::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
	
	Destroy();
}
