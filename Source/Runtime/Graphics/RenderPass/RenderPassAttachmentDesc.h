#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureLayout.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassAttachmentDesc
	{
		SharedPtr<TextureView> pTextureView;
		TextureFormat Format;
		TextureSampleCount SampleCount;
		RenderPassLoadOperation ColorLoadOp;
		RenderPassStoreOperation ColorStoreOp;
		RenderPassLoadOperation DepthLoadOp;
		RenderPassStoreOperation DepthStoreOp;
		TextureLayout InputLayout;
		TextureLayout OutputLayout;
		byte MipLevel;
		byte ArrayLevel;
	};
}