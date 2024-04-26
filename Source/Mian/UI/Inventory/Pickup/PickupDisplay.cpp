// 


#include "PickupDisplay.h"

#include "Components/TextBlock.h"

#define LOCTEXT_NAMESPACE "PickupDisplay"
void UPickupDisplay::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPickupDisplay::SetPickupText(FText InText, const int32& InAmount)
{
	FString StrAmount;
	if (InAmount!=1)
	{
		StrAmount = FString::Printf(TEXT("(%d)"),InAmount);
	}

	FText TempText = FText::Format(LOCTEXT("PickupDisplay1","{0}{1}"),InText,FText::FromString(StrAmount));
	Txt_DisplayName->SetText(TempText);
}
#undef LOCTEXT_NAMESPACE