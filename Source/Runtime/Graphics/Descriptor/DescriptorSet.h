#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorSet : public GraphicsDeviceObject
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice) 
		{}
		virtual ~DescriptorSet() override = default;

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::DescriptorSet; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		SharedPtr<DescriptorPool> mOwnerPool;
		SharedPtr<DescriptorLayout> mLayout;
	};
}