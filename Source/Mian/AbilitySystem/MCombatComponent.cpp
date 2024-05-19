#include "AbilitySystem/MCombatComponent.h"

#include "Abilities/MGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "MLogChannels.h"
#include "Attributes/MAttributeSet.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MCombatComponent)

UMCombatComponent::UMCombatComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatComponent, ComboIndex, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatComponent, bComboWindowOpened, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatComponent, bShouldTriggerCombo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatComponent, bRequestTriggerCombo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMCombatComponent, bNextComboAbilityActivated, COND_None, REPNOTIFY_Always);
}

void UMCombatComponent::SetupOwner()
{
	if(!GetOwner())
	{
		return;
	}

	OwnerActor = GetOwner();
	if(!OwnerActor)
	{
		return;
	}

	OwnerPawn = Cast<APawn>(OwnerActor);
	OwnerCharacter = Cast<ACharacter>(OwnerActor);

	OwnerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
}

void UMCombatComponent::RegisterAbilitySystemDelegates(UAbilitySystemComponent* ASC)
{
	if (!ASC)
	{
		return;
	}
	
	// Make sure to shutdown delegates previously registered, if RegisterAbilitySystemDelegates is called more than once (likely from AbilityActorInfo)
	ShutdownAbilitySystemDelegates(ASC);

	// TArray<FGameplayAttribute> Attributes;
	// ASC->GetAllAttributes(Attributes);

	// for (FGameplayAttribute Attribute : Attributes)
	// {
	// 	if (Attribute == UMCombatComponent::GetDamageAttribute() || Attribute == UMCombatComponent::GetStaminaDamageAttribute())
	// 	{
	// 		ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UMCombatComponent::OnDamageAttributeChanged);
	// 	}
	// 	else
	// 	{
	// 		ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UMCombatComponent::OnAttributeChanged);
	// 	}
	// }
	//
	// // Handle GameplayEffects added / remove
	// ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UMCombatComponent::OnActiveGameplayEffectAdded);
	// ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UMCombatComponent::OnAnyGameplayEffectRemoved);
	//
	// // Handle generic GameplayTags added / removed
	// ASC->RegisterGenericGameplayTagEvent().AddUObject(this, &UMCombatComponent::OnAnyGameplayTagChanged);
	//
	// // Handle Ability Commit events
	// ASC->AbilityCommittedCallbacks.AddUObject(this, &UMCombatComponent::OnAbilityCommitted);
}

void UMCombatComponent::ShutdownAbilitySystemDelegates(UAbilitySystemComponent* ASC)
{
	if (!ASC)
	{
		return;
	}

	// TArray<FGameplayAttribute> Attributes;
	// ASC->GetAllAttributes(Attributes);
	//
	// for (const FGameplayAttribute& Attribute : Attributes)
	// {
	// 	ASC->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
	// }
	//
	// ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
	// ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	// ASC->RegisterGenericGameplayTagEvent().RemoveAll(this);
	// ASC->AbilityCommittedCallbacks.RemoveAll(this);
	//
	// for (const FActiveGameplayEffectHandle GameplayEffectAddedHandle : GameplayEffectAddedHandles)
	// {
	// 	if (GameplayEffectAddedHandle.IsValid())
	// 	{
	// 		FOnActiveGameplayEffectStackChange* EffectStackChangeDelegate = ASC->OnGameplayEffectStackChangeDelegate(GameplayEffectAddedHandle);
	// 		if (EffectStackChangeDelegate)
	// 		{
	// 			EffectStackChangeDelegate->RemoveAll(this);
	// 		}
	//
	// 		FOnActiveGameplayEffectTimeChange* EffectTimeChangeDelegate = ASC->OnGameplayEffectTimeChangeDelegate(GameplayEffectAddedHandle);
	// 		if (EffectTimeChangeDelegate)
	// 		{
	// 			EffectTimeChangeDelegate->RemoveAll(this);
	// 		}
	// 	}
	// }
	//
	// for (const FGameplayTag GameplayTagBoundToDelegate : GameplayTagBoundToDelegates)
	// {
	// 	ASC->RegisterGameplayTagEvent(GameplayTagBoundToDelegate).RemoveAll(this);
	// }
}

