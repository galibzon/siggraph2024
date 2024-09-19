/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Atom/RHI.Reflect/ShaderInputNameIndex.h>

#include <Atom/RPI.Reflect/Image/StreamingImageAsset.h>
#include <Atom/RPI.Public/Image/StreamingImage.h>
#include <Atom/RPI.Public/Shader/Shader.h>
#include <Atom/RPI.Public/PipelineState.h>

#include <Render/StickerProperties.h>

#include <Siggraph2024Gem/StickerFeatureProcessorInterface.h>

namespace Siggraph2024Gem
{
    class StickerFeatureProcessor final
        : public StickerFeatureProcessorInterface
    {
    public:
        AZ_RTTI(StickerFeatureProcessor, "{179A8955-D188-42C7-8A58-9148D28DA4C9}", StickerFeatureProcessorInterface);
        AZ_CLASS_ALLOCATOR(StickerFeatureProcessor, AZ::SystemAllocator)

        static void Reflect(AZ::ReflectContext* context);

        StickerFeatureProcessor() = default;
        virtual ~StickerFeatureProcessor() = default;

        // FeatureProcessor overrides
        void Activate() override;
        void Deactivate() override;
        void Simulate(const FeatureProcessor::SimulatePacket& packet) override;
        void Render(const FeatureProcessor::RenderPacket& packet) override;
        void AddRenderPasses(AZ::RPI::RenderPipeline* pipeline) override;

        // StickerFeatureProcessorInterface overrides
        void AddSticker(AZ::EntityId ownerId, const StickerProperties& properties) override;
        void UpdateStickerGeometry(AZ::EntityId ownerId, const StickerProperties& properties) override;
        void UpdateStickerTexture(AZ::EntityId ownerId, const StickerProperties& properties) override;
        void RemoveSticker(AZ::EntityId ownerId) override;

        // AZ::RPI::SceneNotificationBus::Handler overrides...
        void OnRenderPipelineChanged(AZ::RPI::RenderPipeline* pipeline, AZ::RPI::SceneNotification::RenderPipelineChangeType changeType) override;

    private:
        struct StickerVertex
        {
            // It's good to use AZStd::array to keep all the data
            // nicely packed.
            AZStd::array<float, 3> m_position;
            AZStd::array<float, 2> m_uv;
        };

        static constexpr size_t VertexCountPerSticker = 4;
        struct StickerRenderData
        {
            StickerProperties m_properties;
            // Interleaved Position+UV.
            AZStd::array<StickerVertex, VertexCountPerSticker> m_cpuVertices;
            // Interleaved Position+UV.
            AZ::Data::Instance<AZ::RPI::Buffer> m_gpuVertexBuffer;
            AZ::RHI::StreamBufferView m_gpuVertexBufferView;
            AZ::Data::Instance<AZ::RPI::ShaderResourceGroup> m_drawSrg;
            //! We need one DrawPacket for each Sticker object.
            //! Even though a DrawPacket can contain several DrawItems,
            //! The idea is that each DrawItem represents the same object but
            //! rendered at a different Pass. In other words each DrawItem is
            //! connected to a Pass. We only use one RasterPass.
            AZ::RHI::GeometryView m_geometryView;
            AZ::RHI::ConstPtr<AZ::RHI::DrawPacket> m_drawPacket;
            //! The DrawSrg needs update. Most likely the texture asset for this
            //! sticker changed.
            bool m_needsSrgUpdate = true;
        };

        void InitShader();
        void InitPipelineState();
        void InitIndexBuffer();
        
        void InitCpuVertices(AZStd::array<StickerVertex, 4>& cpuVertices, const StickerProperties& properties);
        void InitGpuVertexBuffer(const AZStd::string& bufferName, StickerRenderData& renderData);
        void UpdateGpuVertexBuffer(StickerRenderData& renderData);

        //! Builds a DrawPacket from scratch for a StickerRenderData
        void BuildDrawPacket(StickerRenderData& renderData);

        static constexpr char LogName[] = "StickerFeatureProcessor";

        static constexpr char DefaultImageAssetPath[] = "Textures/Siggraph2024Gem/asset_texture.png.streamingimage";
        AZ::Data::Asset<AZ::RPI::StreamingImageAsset> m_defaultImageAsset;
        AZ::Data::Instance<AZ::RPI::StreamingImage> m_defaultImage;

        // The sticker shader
        static constexpr char StickerShaderPath[] = "Shaders/Siggraph2024Gem/Sticker.azshader";
        AZ::Data::Instance<AZ::RPI::Shader> m_stickerShader;
        AZ::RHI::Ptr<AZ::RHI::ShaderResourceGroupLayout> m_perDrawSrgLayout;
        AZ::RHI::ShaderInputNameIndex m_textureIndex = "m_texture";
        // The index Buffer shared across all Sticker DrawPackets.
        // Created once and lives forever.
        AZ::Data::Instance<AZ::RPI::Buffer> m_stickerIndexBuffer;
        AZ::RHI::IndexBufferView m_stickerIndexBufferView;

        // Common pipeline state. Uses m_stickerShader
        AZ::RPI::Ptr<AZ::RPI::PipelineStateForDraw> m_pipelineState;

        AZStd::unordered_map<AZ::EntityId, StickerRenderData> m_stickers;

    };
}
