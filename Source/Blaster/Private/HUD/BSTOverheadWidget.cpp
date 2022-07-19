// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BSTOverheadWidget.h"
#include "Components/TextBlock.h"

void UBSTOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText != nullptr)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}	
}

void UBSTOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;

	switch (LocalRole)
	{
	case ROLE_Authority:
		Role = FString("Authority");
		break;
		
	case ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
		
	case ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
		
	case ROLE_None:
		Role = FString("None");
		break;
	}

	FString LocalRoleString = FString::Printf(TEXT("Local Role %s"), *Role);
	SetDisplayText(LocalRoleString);
}

void UBSTOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);	
}
