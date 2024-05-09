#pragma once

#include "Components/StateTreeComponent.h"
#include "MStateTreeComponent.generated.h"

class AMPatrolRoute;

USTRUCT()
struct FStateTreeWithTags
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category ="StateTreeWithTags", meta=(Categories="StateTree"))
	FGameplayTagContainer StateTreeTags;
	UPROPERTY(EditAnywhere, Category ="StateTreeWithTags")
	TObjectPtr<UStateTree> StateTree;
};

UENUM(BlueprintType)
enum class EPatrolType : uint8
{
	Single       = 0,
	Looping      = 1,
	BackAndForth = 2
};

UCLASS(ClassGroup = AI, HideCategories = (Activation, Collision, AI), meta = (BlueprintSpawnableComponent))
class MIAN_API UMStateTreeComponent : public UStateTreeComponent
{
	GENERATED_BODY()

public:
	UMStateTreeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:


protected:
	UPROPERTY(EditAnywhere, Category ="StateTreeComponent|Settings")
	TArray<FStateTreeWithTags> StateTrees;

	//路径样条线实例.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StateTreeComponent|Patrol")
	TObjectPtr<AMPatrolRoute> PatrolRoute;
	//路径点编号.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="StateTreeComponent|Patrol")
	int32 PatrolSplineIndex = INDEX_NONE;
	//路径巡逻方向.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="StateTreeComponent|Patrol")
	bool PatrolDirection = true;
	//路径巡逻类型.
	UPROPERTY(EditAnywhere,Category="StateTreeComponent|Patrol")
	EPatrolType PatrolType;

public:
	UFUNCTION(BlueprintCallable,Category="StateTreeComponent")
	void SetStateTree(UStateTree* InStateTree);
	UFUNCTION(BlueprintCallable,Category="StateTreeComponent")
	void SetStateTreeByTagContainer(const FGameplayTagContainer& GameplayTagContainer);
	UFUNCTION(BlueprintCallable,Category="StateTreeComponent")
	void SetStateTreeByTag(const FGameplayTag& GameplayTag);

	UFUNCTION(BlueprintCallable,Category="StateTreeComponent|Patrol")
	void FindNextPatrolLocation(FVector& TargetLocation);
};
