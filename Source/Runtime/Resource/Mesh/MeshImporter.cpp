#include "MeshImporter.h"

#include <Runtime/Platform/PlatformFile.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hollow
{
	struct Vertex
	{
		Vector3f Position;
		Vector2f UV;
	};

	MeshResourceLayout MeshImporter::Import(String path)
	{
		if (!PlatformFile::Exists(path))
			return MeshResourceLayout();

		Assimp::Importer assimpImporter;

		const aiScene* scene = assimpImporter.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CORE_LOG(HE_ERROR, "MeshImporter", "Failed to load mesh file with error: %s", assimpImporter.GetErrorString());
			return MeshResourceLayout();
		}

		MeshResourceLayout mainLayout;

		auto mainMesh = scene->mMeshes[0];

		if(mainMesh->HasPositions())
		{
			for (uint32 i = 0; i < mainMesh->mNumVertices; ++i)
				mainLayout.Vertices.Position.push_back({ mainMesh->mVertices[i].x, mainMesh->mVertices[i].y, mainMesh->mVertices[i].z });

			for (uint32 i = 0; i < mainMesh->mNumFaces; ++i)
			{
				aiFace face = mainMesh->mFaces[i];
				for (uint32 j = 0; j < face.mNumIndices; ++j)
					mainLayout.Indices.push_back(face.mIndices[j]);
			}
		}

		/*if (mainMesh->HasNormals())
		{
			for (uint32 i = 0; i < mainMesh->mNumVertices; ++i)
				mainLayout.Vertices.Normal.push_back({ mainMesh->mNormals[i].x, mainMesh->mNormals[i].y, mainMesh->mNormals[i].z });
		}

		if (mainMesh->HasTangentsAndBitangents())
		{
			for (uint32 i = 0; i < mainMesh->mNumVertices; ++i)
			{
				mainLayout.Vertices.Tangent.push_back({ mainMesh->mTangents[i].x, mainMesh->mTangents[i].y, mainMesh->mTangents[i].z });
				mainLayout.Vertices.Bitangent.push_back({ mainMesh->mBitangents[i].x, mainMesh->mBitangents[i].y, mainMesh->mBitangents[i].z });
			}
		}*/

		if (mainMesh->HasTextureCoords(0))
		{
			for (uint32 i = 0; i < mainMesh->mNumVertices; ++i)
				mainLayout.Vertices.UV.push_back({ mainMesh->mTextureCoords[0][i].x, mainMesh->mTextureCoords[0][i].y });
		}

		/*if (mainMesh->HasVertexColors(0))
		{
			for (uint32 i = 0; i < mainMesh->mNumVertices; ++i)
				mainLayout.Vertices.Color.push_back({ mainMesh->mColors[0][i].r, mainMesh->mColors[0][i].g, mainMesh->mColors[0][i].b, mainMesh->mColors[0][i].a });
		}*/

		return mainLayout;
	}

	uint64 MeshImporter::CalculateTotalMesh(const MeshResourceLayout& layout)
	{
		uint64 totalsize = 0;

		totalsize += layout.Vertices.Position.size() * sizeof(Vector3f);
		totalsize += layout.Vertices.UV.size() * sizeof(Vector2f);
		
		return totalsize;
	}

	uint64 MeshImporter::CalculateIndexSize(const MeshResourceLayout& layout)
	{
		uint64 totalsize = 0;

		totalsize += layout.Indices.size() * sizeof(uint32);

		return totalsize;
	}

	MemoryBuffer* MeshImporter::MeshToMemoryBuffer(const MeshResourceLayout& layout)
	{
		ArrayList<Vertex>* vertices = new ArrayList<Vertex>();

		for (uint32 i = 0; i < layout.Vertices.Position.size(); ++i)
		{
			Vertex vertex;
			vertex.Position = layout.Vertices.Position[i];
			vertex.UV = layout.Vertices.UV[i];

			vertices->push_back(vertex);
		}

		auto buffer = new MemoryBuffer((void*)vertices->data(), vertices->size() * sizeof(Vertex));
		return buffer;
	}

	MemoryBuffer* MeshImporter::IndexToMemoryBuffer(const MeshResourceLayout& layout)
	{
		auto buffer = new MemoryBuffer((void*)layout.Indices.data(), layout.Indices.size() * sizeof(uint32));
		return buffer;
	}
}
