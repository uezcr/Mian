#include "AbilitySystem/MCombatComponent.h"

#include "Abilities/MGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
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
}

void UMCombatComponent::ShutdownAbilitySystemDelegates(UAbilitySystemComponent* ASC)
{
}

UGameplayAbility* UMCombatComponent::GetCurrentActiveComboAbility() const
{
	return nullptr;
}

void UMCombatComponent::IncrementCombo()
{
}

void UMCombatComponent::ResetCombo()
{
}

void UMCombatComponent::ActivateComboAbility(TSubclassOf<UMGameplayAbility> AbilityClass, bool bAllowRemoteActivation)
{
}

void UMCombatComponent::SetComboIndex(int32 InComboIndex)
{
}

bool UMCombatComponent::IsOwnerActorAuthoritative() const
{
	return false;
}

void UMCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMCombatComponent::OnRegister()
{
	Super::OnRegister();
}

void UMCombatComponent::ActivateComboAbilityInternal(TSubclassOf<UMGameplayAbility> AbilityClass,
	bool bAllowRemoteActivation)
{
}

void UMCombatComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UMCombatComponent::MulticastSetComboIndex_Implementation(int32 InComboIndex)
{
}

void UMCombatComponent::ServerSetComboIndex_Implementation(int32 InComboIndex)
{
}

void UMCombatComponent::MulticastActivateComboAbility_Implementation(TSubclassOf<UMGameplayAbility> AbilityClass,
                                                                     bool bAllowRemoteActivation)
{
}

void UMCombatComponent::ServerActivateComboAbility_Implementation(TSubclassOf<UMGameplayAbility> AbilityClass,
                                                                  bool bAllowRemoteActivation)
{
}

