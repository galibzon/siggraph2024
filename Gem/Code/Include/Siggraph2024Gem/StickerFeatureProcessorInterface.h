/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/base.h>
#include <Atom/RPI.Public/FeatureProcessor.h>
#include <Atom/RPI.Public/Image/StreamingImage.h>

namespace Siggraph2024Gem
{
    class StickerProperties;

    // StickerFeatureProcessorInterface provides an interface to the feature processor for code outside of Atom
    class StickerFeatureProcessorInterface
        : public AZ::RPI::FeatureProcessor
    {
    public:
        AZ_RTTI(StickerFeatureProcessorInterface, "{49B5AE45-85FD-4B3A-90EC-048BE1F65FDD}", AZ::RPI::FeatureProcessor);

        virtual void AddSticker(AZ::EntityId ownerId, const StickerProperties& properties) = 0;
        virtual void UpdateStickerGeometry(AZ::EntityId ownerId, const StickerProperties& properties) = 0;
        virtual void UpdateStickerTexture(AZ::EntityId ownerId, const StickerProperties& properties) = 0;
        virtual void RemoveSticker(AZ::EntityId ownerId) = 0;

    };
}
