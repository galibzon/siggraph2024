
#include "Siggraph2024GemModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <Siggraph2024Gem/Siggraph2024GemTypeIds.h>

#include <Clients/Siggraph2024GemSystemComponent.h>

namespace Siggraph2024Gem
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(Siggraph2024GemModuleInterface,
        "Siggraph2024GemModuleInterface", Siggraph2024GemModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(Siggraph2024GemModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(Siggraph2024GemModuleInterface, AZ::SystemAllocator);

    Siggraph2024GemModuleInterface::Siggraph2024GemModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            Siggraph2024GemSystemComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList Siggraph2024GemModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<Siggraph2024GemSystemComponent>(),
        };
    }
} // namespace Siggraph2024Gem
