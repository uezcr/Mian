#include "EquipmentDefinition.h"
#include "EquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(EquipmentDefinition)

UEquipmentDefinition::UEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	InstanceType = UEquipmentInstance::StaticClass();
}
