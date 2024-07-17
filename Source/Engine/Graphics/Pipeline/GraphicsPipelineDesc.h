#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/BlendState/BlendStateDesc.h>
#include <Engine/Graphics/Pipeline/DepthStencilState/DepthStencilStateDesc.h>
#include <Engine/Graphics/Pipeline/InputLayout/InputLayoutDesc.h>
#include <Engine/Graphics/Pipeline/Multisample/MultisampleDesc.h>
#include <Engine/Graphics/Pipeline/RasterizerState/RasterizerStateDesc.h>
#include <Engine/Graphics/Pipeline/ResourceLayout/ResourceLayoutDesc.h>
#include <Engine/Graphics/Shader/Shader.h>
#include <Engine/Graphics/Pipeline/PushConstant/PushConstantDesc.h>
#include <Engine/Graphics/Pipeline/Core/ViewportDesc.h>
#include <Engine/Graphics/Pipeline/Core/ScissorDesc.h>


namespace Hollow
{
	struct ENGINE_API GraphicsPipelineDesc
	{
        BlendStateDesc BlendState;
        DepthStencilStateDesc DepthStencilState;
        InputLayoutDesc InputLayout;
        MultisampleDesc Multisample;
        RasterizerStateDesc RasterizerState;
        ResourceLayoutDesc ResourceLayout;
        DArray<WeakPtr<Shader>> GraphicsShaders;
        PushConstantDesc PushConstants;
        ViewportDesc Viewport;
        ScissorDesc Scissor;
        u32 ColorAttachmentCount;
        DArray<TextureFormat> ColorAttachmentFormats;
        TextureFormat DepthAttachmentFormat;
        TextureFormat StencilAttachmentFormat;
	};
}
