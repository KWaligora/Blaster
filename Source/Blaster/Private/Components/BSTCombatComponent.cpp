#include "Components/BSTCombatComponent.h"

UBSTCombatComponent::UBSTCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBSTCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

