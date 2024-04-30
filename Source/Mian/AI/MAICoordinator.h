#pragma once

#include "GameFramework/Actor.h"
#include "MAICoordinator.generated.h"

class UBillboardComponent;
class UMStateTreeComponent;

UENUM(BlueprintType)
enum class EAIPreferenceState : uint8
{
	EAIPREFERSTATE_IDLE      = 0,
	EAIPREFERSTATE_PATROL    = 1,
	EAIPREFERSTATE_OBSERVE   = 2,
	EAIPREFERSTATE_MELEE     = 3,
	EAIPREFERSTATE_RANGE     = 4,
	EAIPREFERSTATE_SUPPORT   = 5,
	EAIPREFERSTATE_AVOIDANCE = 6
};

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

protected:
	UFUNCTION(BlueprintCallable,Category="AICoordinator|RuntimeFunctions")
	virtual void BeginCoordination();

};
