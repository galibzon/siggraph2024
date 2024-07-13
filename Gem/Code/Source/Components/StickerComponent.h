/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Components/StickerComponentController.h>
#include <AzFramework/Components/ComponentAdapter.h>

namespace Siggraph2024Gem
{
    inline constexpr AZ::TypeId StickerComponentTypeId { "{7C780489-C393-4DEE-86AD-752FECBCDED8}" };

    class StickerComponent final
        : public AzFramework::Components::ComponentAdapter<StickerComponentController, StickerComponentConfig>
    {
    public:
        using BaseClass = AzFramework::Components::ComponentAdapter<StickerComponentController, StickerComponentConfig>;
        AZ_COMPONENT(StickerComponent, StickerComponentTypeId, BaseClass);

        StickerComponent() = default;
        StickerComponent(const StickerComponentConfig& config);

        static void Reflect(AZ::ReflectContext* context);
    };
}
