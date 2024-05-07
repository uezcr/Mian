#pragma once

#include "Components/StateTreeComponent.h"
#include "MStateTreeComponent.generated.h"

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

protected:
	UPROPERTY(EditAnywhere, Category ="StateTreeComponent")
	TArray<FStateTreeWithTags> StateTrees;

public:
	UFUNCTION(BlueprintCallable,Category="StateTreeComponent")
	void SetStateTree(UStateTree* InStateTree);
	UFUNCTION(BlueprintCallable,Category="StateTreeComponent")
	void SetStateTreeByTag(const FGameplayTagContainer& GameplayTagContainer);
	void SetStateTreeByTag(const FGameplayTag& GameplayTag);
};
