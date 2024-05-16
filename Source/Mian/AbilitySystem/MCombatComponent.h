#pragma once

#include "Components/PawnComponent.h"
#include "Abilities/GameplayAbility.h"
#include "MCombatComponent.generated.h"

class UAbilitySystemComponent;
class UMGameplayAbility;

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
	//Combo.
	UGameplayAbility* GetCurrentActiveComboAbility() const;
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Combo")
	void IncrementCombo();
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Combo")
	void ResetCombo();
	UFUNCTION(BlueprintCallable, Category = "CombatComponent|Combo")
	void ActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	void SetComboIndex(int32 InComboIndex);
	virtual bool IsOwnerActorAuthoritative() const;

protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

	//Combo.
	UFUNCTION(Server, Reliable, Category = "CombatComponent|Combo")
	void ServerActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	UFUNCTION(NetMulticast, Reliable, Category = "CombatComponent|Combo")
	void MulticastActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	void ActivateComboAbilityInternal(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation = true);
	UFUNCTION(Server, Reliable, Category = "CombatComponent|Combo")
	void ServerSetComboIndex(int32 InComboIndex);
	UFUNCTION(NetMulticast, Reliable, Category = "CombatComponent|Combo")
	void MulticastSetComboIndex(int32 InComboIndex);

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
