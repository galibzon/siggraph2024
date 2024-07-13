/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TransformBus.h>

#include <Atom/RPI.Public/ViewportContextBus.h>

#include <Render/StickerProperties.h>

#include <Siggraph2024Gem/StickerFeatureProcessorInterface.h>

namespace Siggraph2024Gem
{
    // User facing data that will be displayed in the Editor. This class holds
    // the data the User can configure about one particular Sticker.
    class StickerComponentConfig final
        : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(StickerComponentConfig, "{4BE7AD19-17D4-48C3-9EBB-032384073D07}", ComponentConfig);
        AZ_CLASS_ALLOCATOR(StickerComponentConfig, AZ::SystemAllocator);
        static void Reflect(AZ::ReflectContext* context);

        StickerComponentConfig() = default;

        //! Sticker dimensions, with normalized values.
        StickerProperties m_stickerProperties;
        //! The image asset, which will become the texture of the sticker at runtime.
        AZ::Data::Asset<AZ::RPI::StreamingImageAsset> m_imageAsset;
        //! If the user enables this flag, the m_stickerProperties.m_height property
        //! will be re-calculated on the fly as the user changes the Viewport
        //! Width, Height, and Sticker normalized width. The visual width and height ratio
        //! of the sticker will atuomatically match the width and height ratio of its source
        //! asset image.
        bool m_preserveOriginalImageProportions = true;
    };

    // Common Controller class for EditorStickerComponent and StickerComponent.
    // EditorStickerComponent is instantiated when We are running the Editor.
    // StickerComponent is instantiated when We are running the Game Launcher (aka Game Client). 
    class StickerComponentController final
        : private AZ::Data::AssetBus::Handler
        , private AZ::RPI::ViewportContextNotificationBus::Handler
    {
    public:
        friend class EditorStickerComponent;

        AZ_RTTI(StickerComponentController, "{0B87AB8A-9A3A-459D-BCF6-9BDE95E4C642}");
        AZ_CLASS_ALLOCATOR(StickerComponentController, AZ::SystemAllocator);

        static void Reflect(AZ::ReflectContext* context);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);

        StickerComponentController() = default;
        StickerComponentController(const StickerComponentConfig& config);

        void Activate(AZ::EntityId entityId);
        void Deactivate();
        void SetConfiguration(const StickerComponentConfig& config);
        const StickerComponentConfig& GetConfiguration() const;

    private:

        AZ_DISABLE_COPY(StickerComponentController);

        //! Data::AssetBus overrides
        void OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        void OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset) override;
        //! Common functionality of AssetBus events.
        void OnAssetStateChanged(AZ::Data::Asset<AZ::Data::AssetData> asset, bool isReload);

        // AZ::RPI::ViewportContextNotificationBus::Handler interface
        void OnViewportSizeChanged(AzFramework::WindowSize size) override;

        void OnConfigurationChanged();

        //! Helper function that calculates the Normalized Height of the Sticker, such
        //! that the visual Width To Height ration of the Sticker matches that of its source
        //! image asset, regardless of the Viewport width to height ratio.
        float CalculateProportionalStickerHeight(const StickerProperties& stickerProperties) const;

        static constexpr char LogName[] = "StickerComponentController";

        //! This is our Feature Processor, capable of rendering Stickers.
        StickerFeatureProcessorInterface* m_featureProcessor = nullptr;

        //! Tne EntityId that owns this Component.
        AZ::EntityId m_entityId;
        
        //! We keep track the previous value of the User configuration so we can detect
        //! what really changed (Geometry of the Sticker or Texture). 
        StickerComponentConfig m_configuration;
        StickerComponentConfig m_prevConfiguration;

        //! We can track of the Viewport Size because this information is needed
        //! by the helper function CalculateProportionalStickerHeight(...)
        AzFramework::WindowSize m_viewportSize;
    };
}
