#pragma once

#include "AI/MAICoordinator.h"
#include "MAICoordinator_Honeycomb.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MIAN_API AMAICoordinator_Honeycomb : public AMAICoordinator
{
	GENERATED_BODY()

public:
	AMAICoordinator_Honeycomb(const FObjectInitializer& ObjectInitializer);


protected:
	UPROPERTY(EditAnywhere,Category="MAICoordinator|Settings",meta=(DisplayName="使用Actor作为逻辑核心"))
	bool bActorCore = false;
	UPROPERTY(EditAnywhere,Category="MAICoordinator|Settings",meta=(DisplayName="逻辑核心Actor",EditCondition="bActorCore"))
	TObjectPtr<AActor> LogicCoreActor = nullptr;


	
};
