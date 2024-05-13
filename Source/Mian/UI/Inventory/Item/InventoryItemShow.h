// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryType.h"
#include "InventoryItemShow.generated.h"

class UInventoryComponent;
struct FItemInfoDef;
class UTextBlock;
class UImage;
class USizeBox;
class UOverlay;
class UBorder;
class UInventoryItemInfo;
/**
 * 
 */
UCLASS()
class MIAN_API UInventoryItemShow : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="InventoryItemShow|Widget",meta=(BindWidget))
	UOverlay*Over_Main;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="InventoryItemShow|Widget",meta=(BindWidget))
	USizeBox*Size_Item;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="InventoryItemShow|Widget",meta=(BindWidget))
	UBorder*Broder_ItemBG;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="InventoryItemShow|Widget",meta=(BindWidget))
	UBorder*Broder_ItemHover;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="InventoryItemShow|Widget",meta=(BindWidget))
	UImage*Image_Item;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="InventoryItemShow|Widget",meta=(BindWidget))
	UTextBlock*Txt_Amount;

	/*道具信息*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryItemShow|Setting")
	FItemInfoDef Item;

	/*道具图像展示的大小*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryItemShow|Setting")
	FVector2D OneSlotSize;

	/*检查钱够吗*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventoryItemShow|Setting")
	bool bCheckMoney;

	/*鼠标进来了？*/
	UPROPERTY(BlueprintReadWrite,Category="InventoryItemShow|Setting")
	bool bMouseIn;

	/*展示道具信息的UI*/
	UPROPERTY(BlueprintReadWrite,Category="InventoryItemShow|Setting")
	UInventoryItemInfo* ItemInfoWidget;

	/*道具在库存中的默认结构*/
	UPROPERTY(BlueprintReadWrite,Category="InventoryItemShow|Setting")
	FInventoryItem ItemDefault;

	/*玩家库存组件*/
	UPROPERTY(BlueprintReadWrite,Category="InventoryItemShow|Setting")
	UInventoryComponent*PlayerInventoryComponent;
	
public:

	/*初始化道具展示UI*/
	UFUNCTION(BlueprintCallable,Category="InventoryItemShow")
	void InitializeWidget(const FItemInfoDef& InItem,const FVector2D&InOneSlotSize,const bool& InbCheckMoney);
	
	/*刷新背景颜色*/
	UFUNCTION(BlueprintCallable,Category="InventoryItemShow")
	void ResetBackgroundColor();

	/*设置当前道具的数量*/
	UFUNCTION(BlueprintCallable,Category="InventoryItemShow")
	void SetQuantity(const int32&InNewAmount);

	/*获取当前储存的道具*/
	UFUNCTION(BlueprintPure,Category="InventoryItemShow")
	FORCEINLINE FItemInfoDef GetItemInfo()const {return Item;};

	/*获取储存图片的SizeBox*/
	UFUNCTION(BlueprintPure,Category="InventoryItemShow")
	FORCEINLINE USizeBox* GetSizeBoxItem()const {return Size_Item;};

	/*道具旋转了？吗*/
	UFUNCTION(BlueprintPure,Category="InventoryItemShow")
	FORCEINLINE bool IsRotated()const {return Item.bRotated;};
};
