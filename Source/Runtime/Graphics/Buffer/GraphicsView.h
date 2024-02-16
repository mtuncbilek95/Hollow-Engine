#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Buffer/GraphicsViewDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsView : public GraphicsDeviceObject
	{
	public:
		GraphicsView(const GraphicsViewDesc& desc) : mUsage(desc.Usage), mCPUAccess(desc.CPUAccess), 
			mUsageType(desc.UsageType), mMiscFlags(desc.MiscFlags), mSizeInBytes(desc.SizeInBytes), 
			mStructureByteStride(desc.StructureByteStride), mInitialData(desc.pInitialData) 
		{}
		virtual ~GraphicsView() override = default;

		BufferUsage GetUsage() const noexcept { return mUsage; }
		BufferCPUAccess GetCPUAccess() const noexcept { return mCPUAccess; }
		ResourceUsageType GetResourceUsage() const noexcept { return mUsageType; }
		uint8 GetMiscFlags() const noexcept { return mMiscFlags; }
		uint32 GetSizeInBytes() const noexcept { return mSizeInBytes; }
		uint64 GetStructureByteStride() const noexcept { return mStructureByteStride; }
		void* GetInitialData() const noexcept { return mInitialData; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept override { return GraphicsDeviceObjectType::GraphicsView; }

	private:
		BufferUsage mUsage;
		BufferCPUAccess mCPUAccess;
		ResourceUsageType mUsageType;
		uint8 mMiscFlags;
		uint32 mSizeInBytes;
		uint64 mStructureByteStride;

		void* mInitialData;
	};
}
