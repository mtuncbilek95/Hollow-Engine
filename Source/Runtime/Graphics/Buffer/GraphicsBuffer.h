#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>

namespace Hollow
{
	class GraphicsMemory;

	/**
	 * @class GraphicsBuffer
	 * @brief Represents a buffer object that can be used for various purposes.
	 */
	class RUNTIME_API GraphicsBuffer : public GraphicsDeviceObject
	{
	public:
		GraphicsBuffer(const GraphicsBufferDesc& desc, const SharedPtr<GraphicsDevice> pDevice) : GraphicsDeviceObject(pDevice), 
			mBufferUsage(desc.Usage), mShareMode(desc.ShareMode), mSubSizeInBytes(desc.SubSizeInBytes), mSubResourceCount(desc.SubResourceCount), 
			mTotalSizeInBytes(desc.SubSizeInBytes * desc.SubResourceCount),
			mMemory(desc.pMemory)
		{}
		virtual ~GraphicsBuffer() override = default;

		GraphicsBufferUsage GetBufferUsage() const { return mBufferUsage; }
		ShareMode GetShareMode() const { return mShareMode; }
		u32 GetSubSizeInBytes() const { return mSubSizeInBytes; }
		u32 GetSubResourceCount() const { return mSubResourceCount; }
		u32 GetTotalSizeInBytes() const { return mTotalSizeInBytes; }
		SharedPtr<GraphicsMemory> GetMemory() const { return mMemory; }

		// Inherited via GraphicsDeviceObject
		FORCEINLINE virtual GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::GraphicsBuffer; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		GraphicsBufferUsage mBufferUsage;
		ShareMode mShareMode;
		u32 mSubSizeInBytes;
		u32 mSubResourceCount;
		u32 mTotalSizeInBytes;

		SharedPtr<GraphicsMemory> mMemory;
	};
}