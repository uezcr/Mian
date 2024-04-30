#pragma once

#include "ModularCharacter.h"
#include "MCharacter.generated.h"

class UMCameraComponent;

/**
 * FTouReplicatedAcceleration: 加速度的压缩表示
 */
USTRUCT()
struct FTouReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// XY加速度分量的方向，量化表示[0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	// XY分量的加速度率，量化表示[0, 最大加速度]

	UPROPERTY()
	int8 AccelZ = 0;	// 原始Z方向的加速度率分量，量化表示[-最大加速度, 最大加速度]
};

UCLASS(Config = Game, Meta = (ShortTooltip = "这个项目使用的基本角色Pawn类."))
class MIAN_API AMCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	AMCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mian|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMCameraComponent> CameraComponent;
	
};
