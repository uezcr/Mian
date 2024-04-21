#include "MInputComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Player/MLocalPlayer.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(MInputComponent)

class UMInputConfig;

UMInputComponent::UMInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UMInputComponent::AddInputMappings(const UMInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UMInputComponent::RemoveInputMappings(const UMInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UMInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
