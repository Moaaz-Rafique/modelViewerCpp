#include "object.h"
#include <iostream>
#include <string>

namespace Models
{

	assimpModel::assimpModel(shaderCompilation& shader, std::string filePath, std::string Texture, std::string specular_)
		:program(shader)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate| aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}

		aiNode* node = scene->mRootNode;
		aiMesh* Mesh_data;
		aiNode* Childnode;

		for (int j = 0; j < node->mNumChildren; j++)
		{
			Childnode = node->mChildren[j];

			for (int k = 0; k < Childnode->mNumMeshes; k++)
			{
				Mesh_data = scene->mMeshes[Childnode->mMeshes[k]];

				for (unsigned int a = 0; a < Mesh_data->mNumFaces; a++)
				{
					aiFace face = Mesh_data->mFaces[a];

					if (face.mNumIndices != 3)
					{
						__debugbreak();
					}

					for (unsigned int b = 0; b < face.mNumIndices; b++)
					{
						indices.push_back(face.mIndices[b]);

						auto vert = Mesh_data->mVertices[face.mIndices[b]];

						vertices.push_back(vert.x);
						vertices.push_back(vert.y);
						vertices.push_back(vert.z);

						auto norm = Mesh_data->mNormals[face.mIndices[b]];

						normals.push_back(norm.x);
						normals.push_back(norm.y);
						normals.push_back(norm.z);

						auto tex = Mesh_data->mTextureCoords[0][face.mIndices[b]];

						if (Mesh_data->mTextureCoords[0])
						{
							texCoords.push_back(tex.x);
							texCoords.push_back(tex.y);
						}
						else
						{
							texCoords.push_back(0.0f);
							texCoords.push_back(0.0f);
						}
					}
				}


			}
		}

		Hsize = vertices.size() * sizeof(float);
		Hsize2 = normals.size() * sizeof(float);
		Hsize3 = texCoords.size() * sizeof(float);

		Vbo = new Buffer(vertices.data(), Hsize);
		normalVbo = new Buffer(normals.data(), Hsize2);
		TextureVbo = new Buffer(texCoords.data(), Hsize3);

		Vbo->Bind();

		 glGenVertexArrays(1, &VAO);
		 glBindVertexArray(VAO);

		Vbo->Bind();
		 glVertexAttribPointer(0, 3, GL_FLOAT, false,  3 * sizeof(float), 0);
		 glEnableVertexAttribArray(0);

		normalVbo->Bind();
		 glVertexAttribPointer(1, 3, GL_FLOAT, false,  3 * sizeof(float), 0);
		 glEnableVertexAttribArray(1);

		TextureVbo->Bind();
		 glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
		 glEnableVertexAttribArray(2);

		this->Texture = Texture_2D(Texture, Texture, GL_RGBA, GL_RGBA);
		this->Texture.bindTextureToSlot(2);

		this->spec_map = Texture_2D(specular_, specular_, GL_RGBA, GL_RGBA);
		this->spec_map.bindTextureToSlot(3);

		 glGenBuffers(1, &IBO);
		 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		 glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		program = shader;
		program.Bind();
	}


}
