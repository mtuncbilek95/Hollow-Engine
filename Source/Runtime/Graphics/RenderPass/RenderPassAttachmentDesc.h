#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureSample.h>
#include <Runtime/Graphics/RenderPass/RenderPassLoadOperation.h>
#include <Runtime/Graphics/RenderPass/RenderPassStoreOperation.h>
#include <Runtime/Graphics/Texture/TextureMemoryLayout.h>

namespace Hollow
{
	struct RUNTIME_API RenderPassAttachmentDesc
	{
		SharedPtr<TextureView> pTextureView;
		TextureFormat Format;
		TextureSample SampleCount;
		RenderPassLoadOperation ColorLoadOp;
		RenderPassStoreOperation ColorStoreOp;
		RenderPassLoadOperation DepthLoadOp;
		RenderPassStoreOperation DepthStoreOp;
		TextureMemoryLayout InputLayout;
		TextureMemoryLayout OutputLayout;
		byte MipLevel;
		byte ArrayLevel;
	};
}