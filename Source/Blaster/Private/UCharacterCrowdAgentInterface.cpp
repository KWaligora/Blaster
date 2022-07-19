// Fill out your copyright notice in the Description page of Project Settings.

#include "UCharacterCrowdAgentInterface.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/CrowdManager.h"

UUCharacterCrowdAgentInterface::UUCharacterCrowdAgentInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void UUCharacterCrowdAgentInterface::InitializeComponent()
{
	Super::InitializeComponent();
	
	UCrowdManager* CrowdManager =  UCrowdManager::GetCurrent(GetWorld());
	if (CrowdManager != nullptr)
	{
		ICrowdAgentInterface* IAgent = Cast<ICrowdAgentInterface>(this);
		CrowdManager->RegisterAgent(IAgent);
	}	
}

void UUCharacterCrowdAgentInterface::BeginPlay()
{
	Super::BeginPlay();
}

FVector UUCharacterCrowdAgentInterface::GetCrowdAgentLocation() const
{
	return GetOwner()->GetActorLocation();
}

FVector UUCharacterCrowdAgentInterface::GetCrowdAgentVelocity() const
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	return Owner->GetCharacterMovement()->Velocity;
}

void UUCharacterCrowdAgentInterface::GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	Owner->GetCharacterMovement()->UpdatedComponent->CalcBoundingCylinder(CylinderRadius, CylinderHalfHeight);
}

float UUCharacterCrowdAgentInterface::GetCrowdAgentMaxSpeed() const
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	return Owner->GetCharacterMovement()->GetMaxSpeed();
}

int32 UUCharacterCrowdAgentInterface::GetCrowdAgentAvoidanceGroup() const
{
	return 1;
}

int32 UUCharacterCrowdAgentInterface::GetCrowdAgentGroupsToAvoid() const
{
	return MAX_int32;
}

int32 UUCharacterCrowdAgentInterface::GetCrowdAgentGroupsToIgnore() const
{
	return 0;
}
