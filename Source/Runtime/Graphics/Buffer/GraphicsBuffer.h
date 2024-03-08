#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>

namespace Hollow
{
	class GraphicsMemory;
	class RUNTIME_API GraphicsBuffer : public GraphicsDeviceObject
	{
	public:
		GraphicsBuffer(const GraphicsBufferDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), 
			mBufferUsage(desc.Usage), mShareMode(desc.ShareMode), mSizeInBytes(desc.SizeInBytes), mMemoryType(desc.MemoryType), 
			mMemory(nullptr)
		{}
		virtual ~GraphicsBuffer() override = default;

		void AllocateMemory();

		GraphicsBufferUsage GetBufferUsage() const { return mBufferUsage; }
		ShareMode GetShareMode() const { return mShareMode; }
		uint32 GetSizeInBytes() const { return mSizeInBytes; }
		GraphicsMemory* GetMemory() const { return mMemory.get(); }

		// Inherited via GraphicsDeviceObject
		FORCEINLINE virtual GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::GraphicsBuffer; }

		virtual void OnShutdown() noexcept override = 0;

	private:
		GraphicsBufferUsage mBufferUsage;
		GraphicsMemoryType mMemoryType;
		ShareMode mShareMode;
		uint32 mSizeInBytes;

		SharedPtr<GraphicsMemory> mMemory;
	};
}