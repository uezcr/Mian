#pragma once

#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"
#include "MPlayerState.generated.h"

class AController;
class APlayerState;
class FName;
class UAbilitySystemComponent;
class UMAbilitySystemComponent;
class UObject;
struct FFrame;
struct FGameplayTag;

UCLASS()
class MIAN_API AMPlayerState : public AModularPlayerState
{
	GENERATED_BODY()

public:
	AMPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Mian|PlayerState")
	AMPlayerController* GetMPlayerController() const;
};
