/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Atom/RHI.Reflect/InputStreamLayoutBuilder.h>
#include <Atom/RHI/DrawPacketBuilder.h>

#include <Atom/RPI.Reflect/Asset/AssetUtils.h>
#include <Atom/RPI.Public/RPIUtils.h>
#include <Atom/RPI.Public/View.h>
#include <Atom/RPI.Public/RenderPipeline.h>
#include <Atom/RPI.Public/Pass/PassFilter.h>

#include "StickerFeatureProcessor.h"

namespace Siggraph2024Gem
{
    void StickerFeatureProcessor::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext
                ->Class<StickerFeatureProcessor, FeatureProcessor>()
                ;
        }
    }

    //////////////////////////////////////////////////////////////////
    // FeatureProcessor overrides START
    void StickerFeatureProcessor::Activate()
    {
        // Typically loading texture assets should be done asynchronously.
        // But for small textures it's ok to block the main thread.
        m_defaultImageAsset = AZ::RPI::AssetUtils::LoadCriticalAsset<AZ::RPI::StreamingImageAsset>(DefaultImageAssetPath);
        AZ_Assert(m_defaultImageAsset.IsReady(), "It seems default image asset '%s' doesn't exist.", DefaultImageAssetPath);
        m_defaultImage = AZ::RPI::StreamingImage::FindOrCreate(m_defaultImageAsset);

        // We need to be notified whenever the Render Pipeline is valid.
        // At that time we can initialize the PipelineState aka Pipeline State Object.
        EnableSceneNotification();      
    }

    void StickerFeatureProcessor::Deactivate()
    {
        DisableSceneNotification();
    }

    void StickerFeatureProcessor::Simulate([[maybe_unused]] const FeatureProcessor::SimulatePacket& packet)
    {
        for (auto & [entityId, renderData] : m_stickers)
        {
            if (!renderData.m_needsSrgUpdate)
            {
                continue;
            }
            // Use our default sticker texture if needed.
            if (!renderData.m_properties.m_texture)
            {
                renderData.m_drawSrg->SetImage(m_textureIndex, m_defaultImage);
            }
            else
            {
                renderData.m_drawSrg->SetImage(m_textureIndex, renderData.m_properties.m_texture);
            }
            renderData.m_drawSrg->Compile();
            renderData.m_needsSrgUpdate = false;
        }
    }

    void StickerFeatureProcessor::Render(const FeatureProcessor::RenderPacket& packet)
    {
        if (m_stickers.empty())
        {
            return;
        }
        for (auto& view : packet.m_views)
        {
            if (!view->HasDrawListTag(m_stickerShader->GetDrawListTag()))
            {
                continue;
            }
            constexpr float depth = 0.f;
            for (auto const& [entityId, renderData] : m_stickers)
            {
                view->AddDrawPacket(renderData.m_drawPacket.get(), depth);
            }
        }
    }

    void StickerFeatureProcessor::AddRenderPasses(AZ::RPI::RenderPipeline* pipeline)
    {
        // If this function is called it means @pipeline allows modifications.
        // The Sticker Pass should be added only if @pipeline has a Pass called "UIPass".
        // Our StickerPass will be added before "UIPass".

        const auto StickerPassName = AZ::Name("StickerPass");
        const auto UiPassName = AZ::Name("UIPass");

        // Early exit if there's no "UIPass".
        {
            AZ::RPI::PassFilter passFilter = AZ::RPI::PassFilter::CreateWithPassName(UiPassName, pipeline);
            AZ::RPI::Pass* existingPass = AZ::RPI::PassSystemInterface::Get()->FindFirstPass(passFilter);
            if (existingPass == nullptr)
            {
                AZ_Printf(LogName, "Can not add '%s' because the pipeline doesn't have a pass named '%s'", StickerPassName.GetCStr(), UiPassName.GetCStr());
                return;
            }
        }

        // Early exit if the Sticker Pass already exists
        {
            AZ::RPI::PassFilter passFilter = AZ::RPI::PassFilter::CreateWithPassName(StickerPassName, pipeline);
            AZ::RPI::Pass* existingPass = AZ::RPI::PassSystemInterface::Get()->FindFirstPass(passFilter);
            if (existingPass != nullptr)
            {
                AZ_Printf(LogName, "The pass '%s' already exists.", StickerPassName.GetCStr());
                return;
            }
        }

        // We can safely add the StickerPass to the pipeline
        static constexpr bool AddBefore = false;
        AddPassRequestToRenderPipeline(pipeline, "Passes/Siggraph2024Gem/StickerPassRequest.azasset", UiPassName.GetCStr(), AddBefore);

        // Validate we succeeded.
        {
            AZ::RPI::PassFilter passFilter = AZ::RPI::PassFilter::CreateWithPassName(StickerPassName, pipeline);
            AZ::RPI::Pass* existingPass = AZ::RPI::PassSystemInterface::Get()->FindFirstPass(passFilter);
            if (existingPass != nullptr)
            {
                AZ_Printf(LogName, "Successfully added '%s' to pipeline with name '%s'.", StickerPassName.GetCStr(), pipeline->GetDescriptor().m_name.c_str());
            }
            else
            {
                AZ_Error(LogName, false, "Failed to add '%s' to pipeline with name '%s'.", StickerPassName.GetCStr(), pipeline->GetDescriptor().m_name.c_str());
            }
        }

    }
    // FeatureProcessor overrides END
    //////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    // StickerFeatureProcessorInterface overrides start
    void StickerFeatureProcessor::AddSticker(AZ::EntityId ownerId, const StickerProperties& properties)
    {
        auto itemItor = m_stickers.find(ownerId);
        if (itemItor == m_stickers.end())
        {
            // Doesn't exist.
            m_stickers[ownerId] = {};
        }
        StickerRenderData& renderData = m_stickers[ownerId];
        renderData.m_properties = properties;

        InitCpuVertices(renderData.m_cpuVertices, properties);
        if (!renderData.m_gpuVertexBuffer)
        {
            InitGpuVertexBuffer(ownerId.ToString(), renderData);
        }
        else
        {
            UpdateGpuVertexBuffer(renderData);
        }
        // Create the DrawSrg if necessary
        if (!renderData.m_drawSrg)
        {
            renderData.m_drawSrg = AZ::RPI::ShaderResourceGroup::Create(m_stickerShader->GetAsset(), m_stickerShader->GetSupervariantIndex(), m_perDrawSrgLayout->GetName());
        }
        BuildDrawPacket(renderData);
        renderData.m_needsSrgUpdate = true;
    }

    void StickerFeatureProcessor::UpdateStickerGeometry(AZ::EntityId ownerId, const StickerProperties& properties)
    {
        auto itemItor = m_stickers.find(ownerId);
        if (itemItor == m_stickers.end())
        {
            // Doesn't exist.
            AddSticker(ownerId, properties);
            return;
        }
        StickerRenderData& renderData = m_stickers[ownerId];
        renderData.m_properties = properties;
        InitCpuVertices(renderData.m_cpuVertices, properties);
        UpdateGpuVertexBuffer(renderData);
    }

    void StickerFeatureProcessor::UpdateStickerTexture(AZ::EntityId ownerId, const StickerProperties& properties)
    {
        auto itemItor = m_stickers.find(ownerId);
        if (itemItor == m_stickers.end())
        {
            // Doesn't exist.
            AddSticker(ownerId, properties);
            return;
        }
        StickerRenderData& renderData = m_stickers[ownerId];
        renderData.m_properties.m_texture = properties.m_texture;
        renderData.m_needsSrgUpdate = true;
    }

    void StickerFeatureProcessor::RemoveSticker(AZ::EntityId ownerId)
    {
        auto itemItor = m_stickers.find(ownerId);
        if (itemItor == m_stickers.end())
        {
            return;
        }
        m_stickers.erase(itemItor);
    }
    // StickerFeatureProcessorInterface overrides end
    ////////////////////////////////////////////////////

    void StickerFeatureProcessor::InitShader()
    {
        m_stickerShader = AZ::RPI::LoadCriticalShader(StickerShaderPath);
        if (!m_stickerShader)
        {
            AZ_Error(LogName, false, "Failed to get shader '%s'", StickerShaderPath);
            return;
        }
        m_perDrawSrgLayout = m_stickerShader->FindShaderResourceGroupLayout(AZ::RPI::SrgBindingSlot::Draw);
        if (!m_perDrawSrgLayout)
        {
            AZ_Error(LogName, false, "Failed to get shader resource group layout");
            return;
        }
        // Force to discover the Texture Resource Index the next time
        // we set the texture to any of the Stickers.
        m_textureIndex.Reset();
    }

    void StickerFeatureProcessor::InitPipelineState()
    {
        if (m_pipelineState)
        {
            return;
        }

        m_pipelineState = aznew AZ::RPI::PipelineStateForDraw;
        m_pipelineState->Init(m_stickerShader);

        AZ::RHI::InputStreamLayoutBuilder layoutBuilder;
        layoutBuilder.AddBuffer()
            ->Channel("POSITION", AZ::RHI::Format::R32G32B32_FLOAT)
            ->Channel("UV0", AZ::RHI::Format::R32G32_FLOAT);
        layoutBuilder.SetTopology(AZ::RHI::PrimitiveTopology::TriangleStrip);
        auto inputStreamLayout = layoutBuilder.End();

        m_pipelineState->SetInputStreamLayout(inputStreamLayout);
        m_pipelineState->SetOutputFromScene(GetParentScene());
        m_pipelineState->Finalize();
    }

    void StickerFeatureProcessor::InitIndexBuffer()
    {
        if (m_stickerIndexBuffer)
        {
            return;
        }

        AZStd::array<uint16_t, VertexCountPerSticker> cpuIndices;
        for (auto i = 0; i < VertexCountPerSticker; i++)
        {
            cpuIndices[i] = aznumeric_cast<uint16_t>(i);
        }
        const auto triangleIndexDataSize = static_cast<uint32_t>(VertexCountPerSticker * sizeof(uint16_t));

        AZ::RPI::CommonBufferDescriptor desc;
        desc.m_poolType = AZ::RPI::CommonBufferPoolType::StaticInputAssembly;
        desc.m_bufferName = "StickerIndices";
        desc.m_byteCount = triangleIndexDataSize;
        desc.m_elementSize = sizeof(uint16_t);
        desc.m_bufferData = cpuIndices.data();
        m_stickerIndexBuffer = AZ::RPI::BufferSystemInterface::Get()->CreateBufferFromCommonPool(desc);

        // Setup triangle index buffer view
        AZ::RHI::IndexBufferView ibv(*m_stickerIndexBuffer->GetRHIBuffer(), 0, triangleIndexDataSize, AZ::RHI::IndexFormat::Uint16);
        m_stickerIndexBufferView = ibv;
    }

    void StickerFeatureProcessor::InitCpuVertices(AZStd::array<StickerVertex, VertexCountPerSticker>& cpuVertices, const StickerProperties& properties)
    {
        // We are building a 4 vertices triangle strip.
        // Triangles facing the user using right hand.
        // 0, 1, 2 -- 1, 3, 2.
        // 1--0 
        // |\ |
        // | \|
        // 3--2
        //Clip Space Coordinates
        //X goes from -1(left)   to 1(right)
        //Y goes from -1(bottom) to 1(top)
        //Z goes from  0(front)  to 1(back)
        //W should be 1 to avoid perspective divide.
        // (X, Y) = (0, 0) is the center of the screen.
        cpuVertices[0].m_position[0] = AZ::Lerp(-1.0f,  1.0f, properties.m_posX + properties.m_width);
        cpuVertices[0].m_position[1] = AZ::Lerp( 1.0f, -1.0f, properties.m_posY);
        cpuVertices[0].m_position[2] = 0.0f; // 0 because we don't care about depth.
        cpuVertices[0].m_uv[0] = 1.0f;
        cpuVertices[0].m_uv[1] = 0.0f;

        cpuVertices[1].m_position[0] = AZ::Lerp(-1.0f,  1.0f, properties.m_posX);
        cpuVertices[1].m_position[1] = cpuVertices[0].m_position[1];
        cpuVertices[1].m_position[2] = 0.0f; // 0 because we don't care about depth.
        cpuVertices[1].m_uv[0] = 0.0f;
        cpuVertices[1].m_uv[1] = 0.0f;

        cpuVertices[2].m_position[0] = cpuVertices[0].m_position[0];
        cpuVertices[2].m_position[1] = AZ::Lerp( 1.0f, -1.0f, properties.m_posY + properties.m_height);
        cpuVertices[2].m_position[2] = 0.0f; // 0 because we don't care about depth.
        cpuVertices[2].m_uv[0] = 1.0f;
        cpuVertices[2].m_uv[1] = 1.0f;

        cpuVertices[3].m_position[0] = cpuVertices[1].m_position[0];
        cpuVertices[3].m_position[1] = cpuVertices[2].m_position[1];
        cpuVertices[3].m_position[2] = 0.0f; // 0 because we don't care about depth.
        cpuVertices[3].m_uv[0] = 0.0f;
        cpuVertices[3].m_uv[1] = 1.0f;
    }


    void StickerFeatureProcessor::InitGpuVertexBuffer(const AZStd::string& bufferName, StickerRenderData& renderData)
    {
        const uint32_t elementCount = static_cast<uint32_t>(VertexCountPerSticker);
        const uint32_t elementSize = sizeof(StickerVertex);
        const uint32_t bufferSize = elementCount * elementSize; // bytecount

        AZ::RPI::CommonBufferDescriptor desc;
        desc.m_poolType = AZ::RPI::CommonBufferPoolType::StaticInputAssembly;
        desc.m_bufferName = bufferName;
        desc.m_byteCount = bufferSize;
        desc.m_elementSize = elementSize;
        desc.m_bufferData = renderData.m_cpuVertices.data();
        renderData.m_gpuVertexBuffer = AZ::RPI::BufferSystemInterface::Get()->CreateBufferFromCommonPool(desc);

        // While we are at it, initialize the StreamBufferView.
        renderData.m_gpuVertexBufferView[0] = AZ::RHI::StreamBufferView(*renderData.m_gpuVertexBuffer->GetRHIBuffer(), 0, bufferSize, elementSize);
    }

    void StickerFeatureProcessor::UpdateGpuVertexBuffer(StickerRenderData& renderData)
    {
        const uint32_t elementCount = static_cast<uint32_t>(VertexCountPerSticker);
        const uint32_t elementSize = sizeof(StickerVertex);
        const uint32_t bufferSize = elementCount * elementSize; // bytecount

        renderData.m_gpuVertexBuffer->UpdateData(renderData.m_cpuVertices.data(), bufferSize);
    }

    void StickerFeatureProcessor::BuildDrawPacket(StickerRenderData& renderData)
    {
        if (renderData.m_drawPacket)
        {
            return;
        }
        AZ::RHI::DrawIndexed drawIndexed;
        drawIndexed.m_instanceCount = 1;
        drawIndexed.m_instanceOffset = 0;
        drawIndexed.m_vertexOffset = 0;
        drawIndexed.m_indexCount = VertexCountPerSticker;
        drawIndexed.m_indexOffset = 0;

        AZ::RHI::DrawPacketBuilder drawPacketBuilder{ AZ::RHI::MultiDevice::AllDevices };
        drawPacketBuilder.Begin(nullptr);
        drawPacketBuilder.SetDrawArguments(drawIndexed);
        drawPacketBuilder.SetIndexBufferView(m_stickerIndexBufferView);
        drawPacketBuilder.AddShaderResourceGroup(renderData.m_drawSrg->GetRHIShaderResourceGroup());

        AZ::RHI::DrawPacketBuilder::DrawRequest drawRequest;
        drawRequest.m_listTag = m_stickerShader->GetDrawListTag();
        drawRequest.m_pipelineState = m_pipelineState->GetRHIPipelineState();
        drawRequest.m_streamBufferViews = renderData.m_gpuVertexBufferView;
        drawPacketBuilder.AddDrawItem(drawRequest);
        renderData.m_drawPacket = drawPacketBuilder.End();
    }

    // AZ::RPI::SceneNotificationBus::Handler overrides...
    void StickerFeatureProcessor::OnRenderPipelineChanged([[maybe_unused]] AZ::RPI::RenderPipeline* pipeline, AZ::RPI::SceneNotification::RenderPipelineChangeType changeType)
    {
        if (m_pipelineState || (changeType != AZ::RPI::SceneNotification::RenderPipelineChangeType::Added))
        {
            return;
        }
        InitShader();
        InitPipelineState();
        InitIndexBuffer();
    }




}
