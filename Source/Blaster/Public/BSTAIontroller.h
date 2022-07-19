// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BSTAIontroller.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABSTAIontroller : public AAIController
{
	GENERATED_BODY()
	
public:
	ABSTAIontroller(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
