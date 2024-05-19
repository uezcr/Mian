#include "MAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "AbilitySystem/MAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MAttributeSet)

class UWorld;


UMAttributeSet::UMAttributeSet()
{
}

UWorld* UMAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UMAbilitySystemComponent* UMAttributeSet::GetMAbilitySystemComponent() const
{
	return Cast<UMAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

const float UMAttributeSet::GetAttributeByGamePlayTag(const FGameplayTag& GameplayTag) const
{
	for (auto& Attrubute: TagsToAttributes)
	{
		if(GameplayTag == Attrubute.Key)
		{
			return Attrubute.Value().GetNumericValue(this);
		}
	}
	return 0.0f;
}

void UMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UMAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, Health, OldValue);
}

void UMAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMAttributeSet, MaxHealth, OldValue);
}

