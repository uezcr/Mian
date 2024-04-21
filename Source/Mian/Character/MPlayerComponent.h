#pragma once

#include "Components/PawnComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "MPlayerComponent.generated.h"

class UMCameraMode;
class UMInputConfig;
class UInputMappingContext;

struct FInputActionValue;
struct FActorInitStateChangedParams;
struct FFrame;
struct FGameplayTag;


/** 为玩家控制的角色（或模拟玩家的机器人）设置输入和摄像机处理的组件 */

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class MIAN_API UMPlayerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:

	UMPlayerComponent(const FObjectInitializer& ObjectInitializer);
	

	/** 如果指定的角色存在玩家组件,则返回该组件 */
	UFUNCTION(BlueprintPure, Category = "Mian|PlayerComponent")
	static UMPlayerComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UMPlayerComponent>() : nullptr); }

	/** 覆盖来自活动GA的摄像机 */
	void SetAbilityCameraMode(TSubclassOf<UMCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** 如果已设置,清除摄像机覆盖 */
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);
	
protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

	TSubclassOf<UMCameraMode> DetermineCameraMode() const;
	
protected:
	/** 默认输入 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tou|Input")
	TObjectPtr<UMInputConfig> DefaultInputConfig;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mian|PlayerComponent")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	
	/** 由GA设置的摄像机模式 */
	UPROPERTY()
	TSubclassOf<UMCameraMode> AbilityCameraMode;

	/** 上一个设置摄像机模式的GA的Spec句柄 */
	FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;
	
	/** 玩家控制的角色使用的默认摄像机模式 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mian|PlayerComponent")
	TSubclassOf<UMCameraMode> DefaultCameraMode;

private:
};
