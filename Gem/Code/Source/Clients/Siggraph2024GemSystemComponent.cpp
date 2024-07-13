/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "Siggraph2024GemSystemComponent.h"

#include <Siggraph2024Gem/Siggraph2024GemTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

#include <Atom/RPI.Public/FeatureProcessorFactory.h>

#include <Render/StickerProperties.h>
#include <Render/StickerFeatureProcessor.h>

namespace Siggraph2024Gem
{
    AZ_COMPONENT_IMPL(Siggraph2024GemSystemComponent, "Siggraph2024GemSystemComponent",
        Siggraph2024GemSystemComponentTypeId);

    void Siggraph2024GemSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<Siggraph2024GemSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }

        StickerProperties::Reflect(context);
        StickerFeatureProcessor::Reflect(context);
    }

    void Siggraph2024GemSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("Siggraph2024GemSystemService"));
    }

    void Siggraph2024GemSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("Siggraph2024GemSystemService"));
    }

    void Siggraph2024GemSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("RPISystem"));
    }

    void Siggraph2024GemSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    Siggraph2024GemSystemComponent::Siggraph2024GemSystemComponent()
    {
        if (Siggraph2024GemInterface::Get() == nullptr)
        {
            Siggraph2024GemInterface::Register(this);
        }
    }

    Siggraph2024GemSystemComponent::~Siggraph2024GemSystemComponent()
    {
        if (Siggraph2024GemInterface::Get() == this)
        {
            Siggraph2024GemInterface::Unregister(this);
        }
    }

    void Siggraph2024GemSystemComponent::Init()
    {
    }

    void Siggraph2024GemSystemComponent::Activate()
    {
        Siggraph2024GemRequestBus::Handler::BusConnect();

        AZ::RPI::FeatureProcessorFactory::Get()->RegisterFeatureProcessorWithInterface<StickerFeatureProcessor, StickerFeatureProcessorInterface>();

        // Register the event handler that helps us register our custom pass templates.
        m_loadTemplatesHandler = AZ::RPI::PassSystemInterface::OnReadyLoadTemplatesEvent::Handler(
            [&]() {
                LoadPassTemplateMappings();
            }
        );
        AZ::RPI::PassSystemInterface::Get()->ConnectEvent(m_loadTemplatesHandler);

    }

    void Siggraph2024GemSystemComponent::Deactivate()
    {
        AZ::RPI::FeatureProcessorFactory::Get()->UnregisterFeatureProcessor<StickerFeatureProcessor>();

        Siggraph2024GemRequestBus::Handler::BusDisconnect();
    }

    void Siggraph2024GemSystemComponent::LoadPassTemplateMappings()
    {
        constexpr char passTemplatesFile[] = "Passes/Siggraph2024Gem/PassTemplates.azasset";
        AZ::RPI::PassSystemInterface::Get()->LoadPassTemplateMappings(passTemplatesFile);
    }

} // namespace Siggraph2024Gem
