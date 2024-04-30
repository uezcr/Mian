#pragma once

#include "LocalizationDescriptor.h"
#include "AI/MAICoordinator.h"
#include "MAICoordinator_Honeycomb.generated.h"

class AMBotCharacter;

UCLASS(Abstract,Blueprintable)
class MIAN_API AMAICoordinator_Honeycomb : public AMAICoordinator
{
	GENERATED_BODY()

public:
	AMAICoordinator_Honeycomb(const FObjectInitializer& ObjectInitializer);

public:
	//设置是否受威胁.
	UFUNCTION(BlueprintCallable,Category="AICoordinator|RuntimeFunctions")
	void SetThreatened(const bool NewThreatened) {bThreatened = NewThreatened;}
	//是否受威胁.
	UFUNCTION(BlueprintCallable,Category="AICoordinator|RuntimeFunctions")
	bool IsCoreThreatened() const {return bThreatened;}
	//添加AI到协调器.
	UFUNCTION(BlueprintCallable,Category="AICoordinator|RuntimeFunctions")
	void AddChildAIToCoordinator(AMBotCharacter* NewCharacter);
	//进入安全状态.
	UFUNCTION(BlueprintCallable,Category="AICoordinator|RuntimeFunctions")
	void EnterNotUnderThreatState();
	//进入威胁状态.
	UFUNCTION(BlueprintCallable,Category="AICoordinator|RuntimeFunctions")
	void EnterUnderThreatState();
	
protected:
	//是否使用一个Actor作为逻辑核心.
	UPROPERTY(EditAnywhere,Category="AICoordinator|Settings",meta=(DisplayName="使用Actor作为逻辑核心"))
	bool bActorCore = false;
	//逻辑核心Actor.
	UPROPERTY(EditAnywhere,Category="AICoordinator|Settings",meta=(DisplayName="逻辑核心Actor",EditCondition="bActorCore"))
	TObjectPtr<AActor> LogicCoreActor = nullptr;
	//核心是否受到威胁.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AICoordinator|RuntimeParams",meta=(DisplayName="核心受到威胁"))
	bool bThreatened = false;
	//接受控制的AI.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AICoordinator|RuntimeParams",meta=(DisplayName="核心受到威胁"))
	TArray<TObjectPtr<AMBotCharacter>> AIPawns;

protected:
	virtual void BeginCoordination() override;
};
