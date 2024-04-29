#include "AI/Evaluators/STE_CheckCoreThreatened.h"
#include "Mian/AI/Coordinators/MAICoordinator_Honeycomb.h"

USTE_CheckCoreThreatened::USTE_CheckCoreThreatened(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void USTE_CheckCoreThreatened::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);
	CheckCoreThreatened();
}

void USTE_CheckCoreThreatened::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);
}

void USTE_CheckCoreThreatened::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	CheckCoreThreatened();
}

void USTE_CheckCoreThreatened::CheckCoreThreatened()
{
	if(IsValid(Actor))
	{
		AMAICoordinator_Honeycomb* Coordinator = Cast<AMAICoordinator_Honeycomb>(Actor);
		if(IsValid(Coordinator))
		{
			if(Coordinator->IsCoreThreatened())
			{
				bThreatened = true;
			}
			else
			{
				bThreatened = false;
			}
		}
	}
}
