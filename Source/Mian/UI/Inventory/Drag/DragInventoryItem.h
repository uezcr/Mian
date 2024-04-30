// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "UI/Inventory/Item/InventoryItemShow.h"
#include "DragInventoryItem.generated.h"

class UInventorySlot;
class UInventoryComponent;
/**
 * lzy TODO:拖拽丢弃
 */
UCLASS()
class MIAN_API UDragInventoryItem : public UDragDropOperation
{
	GENERATED_BODY()
public:
	//拖拽的那个人的库存组件
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DragInventoryItem")
	UInventoryComponent*DragOwner;

	//拖拽的Widget
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DragInventoryItem")
	UInventoryItemShow* DraggingItem;

	//这个道具在原来的那个SlotWidget上
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DragInventoryItem")
	UInventorySlot*SourceSlot;

	//这个道具现在占用的Slot编号
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="DragInventoryItem")
	TArray<int32> SourceItemSlotIDs;

	//获取拖拽的是那个Container中的那个Slot
	UFUNCTION(BlueprintPure,Category="DragInventoryItem")
	bool GetSource(int32&OutContainerUId,int32&OutSlotId);

	/*拖拽源的交互组件*/
	UFUNCTION(BlueprintPure,Category="DragInventoryItem")
	UInventoryComponent*GetSourceInventory();

	//拖拽完成
	virtual void Drop_Implementation(const FPointerEvent& PointerEvent) override;
	//拖拽失败/取消
	virtual void DragCancelled_Implementation(const FPointerEvent& PointerEvent) override;
	//拖拽中
	virtual void Dragged_Implementation(const FPointerEvent& PointerEvent) override;

	//拖拽完成/失败时恢复原本的样子
	void DropFinish();
};
