// 


#include "InventoryItemShow.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void UInventoryItemShow::InitializeWidget(const FItemInfoDef& InItem, const FVector2D& InOneSlotSize,const bool& InbCheckMoney)
{
	Item = InItem;
	OneSlotSize = InOneSlotSize;
	bCheckMoney = InbCheckMoney;
}

void UInventoryItemShow::ResetBackgroundColor()
{
	if (Broder_ItemBG)
	{
		Broder_ItemBG->SetBrushColor(ItemDefault.BackgroundColor);
	}
}

void UInventoryItemShow::SetQuantity(const int32& InNewAmount)
{
	Item.ItemAmount = InNewAmount;

	Txt_Amount->SetText(FText::FromString(FString::FromInt(InNewAmount)));
	if (InNewAmount == 1)
	{
		Txt_Amount->SetVisibility(ESlateVisibility::Hidden);
	}else
	{
		Txt_Amount->SetVisibility(ESlateVisibility::Visible);
	}
}
