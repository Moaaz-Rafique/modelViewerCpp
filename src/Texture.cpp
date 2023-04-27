#include "Texture.h"


#include <glad/glad.h>


#include <iostream>
#define STB_IMAGE_IMPLEMENTATION    
#include "../stb_image/stb_image.h"

	unsigned int ITexture::presentlyBounded = NULL;
	unsigned int  ITexture::presentTextureType = NULL;

	Texture_2D::Texture_2D(unsigned char* data)
	{
		glGenTextures(1, &m_rendererID);
		glBindTexture(GL_TEXTURE_2D, m_rendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1024, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindTexture(GL_TEXTURE_2D, 0);


		if (presentlyBounded != NULL)
		{
			 glBindTexture(presentTextureType, presentlyBounded);
		}
	}

	Texture_2D::Texture_2D(const std::string& texture_filePath, const std::string& name /*= "m_texture"*/, unsigned int format, unsigned internalFormat)
		:ITexture()
	{
		m_width = 0; m_hieght = 0; m_presentTextureSlot = 0; m_textureName = name;
		unsigned char* tex_data = nullptr;
		if (texture_filePath.find("png") != std::string::npos || texture_filePath.find(".png") != std::string::npos)
		{
			 stbi_set_flip_vertically_on_load(1);
		}
		tex_data = stbi_load(texture_filePath.c_str(), &m_width, &m_hieght, &m_BPP, 4);//TODO chek the meaning of bits per pixel

		if (!tex_data)
		{
			std::cerr << "[ERROR:] failed to load texture:  " << name << std::endl;
		}

		 glGenTextures(1, &m_rendererID);
		 glBindTexture(GL_TEXTURE_2D, m_rendererID);

		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		 glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_hieght, 0, format, GL_UNSIGNED_BYTE, tex_data);
		 glBindTexture(GL_TEXTURE_2D, 0);


		if (tex_data)
		{
			stbi_image_free(tex_data);
		}

	}

	
	Texture_2D::~Texture_2D()
	{
	}

	void ITexture::setTexturesName(const std::string& name)
	{
		m_textureName = name;
	}

	void ITexture::setTexturesName(const std::string&& name)
	{
		m_textureName = name;
	}

	void Texture_2D::bindTextureToSlot(unsigned int slot /*= 0*/)
	{
		presentlyBounded = m_rendererID;
		presentTextureType = GL_TEXTURE_2D;

		m_presentTextureSlot =  slot;
		 glActiveTexture((GL_TEXTURE0 + slot));
		 glBindTexture(GL_TEXTURE_2D, m_rendererID);
	}

	void ITexture::unbind(unsigned int slot /*= 0*/)
	{
		if (presentlyBounded == this->m_rendererID)
		{
			presentlyBounded = NULL;
			presentTextureType = NULL;
		}

		 glActiveTexture((GL_TEXTURE0 + slot));
		 glBindTexture(GL_TEXTURE_2D, 0);
	}

	CubeMap::CubeMap(std::vector<std::string>& texture_filePaths, std::string& name, GLenum Format, GLenum internalFormat)
	{
		 glGenTextures(1, &m_rendererID);
		 glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);

		m_width = 0; m_hieght = 0; m_presentTextureSlot = 0; m_textureName = name;
		unsigned char* tex_data = nullptr;
		int j = 0;

		for (int i = 0; i < 6; i++)
		{
			if (texture_filePaths[i].find("png") != std::string::npos || texture_filePaths[i].find(".png") != std::string::npos)
			{
				stbi_set_flip_vertically_on_load(1);
			}

			tex_data = stbi_load(texture_filePaths[i].c_str(), &m_width, &m_hieght, &m_BPP, 4);//TODO chek the meaning of bits per pixel

			 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_width, m_hieght, 0, Format, GL_UNSIGNED_BYTE, tex_data);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		 glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		if (tex_data)
		{
			stbi_image_free(tex_data);
		}

		if (presentlyBounded != NULL)
		{
			 glBindTexture(presentTextureType, presentlyBounded);
		}
	}



	CubeMap::CubeMap(std::array<Texture_2D, 6> textures)
	{
		 glGenTextures(1, &m_rendererID);
		 glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);

		m_width = 0; m_hieght = 0; m_presentTextureSlot = 0;
		unsigned char* data = (unsigned char*)malloc(1280 * 1280 * 4);
		int j = 0;

		for (int i = 0; i < 6; i++)
		{
			textures[i].bindTextureToSlot(2);
			 glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 1280, 1280, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		 glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


		if (presentlyBounded != NULL)
		{
			 glBindTexture(presentTextureType, presentlyBounded);
		}
	}


	

	void CubeMap::operator=(const CubeMap&& texture)
	{
		(*this).m_BPP = texture.m_BPP;
		(*this).m_hieght = texture.m_hieght;
		(*this).m_presentTextureSlot = texture.m_presentTextureSlot;
		(*this).m_rendererID = texture.m_rendererID;
		(*this).m_textureName = texture.m_textureName;
		(*this).m_width = texture.m_width;
	}

	void CubeMap::bindTextureToSlot(unsigned int slot)
	{
		presentlyBounded = m_rendererID;
		presentTextureType = GL_TEXTURE_CUBE_MAP;

		m_presentTextureSlot = slot;
		 glActiveTexture((GL_TEXTURE0 + slot));
		 glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);
	}

	CubeMap::~CubeMap()
	{

	}
