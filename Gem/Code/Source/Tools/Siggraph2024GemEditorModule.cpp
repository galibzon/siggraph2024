
#include <Siggraph2024Gem/Siggraph2024GemTypeIds.h>
#include <Siggraph2024GemModuleInterface.h>
#include "Siggraph2024GemEditorSystemComponent.h"
#include "Components/EditorStickerComponent.h"

namespace Siggraph2024Gem
{
    class Siggraph2024GemEditorModule
        : public Siggraph2024GemModuleInterface
    {
    public:
        AZ_RTTI(Siggraph2024GemEditorModule, Siggraph2024GemEditorModuleTypeId, Siggraph2024GemModuleInterface);
        AZ_CLASS_ALLOCATOR(Siggraph2024GemEditorModule, AZ::SystemAllocator);

        Siggraph2024GemEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                Siggraph2024GemEditorSystemComponent::CreateDescriptor(),
                StickerComponent::CreateDescriptor(),
                EditorStickerComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<Siggraph2024GemEditorSystemComponent>(),
            };
        }
    };
}// namespace Siggraph2024Gem

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), Siggraph2024Gem::Siggraph2024GemEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_Siggraph2024Gem_Editor, Siggraph2024Gem::Siggraph2024GemEditorModule)
#endif
