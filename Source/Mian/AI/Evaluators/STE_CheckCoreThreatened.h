#pragma once

#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "STE_CheckCoreThreatened.generated.h"

UCLASS(NotBlueprintable)
class MIAN_API USTE_CheckCoreThreatened : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()

public:
	USTE_CheckCoreThreatened(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;
	virtual void TreeStop(FStateTreeExecutionContext& Context) override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Context")
	TObjectPtr<AActor> Actor;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Output")
	bool bThreatened = false;

private:
	void CheckCoreThreatened();
	
};
