// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupDisplay.generated.h"

class UTextBlock;

/**
 * 拾取的UI
 */
UCLASS()
class MIAN_API UPickupDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite,Category="PickupDisplay",meta=(BindWidget))
	UTextBlock*Txt_DisplayName;

	void SetPickupText(FText InText,const int32&InAmount);
};
