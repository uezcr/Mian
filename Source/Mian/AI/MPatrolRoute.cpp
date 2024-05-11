#include "AI/MPatrolRoute.h"

#include "Components/BillboardComponent.h"
#include "Components/SplineComponent.h"

AMPatrolRoute::AMPatrolRoute()
{
	PrimaryActorTick.bCanEverTick = false;
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	SetRootComponent(Billboard);
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(Billboard);
}

// Called when the game starts or when spawned
void AMPatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMPatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

