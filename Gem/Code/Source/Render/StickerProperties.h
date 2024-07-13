/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once


#include <AzCore/RTTI/TypeInfoSimple.h>
#include <AzCore/Math/Color.h>

#include <AtomCore/Instance/Instance.h>
#include <Atom/RPI.Public/Image/StreamingImage.h>

namespace Siggraph2024Gem
{
    //! The sticker is a flat rectangular object, overlayed on top of the
    //! rendered scene.
    //! The user defines its location and size based on normalized
    //! device coordinates:
    //! X (Left) = 0.0, X (Right) = 1.0
    //! Y (Top) = 0.0, Y (Bottom) = 1.0
    //! Width = Always a positive number. When equal to 1.0 it matches the viewport width.
    //! Height = Always a positive number. When equal to 1.0 it matches the viewport height.
    class StickerProperties final
    {
    public:
        AZ_CLASS_ALLOCATOR_DECL;
        AZ_TYPE_INFO_WITH_NAME_DECL(StickerProperties);
        AZ_RTTI_NO_TYPE_INFO_DECL();

        static void Reflect(AZ::ReflectContext* context);

        StickerProperties() = default;

        bool operator==(const StickerProperties& rhs) const;
        bool operator!=(const StickerProperties& rhs) const;

        // Typically a value between 0.0 and 1.0. It can be negative.
        float m_posX = 0.4;
        // Typically a value between 0.0 and 1.0. It can be negative.
        float m_posY = 0.4;
        // Typically a value between 0.0 and 1.0. Always positive.
        float m_width = 0.2;
        // Typically a value between 0.0 and 1.0. Always positive.
        float m_height = 0.2;

        // DO NOT Serialize.
        // Set at runtime by the StickerComponent upon User selection.
        AZ::Data::Instance<AZ::RPI::StreamingImage> m_texture;
    };

} //namespace Siggraph2024Gem
