#pragma once

#include "GameFramework/Actor.h"
#include "MAICoordinator.generated.h"

class UBillboardComponent;
class UMStateTreeComponent;

UCLASS(Abstract,NotBlueprintable)
class MIAN_API AMAICoordinator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMAICoordinator(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere,Category="MAICoordinator")
	TObjectPtr<UBillboardComponent> CoordinatorScene = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AICoordinator", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMStateTreeComponent> StateTree;

};
