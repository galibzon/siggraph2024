/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/Siggraph2024GemSystemComponent.h>

namespace Siggraph2024Gem
{
    /// System component for Siggraph2024Gem editor
    class Siggraph2024GemEditorSystemComponent
        : public Siggraph2024GemSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = Siggraph2024GemSystemComponent;
    public:
        AZ_COMPONENT_DECL(Siggraph2024GemEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        Siggraph2024GemEditorSystemComponent();
        ~Siggraph2024GemEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace Siggraph2024Gem
