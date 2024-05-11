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
            mRenderPass(desc.pRenderPass), mViewport(desc.Viewport), mScissor(desc.Scissor), mSubpassIndex(desc.SubpassIndex),
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
        SharedPtr<RenderPass> GetRenderPass() const { return mRenderPass; }
        ViewportDesc GetViewport() const { return mViewport; }
        ScissorDesc GetScissor() const { return mScissor; }
        byte GetSubpassIndex() const { return mSubpassIndex; }
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
        SharedPtr<RenderPass> mRenderPass;
        ViewportDesc mViewport;
        ScissorDesc mScissor;
        byte mSubpassIndex;

        PipelineBindPoint mBindPoint;
	};
}