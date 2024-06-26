#include "AI/MAICoordinator.h"

#include "Components/BillboardComponent.h"
#include "Components/StateTreeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MAICoordinator)

AMAICoordinator::AMAICoordinator(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	CoordinatorScene = CreateDefaultSubobject<UBillboardComponent>(TEXT("CoordinatorScene"));
	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}

// Called when the game starts or when spawned
void AMAICoordinator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMAICoordinator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMAICoordinator::BeginCoordination()
{
}

