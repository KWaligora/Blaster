#include "Weapon/BSTProjectileWeapon.h"

#include "Weapon/BSTProjectile.h"

void ABSTProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	
	FTransform SocketTransform = GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"));
	FRotator FireDirection = (HitTarget - SocketTransform.GetLocation()).Rotation();
	
	if (ProjectileClass && InstigatorPawn)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.Instigator = InstigatorPawn;
		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<ABSTProjectile>(ProjectileClass, SocketTransform.GetLocation(), FireDirection, SpawnParameters);
		}
	}
}
