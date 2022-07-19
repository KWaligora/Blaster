#include "BSTAIontroller.h"

#include "Navigation/CrowdFollowingComponent.h"

ABSTAIontroller::ABSTAIontroller(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	
}
