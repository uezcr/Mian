#include "MAimSensitivityData.h"

#include "Settings/MSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MAimSensitivityData)

UMAimSensitivityData::UMAimSensitivityData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SensitivityMap =
	{
		{ EMGamepadSensitivity::Slow,			0.5f },
		{ EMGamepadSensitivity::SlowPlus,		0.75f },
		{ EMGamepadSensitivity::SlowPlusPlus,	0.9f },
		{ EMGamepadSensitivity::Normal,		1.0f },
		{ EMGamepadSensitivity::NormalPlus,	1.1f },
		{ EMGamepadSensitivity::NormalPlusPlus,1.25f },
		{ EMGamepadSensitivity::Fast,			1.5f },
		{ EMGamepadSensitivity::FastPlus,		1.75f },
		{ EMGamepadSensitivity::FastPlusPlus,	2.0f },
		{ EMGamepadSensitivity::Insane,		2.5f },
	};
}

const float UMAimSensitivityData::SensitivtyEnumToFloat(const EMGamepadSensitivity InSensitivity) const
{
	if (const float* Sens = SensitivityMap.Find(InSensitivity))
	{
		return *Sens;
	}

	return 1.0f;
}