void UMCombatComponent::HandleDamage(float DamageAmount, const FGameplayTagContainer& DamageTags, AActor* SourceActor)
{
	OnDamage.Broadcast(DamageAmount, SourceActor, DamageTags);
}

void UMCombatComponent::HandleHealthChange(float DeltaValue, const FGameplayTagContainer& EventTags)
{
}

void UMCombatComponent::HandleAttributeChange(FGameplayAttribute Attribute, float DeltaValue,
	const FGameplayTagContainer& EventTags)
{
}

void UMCombatComponent::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
}

void UMCombatComponent::OnDamageAttributeChanged(const FOnAttributeChangeData& Data)
{
}

void UMCombatComponent::Die()
{
	OnDeath.Broadcast();
}

float UMCombatComponent::GetHealth() const
{
	if (!OwnerAbilitySystemComponent)
	{
		return 0.0f;
	}

	return GetAttributeValue(UMAttributeSet::GetHealthAttribute());
}

float UMCombatComponent::GetMaxHealth() const
{
	if (!OwnerAbilitySystemComponent)
	{
		return 0.0f;
	}

	return GetAttributeValue(UMAttributeSet::GetMaxHealthAttribute());
}

float UMCombatComponent::GetAttributeValue(FGameplayAttribute Attribute) const
{
	if (!OwnerAbilitySystemComponent)
	{
		return 0.0f;
	}
	if (!OwnerAbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		const UObject* Owner = Cast<UObject>(this);
		const FString OwnerName = OwnerActor ? OwnerActor->GetName() : Owner->GetName();
		return 0.0f;
	}
	return OwnerAbilitySystemComponent->GetNumericAttributeBase(Attribute);
}

float UMCombatComponent::GetCurrentAttributeValue(FGameplayAttribute Attribute) const
{
	if (!OwnerAbilitySystemComponent)
	{
		return 0.0f;
	}

	if (!OwnerAbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		const UObject* Owner = Cast<UObject>(this);
		const FString OwnerName = OwnerActor ? OwnerActor->GetName() : Owner->GetName();
		return 0.0f;
	}

	return OwnerAbilitySystemComponent->GetNumericAttribute(Attribute);
}

bool UMCombatComponent::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void UMCombatComponent::SetAttributeValue(FGameplayAttribute Attribute, float NewValue)
{
	if (!OwnerAbilitySystemComponent)
	{
		return;
	}
	OwnerAbilitySystemComponent->SetNumericAttributeBase(Attribute, NewValue);
}

void UMCombatComponent::ClampAttributeValue(FGameplayAttribute Attribute, float MinValue, float MaxValue)
{
	const float CurrentValue = GetAttributeValue(Attribute);
	const float NewValue = FMath::Clamp(CurrentValue, MinValue, MaxValue);
	SetAttributeValue(Attribute, NewValue);
}

void UMCombatComponent::GrantAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level)
{
	if (!OwnerActor || !OwnerAbilitySystemComponent || !Ability)
	{
		return;
	}

	if (!OwnerAbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		return;
	}

	FGameplayAbilitySpec Spec;
	Spec.Ability = Ability.GetDefaultObject();

	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, Level, INDEX_NONE, OwnerActor);
	OwnerAbilitySystemComponent->GiveAbility(AbilitySpec);
}

void UMCombatComponent::ClearAbility(TSubclassOf<UGameplayAbility> Ability)
{
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToRemove;
	AbilitiesToRemove.Add(Ability);
	return ClearAbilities(AbilitiesToRemove);
}

void UMCombatComponent::ClearAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	if (!OwnerActor || !OwnerAbilitySystemComponent || !OwnerAbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		return;
	}
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : OwnerAbilitySystemComponent->GetActivatableAbilities())
	{
		if (Abilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}
	for (const FGameplayAbilitySpecHandle AbilityToRemove : AbilitiesToRemove)
	{
		OwnerAbilitySystemComponent->ClearAbility(AbilityToRemove);
	}
}

