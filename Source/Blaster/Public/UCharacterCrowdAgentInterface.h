// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/CrowdAgentInterface.h"
#include "UCharacterCrowdAgentInterface.generated.h"


UCLASS( BlueprintType )
class BLASTER_API UUCharacterCrowdAgentInterface : public UActorComponent, public ICrowdAgentInterface
{
	GENERATED_BODY()
	
public:
	UUCharacterCrowdAgentInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	virtual FVector GetCrowdAgentLocation() const override;
	virtual FVector GetCrowdAgentVelocity() const override;
	virtual void GetCrowdAgentCollisions(float& CylinderRadius, float& CylinderHalfHeight) const override;
	virtual float GetCrowdAgentMaxSpeed() const override;
	virtual int32 GetCrowdAgentAvoidanceGroup() const override;
	virtual int32 GetCrowdAgentGroupsToAvoid() const override;
	virtual int32 GetCrowdAgentGroupsToIgnore() const override;
};
