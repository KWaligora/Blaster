#include "Weapon/BSTProjectile.h"

#include "Characters/BSTCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Blaster/Blaster.h"

ABSTProjectile::ABSTProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 600.0f;
	ProjectileMovementComponent->MaxSpeed = 600.0f;
}

void ABSTProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer != nullptr)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition);
	}
}

void ABSTProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ABSTProjectile::OnHit);
	}
}

void ABSTProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& HitResult)
{
	ABSTCharacter* BSTCharacter = Cast<ABSTCharacter>(OtherActor);
	if (BSTCharacter != nullptr)
	{
		BSTCharacter->Multicast_Hit();
	}
	
	Destroy();
}

void ABSTProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABSTProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorTransform());
	}

	if (ImpactSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}	
}

