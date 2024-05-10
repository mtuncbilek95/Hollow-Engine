#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorSet : public GraphicsDeviceObject
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) 
		{}
		virtual ~DescriptorSet() override = default;

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::DescriptorSet; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		DescriptorPool* mOwnerPool;
		DescriptorLayout* mLayout;
	};
}