// 


#include "InventorySlot.h"

#include "NiagaraSpriteRendererProperties.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Inventory/Container/Container.h"
#include "UI/Inventory/Container/MovableWindow.h"
#include "UI/Inventory/Drag/DragInventoryItem.h"

#define LOCTEXT_NAMESPACE "InventorySlot"
void UInventorySlot::InitializeWidget(UContainer* InContainer, const bool& InbDebugText, const FGameplayTag& InSlotTag,
	const FSlotAppearance& InSlotAppearance, const int32& InSlotId, const int32& InParentContainerUID)
{
	Container = InContainer;
	bDebugText = InbDebugText;
	SlotTag = InSlotTag;
	SlotAppearance = InSlotAppearance;
	SlotId = InSlotId;
	ParentContainerUID = InParentContainerUID;
}

void UInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventorySlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	UDragInventoryItem*DragInventoryItem = Cast<UDragInventoryItem>(InOperation);
	if(!DragInventoryItem) return false;
	
	//确定鼠标在Slot的那一侧
	FVector2d MousePosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	bool bRight = MousePosition.X > SlotAppearance.SlotSize.X / 2;
	bool bDown = MousePosition.Y > SlotAppearance.SlotSize.Y / 2;
	
	return true;
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	DropSlotID = -1;
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlot::SetParentContainerUId(const int32& InContainerId)
{
	ParentContainerUID = InContainerId;
}

void UInventorySlot::SetSlotColor(const FLinearColor& InNewColor)
{
	if (Border_SlotBG)
	{
		FSlateBrush TempBrush = Border_SlotBG->Background;
		TempBrush.TintColor = InNewColor;
		TempBrush.OutlineSettings.Color = SlotAppearance.OutlineColor;
		TempBrush.OutlineSettings.Width = SlotAppearance.OutlineWidth;
		Border_SlotBG->SetBrush(TempBrush);

		if (UInventoryComponent*LocalPlayerInventoryComponent = UInventoryLibrary::GetLocalPlayerInventoryComponent(GetWorld()))
		{
			LocalPlayerInventoryComponent->AddColorChangedSlot(this);
		}
	}
}

void UInventorySlot::ApplySettings(const FSlotAppearance& InSlotAppearance)
{
	if(Size_Main)
	{
		Size_Main->SetWidthOverride(InSlotAppearance.SlotSize.X);
		Size_Main->SetHeightOverride(InSlotAppearance.SlotSize.Y);
	}
	
	if (Border_SlotBG)
	{
		FSlateBrush TempBrush = Border_SlotBG->Background;
		TempBrush.TintColor = InSlotAppearance.DefaultSlotColor;
		TempBrush.OutlineSettings.Color = InSlotAppearance.OutlineColor;
		TempBrush.OutlineSettings.Width = InSlotAppearance.OutlineWidth;
		Border_SlotBG->SetBrush(TempBrush);
	}
}

void UInventorySlot::RemoveItemWidget()
{
	if (ItemWidgetIsValid())
	{
		ItemWidget->RemoveFromRoot();
		Container->OnContainerInteract.Broadcast(FItemInfoDef(),Container->InventoryComponent->GetContainers());
	}
	if (ItemInventoryTab)
	{
		ItemInventoryTab->RemoveFromRoot();
	}
}

void UInventorySlot::SlotDropEvent()
{
	//lzy TODO:明日待办
}

