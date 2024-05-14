#include "Player/MPlayerController.h"

#include "MPlayerState.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "Character/MPlayerCharacter.h"

AMPlayerState* AMPlayerController::GetMPlayerState() const
{
	return CastChecked<AMPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

AMPlayerCharacter* AMPlayerController::GetMPlayerCharacter() const
{
	return CastChecked<AMPlayerCharacter>(GetPawn(), ECastCheckedType::NullAllowed);
}

UMAbilitySystemComponent* AMPlayerController::GetMAbilitySystemComponent() const
{
	const AMPlayerCharacter* MPlayer = GetMPlayerCharacter();
	return (MPlayer ? MPlayer->GetMAbilitySystemComponent() : nullptr);
}

void AMPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AMPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UMAbilitySystemComponent* TouASC = GetMAbilitySystemComponent())
	{
		TouASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
