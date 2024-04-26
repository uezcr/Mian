// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryBackground.generated.h"

/*
 * lzy TODO:拖拽丢弃
 */

class UDragInventoryItem;
UCLASS()
class MIAN_API UInventoryBackground : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="InventoryBackground")
	void DragConfirmed(const int32&InQuantity,UDragInventoryItem*InDragItem);

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
