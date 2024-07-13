
#pragma once

#include <Siggraph2024Gem/Siggraph2024GemTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace Siggraph2024Gem
{
    class Siggraph2024GemRequests
    {
    public:
        AZ_RTTI(Siggraph2024GemRequests, Siggraph2024GemRequestsTypeId);
        virtual ~Siggraph2024GemRequests() = default;
        // Put your public methods here
    };

    class Siggraph2024GemBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using Siggraph2024GemRequestBus = AZ::EBus<Siggraph2024GemRequests, Siggraph2024GemBusTraits>;
    using Siggraph2024GemInterface = AZ::Interface<Siggraph2024GemRequests>;

} // namespace Siggraph2024Gem
