#include "Player/MLocalPlayer.h"

#include "Settings/MSettingsShared.h"

UMSettingsShared* UMLocalPlayer::GetSharedSettings() const
{
	if (!SharedSettings)
	{
		// On PC it's okay to use the sync load because it only checks the disk
		// This could use a platform tag to check for proper save support instead
		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;
		
		if (bCanLoadBeforeLogin)
		{
			SharedSettings = UMSettingsShared::LoadOrCreateSettings(this);
		}
		else
		{
			// We need to wait for user login to get the real settings so return temp ones
			SharedSettings = UMSettingsShared::CreateTemporarySettings(this);
		}
	}

	return SharedSettings;
}
