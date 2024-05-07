#pragma once

#include "GameFramework/Actor.h"
#include "MPatrolRoute.generated.h"

class USplineComponent;
class UBillboardComponent;

UCLASS()
class MIAN_API AMPatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPatrolRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatrolRoute")
    TObjectPtr<UBillboardComponent> Billboard;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatrolRoute")
	TObjectPtr<USplineComponent> Spline;
};