void UInventorySlot::UpdateItemWidget(const FItemInfoDef& InItemInfoDef, TArray<FContainerInfo> InContainers)
{
	if(InItemInfoDef.ItemID.IsNone()) return;

	FInventoryItem InventoryItem;
	UInventoryLibrary::GetDefaultIneventoryItemById(InItemInfoDef.ItemID,InventoryItem);
	
	/*道具存在*/
	if (ItemWidgetIsValid())
	{
		//有相同的存在
		bool bIsEqual = ItemWidget->GetItemInfo().ItemID == InItemInfoDef.ItemID;
		if(!bIsEqual) return;

		ItemWidget->SetQuantity(InItemInfoDef.ItemAmount);
		ItemWidget->SetRenderOpacity(1.0f);
	}

	//创建道具UI
	check(InventoryItemClass);
	ItemWidget = CreateWidget<UInventoryItemShow>(GetOwningPlayer(),InventoryItemClass);
	//图像大小
	FVector2D SlotSize = SlotAppearance.SlotSize;
	if (!UInventoryLibrary::IsInventorySlot(SlotTag))
	{
		SlotSize = SlotAppearance.SlotSize / InventoryItem.ItemSize;
	}
	ItemWidget->InitializeWidget(InItemInfoDef,SlotSize,Container->IsVendorContainer());
	float XPosition = (InventoryItem.ItemSize.Y * 0.5) / InventoryItem.ItemSize.X;
	FVector2D TransformPivot(XPosition,0.5);
	ItemWidget->SetRenderTransformPivot(TransformPivot);
	UCanvasPanelSlot*CanvasPanelSlot = Canvas_Slot->AddChildToCanvas(ItemWidget);
	CanvasPanelSlot->SetAutoSize(true);
	if (Container)
	{
		Container->OnContainerInteract.Broadcast(InItemInfoDef,InContainers);
	}

	//不是库存插槽,那就是装备槽
	if (UInventoryLibrary::IsInventorySlot(SlotTag)) return;
	UBorderSlot*BorderSlot = Cast<UBorderSlot>(ItemWidget->GetSizeBoxItem()->Slot);
	if (!BorderSlot) return;
	//拉伸图片
	BorderSlot->SetHorizontalAlignment(HAlign_Center);
	BorderSlot->SetVerticalAlignment(VAlign_Center);
	//图片的大小
	float ImageSize = SlotAppearance.SlotSize.GetMin();
	//占用的最大格子数量
	float ItemSize = InventoryItem.ItemSize.GetMin();
	if (InventoryItem.bStretchTexture)
	{
		ImageSize = SlotAppearance.SlotSize.GetMax();
		ItemSize= InventoryItem.ItemSize.GetMax();
	}
	FVector2D ActualItemSize(InventoryItem.ItemSize.X / ItemSize,InventoryItem.ItemSize.Y / ItemSize);
	FVector2D ActualImageSize(ImageSize * ActualItemSize.X,ImageSize * ActualItemSize.Y);
	ItemWidget->GetSizeBoxItem()->SetWidthOverride(ActualImageSize.X);
	ItemWidget->GetSizeBoxItem()->SetHeightOverride(ActualImageSize.Y);
}

void UInventorySlot::ResetSlotColor()
{
	if (Border_SlotBG)
	{
		FSlateBrush TempBrush = Border_SlotBG->Background;
		TempBrush.TintColor = SlotAppearance.DefaultSlotColor;
		TempBrush.OutlineSettings.Color = SlotAppearance.OutlineColor;
		TempBrush.OutlineSettings.Width = SlotAppearance.OutlineWidth;
		Border_SlotBG->SetBrush(TempBrush);
		if (UInventoryComponent*LocalPlayerInventoryComponent = UInventoryLibrary::GetLocalPlayerInventoryComponent(GetWorld()))
		{
			LocalPlayerInventoryComponent->AddColorChangedSlot(this);
		}
	}
}

bool UInventorySlot::ItemWidgetIsValid()
{
	if (Canvas_Slot && ItemWidget)
	{
		Canvas_Slot->HasChild(ItemWidget);
		return true;
	}
	return false;
}

void UInventorySlot::GetItemOnSlotID(int32& OutItemSlotId, FItemInfoDef& OutItemInfo)
{
	if (Container && Container->InventoryComponent)
	{
		OutItemSlotId = Container->InventoryComponent->GetContainerByUId(ParentContainerUID).Slots[SlotId].ItemSlotID;
		OutItemInfo = Container->InventoryComponent->GetItemByUIdAndSlotId(ParentContainerUID,OutItemSlotId);
	}
}

void UInventorySlot::DebugPrintInventorySlot()
{
	if(!Container) return;

	FContainerInfo ContainerInfo = Container->InventoryComponent->GetContainerByUId(ParentContainerUID);
	FSlotInfo SlotInfo = ContainerInfo.Slots[SlotId];
	FString Items;
	for (FItemInfoDef&ItemInfoDef:ContainerInfo.Items)
	{
		Items = FString::Printf(TEXT("%s \n Item%s (Quantity:%d) (Slot:%d)"),*Items,*ItemInfoDef.ItemID.ToString(),ItemInfoDef.ItemAmount,ItemInfoDef.CurrentSlotID);
	}
	
	FText PrintMessage=FText::Format(LOCTEXT("Print","Container UID:{Containerindex}\n Slot ID:{Slotindex}\n ItemTopLeft:{Topleftid}\n Empty:{isempty}\n {Items}")
		,ParentContainerUID,SlotId,SlotInfo.SlotID,SlotInfo.bIsEmpty,FText::FromString(Items));

	UKismetSystemLibrary::PrintText(GetWorld(),PrintMessage,true,true,FLinearColor::Green,300.f,TEXT("ContainerDebug"));
}

#undef LOCTEXT_NAMESPACE 