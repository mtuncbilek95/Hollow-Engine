#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API MeshResourceSubType
	{
		Unknown,
		Static,
		Skinned,
		Count
	};

	enum class RUNTIME_API TextureResourceSubType
	{
		Unknown,
		Albedo,
		Normal,
		Metallic,
		Roughness,
		AmbientOcclusion,
		Emissive,
		Count
	};

	enum class RUNTIME_API MaterialResourceSubType
	{
		Unknown,
		Standard,
		Count
	};

	enum class RUNTIME_API ShaderResourceSubType
	{
		Unknown,
		Vertex,
		Compute,
		Geometry,
		TesselationEval,
		TesselationControl,
		Fragment,
		Count
	};

	enum class RUNTIME_API AudioResourceSubType
	{
		Unknown,
		Wav,
		Mp3,
		Flac,
		Count
	};

	enum class RUNTIME_API FontResourceSubType
	{
		Unknown,
		TTF,
		OTF,
		Count
	};


}
