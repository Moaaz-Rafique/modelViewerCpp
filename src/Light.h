#ifndef LIGHT_H
#define LIGHT_H

#include "../vendor/glm/glm.hpp"
#include <string>



class Light
{
	float intensity;

	float ambient_intensity;

	float specular_intensity;

	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 attenuation = glm::vec3(0.0f);

	glm::mat4 transform;

	std::string name;

	glm::vec3 Position = glm::vec3(0.0f);

	glm::vec3 Camera_Position;

	static std::string Lposition;
	static std::string Lambient;
	static std::string Lspecular;
	static std::string Ldiffuse;
	static std::string LColor;
	static std::string Lkc;
	static std::string Lkl;
	static std::string Lkq;
	static std::string Lexponent;


	public:
		Light() {}
		~Light() {}


	Light(glm::vec3 color, std::string name = "light",  float intensity = 1.f, float ambiental = 0.25f, float specular = 0.5f);

	void set_position(glm::vec3 value);
	
	void set_view_position(glm::vec3 value);

	void set_Spotdirection(glm::vec3 value);

	void set_ambient_intensity(float value);

	void set_intensity(float value);

	void set_specular_intensity(float value);

	void render(unsigned int shaderID);


	private:
};


#endif
