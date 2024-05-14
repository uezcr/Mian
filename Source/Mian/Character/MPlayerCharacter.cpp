#include "MPlayerCharacter.h"

#include "AbilitySystem/MAbilitySystemComponent.h"
#include "Camera/MCameraComponent.h"
#include "Player/MPlayerState.h"

AMPlayerCharacter::AMPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CameraComponent = CreateDefaultSubobject<UMCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AMPlayerCharacter::GetAbilitySystemComponent() const
{
	return GetMAbilitySystemComponent();
}

void AMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

