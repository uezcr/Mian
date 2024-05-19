#pragma once

#include "Components/PawnComponent.h"
#include "Abilities/GameplayAbility.h"
#include "MCombatComponent.generated.h"

class UAbilitySystemComponent;
class UMGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitAbilityActorInfoCore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDefaultAttributeChange, float, DeltaValue, const struct FGameplayTagContainer, EventTags);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChange, FGameplayAttribute, Attribute, float, DeltaValue, const struct FGameplayTagContainer, EventTags);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamage, float, DamageAmount, AActor*, SourceCharacter, const struct FGameplayTagContainer&, DamageTags);

UCLASS(BlueprintType, Blueprintable, ClassGroup=("MianCompanion"), meta=(BlueprintSpawnableComponent))
class MIAN_API UMCombatComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UMCombatComponent(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetupOwner();
	void RegisterAbilitySystemDelegates(UAbilitySystemComponent* ASC);
	void ShutdownAbilitySystemDelegates(UAbilitySystemComponent* ASC);

	virtual void HandleDamage(float DamageAmount, const FGameplayTagContainer& DamageTags, AActor* SourceActor);
	virtual void HandleHealthChange(float DeltaValue, const FGameplayTagContainer& EventTags);
	virtual void HandleAttributeChange(FGameplayAttribute Attribute, float DeltaValue, const FGameplayTagContainer& EventTags);

	//~Start Attribute Logic.
	//通用属性变化回调，用于属性
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data);
	//通用的元属性变化回调，用于处理伤害属性
	virtual void OnDamageAttributeChanged(const FOnAttributeChangeData& Data);
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Actor")
	virtual void Die();
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Attributes")
	virtual float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Attributes")
	virtual float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Attributes")
	virtual float GetAttributeValue(FGameplayAttribute Attribute) const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Attributes")
	virtual float GetCurrentAttributeValue(FGameplayAttribute Attribute) const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Attributes")
	virtual bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Attributes")
	virtual void SetAttributeValue(FGameplayAttribute Attribute, float NewValue);
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Attributes")
	virtual void ClampAttributeValue(FGameplayAttribute Attribute, float MinValue, float MaxValue);
	// UFUNCTION(BlueprintCallable, Category = "GAS Companion|Attributes")
	// virtual void AdjustAttributeForMaxChange(UPARAM(ref) UGSCAttributeSetBase* AttributeSet, const FGameplayAttribute AffectedAttributeProperty, const FGameplayAttribute MaxAttribute, float NewMaxValue);
	//~End Attribute Logic.

	//~Start GameplayAbility Logic.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "CombatComponent|Abilities")
	virtual void GrantAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level = 1);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "CombatComponent|Abilities")
	virtual void ClearAbility(TSubclassOf<UGameplayAbility> Ability);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "CombatComponent|Abilities")
	virtual void ClearAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities);
	UFUNCTION(BlueprintPure, Category="CombatComponent|Abilities")
	bool IsUsingAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const;
	UFUNCTION(BlueprintPure, Category="CombatComponent|Abilities")
	bool IsUsingAbilityByTags(const FGameplayTagContainer& AbilityTags) const;
	UFUNCTION(BlueprintCallable, Category="CombatComponent|Abilities")
	TArray<UGameplayAbility*> GetActiveAbilitiesByClass(TSubclassOf<UGameplayAbility> AbilityToSearch) const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Abilities")
	virtual TArray<UGameplayAbility*> GetActiveAbilitiesByTags(const FGameplayTagContainer GameplayTagContainer) const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Abilities")
	virtual bool ActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass, UMGameplayAbility*& ActivatedAbility, bool bAllowRemoteActivation = true);
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Abilities")
	virtual bool ActivateAbilityByTags(const FGameplayTagContainer AbilityTags, UMGameplayAbility*& ActivatedAbility, const bool bAllowRemoteActivation = true);
	//~End GameplayAbility Logic.

	//~End GameplayTags Logic.
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|GameplayTags")
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer TagContainer) const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|GameplayTags")
	virtual bool HasMatchingGameplayTag(const FGameplayTag TagToCheck) const;

	
	//~Start Combo Logic.
	UGameplayAbility* GetCurrentActiveComboAbility() const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Combo")
	void IncrementCombo();
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Combo")
	void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Combo")
	void ActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	void SetComboIndex(int32 InComboIndex);
	virtual bool IsOwnerActorAuthoritative() const;
	//~End Combo Logic.
	


protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

	//~Start Combo Logic.
	UFUNCTION(Server, Reliable, Category = "CombatComponent|Combo")
	void ServerActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	UFUNCTION(NetMulticast, Reliable, Category = "CombatComponent|Combo")
	void MulticastActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	void ActivateComboAbilityInternal(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	UFUNCTION(Server, Reliable, Category = "CombatComponent|Combo")
	void ServerSetComboIndex(int32 InComboIndex);
	UFUNCTION(NetMulticast, Reliable, Category = "CombatComponent|Combo")
	void MulticastSetComboIndex(int32 InComboIndex);
	//~End Combo Logic.
	
private:
	void CacheIsNetSimulated();

public:
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
	UPROPERTY()
	TObjectPtr<APawn> OwnerPawn;
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerAbilitySystemComponent;

	/**
     * 初始化InitAbilityActorInfo之后立即调用的事件，一旦GA和AS被授予
     *
     * 这将在客户端和服务器端多次发生：
     *
     * - 服务器端：在组件初始化后
     * - 服务器端：在拥有者Actor（Player State被占有）复制后
     * - 客户端：在组件初始化后
     * - 客户端：在拥有者Actor复制后（再次针对Player State的OnRep_PlayerState）
     *
     * 这也取决于该组件是位于Pawns还是Player States上
     *
     * 注意：这个事件也暴露在AbilitySystemComponent本身上，但使用PlayerState上的该组件的Pawns可能也希望能够从Pawns中对这个事件做出反应
     */
	UPROPERTY(BlueprintAssignable, Category="CombatComponent|Abilities")
	FOnInitAbilityActorInfoCore OnInitAbilityActorInfo;

	/**
	 * 当角色受到伤害时调用，该伤害可能致命
	 *
	 * @param DamageAmount 造成的伤害量，不根据当前健康值进行限制
	 * @param SourceCharacter 造成伤害的实际演员
	 * @param DamageTags 造成伤害事件的游戏玩法标签
	 */
	UPROPERTY(BlueprintAssignable, Category="CombatComponent|Abilities")
	FOnDamage OnDamage;

	/**
	 * 当生命值发生变化时调用，无论是治疗还是受伤
	 * 对于伤害，除了OnDamaged/OnDeath之外，还会调用此函数
	 *
	 * @param DeltaValue 生命值变化量，正值表示治疗，负值表示受伤。如果为0，则变化量未知
	 * @param EventTags 改变生命值事件的游戏玩法标签
	 */
	UPROPERTY(BlueprintAssignable, Category="CombatComponent|Abilities")
	FOnDefaultAttributeChange OnHealthChange;
	
	/**
	 * 当该角色拥有的任意属性发生变化时调用
	 *
	 * @param Attribute 被改变的属性
	 * @param DeltaValue 如果是加法操作，则返回修正值
	 *                   或者如果是来自伤害元属性的变化（例如健康值）
	 *                   则返回造成的伤害值
	 * @param EventTags 改变此属性的事件的游戏玩法标签
	 */
	UPROPERTY(BlueprintAssignable, Category="CombatComponent|Abilities")
	FOnAttributeChange OnAttributeChange;
	
	UPROPERTY(BlueprintAssignable, Category = "CombatComponent|Actor")
	FOnDeath OnDeath;




	
	//Combo.
	//连招GA类.
	TSubclassOf<UMGameplayAbility> MeleeBaseAbility;
	//当前段数.
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CombatComponent|Combo")
	int32 ComboIndex = 0;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CombatComponent|Combo")
	bool bComboWindowOpened = false;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CombatComponent|Combo")
	bool bShouldTriggerCombo = false;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CombatComponent|Combo")
	bool bRequestTriggerCombo = false;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "CombatComponent|Combo")
	bool bNextComboAbilityActivated = false;

protected:
	UPROPERTY()
	bool bCachedIsNetSimulated;
	



	
};
