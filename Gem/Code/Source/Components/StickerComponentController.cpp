/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Components/StickerComponentController.h>

#include <AzCore/Asset/AssetManager.h>
#include <AzCore/Asset/AssetManagerBus.h>
#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <AzFramework/Entity/EntityContextBus.h>
#include <AzFramework/Entity/EntityContext.h>
#include <AzFramework/Scene/Scene.h>
#include <AzFramework/Scene/SceneSystemInterface.h>

#include <AzCore/RTTI/BehaviorContext.h>

#include <Atom/RPI.Public/Scene.h>
#include <Atom/RPI.Public/ViewportContextBus.h>
#include <Atom/RPI.Public/ViewportContext.h>

namespace Siggraph2024Gem
{
    void StickerComponentConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<StickerComponentConfig>()
                ->Version(1)
                ->Field("StickerProperties", &StickerComponentConfig::m_stickerProperties)
                ->Field("ImageAsset", &StickerComponentConfig::m_imageAsset)
                ->Field("PreserveImageProportions", &StickerComponentConfig::m_preserveOriginalImageProportions)
                ;

            if (auto editContext = serializeContext->GetEditContext())
            {
                editContext->Class<StickerComponentConfig>(
                    "SigParticlesComponentConfig", "Properties of the Sticker.")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::Show)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &StickerComponentConfig::m_stickerProperties, "Sticker Properties", "")
                    ->DataElement(AZ::Edit::UIHandlers::Default, &StickerComponentConfig::m_imageAsset, "Image Asset", "")
                    ->DataElement(AZ::Edit::UIHandlers::CheckBox, &StickerComponentConfig::m_preserveOriginalImageProportions, "Preserve proportions", "")
                    ;
            }
        }
    }

    void StickerComponentController::Reflect(AZ::ReflectContext* context)
    {
        StickerComponentConfig::Reflect(context);

        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<StickerComponentController>()
                ->Version(0)
                ->Field("Configuration", &StickerComponentController::m_configuration);

            if (AZ::EditContext* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<StickerComponentController>(
                    "StickerComponentController", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ->DataElement(AZ::Edit::UIHandlers::Default, &StickerComponentController::m_configuration, "Configuration", "")
                        ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ;
            }
        }
    }

    void StickerComponentController::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        dependent.push_back(AZ_CRC_CE("TransformService"));
    }

    void StickerComponentController::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("StickerService"));
    }

    void StickerComponentController::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("StickerService"));
    }

    void StickerComponentController::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("TransformService"));
    }

    StickerComponentController::StickerComponentController(const StickerComponentConfig& config)
        : m_configuration(config)
    {
    }

    void StickerComponentController::Activate(AZ::EntityId entityId)
    {
        m_entityId = entityId;

        // We need to listed to Viewport Size changes.
        auto atomViewportRequests = AZ::RPI::ViewportContextRequests::Get();
        AZ_Assert(atomViewportRequests, "AtomViewportContextRequests interface not found!");
        const AZ::Name contextName = atomViewportRequests->GetDefaultViewportContextName();
        m_viewportSize = atomViewportRequests->GetViewportContextByName(contextName)->GetViewportSize();
        AZ::RPI::ViewportContextNotificationBus::Handler::BusConnect(contextName);

        m_featureProcessor = AZ::RPI::Scene::GetFeatureProcessorForEntity<StickerFeatureProcessorInterface>(entityId);
        AZ_Assert(m_featureProcessor, "StickerComponentController was unable to find a StickerFeatureProcessor on the EntityContext provided.");

        // Command the Feature Processor to create a Renderable Sticker.
        m_featureProcessor->AddSticker(entityId, m_configuration.m_stickerProperties);

        // If the image asset appears to be valid, let's load it asynchronously.
        auto textureAssetId = m_configuration.m_imageAsset.GetId();
        if (textureAssetId.IsValid())
        {
            AZ::Data::AssetBus::Handler::BusConnect(textureAssetId);
            m_configuration.m_imageAsset.QueueLoad();
        }
        m_prevConfiguration = m_configuration;
    }

    void StickerComponentController::Deactivate()
    {
        // This Sticker should not be rendered anymore.
        m_featureProcessor->RemoveSticker(m_entityId);

        AZ::Data::AssetBus::Handler::BusDisconnect();
        AZ::RPI::ViewportContextNotificationBus::Handler::BusDisconnect();
    }

    void StickerComponentController::SetConfiguration(const StickerComponentConfig& config)
    {
        m_configuration = config;
    }

    const StickerComponentConfig& StickerComponentController::GetConfiguration() const
    {
        return m_configuration;
    }

    void StickerComponentController::OnConfigurationChanged()
    {
        if (m_prevConfiguration.m_stickerProperties != m_configuration.m_stickerProperties)
        {
            if (m_configuration.m_preserveOriginalImageProportions)
            {
                m_configuration.m_stickerProperties.m_height = CalculateProportionalStickerHeight(m_configuration.m_stickerProperties);
            }
            m_featureProcessor->UpdateStickerGeometry(m_entityId, m_configuration.m_stickerProperties);
        }
        if (m_prevConfiguration.m_imageAsset != m_configuration.m_imageAsset)
        {
            if (m_prevConfiguration.m_imageAsset.GetId().IsValid())
            {
                AZ::Data::AssetBus::Handler::BusDisconnect();
                m_prevConfiguration.m_imageAsset.Reset();
                m_prevConfiguration.m_stickerProperties.m_texture.reset();
                m_featureProcessor->UpdateStickerTexture(m_entityId, m_prevConfiguration.m_stickerProperties);
            }
            auto textureAssetId = m_configuration.m_imageAsset.GetId();
            if (textureAssetId.IsValid())
            {
                AZ::Data::AssetBus::Handler::BusConnect(textureAssetId);
                m_configuration.m_imageAsset.QueueLoad();
            }
        }

        m_prevConfiguration = m_configuration;
    }

    void StickerComponentController::OnAssetStateChanged(AZ::Data::Asset<AZ::Data::AssetData> asset, [[maybe_unused]] bool isReload)
    {
        if (m_configuration.m_imageAsset.GetId() == asset.GetId())
        {
            AZ_Printf(LogName, "The sticker texture asset is ready: %s", asset.GetHint().c_str());
            m_configuration.m_imageAsset = asset;
            auto updateTexture = [this]()
                {
                    m_configuration.m_stickerProperties.m_texture = AZ::RPI::StreamingImage::FindOrCreate(m_configuration.m_imageAsset);
                    if (m_configuration.m_preserveOriginalImageProportions)
                    {
                        m_configuration.m_stickerProperties.m_height = CalculateProportionalStickerHeight(m_configuration.m_stickerProperties);
                        m_featureProcessor->UpdateStickerGeometry(m_entityId, m_configuration.m_stickerProperties);
                    }
                    m_featureProcessor->UpdateStickerTexture(m_entityId, m_configuration.m_stickerProperties);
                };
            AZ::TickBus::QueueFunction(AZStd::move(updateTexture));
        }
    }

    ////////////////////////////////////////////////////////////////////////
    //! Data::AssetBus START
    void StickerComponentController::OnAssetReady(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        const bool isReload = false;
        OnAssetStateChanged(asset, isReload);
    }

    void StickerComponentController::OnAssetReloaded(AZ::Data::Asset<AZ::Data::AssetData> asset)
    {
        const bool isReload = true;
        OnAssetStateChanged(asset, isReload);
    }
    //! Data::AssetBus END
    ////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // AZ::RPI::ViewportContextNotificationBus::Handler interface START
    void StickerComponentController::OnViewportSizeChanged(AzFramework::WindowSize size)
    {
        if (!m_configuration.m_preserveOriginalImageProportions)
        {
            return;
        }

        m_viewportSize = size;
        m_configuration.m_stickerProperties.m_height = CalculateProportionalStickerHeight(m_configuration.m_stickerProperties);
        m_prevConfiguration.m_stickerProperties.m_height = m_configuration.m_stickerProperties.m_height;
        m_featureProcessor->UpdateStickerGeometry(m_entityId, m_configuration.m_stickerProperties);
    }
    // AZ::RPI::ViewportContextNotificationBus::Handler interface END
    ////////////////////////////////////////////////////////////////////////////

    float StickerComponentController::CalculateProportionalStickerHeight(const StickerProperties& stickerProperties) const
    {
        if (!stickerProperties.m_texture)
        {
            return stickerProperties.m_height;
        }

        AZ_Assert(m_viewportSize.m_height > 0, "m_viewportSize is not initialized.");

        const float viewportRatio = aznumeric_cast<float>(m_viewportSize.m_width) / aznumeric_cast<float>(m_viewportSize.m_height);
        const auto imageSize = stickerProperties.m_texture->GetDescriptor().m_size;
        const float imageRatio = aznumeric_cast<float>(imageSize.m_width) / aznumeric_cast<float>(imageSize.m_height);
        const float normalizedProportionalHeight = (stickerProperties.m_width / imageRatio) * viewportRatio;
        return normalizedProportionalHeight;
    }
}
