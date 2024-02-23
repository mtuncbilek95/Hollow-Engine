#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorSet : public GraphicsDeviceObject
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mLayout(desc.pLayout), mPool(desc.pPool) {}
		virtual ~DescriptorSet() override = default;

		const SharedPtr<DescriptorLayout>& GetLayout() const { return mLayout; }
		const SharedPtr<DescriptorPool>& GetPool() const { return mPool; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept { return GraphicsDeviceObjectType::DescriptorSet; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		SharedPtr<DescriptorLayout> mLayout;
		SharedPtr<DescriptorPool> mPool;
	};
}
