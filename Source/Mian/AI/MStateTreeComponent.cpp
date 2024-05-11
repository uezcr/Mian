#include "AI/MStateTreeComponent.h"
#include "MPatrolRoute.h"
#include "Components/SplineComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MStateTreeComponent)

UMStateTreeComponent::UMStateTreeComponent(): PatrolType(EPatrolType::BackAndForth)
{
	bStartLogicAutomatically = false;
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

void UMStateTreeComponent::SetStateTreeByTagContainer(const FGameplayTagContainer& GameplayTagContainer)
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

void UMStateTreeComponent::SetStateTreeByTag(const FGameplayTag& GameplayTag)
{
	if(!GameplayTag.IsValid())
	{
		return;
	}
	for(const FStateTreeWithTags& StateTreeSpec : StateTrees)
	{
		if(StateTreeSpec.StateTree&&StateTreeSpec.StateTreeTags.HasTag(GameplayTag))
		{
			SetStateTree(StateTreeSpec.StateTree);
			break;
		}
	}
}

void UMStateTreeComponent::FindNextPatrolLocation(FVector& TargetLocation)
{
	if(!IsValid(PatrolRoute)) return;
	const USplineComponent* Spline = PatrolRoute->Spline;
	if(!IsValid(Spline)) return;
	int32 PointIndex = 0;
	if(PatrolType == EPatrolType::BackAndForth)
	{
		if(PatrolDirection)
		{
			if(++PatrolSplineIndex >= Spline->GetNumberOfSplinePoints())
			{
				PatrolDirection = false;
			}
		}
		else
		{
			if(--PatrolSplineIndex < 0)
			{
				PatrolDirection = true;
			}
		}
		PointIndex = PatrolSplineIndex;
	}
	else if(PatrolType == EPatrolType::Single)
	{
		++PatrolSplineIndex;
		PointIndex = PatrolSplineIndex;
	}
	else
	{
		++PatrolSplineIndex;
		PointIndex = PatrolSplineIndex % Spline->GetNumberOfSplinePoints();
	}
	TargetLocation = Spline->GetLocationAtSplinePoint(PointIndex,ESplineCoordinateSpace::World);
}

