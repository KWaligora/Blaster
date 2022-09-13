#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSTCasing.generated.h"

UCLASS()
class BLASTER_API ABSTCasing : public AActor
{
	GENERATED_BODY()

public:
	ABSTCasing();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;
};
