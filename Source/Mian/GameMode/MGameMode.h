// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MGameMode.generated.h"

class UInventoryComponent;
struct FContainerInfo;
/**
 * 
 */
UCLASS()
class MIAN_API AMGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	//TODO:lzy Begin
	//给容器添加世界唯一标识符
	TArray<int32> AddUniqueContainers(TArray<FContainerInfo>&InContainers,UInventoryComponent*InTargetInventoryComp);

	//容器ID,ID会在一句游戏内累加
	int32 CreatedContainers=0;
	//TODO:lzy End

	
};
