#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Pipeline/GraphicsPipelineDesc.h>
#include <Engine/Graphics/Pipeline/PipelineBindPoint.h>

namespace Hollow
{
	class ENGINE_API Pipeline : public DeviceObject
	{
	public:
		Pipeline(const GraphicsPipelineDesc& desc, SharedPtr<GraphicsDevice> device) : DeviceObject(device), mBlendState(desc.BlendState), 
            mDepthStencilState(desc.DepthStencilState), mInputLayout(desc.InputLayout), mMultisample(desc.Multisample), 
            mRasterizerState(desc.RasterizerState), mResourceLayout(desc.ResourceLayout), mGraphicsShaders(desc.GraphicsShaders), 
            mPushConstants(desc.PushConstants), mViewport(desc.Viewport), mScissor(desc.Scissor), mColorAttachmentCount(desc.ColorAttachmentCount),
            mColorAttachmentFormats(desc.ColorAttachmentFormats), mDepthAttachmentFormat(desc.DepthAttachmentFormat), 
            mStencilAttachmentFormat(desc.StencilAttachmentFormat), mBindPoint(PipelineBindPoint::Graphics) {}
		virtual ~Pipeline() override = default;

        const BlendStateDesc& GetBlendState() const { return mBlendState; }
        const DepthStencilStateDesc& GetDepthStencilState() const { return mDepthStencilState; }
        const InputLayoutDesc& GetInputLayout() const { return mInputLayout; }
        const MultisampleDesc& GetMultisample() const { return mMultisample; }
        const RasterizerStateDesc& GetRasterizerState() const { return mRasterizerState; }
        const ResourceLayoutDesc& GetResourceLayout() const { return mResourceLayout; }
        const DArray<SharedPtr<Shader>>& GetGraphicsShaders() const { return mGraphicsShaders; }
        const PushConstantDesc& GetPushConstants() const { return mPushConstants; }
        const ViewportDesc& GetViewport() const { return mViewport; }
        const ScissorDesc& GetScissor() const { return mScissor; }
        u32 GetColorAttachmentCount() const { return mColorAttachmentCount; }
        const DArray<TextureFormat>& GetColorAttachmentFormats() const { return mColorAttachmentFormats; }
        TextureFormat GetDepthAttachmentFormat() const { return mDepthAttachmentFormat; }
        TextureFormat GetStencilAttachmentFormat() const { return mStencilAttachmentFormat; }

	private:
        BlendStateDesc mBlendState;
        DepthStencilStateDesc mDepthStencilState;
        InputLayoutDesc mInputLayout;
        MultisampleDesc mMultisample;
        RasterizerStateDesc mRasterizerState;
        ResourceLayoutDesc mResourceLayout;
        DArray<SharedPtr<Shader>> mGraphicsShaders;
        PushConstantDesc mPushConstants;
        ViewportDesc mViewport;
        ScissorDesc mScissor;
        u32 mColorAttachmentCount;
        DArray<TextureFormat> mColorAttachmentFormats;
        TextureFormat mDepthAttachmentFormat;
        TextureFormat mStencilAttachmentFormat;

        PipelineBindPoint mBindPoint;
	};
}
