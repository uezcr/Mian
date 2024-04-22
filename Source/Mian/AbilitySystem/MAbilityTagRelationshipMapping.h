#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "MAbilityTagRelationshipMapping.generated.h"

class UObject;

/** 定义不同GameplayTag之间关系的结构体 */
USTRUCT()
struct FMAbilityTagRelationship
{
	GENERATED_BODY()

	/** 此容器关系涉及的Tag,单个Tag,但GA可以具有多个Tag */
	UPROPERTY(EditAnywhere, Category = Ability, meta = (Categories = "Gameplay.Action"))
	FGameplayTag AbilityTag;

	/** 假设有两个GA X和Y,如果X的AbilityTagsToBlock里包含Y的Tag,当X执行时会阻止Y的执行 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	/** 假设有两个GA X和Y,如果X的AbilityTagsToBlock里包含Y的Tag,当X执行时会取消Y的执行 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	/** 如果一个GA具有该Tag,则此Tag将被隐式添加到GA的激活所需Tag中 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	/** 如果一个GA具有该Tag,则此Tag将被隐式添加到GA的阻止激活Tag中 */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationBlockedTags;
};
UCLASS()
class UMAbilityTagRelationshipMapping : public UDataAsset
{
	GENERATED_BODY()
	
private:
	/** 不同GameplayTag之间关系的列表（哪些Tag阻止或取消其他Tag） */
	UPROPERTY(EditAnywhere, Category = Ability, meta=(TitleProperty="AbilityTag"))
	TArray<FMAbilityTagRelationship> AbilityTagRelationships;

public:
	/** 给定一组GameplayTag，解析Tag关系并填充要阻止和取消的Tag */
	void GetAbilityTagsToBlockAndCancel(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutTagsToBlock, FGameplayTagContainer* OutTagsToCancel) const;

	/** 给定一组GameplayTag，添加额外的所需和阻止Tag */
	void GetRequiredAndBlockedActivationTags(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutActivationRequired, FGameplayTagContainer* OutActivationBlocked) const;

	/** 如果指定的GameplayTag由传入的动作Tag取消，则返回 true */
	bool IsAbilityCancelledByTag(const FGameplayTagContainer& AbilityTags, const FGameplayTag& ActionTag) const;
};
