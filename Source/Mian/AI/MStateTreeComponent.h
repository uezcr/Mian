#pragma once

#include "Components/StateTreeComponent.h"
#include "MStateTreeComponent.generated.h"

UCLASS(ClassGroup = AI, HideCategories = (Activation, Collision), meta = (BlueprintSpawnableComponent))
class MIAN_API UMStateTreeComponent : public UStateTreeComponent
{
	GENERATED_BODY()

public:
	UMStateTreeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="MStateTreeComponent")
	void SetStateTree(UStateTree* InStateTree);
	
};
