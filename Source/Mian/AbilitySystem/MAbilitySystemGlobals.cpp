#include "AbilitySystem/MAbilitySystemGlobals.h"
#include "MGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MAbilitySystemGlobals)

FGameplayEffectContext* UMAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMGameplayEffectContext();
}
