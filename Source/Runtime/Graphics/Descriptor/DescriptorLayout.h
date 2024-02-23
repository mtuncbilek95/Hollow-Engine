#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutDesc.h>

namespace Hollow
{
	class RUNTIME_API DescriptorLayout : public GraphicsDeviceObject
	{
	public:
		DescriptorLayout(const DescriptorLayoutDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice) , mEntries(desc.Entries) {}
		virtual ~DescriptorLayout() override = default;

		const Array<DescriptorLayoutEntry>& GetEntries() const { return mEntries; }

		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept { return GraphicsDeviceObjectType::DescriptorLayout; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		Array<DescriptorLayoutEntry> mEntries;
	};
}
