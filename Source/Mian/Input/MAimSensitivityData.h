#pragma once

#include "Engine/DataAsset.h"

#include "MAimSensitivityData.generated.h"

enum class EMGamepadSensitivity : uint8;

class UObject;

/** Defines a set of gamepad sensitivity to a float value. */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "M Aim Sensitivity Data", ShortTooltip = "Data asset used to define a map of Gamepad Sensitivty to a float value."))
class MIAN_API UMAimSensitivityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMAimSensitivityData(const FObjectInitializer& ObjectInitializer);
	
	const float SensitivtyEnumToFloat(const EMGamepadSensitivity InSensitivity) const;
	
protected:
	/** Map of SensitivityMap settings to their corresponding float */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMGamepadSensitivity, float> SensitivityMap;
};
