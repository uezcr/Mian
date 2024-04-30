#include "AI/Coordinators/MAICoordinator_Honeycomb.h"

#include "Character/MBotCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MAICoordinator_Honeycomb)

AMAICoordinator_Honeycomb::AMAICoordinator_Honeycomb(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void AMAICoordinator_Honeycomb::AddChildAIToCoordinator(AMBotCharacter* NewCharacter)
{
	if(IsValid(NewCharacter))
	{
		AIPawns.AddUnique(NewCharacter);
	}
}

void AMAICoordinator_Honeycomb::EnterNotUnderThreatState()
{
	
}

void AMAICoordinator_Honeycomb::EnterUnderThreatState()
{
	
}

void AMAICoordinator_Honeycomb::BeginCoordination()
{
	Super::BeginCoordination();
}
