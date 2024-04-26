// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MHUD.generated.h"

class UPickupDisplay;
/**
 * 
 */
UCLASS()
class MIAN_API AMHUD : public AHUD
{
	GENERATED_BODY()
public:
	/*玩家瞄准物体后要显示的UI,只有玩家有,其他不需要*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="HUD|Inventory")
	TSubclassOf<UPickupDisplay> PickupWidgetClass;

	/*获取提示信息版*/
	UFUNCTION(BlueprintPure,Category="HUD|Inventory")
	UPickupDisplay*GetPickupDisplay();

	/*销毁提示信息版*/
	UFUNCTION(BlueprintCallable,Category="HUD|Inventory")
	bool DestroyPickupDisplay();
private:
	UPROPERTY()
	UPickupDisplay*PickupDisplayWidget;
};
