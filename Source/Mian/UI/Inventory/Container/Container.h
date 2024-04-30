// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryType.h"
#include "Container.generated.h"

class UEquipSlot;
class UInventorySlot;
class UInventoryComponent;
class UBorder;
class UUniformGridPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContainerInteract,FItemInfoDef,InItemInfo,TArray<FContainerInfo>,InCurrentContainers);

UCLASS()
class MIAN_API UContainer : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container",meta=(BindWidget))
	UUniformGridPanel*Grid_Slots;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container")
	UBorder*OutlineBorder;

	//容器的大小
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container|SlotSetting")
	FIntPoint ContainerSize;

	//Container的样式
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container|SlotSetting")
	FSlotAppearance SlotAppearance;
	
	//显示库存？
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container|SlotSetting")
	bool bShowInventory;
	
	//这个是什么类型的容器
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container|SlotSetting")
	FGameplayTag ContainerTag;
	
	//容器规则
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container|SlotSetting")
	EContainerRule NestingRule;

	//禁用物品类型
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container|SlotSetting")
	TArray<EItemType> BannedItemTypes;
	
	//要生成的Slot具体类型
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Container|SlotSetting")
	TSubclassOf<UInventorySlot> SlotClass;

	UPROPERTY(BlueprintReadWrite,Category="Container")
	UInventoryComponent*InventoryComponent;

	//这个容器拥有的所有Slots
	UPROPERTY(BlueprintReadWrite,Category="Container")
	TArray<UInventorySlot*> SlotWidgets;

	//这个容器的Id
	UPROPERTY(BlueprintReadWrite,Category="Container")
	int32 ContainerUId;

	//容器交互
	UPROPERTY(BlueprintReadWrite,Category="Container")
	FOnContainerInteract OnContainerInteract;

	//创建Slots
	UFUNCTION(BlueprintCallable,Category="Container")
	void CreateSlots(const int32&InContainerUId,UInventoryComponent*InInventoryComponent);

	//设置容器ID
	UFUNCTION(BlueprintCallable,Category="Container")
	void SetContainerUId(const int32 &InContainerUId);
	
	//获取装备容器 有可能为空
	UFUNCTION(BlueprintCallable,Category="Container")
	UEquipSlot*GetParentEquipSlot();

	/*设置当前容器的边框颜色为成功/允许*/
	UFUNCTION(BlueprintCallable,Category="Container")
	void SetOutlineSupported();

	/*当前容器的边框颜色恢复成默认*/
	UFUNCTION(BlueprintCallable,Category="Container")
	void ResetOutline();

	/*删除所有ItemWidget*/
	UFUNCTION(BlueprintCallable,Category="Container")
	void RemoveAllItemWidgets();

	/*加载所有的ItemWidget*/
	UFUNCTION(BlueprintCallable,Category="Container")
	void LoadItemWidget();

	/*商人吗*/
	UFUNCTION(BlueprintPure,Category="Container")
	bool IsVendorContainer();

	/*生成Slot信息*/
	UFUNCTION(BlueprintCallable,Category="Container")
	TArray<FSlotInfo> GenerateSlotInfos();
};
