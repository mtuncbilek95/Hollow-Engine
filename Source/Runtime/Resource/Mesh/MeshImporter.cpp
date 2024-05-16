#include "MeshImporter.h"

#include <Runtime/Platform/PlatformFile.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hollow
{
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

		for (uint64 meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
		{
			SubMeshObject subMesh = {};
			const aiMesh* mesh = scene->mMeshes[meshIndex];

			if (mesh->HasPositions())
			{
				for (uint64 vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++)
				{
					subMesh.Positions.push_back(Vector3f(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z));
				}
			}

			if (mesh->HasNormals())
			{
				for (uint64 normalIndex = 0; normalIndex < mesh->mNumVertices; normalIndex++)
				{
					subMesh.Normals.push_back(Vector3f(mesh->mNormals[normalIndex].x, mesh->mNormals[normalIndex].y, mesh->mNormals[normalIndex].z));
				}
			}

			if (mesh->HasTangentsAndBitangents())
			{
				for (uint64 tangentIndex = 0; tangentIndex < mesh->mNumVertices; tangentIndex++)
				{
					subMesh.Tangents.push_back(Vector3f(mesh->mTangents[tangentIndex].x, mesh->mTangents[tangentIndex].y, mesh->mTangents[tangentIndex].z));
					subMesh.Bitangents.push_back(Vector3f(mesh->mBitangents[tangentIndex].x, mesh->mBitangents[tangentIndex].y, mesh->mBitangents[tangentIndex].z));
				}
			}

			for (uint64 uvIndex = 0; uvIndex < AI_MAX_NUMBER_OF_TEXTURECOORDS; uvIndex++)
			{
				if (mesh->HasTextureCoords(uvIndex))
				{
					ArrayList<Vector2f> uvSet;
					for (uint64 uvSetIndex = 0; uvSetIndex < mesh->mNumVertices; uvSetIndex++)
					{
						uvSet.push_back(Vector2f(mesh->mTextureCoords[uvIndex][uvSetIndex].x, mesh->mTextureCoords[uvIndex][uvSetIndex].y));
					}
					subMesh.UVs.push_back(uvSet);
				}
			}

			for (uint64 colorIndex = 0; colorIndex < AI_MAX_NUMBER_OF_COLOR_SETS; colorIndex++)
			{
				if (mesh->HasVertexColors(colorIndex))
				{
					ArrayList<Vector4f> colorSet;
					for (uint64 colorSetIndex = 0; colorSetIndex < mesh->mNumVertices; colorSetIndex++)
					{
						colorSet.push_back(Vector4f(mesh->mColors[colorIndex][colorSetIndex].r, mesh->mColors[colorIndex][colorSetIndex].g, mesh->mColors[colorIndex][colorSetIndex].b, mesh->mColors[colorIndex][colorSetIndex].a));
					}
					subMesh.Colors.push_back(colorSet);
				}
			}

			if (mesh->HasFaces())
			{
				for (uint64 faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
				{
					const aiFace& face = mesh->mFaces[faceIndex];
					for (uint64 indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++)
					{
						subMesh.Indices.push_back(face.mIndices[indexIndex]);
					}
				}
			}

			mainLayout.SubMeshes.push_back(subMesh);
		}

		return mainLayout;
	}
}