bool UMCombatComponent::IsUsingAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!AbilityClass)
	{
		return false;
	}
	return GetActiveAbilitiesByClass(AbilityClass).Num() > 0;
}

bool UMCombatComponent::IsUsingAbilityByTags(const FGameplayTagContainer& AbilityTags) const
{
	if (!OwnerAbilitySystemComponent)
	{
		return false;
	}

	return GetActiveAbilitiesByTags(AbilityTags).Num() > 0;
}

TArray<UGameplayAbility*> UMCombatComponent::GetActiveAbilitiesByClass(
	TSubclassOf<UGameplayAbility> AbilityToSearch) const
{
	if (!OwnerAbilitySystemComponent)
	{
		return {};
	}
	TArray<FGameplayAbilitySpec> Specs = OwnerAbilitySystemComponent->GetActivatableAbilities();
	TArray<struct FGameplayAbilitySpec*> MatchingGameplayAbilities;
	TArray<UGameplayAbility*> ActiveAbilities;
	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		if (Spec.Ability && Spec.Ability->GetClass()->IsChildOf(AbilityToSearch))
		{
			MatchingGameplayAbilities.Add(const_cast<FGameplayAbilitySpec*>(&Spec));
		}
	}
	for (const FGameplayAbilitySpec* Spec : MatchingGameplayAbilities)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			if (ActiveAbility->IsActive())
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}
	return ActiveAbilities;
}

TArray<UGameplayAbility*> UMCombatComponent::GetActiveAbilitiesByTags(
	const FGameplayTagContainer GameplayTagContainer) const
{
	if (!OwnerAbilitySystemComponent)
	{
		return {};
	}
	TArray<UGameplayAbility*> ActiveAbilities;
	TArray<FGameplayAbilitySpec*> MatchingGameplayAbilities;
	OwnerAbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, MatchingGameplayAbilities, false);
	for (const FGameplayAbilitySpec* Spec : MatchingGameplayAbilities)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			if (ActiveAbility->IsActive())
			{
				ActiveAbilities.Add(ActiveAbility);
			}
		}
	}
	return ActiveAbilities;
}

bool UMCombatComponent::ActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass,
	UMGameplayAbility*& ActivatedAbility, bool bAllowRemoteActivation)
{
	if (!OwnerAbilitySystemComponent || !AbilityClass)
	{
		return false;
	}
	const bool bSuccess = OwnerAbilitySystemComponent->TryActivateAbilityByClass(AbilityClass, bAllowRemoteActivation);
	TArray<UGameplayAbility*> ActiveAbilities = GetActiveAbilitiesByClass(AbilityClass);
	if (ActiveAbilities.Num() == 0)
	{
		//
	}
	if (bSuccess && ActiveAbilities.Num() > 0)
	{
		UMGameplayAbility* MAbility = Cast<UMGameplayAbility>(ActiveAbilities[0]);
		if (MAbility)
		{
			ActivatedAbility = MAbility;
		}
	}
	return bSuccess;
}

bool UMCombatComponent::ActivateAbilityByTags(const FGameplayTagContainer AbilityTags,
	UMGameplayAbility*& ActivatedAbility, const bool bAllowRemoteActivation)
{
	if (!OwnerAbilitySystemComponent)
	{
		return false;
	}
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	OwnerAbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate);

	const uint32 Count = AbilitiesToActivate.Num();

	if (Count == 0)
	{
		return false;
	}
	const FGameplayAbilitySpec* Spec = AbilitiesToActivate[FMath::RandRange(0, Count - 1)];
	const bool bSuccess = OwnerAbilitySystemComponent->TryActivateAbility(Spec->Handle, bAllowRemoteActivation);
	TArray<UGameplayAbility*> ActiveAbilities = GetActiveAbilitiesByTags(AbilityTags);
	if (ActiveAbilities.Num() == 0)
	{
		//
	}
	if (bSuccess && ActiveAbilities.Num() > 0)
	{
		UMGameplayAbility* MAbility = Cast<UMGameplayAbility>(ActiveAbilities[0]);
		if (MAbility)
		{
			ActivatedAbility = MAbility;
		}
	}
	return bSuccess;
}

