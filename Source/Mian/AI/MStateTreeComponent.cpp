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
	StateTreeRef.SetStateTree(InStateTree);
	InitializeComponent();
}
