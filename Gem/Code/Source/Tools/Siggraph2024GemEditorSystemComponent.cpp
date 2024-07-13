/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Serialization/SerializeContext.h>
#include "Siggraph2024GemEditorSystemComponent.h"

#include <Siggraph2024Gem/Siggraph2024GemTypeIds.h>

namespace Siggraph2024Gem
{
    AZ_COMPONENT_IMPL(Siggraph2024GemEditorSystemComponent, "Siggraph2024GemEditorSystemComponent",
        Siggraph2024GemEditorSystemComponentTypeId, BaseSystemComponent);

    void Siggraph2024GemEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<Siggraph2024GemEditorSystemComponent, Siggraph2024GemSystemComponent>()
                ->Version(0);
        }
    }

    Siggraph2024GemEditorSystemComponent::Siggraph2024GemEditorSystemComponent() = default;

    Siggraph2024GemEditorSystemComponent::~Siggraph2024GemEditorSystemComponent() = default;

    void Siggraph2024GemEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("Siggraph2024GemSystemEditorService"));
    }

    void Siggraph2024GemEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("Siggraph2024GemSystemEditorService"));
    }

    void Siggraph2024GemEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void Siggraph2024GemEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void Siggraph2024GemEditorSystemComponent::Activate()
    {
        Siggraph2024GemSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void Siggraph2024GemEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        Siggraph2024GemSystemComponent::Deactivate();
    }

} // namespace Siggraph2024Gem
