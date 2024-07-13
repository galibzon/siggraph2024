/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>

#include "StickerProperties.h"

namespace Siggraph2024Gem
{
    AZ_CLASS_ALLOCATOR_IMPL(StickerProperties, AZ::SystemAllocator);
    AZ_TYPE_INFO_WITH_NAME_IMPL(StickerProperties, "Siggraph2024Gem::StickerProperties", "{060FA8E3-DFC0-4A91-B5BE-ECA8E8819D33}");
    AZ_RTTI_NO_TYPE_INFO_IMPL(StickerProperties);

    bool StickerProperties::operator==(const StickerProperties& rhs) const
    {
        return  AZ::IsClose(m_posX, rhs.m_posX) &&
                AZ::IsClose(m_posY, rhs.m_posY) &&
                AZ::IsClose(m_width, rhs.m_width) &&
                AZ::IsClose(m_height, rhs.m_height)
                ;
    }

    bool StickerProperties::operator!=(const StickerProperties& rhs) const
    {
        return !(*this == rhs);
    }

    void StickerProperties::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<StickerProperties>()
                ->Version(1)
                ->Field("PosX", &StickerProperties::m_posX)
                ->Field("PosY", &StickerProperties::m_posY)
                ->Field("Width", &StickerProperties::m_width)
                ->Field("Height", &StickerProperties::m_height)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<StickerProperties>(
                    "Sticker Properties", "Properties of a sticker overlay")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &StickerProperties::m_posX, "Position X", "Viewport Normalized Sticker Position X.\
 0.0 is viewport left. 1.0 is viewport right. Can be negative")
                        ->Attribute(AZ::Edit::Attributes::Min, -2.0)
                        ->Attribute(AZ::Edit::Attributes::Max, 2.0)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &StickerProperties::m_posY, "Position Y", "Viewport Normalized Sticker Position Y.\
 0.0 is viewport yop. 1.0 is viewport bottom. Can be negative")
                        ->Attribute(AZ::Edit::Attributes::Min, -2.0)
                        ->Attribute(AZ::Edit::Attributes::Max, 2.0)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &StickerProperties::m_width, "Width", "Viewport Normalized Sticker Width.\
 1.0 means the Sticker matches the width of the viewport. Always positive")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.0)
                        ->Attribute(AZ::Edit::Attributes::Max, 4.0)
                    ->DataElement(AZ::Edit::UIHandlers::Slider, &StickerProperties::m_height, "Height", "Viewport Normalized Sticker HEight.\
 1.0 means the Sticker matches the height of the viewport. Always positive")
                        ->Attribute(AZ::Edit::Attributes::Min, 0.0)
                        ->Attribute(AZ::Edit::Attributes::Max, 4.0)
                    ;
            }
        }
    }

} //namespace Siggraph2024Gem
