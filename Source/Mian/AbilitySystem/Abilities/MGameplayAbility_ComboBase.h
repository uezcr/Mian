#pragma once

#include "AbilitySystem/Abilities/MGameplayAbility.h"
#include "MGameplayAbility_ComboBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input)
class MIAN_API UMGameplayAbility_ComboBase : public UMGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Combo")
	TArray<TObjectPtr<UAnimMontage>> ComboMontages;
	UPROPERTY(BlueprintReadWrite,Category="Combo")
	int32 ComboAttackIndex = 0;
	UPROPERTY(BlueprintReadWrite,Category="Combo")
	float AmountToRemove = 0;
};
