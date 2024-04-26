#pragma once

#include "AbilitySystemGlobals.h"
#include "MAbilitySystemGlobals.generated.h"

class UObject;
struct FGameplayEffectContext;

UCLASS(Config=Game)
class MIAN_API UMAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
	
};
