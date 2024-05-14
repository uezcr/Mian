#include "Player/MPlayerState.h"

#include "MPlayerController.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MPlayerState)

AMPlayerState::AMPlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

AMPlayerController* AMPlayerState::GetMPlayerController() const
{
	return Cast<AMPlayerController>(GetOwner());
}

