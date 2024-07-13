/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Atom/Feature/Utils/EditorRenderComponentAdapter.h>

#include <AzCore/Component/TickBus.h>
#include <AzFramework/Entity/EntityDebugDisplayBus.h>
#include <AzToolsFramework/API/ComponentEntitySelectionBus.h>
#include <AzToolsFramework/Entity/EditorEntityInfoBus.h>
#include <AzToolsFramework/ToolsComponents/EditorComponentAdapter.h>
#include <Components/StickerComponent.h>

#include <Siggraph2024Gem/Siggraph2024GemTypeIds.h>

namespace Siggraph2024Gem
{
    inline constexpr AZ::TypeId EditorComponentTypeId { "{BB15AE02-9047-4BB9-AC50-66C7B172BD77}" };

    class EditorStickerComponent final
        : public AZ::Render::EditorRenderComponentAdapter<StickerComponentController, StickerComponent, StickerComponentConfig>
        , private AzToolsFramework::EditorComponentSelectionRequestsBus::Handler
        , private AzFramework::EntityDebugDisplayEventBus::Handler
        , private AZ::TickBus::Handler
        , private AzToolsFramework::EditorEntityInfoNotificationBus::Handler
    {
    public:
        using BaseClass = AZ::Render::EditorRenderComponentAdapter <StickerComponentController, StickerComponent, StickerComponentConfig>;
        AZ_EDITOR_COMPONENT(EditorStickerComponent, EditorComponentTypeId, BaseClass);

        static void Reflect(AZ::ReflectContext* context);

        EditorStickerComponent();
        EditorStickerComponent(const StickerComponentConfig& config);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

    private:

        // AZ::TickBus overrides
        void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;

        // EditorComponentAdapter overrides
        AZ::u32 OnConfigurationChanged() override;

    };
}
