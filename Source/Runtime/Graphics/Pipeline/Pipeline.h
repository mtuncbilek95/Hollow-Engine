#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Pipeline/GraphicsPipelineDesc.h>

namespace Hollow
{
	class RUNTIME_API Pipeline : public GraphicsDeviceObject
	{
	public:
		Pipeline(const GraphicsPipelineDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice), mBlendState(desc.BlendState), 
            mDepthStencilState(desc.DepthStencilState), mInputLayout(desc.InputLayout), mMultisample(desc.Multisample), 
            mRasterizerState(desc.RasterizerState), mResourceLayout(desc.ResourceLayout), mGraphicsShaders(desc.GraphicsShaders), 
			mViewport(desc.Viewport), mScissor(desc.Scissor), mColorAttachmentCount(desc.ColorAttachmentCount), mColorAttachmentFormats(desc.ColorAttachmentFormats),
			mDepthAttachmentFormat(desc.DepthAttachmentFormat), mStencilAttachmentFormat(desc.StencilAttachmentFormat),
            mBindPoint(PipelineBindPoint::Graphics)
        {}
		virtual ~Pipeline() override = default;

        BlendStateDesc GetBlendState() const { return mBlendState; }
        DepthStencilStateDesc GetDepthStencilState() const { return mDepthStencilState; }
        InputLayoutDesc GetInputLayout() const { return mInputLayout; }
        MultisampleDesc GetMultisample() const { return mMultisample; }
        RasterizerStateDesc GetRasterizerState() const { return mRasterizerState; }
        ResourceLayoutDesc GetResourceLayout() const { return mResourceLayout; }
        ArrayList<SharedPtr<Shader>> GetGraphicsShaders() const { return mGraphicsShaders; }
        ViewportDesc GetViewport() const { return mViewport; }
        ScissorDesc GetScissor() const { return mScissor; }
        PipelineBindPoint GetBindPoint() const { return mBindPoint; }

        FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::Pipeline; }

        virtual void OnShutdown() noexcept override = 0;

	private:
        BlendStateDesc mBlendState;
        DepthStencilStateDesc mDepthStencilState;
        InputLayoutDesc mInputLayout;
        MultisampleDesc mMultisample;
        RasterizerStateDesc mRasterizerState;
        ResourceLayoutDesc mResourceLayout;
        ArrayList<SharedPtr<Shader>> mGraphicsShaders;
        ViewportDesc mViewport;
        ScissorDesc mScissor;
		u32 mColorAttachmentCount;
		ArrayList<TextureFormat> mColorAttachmentFormats;
		TextureFormat mDepthAttachmentFormat;
		TextureFormat mStencilAttachmentFormat;

        PipelineBindPoint mBindPoint;
	};
}