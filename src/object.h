#pragma once
#include "GLAD/include/glad/glad.h"
#include <string>
#include <iostream>
#include <vector>

#include "../vendor/glm/glm.hpp"

#include "buffers.h"
#include "shader.h"
#include "Texture.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

namespace Models
{

	class assimpModel
	{
		public:
			std::vector<float> vertices;
			std::vector<float> normals;
			std::vector<float> texCoords;
			std::vector<unsigned int> indices;

			unsigned int IBO;

			assimpModel(shaderCompilation& shader, std::string filePath, std::string TexturePath, std::string specular_);
			~assimpModel() {}

			void Draw_call(int texture_slot, glm::mat4 model)
			{
				program.Bind();
				program.setUniformMat4("model", model);
				Texture.bindTextureToSlot(texture_slot);
				program.setUniformInt("texture1", Texture.GetTextureSlot());
				program.setUniformInt("spec_map", spec_map.GetTextureSlot());

				 glBindVertexArray(VAO);
				Vbo->Bind();
				  
				 glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			}

		private:
			Buffer* Vbo;
			Buffer* normalVbo;
			Buffer* TextureVbo;
			Texture_2D Texture;
			Texture_2D spec_map;
			shaderCompilation& program;

			unsigned int VAO;

			glm::mat4 m_model_storage;

			unsigned int Hsize = 0;
			unsigned int Hsize2 = 0;
			unsigned int Hsize3 = 0;

	};
}

