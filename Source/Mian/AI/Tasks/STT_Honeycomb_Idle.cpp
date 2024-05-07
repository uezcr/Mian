#include "AI/Tasks/STT_Honeycomb_Idle.h"

#include "AI/MStateTreeComponent.h"
#include "AI/Coordinators/MAICoordinator_Honeycomb.h"
#include "Character/MBotCharacter.h"
#include "MGameplayTags.h"

EStateTreeRunStatus USTT_Honeycomb_Idle::EnterState(FStateTreeExecutionContext& Context,
                                                    const FStateTreeTransitionResult& Transition)
{
	AMAICoordinator_Honeycomb* Coordinator = CastChecked<AMAICoordinator_Honeycomb>(Actor);
	if(IsValid(Coordinator))
	{
		for(AMBotCharacter* Bot : Coordinator->GetAIPawns())
		{
			if(UMStateTreeComponent* StateTree = Bot->FindComponentByClass<UMStateTreeComponent>())
			{
				StateTree->SetStateTreeByTag(MGameplayTags::StateTree_AIState_Patrol);
			}
		}
	}
	return Super::EnterState(Context, Transition);
}

void USTT_Honeycomb_Idle::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}

void USTT_Honeycomb_Idle::StateCompleted(FStateTreeExecutionContext& Context,
	const EStateTreeRunStatus CompletionStatus, const FStateTreeActiveStates& CompletedActiveStates)
{
	Super::StateCompleted(Context, CompletionStatus, CompletedActiveStates);
}

EStateTreeRunStatus USTT_Honeycomb_Idle::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	return Super::Tick(Context, DeltaTime);
}
