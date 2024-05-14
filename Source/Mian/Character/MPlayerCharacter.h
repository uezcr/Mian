#pragma once

#include "AbilitySystemInterface.h"
#include "MCharacter.h"
#include "MPlayerCharacter.generated.h"

UCLASS()
class MIAN_API AMPlayerCharacter : public AMCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Sets default values for this character's properties
	AMPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Mian|GameplayAbility")
	UMAbilitySystemComponent* GetMAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Mian|GameplayAbility")
	TObjectPtr<UMAbilitySystemComponent> AbilitySystemComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mian|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMCameraComponent> CameraComponent;
	
};
