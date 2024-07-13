
#include <AzCore/Serialization/SerializeContext.h>

#include "Siggraph2024SystemComponent.h"

#include <Siggraph2024/Siggraph2024TypeIds.h>

namespace Siggraph2024
{
    AZ_COMPONENT_IMPL(Siggraph2024SystemComponent, "Siggraph2024SystemComponent",
        Siggraph2024SystemComponentTypeId);

    void Siggraph2024SystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<Siggraph2024SystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void Siggraph2024SystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("Siggraph2024Service"));
    }

    void Siggraph2024SystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("Siggraph2024Service"));
    }

    void Siggraph2024SystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void Siggraph2024SystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    Siggraph2024SystemComponent::Siggraph2024SystemComponent()
    {
        if (Siggraph2024Interface::Get() == nullptr)
        {
            Siggraph2024Interface::Register(this);
        }
    }

    Siggraph2024SystemComponent::~Siggraph2024SystemComponent()
    {
        if (Siggraph2024Interface::Get() == this)
        {
            Siggraph2024Interface::Unregister(this);
        }
    }

    void Siggraph2024SystemComponent::Init()
    {
    }

    void Siggraph2024SystemComponent::Activate()
    {
        Siggraph2024RequestBus::Handler::BusConnect();
    }

    void Siggraph2024SystemComponent::Deactivate()
    {
        Siggraph2024RequestBus::Handler::BusDisconnect();
    }
}
