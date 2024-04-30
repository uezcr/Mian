#include "AI/MStateTreeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MStateTreeComponent)

UMStateTreeComponent::UMStateTreeComponent()
{
}

void UMStateTreeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMStateTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMStateTreeComponent::SetStateTree(UStateTree* InStateTree)
{
	StopLogic("ChangeStateTree");
	StateTreeRef.SetStateTree(InStateTree);
	InitializeComponent();
	RestartLogic();
}

void UMStateTreeComponent::SetStateTreeByTag(const FGameplayTagContainer& GameplayTagContainer)
{
	if (!GameplayTagContainer.IsValid())
	{
		return;
	}
	for(const FStateTreeWithTags& StateTreeSpec : StateTrees)
	{
		if(StateTreeSpec.StateTree&&StateTreeSpec.StateTreeTags.HasAll(GameplayTagContainer))
		{
			SetStateTree(StateTreeSpec.StateTree);
			break;
		}
	}
}

