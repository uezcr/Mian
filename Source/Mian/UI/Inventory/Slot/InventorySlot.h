// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryType.h"
#include "InventorySlot.generated.h"

class USizeBox;
class UBorder;
class UInventoryItemShow;
struct FSlotAppearance;
struct FGameplayTag;
class UCanvasPanel;
class UContainer;
class UMovableWindow;
class UDragInventoryItem;
/**
 * 库存中插槽
 */
UCLASS()
class MIAN_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot",meta=(BindWidget))
	USizeBox*Size_Main;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot",meta=(BindWidget))
	UBorder*Border_SlotBG;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot",meta=(BindWidget))
	UCanvasPanel*Canvas_Slot;

	//调试吗
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot|SlotSetting")
	bool bDebugText;

	//这个是什么类型的Slot
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot|SlotSetting")
	FGameplayTag SlotTag;

	//Slot的样式
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot|SlotSetting")
	FSlotAppearance SlotAppearance;

	//这个Slot的ID
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot|SlotSetting")
	int32 SlotId;

	//这个Slot所属于的容器
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot|SlotSetting")
	int32 ParentContainerUID;

	//要创建的道具UI类
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot|SlotSetting")
	TSubclassOf<UInventoryItemShow> InventoryItemClass;
	
	//拖拽的那个SlotID
	UPROPERTY(BlueprintReadWrite,Category="InventorySlot")
	int32 DropSlotID;
	
	//属于的容器UMG
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InventorySlot")
	UContainer*Container;

	/*这个插槽内的道具Widget*/
	UPROPERTY(BlueprintReadWrite,Category="InventorySlot")
	UInventoryItemShow*ItemWidget;

	/*当前插槽内的这个Item是有容器的,这个容器打开的视口UI*/
	UPROPERTY(BlueprintReadWrite,Category="InventorySlot")
	UMovableWindow *ItemInventoryTab;

public:	
	/*初始化当前Slot的信息*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void InitializeWidget(UContainer*InContainer,const bool& InbDebugText,const FGameplayTag& InSlotTag,const FSlotAppearance& InSlotAppearance,const int32& InSlotId,const int32& InParentContainerUID);

	/*设置容器父级容器Id*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void SetParentContainerUId(const int32&InContainerId);

	/*应用这个外观设置*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void ApplySettings(const FSlotAppearance& InSlotAppearance);

	/*更新道具Widget*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void UpdateItemWidget(const FItemInfoDef&InItemInfoDef,TArray<FContainerInfo> InContainers);
	
	/*删除Slot里的道具*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void RemoveItemWidget();

	/** Slot拖拽时的事件
	 * @param InAmountForAction 
	 * @param InOperation 
	 */
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void SlotDropEvent(const int32&InAmountForAction,UDragInventoryItem*InOperation);

	/*设置插槽的背景颜色*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void SetSlotColor(const FLinearColor&InNewColor);

	/*恢复原本slot的默认颜色*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void ResetSlotColor();

	/*道具Widget是否有效*/
	UFUNCTION(BlueprintPure,Category="InventorySlot")
	bool ItemWidgetIsValid();

	/*获取当前Slot的Id和道具*/
	UFUNCTION(BlueprintPure,Category="InventorySlot")
	void GetItemOnSlotID(int32&OutItemSlotId,FItemInfoDef&OutItemInfo);

	/*打印调试信息*/
	UFUNCTION(BlueprintCallable,Category="InventorySlot")
	void DebugPrintInventorySlot();
	
	/*获取插槽里的道具UI*/
	UFUNCTION(BlueprintPure,Category="InventorySlot")
	FORCEINLINE UInventoryItemShow*GetItemWidget(){return ItemWidget;}

	/*获取容器UI*/
	UFUNCTION(BlueprintPure,Category="InventorySlot")
	FORCEINLINE UContainer*GetContainer() {return Container;};

	/*获取容器UI*/
	UFUNCTION(BlueprintPure,Category="InventorySlot")
	FORCEINLINE int32 GetSlotId()const {return SlotId;};
	
	//Begin UserWidget Interface
	virtual void NativeConstruct() override;
	//当Slate检测到一个widget开始被拖动时调用。
	virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation )override;
	//当鼠标被拖动到widget上时，在拖放过程中调用。
	virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	//当用户将内容放置到部件上时调用;终止拖放。
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )override;
	//在拖放过程中，当拖动离开一个widget时调用。
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	//鼠标进入
	virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )override;
	//鼠标离开
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent )override;

	//鼠标按下
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )override;
	//End UserWidget Interface

private:
	
};
