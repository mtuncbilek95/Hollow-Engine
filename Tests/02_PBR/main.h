#pragma once

#include <Runtime/Platform/PlatformAPI.h>
#include <Runtime/Platform/PlatformFile.h>
#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/API/GraphicsManager.h>

#include <Runtime/Graphics/Instance/GraphicsInstance.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/ShaderCompiler/ShaderCompiler.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Resource/Importer/ResourceImporter.h>

#include <Runtime/Resource/RenderTarget/RenderTarget.h>
#include <Runtime/Resource/Texture/TextureResource.h>
#include <Runtime/Resource/Mesh/MeshResource.h>

#include <Runtime/World/Transform/Transform.h>
#include <Runtime/World/Camera/FreeLookCamera.h>

namespace Hollow
{
	struct ModelUniform
	{
		Mat4f Model;
	};

	struct PushConstant
	{
		Mat4f View;
		Mat4f Projection;
	};

	ModelUniform BaseModelBuffer
	{
		.Model = Mat4f(1.f)
	};

	PushConstant BasePushConstant
	{
		.View = Mat4f(1.f),
		.Projection = Mat4f(1.f)
	};

	Transform ObjectTransform
	{
		.Position = Vec3f(0.f),
		.Rotation = Vec3f(Math::radians(90.f), Math::radians(0.f), Math::radians(180.f)),
		.Scale = Vec3f(1.f)
	};

	void UpdateBuffers()
	{
		FreeLookCamera::GetAPI().Update();

		Mat4f model = Mat4f(1.0f);
		model = Math::translate(model, ObjectTransform.Position);
		model = Math::rotate(model, ObjectTransform.Rotation.x, Vec3f(1, 0, 0));
		model = Math::rotate(model, ObjectTransform.Rotation.y, Vec3f(0, 1, 0));
		model = Math::rotate(model, ObjectTransform.Rotation.z, Vec3f(0, 0, 1));
		model = Math::scale(model, ObjectTransform.Scale);

		BaseModelBuffer.Model = model;

		BasePushConstant.View = FreeLookCamera::GetAPI().GetViewMatrix();
		BasePushConstant.Projection = FreeLookCamera::GetAPI().GetProjectionMatrix();
	}
}
