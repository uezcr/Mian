// 


#include "Container.h"

#include "Components/Border.h"
#include "Components/UniformGridPanel.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryLibrary.h"
#include "UI/Inventory/Slot/EquipSlot.h"
#include "UI/Inventory/Slot/InventorySlot.h"

void UContainer::CreateSlots(const int32& InContainerUId, UInventoryComponent* InInventoryComponent)
{
	ContainerUId = InContainerUId;
	InventoryComponent = InInventoryComponent;

	if (SlotWidgets.Num()>0)
	{
		for (UInventorySlot*&InventorySlot:SlotWidgets)
		{
			InventorySlot->RemoveFromParent();
		}
		Grid_Slots->ClearChildren();
		SlotWidgets.Empty();
	}
	
	UInventoryComponent* LocalPlayerInventoryComponent = UInventoryLibrary::GetLocalPlayerInventoryComponent(GetWorld());
	FIntPoint ActualContainerSize = UInventoryLibrary::SelectSize(ContainerTag,ContainerSize);
	//列
	for (int Row =0; Row<ActualContainerSize.Y;Row++)
	{
		//行
		for (int Cloumn =0; Cloumn<ActualContainerSize.X;Cloumn++)
		{
			int32 SlotId = UInventoryLibrary::TileToIndex(FIntPoint(Cloumn,Row),ContainerSize);

			UInventorySlot*InventorySlot = CreateWidget<UInventorySlot>(GetOwningPlayer(),SlotClass);
			InventorySlot->InitializeWidget(this,LocalPlayerInventoryComponent->bDebugMode,ContainerTag,SlotAppearance,SlotId,ContainerUId);
			Grid_Slots->AddChildToUniformGrid(InventorySlot,Row,Cloumn);
			SlotWidgets.Add(InventorySlot);
		}
	}
}

void UContainer::SetContainerUId(const int32& InContainerUId)
{
	ContainerUId = InContainerUId;
	if (SlotWidgets.Num()>0)
	{
		for (UInventorySlot*&InventorySlot:SlotWidgets)
		{
			InventorySlot->SetParentContainerUId(InContainerUId);
		}
	}
}

UEquipSlot* UContainer::GetParentEquipSlot()
{
	if (GetParent()&&GetParent()->GetOutermostObject()&& GetParent()->GetOutermostObject()->GetOutermostObject())
	{
		return Cast<UEquipSlot>(GetParent()->GetOutermostObject()->GetOutermostObject());
	}
	return nullptr;
}

void UContainer::SetOutlineSupported()
{
	if(!OutlineBorder) return;
	
	OutlineBorder->Background.OutlineSettings.Color = UInventoryLibrary::GetSupportedOutline();
}

void UContainer::ResetOutline()
{
	if(!OutlineBorder) return;
	FSlateBrush TempBrush = OutlineBorder->Background;
	TempBrush.OutlineSettings.Color = UInventoryLibrary::GetDefaultOutline();
	OutlineBorder->SetBrush(TempBrush);
}

void UContainer::RemoveAllItemWidgets()
{
	if (SlotWidgets.Num()>0)
	{
		for (UInventorySlot*&InventorySlot:SlotWidgets)
		{
			InventorySlot->RemoveItemWidget();
		}
	}
}

void UContainer::LoadItemWidget()
{
	if(!InventoryComponent) return;
	FContainerInfo ContainerInfo = InventoryComponent->GetContainerByUId(ContainerUId);
	for (FItemInfoDef&ItemInfoDef:ContainerInfo.Items)
	{
		SlotWidgets[ItemInfoDef.CurrentSlotID]->UpdateItemWidget(ItemInfoDef,InventoryComponent->GetContainers());
	}
}

bool UContainer::IsVendorContainer()
{
	if(!InventoryComponent) return false;
	return InventoryComponent->InventoryType == EInventoryType::E_Vendor;
}

TArray<FSlotInfo> UContainer::GenerateSlotInfos()
{
	TArray<FSlotInfo> SlotInfos;
	FIntPoint ActualContainerSize = UInventoryLibrary::SelectSize(ContainerTag,ContainerSize);
	//列
	for (int Row =0; Row<ActualContainerSize.Y;Row++)
	{
		//行
		for (int Cloumn =0; Cloumn<ActualContainerSize.X;Cloumn++)
		{
			FSlotInfo SlotInfo;
			SlotInfo.SlotTag = ContainerTag;
			SlotInfo.SlotID = UInventoryLibrary::TileToIndex(FIntPoint(Cloumn,Row),ContainerSize);
			SlotInfos.Add(SlotInfo);
		}
	}

	return SlotInfos;
}

void UContainer::UpdateSlotsWidget()
{
	
}
