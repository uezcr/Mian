#pragma once

#include "AI/MAICoordinator.h"
#include "MAICoordinator_Honeycomb.generated.h"

UCLASS(Abstract,Blueprintable)
class MIAN_API AMAICoordinator_Honeycomb : public AMAICoordinator
{
	GENERATED_BODY()

public:
	AMAICoordinator_Honeycomb(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable,Category="MAICoordinator|RuntimeFunctions")
	void SetThreatened(const bool NewThreatened) {bThreatened = NewThreatened;}
	const bool IsCoreThreatened() const {return bThreatened;}
	
protected:
	//是否使用一个Actor作为逻辑核心.
	UPROPERTY(EditAnywhere,Category="MAICoordinator|Settings",meta=(DisplayName="使用Actor作为逻辑核心"))
	bool bActorCore = false;
	//逻辑核心Actor.
	UPROPERTY(EditAnywhere,Category="MAICoordinator|Settings",meta=(DisplayName="逻辑核心Actor",EditCondition="bActorCore"))
	TObjectPtr<AActor> LogicCoreActor = nullptr;
	//核心是否受到威胁.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="MAICoordinator|RuntimeParams",meta=(DisplayName="核心受到威胁"))
	bool bThreatened = false;
};
