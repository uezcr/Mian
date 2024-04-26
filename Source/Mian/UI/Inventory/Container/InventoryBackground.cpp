// 


#include "InventoryBackground.h"

void UInventoryBackground::DragConfirmed(const int32& InQuantity, UDragInventoryItem* InDragItem)
{
	
}

bool UInventoryBackground::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	return true;
}
