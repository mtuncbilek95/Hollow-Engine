#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>
#include <Runtime/ShaderCompiler/ShaderIODescriptor.h>
#include <Runtime/ShaderCompiler/ShaderDescriptorSet.h>

namespace Hollow
{
	class RUNTIME_API ShaderReflection : public Object
	{
		friend class ShaderCompiler;
	public:
		ShaderReflection() = default;
		virtual ~ShaderReflection() override = default;

		const DArray<ShaderIODescriptor>& GetInputDescriptors() const { return mInputDescriptors; }
		const DArray<ShaderIODescriptor>& GetOutputDescriptors() const { return mOutputDescriptors; }
		const DArray<ShaderDescriptorSet>& GetDescriptorData() const { return mBufferDescriptors; }

	private:
		void AddInputDescriptor(const ShaderIODescriptor descriptor) { mInputDescriptors.push_back(descriptor); }
		void AddOutputDescriptor(const ShaderIODescriptor descriptor) { mOutputDescriptors.push_back(descriptor); }
		void AddDescriptorData(const ShaderDescriptorSet member) { mBufferDescriptors.push_back(member); }

	private:
		DArray<ShaderIODescriptor> mInputDescriptors;
		DArray<ShaderIODescriptor> mOutputDescriptors;
		DArray<ShaderDescriptorSet> mBufferDescriptors;
	};
}
