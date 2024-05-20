#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/PipelineBindPoint.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendStateDesc.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/DepthStencilStateDesc.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputLayoutDesc.h>
#include <Runtime/Graphics/Pipeline/Multisample/MultisampleDesc.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/RasterizerStateDesc.h>
#include <Runtime/Graphics/Pipeline/ResourceLayout/ResourceLayoutDesc.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/Core/ViewportDesc.h>
#include <Runtime/Graphics/Core/ScissorDesc.h>

namespace Hollow
{
    struct RUNTIME_API GraphicsPipelineDesc
    {
        BlendStateDesc BlendState;
        DepthStencilStateDesc DepthStencilState;
        InputLayoutDesc InputLayout;
        MultisampleDesc Multisample;
        RasterizerStateDesc RasterizerState;
        ResourceLayoutDesc ResourceLayout;
        ArrayList<SharedPtr<Shader>> GraphicsShaders;
        ViewportDesc Viewport;
        ScissorDesc Scissor;
		uint32 ColorAttachmentCount;
		ArrayList<TextureFormat> ColorAttachmentFormats;
		TextureFormat DepthAttachmentFormat;
		TextureFormat StencilAttachmentFormat;
    };
}