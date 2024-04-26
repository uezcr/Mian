// 


#include "MHUD.h"
#include "Inventory/Pickup/PickupDisplay.h"

UPickupDisplay* AMHUD::GetPickupDisplay()
{
	if (PickupDisplayWidget)
	{
		return PickupDisplayWidget;
	}
	
	if (PickupWidgetClass)
	{
		PickupDisplayWidget = CreateWidget<UPickupDisplay>(GetOwningPlayerController(),PickupWidgetClass);
		if(PickupDisplayWidget) return PickupDisplayWidget;
		return nullptr;
	}
	return nullptr;
}

bool AMHUD::DestroyPickupDisplay()
{
	if (PickupDisplayWidget)
	{
		PickupDisplayWidget->RemoveFromRoot();
		PickupDisplayWidget=nullptr;
		return true;
	}
	UE_LOG(LogTemp,Error,TEXT("lzy AHUD PickupDisplayWidget 本身不存在无法销毁"));
	return false;
}
