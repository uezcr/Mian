#include "MGameMode.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryType.h"

TArray<int32> AMGameMode::AddUniqueContainers(TArray<FContainerInfo>& InContainers,UInventoryComponent* InTargetInventoryComp)
{
	TArray<int32> AssignedUIDs;
	if(!InTargetInventoryComp) return AssignedUIDs;
	
	for (FContainerInfo& ContainerInfo:InContainers)
	{
		AssignedUIDs.Add(CreatedContainers);
		ContainerInfo.ContainerUID = CreatedContainers++;
		InTargetInventoryComp->AddContainer(ContainerInfo);
	}
	return AssignedUIDs;
}