bool UMCombatComponent::HasAnyMatchingGameplayTags(const FGameplayTagContainer TagContainer) const
{
	if (OwnerAbilitySystemComponent)
	{
		return OwnerAbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
	}
	return false;
}

bool UMCombatComponent::HasMatchingGameplayTag(const FGameplayTag TagToCheck) const
{
	if (OwnerAbilitySystemComponent)
	{
		return OwnerAbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
	}
	return false;
}

//~Start Combo Logic
void UMCombatComponent::IncrementCombo()
{
	if (bComboWindowOpened)
	{
		ComboIndex = ComboIndex + 1;
	}
}

void UMCombatComponent::ResetCombo()
{
	SetComboIndex(0);
}

void UMCombatComponent::ActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation)
{
	if (IsOwnerActorAuthoritative())
	{
		ActivateComboAbilityInternal(AbilityClass, bAllowRemoteActivation);
	}
	else
	{
		ServerActivateComboAbility(AbilityClass, bAllowRemoteActivation);
	}
}

void UMCombatComponent::SetComboIndex(int32 InComboIndex)
{
	if (IsOwnerActorAuthoritative())
	{
		ComboIndex = InComboIndex;
	}
	else
	{
		ComboIndex = InComboIndex;
		ServerSetComboIndex(InComboIndex);
	}
}

bool UMCombatComponent::IsOwnerActorAuthoritative() const
{
	return !bCachedIsNetSimulated;
}

UGameplayAbility* UMCombatComponent::GetCurrentActiveComboAbility() const
{
	TArray<UGameplayAbility*> Abilities = GetActiveAbilitiesByClass(MeleeBaseAbility);
	return Abilities.IsValidIndex(0) ? Abilities[0] : nullptr;
}

void UMCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheIsNetSimulated();
}

void UMCombatComponent::OnRegister()
{
	Super::OnRegister();

	CacheIsNetSimulated();
}

void UMCombatComponent::ActivateComboAbilityInternal(TSubclassOf<UMGameplayAbility> AbilityClass,
	bool bAllowRemoteActivation)
{
	bShouldTriggerCombo = false;
	if (!OwnerCharacter)
	{
		UE_LOG(LogMianCombat,Error,TEXT("UMCombatComponent::ActivateComboAbility() OwnerCharacter is null."));
		return;
	}
	
	if (!AbilityClass)
	{
		UE_LOG(LogMianCombat,Error,TEXT("UMCombatComponent::ActivateComboAbility() provided AbilityClass is null."));
		return;
	}

	if (IsUsingAbilityByClass(AbilityClass))
	{
		bShouldTriggerCombo = bComboWindowOpened;
	}
	else
	{
		UMGameplayAbility* TempActivateAbility;
		ActivateAbilityByClass(AbilityClass, TempActivateAbility, bAllowRemoteActivation);
	}
}

void UMCombatComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UMCombatComponent::ServerSetComboIndex_Implementation(int32 InComboIndex)
{
	MulticastSetComboIndex(InComboIndex);
}

void UMCombatComponent::MulticastSetComboIndex_Implementation(int32 InComboIndex)
{
	if (OwnerCharacter && !OwnerCharacter->IsLocallyControlled())
	{
		ComboIndex = InComboIndex;
	}
}

void UMCombatComponent::MulticastActivateComboAbility_Implementation(TSubclassOf<UMGameplayAbility> AbilityClass,
                                                                     bool bAllowRemoteActivation)
{
	if (OwnerCharacter && !OwnerCharacter->IsLocallyControlled())
	{
		ActivateComboAbilityInternal(AbilityClass, bAllowRemoteActivation);
	}
}

void UMCombatComponent::ServerActivateComboAbility_Implementation(TSubclassOf<UMGameplayAbility> AbilityClass,
                                                                  bool bAllowRemoteActivation)
{
	MulticastActivateComboAbility(AbilityClass, bAllowRemoteActivation);
}

//~End Combo Logic

