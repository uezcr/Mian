#include "Character/MPlayerComponent.h"

#include "Camera/MCameraComponent.h"
#include "Camera/MCameraMode.h"
#include "EnhancedInputSubsystems.h"
#include "MGameplayTags.h"
#include "MPlayerCharacter.h"
#include "AbilitySystem/MAbilitySystemComponent.h"
#include "Input/MInputComponent.h"
#include "Player/MLocalPlayer.h"
#include "Player/MPlayerController.h"
#include "UserSettings/EnhancedInputUserSettings.h"

namespace MPlayer
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
}

UMPlayerComponent::UMPlayerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
}

void UMPlayerComponent::SetAbilityCameraMode(TSubclassOf<UMCameraMode> CameraMode,
                                             const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UMPlayerComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

TSubclassOf<UMCameraMode> UMPlayerComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}
	return DefaultCameraMode;
}

void UMPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	const APawn* Pawn = GetPawn<APawn>();
	//绑定相机.
	if (UMCameraComponent* CameraComponent = UMCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
	}
	//初始化输入.
	if (AMPlayerController* TouPC = GetController<AMPlayerController>())
	{
		if (Pawn->InputComponent != nullptr)
		{
			InitializePlayerInput(Pawn->InputComponent);
		}
	}
}

void UMPlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMPlayerComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);
	const UMLocalPlayer* LP = Cast<UMLocalPlayer>(PC->GetLocalPlayer());
	check(LP);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	
	if (DefaultInputMapping.Get())
	{
		if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
		{
			Settings->RegisterInputMappingContext(DefaultInputMapping);
		}
		FModifyContextOptions Options = {};
		Options.bIgnoreAllPressedKeysUntilRelease = false;
		Subsystem->AddMappingContext(DefaultInputMapping,0 , Options);
	}

	UMInputComponent* TouIC = Cast<UMInputComponent>(PlayerInputComponent);
	if (ensureMsgf(TouIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UTouInputComponent or a subclass of it.")))
	{
		if(DefaultInputConfig.Get())
		{
			// Add the key mappings that may have been set by the player
			TouIC->AddInputMappings(DefaultInputConfig, Subsystem);

			// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
			// be triggered directly by these input actions Triggered events. 
			TArray<uint32> BindHandles;
			TouIC->BindAbilityActions(DefaultInputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

			TouIC->BindNativeAction(DefaultInputConfig, MGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
			TouIC->BindNativeAction(DefaultInputConfig, MGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
			TouIC->BindNativeAction(DefaultInputConfig, MGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
			TouIC->BindNativeAction(DefaultInputConfig, MGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
		}
	}
}

void UMPlayerComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const AMPlayerCharacter* Player = GetPawn<AMPlayerCharacter>())
	{
		if (UMAbilitySystemComponent* TouASC = Player->GetMAbilitySystemComponent())
		{
			TouASC->AbilityInputTagPressed(InputTag);
		}	
	}
}

void UMPlayerComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const AMPlayerCharacter* Player = GetPawn<AMPlayerCharacter>())
	{
		if (UMAbilitySystemComponent* TouASC = Player->GetMAbilitySystemComponent())
		{
			TouASC->AbilityInputTagReleased(InputTag);
		}	
	}
}

void UMPlayerComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UMPlayerComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UMPlayerComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * MPlayer::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * MPlayer::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UMPlayerComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{

}
