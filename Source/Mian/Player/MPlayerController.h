#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "MPlayerController.generated.h"

class UMAbilitySystemComponent;
class AMPlayerState;
class AMPlayerCharacter;

UCLASS()
class MIAN_API AMPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Mian|PlayerController")
	AMPlayerState* GetMPlayerState() const;
	UFUNCTION(BlueprintCallable, Category = "Mian|PlayerController")
	AMPlayerCharacter* GetMPlayerCharacter() const;
	UFUNCTION(BlueprintCallable, Category = "Mian|PlayerController")
	UMAbilitySystemComponent* GetMAbilitySystemComponent() const;
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
};
