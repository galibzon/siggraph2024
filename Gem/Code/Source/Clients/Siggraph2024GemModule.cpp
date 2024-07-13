
#include <Siggraph2024Gem/Siggraph2024GemTypeIds.h>
#include <Siggraph2024GemModuleInterface.h>
#include "Siggraph2024GemSystemComponent.h"

#include <AzCore/RTTI/RTTI.h>

#include <Components/StickerComponent.h>

namespace Siggraph2024Gem
{
    class Siggraph2024GemModule
        : public Siggraph2024GemModuleInterface
    {
    public:
        AZ_RTTI(Siggraph2024GemModule, Siggraph2024GemModuleTypeId, Siggraph2024GemModuleInterface);
        AZ_CLASS_ALLOCATOR(Siggraph2024GemModule, AZ::SystemAllocator);

        Siggraph2024GemModule()
        {
            m_descriptors.insert(m_descriptors.end(),
                {
                    Siggraph2024GemSystemComponent::CreateDescriptor(),
                    StickerComponent::CreateDescriptor(),
                });
        }

        AZ::ComponentTypeList GetRequiredSystemComponents() const
        {
            return AZ::ComponentTypeList{ azrtti_typeid<Siggraph2024GemSystemComponent>() };
        }
    };
}// namespace Siggraph2024Gem

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), Siggraph2024Gem::Siggraph2024GemModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_Siggraph2024Gem, Siggraph2024Gem::Siggraph2024GemModule)
#endif
