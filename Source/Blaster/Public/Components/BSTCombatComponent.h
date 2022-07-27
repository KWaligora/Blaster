#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BSTCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBSTCombatComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ABSTCharacter;
	
public:	
	UBSTCombatComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ABSTCharacter* BSTCharacter;
	UPROPERTY()
	class ABSTWeapon* Weapon;
};
