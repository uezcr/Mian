#include "DragInventoryItem.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Inventory/Container/Container.h"
#include "UI/Inventory/Slot/InventorySlot.h"

bool UDragInventoryItem::GetSource(int32& OutContainerUId, int32& OutSlotId)
{
	if (SourceSlot && SourceSlot->GetContainer())
	{
		OutContainerUId = SourceSlot->GetContainer()->ContainerUId;
		OutSlotId = SourceSlot->GetSlotId();
		return true;
	}
	return false;
}

UInventoryComponent* UDragInventoryItem::GetSourceInventory()
{
	if (SourceSlot && SourceSlot->GetContainer() && SourceSlot->GetContainer()->InventoryComponent)
	{
		return SourceSlot->GetContainer()->InventoryComponent;
	}
	return nullptr;
}

void UDragInventoryItem::Drop_Implementation(const FPointerEvent& PointerEvent)
{
	Super::Drop_Implementation(PointerEvent);
	DropFinish();
}

void UDragInventoryItem::DragCancelled_Implementation(const FPointerEvent& PointerEvent)
{
	Super::DragCancelled_Implementation(PointerEvent);

	if (SourceSlot && SourceSlot->GetItemWidget())
	{
		SourceSlot->GetItemWidget()->SetRenderOpacity(1.0f);
	}
	
	if (SourceSlot && SourceSlot->GetContainer())
	{
		int32 OutContainerUId,OutSlotId;
		if (GetSource(OutContainerUId,OutSlotId))
		{
			FItemInfoDef ItemInfo = GetSourceInventory()->GetItemByUIdAndSlotId(OutContainerUId,OutSlotId);
			SourceSlot->GetContainer()->OnContainerInteract.Broadcast(ItemInfo,GetSourceInventory()->GetContainers());
		}
	}
	
	DropFinish();
}

void UDragInventoryItem::Dragged_Implementation(const FPointerEvent& PointerEvent)
{
	Super::Dragged_Implementation(PointerEvent);

	TArray<UContainer*> AllContainer = DragOwner->GetAllContainerWidgets();
	for (UContainer*ContainerWidget:AllContainer)
	{
		for (UInventorySlot*SlotWidget:ContainerWidget->SlotWidgets)
		{
			if (SlotWidget->GetItemWidget())
			{
				SlotWidget->GetItemWidget()->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
	if (SourceSlot)
	{
		SourceSlot->GetItemWidget()->SetRenderOpacity(0.2);
		SourceSlot->GetItemWidget()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (DraggingItem)
	{
		DraggingItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (GetSourceInventory())
	{
		int32 OutContainerUId,OutSlotId;
		if (GetSource(OutContainerUId,OutSlotId))
		{
			SourceItemSlotIDs = GetSourceInventory()->GetItemSlotIndexes(OutContainerUId,OutSlotId);
		}
		
	}
}

void UDragInventoryItem::DropFinish()
{
	if (!DragOwner) return;
	
	DragOwner->ResetAllColorSlots();
	for (UContainer*&ContainerWidget:DragOwner->GetAllContainerWidgets())
	{
		ContainerWidget->ResetOutline();
		for (UInventorySlot*&InventorySlot:ContainerWidget->SlotWidgets)
		{
			if (!InventorySlot->GetItemWidget())continue;

			InventorySlot->GetItemWidget()->ResetBackgroundColor();
			InventorySlot->GetItemWidget()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
