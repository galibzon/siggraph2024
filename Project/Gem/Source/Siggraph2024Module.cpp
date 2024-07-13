
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include "Siggraph2024SystemComponent.h"

#include <Siggraph2024/Siggraph2024TypeIds.h>

namespace Siggraph2024
{
    class Siggraph2024Module
        : public AZ::Module
    {
    public:
        AZ_RTTI(Siggraph2024Module, Siggraph2024ModuleTypeId, AZ::Module);
        AZ_CLASS_ALLOCATOR(Siggraph2024Module, AZ::SystemAllocator);

        Siggraph2024Module()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                Siggraph2024SystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<Siggraph2024SystemComponent>(),
            };
        }
    };
}// namespace Siggraph2024

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), Siggraph2024::Siggraph2024Module)
#else
AZ_DECLARE_MODULE_CLASS(Gem_Siggraph2024, Siggraph2024::Siggraph2024Module)
#endif
