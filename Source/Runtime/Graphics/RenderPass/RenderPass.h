#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>

namespace Hollow
{
	class DEPRECATED_CLASS RUNTIME_API RenderPass : public GraphicsDeviceObject
	{
	public:
		RenderPass(const RenderPassDesc& desc, SharedPtr<GraphicsDevice> device);
		virtual ~RenderPass() override = default;

		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept { return GraphicsDeviceObjectType::RenderPass; }
		virtual void OnShutdown() override = 0;
	};
}
