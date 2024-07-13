
#pragma once

#include <Siggraph2024/Siggraph2024TypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace Siggraph2024
{
    class Siggraph2024Requests
    {
    public:
        AZ_RTTI(Siggraph2024Requests, Siggraph2024RequestsTypeId);
        virtual ~Siggraph2024Requests() = default;
        // Put your public methods here
    };

    class Siggraph2024BusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using Siggraph2024RequestBus = AZ::EBus<Siggraph2024Requests, Siggraph2024BusTraits>;
    using Siggraph2024Interface = AZ::Interface<Siggraph2024Requests>;

} // namespace Siggraph2024
