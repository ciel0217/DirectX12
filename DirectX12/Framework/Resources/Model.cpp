#include "Model.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

//TODO::animation–¢‘Î‰ž
void Model::LoadModel()
{
	Assimp::Importer importer;

	int flag = 0;
	flag |= aiProcess_Triangulate;
	flag |= aiProcess_PreTransformVertices;
	flag |= aiProcess_CalcTangentSpace;
	flag |= aiProcess_GenSmoothNormals;
	flag |= aiProcess_GenUVCoords;
	flag |= aiProcess_RemoveRedundantMaterials;
	flag |= aiProcess_OptimizeMeshes;

	auto scene = importer.ReadFile(m_Name, flag);

	if (!scene)
		return;
	
	std::vector<Mesh> meshes;
	meshes.clear();
	meshes.resize(scene->mNumMeshes);

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		const auto  mesh = scene->mMeshes[i];
		
		meshes[i].m_Vertices.resize(mesh->mNumVertices);
		meshes[i].m_Vertices.resize(mesh->mNumFaces * 3);//1Face‚ÍŽOŠpŒ`‚¾‚©‚ç

		for (int j = 0; j < mesh->mNumVertices; j++)
		{
			auto position = mesh->mVertices[j];
			auto normal = mesh->mNormals[j];
			auto texCoord = (mesh->HasTextureCoords(0)) ? (mesh->mTextureCoords[0][i]) : (aiVector3D(0.0f, 0.0f, 0.0f));
			auto color = (mesh->HasVertexColors(0)) ? (mesh->mColors[0][i]) : (aiColor4D(0.0f, 0.0f, 0.0f, 1.0f));

			meshes[i].m_Vertices[j].m_Position = XMFLOAT3(position.x, position.y, position.z);
			meshes[i].m_Vertices[j].m_Normal = XMFLOAT3(normal.x, normal.y, normal.z);
			meshes[i].m_Vertices[j].m_TexCoord = XMFLOAT2(texCoord.x, texCoord.y);
			meshes[i].m_Vertices[j].m_Diffuse = XMFLOAT4(color.r, color.g, color.b, color.a);
		}

		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			const auto & face = mesh->mFaces[j];

			meshes[i].m_Indices[j * 3 + 0] = face.mIndices[0];
			meshes[i].m_Indices[j * 3 + 1] = face.mIndices[1];
			meshes[i].m_Indices[j * 3 + 2] = face.mIndices[2];
		}
	}
	
}
