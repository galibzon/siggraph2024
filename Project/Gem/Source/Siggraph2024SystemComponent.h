
#pragma once

#include <AzCore/Component/Component.h>

#include <Siggraph2024/Siggraph2024Bus.h>

namespace Siggraph2024
{
    class Siggraph2024SystemComponent
        : public AZ::Component
        , protected Siggraph2024RequestBus::Handler
    {
    public:
        AZ_COMPONENT_DECL(Siggraph2024SystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        Siggraph2024SystemComponent();
        ~Siggraph2024SystemComponent();

    protected:
        ////////////////////////////////////////////////////////////////////////
        // Siggraph2024RequestBus interface implementation

        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Init() override;
        void Activate() override;
        void Deactivate() override;
        ////////////////////////////////////////////////////////////////////////
    };
}
