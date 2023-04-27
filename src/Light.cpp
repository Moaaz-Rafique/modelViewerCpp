#include "Light.h"
#include "glad/glad.h"
 std::string  Light::Lposition = ".position";
 std::string  Light::Lambient = ".ambientIntensity";
 std::string  Light::Ldiffuse = ".intensity";
 std::string  Light::Lspecular = ".specularIntensity";
 std::string  Light::LColor = ".color";
 std::string  Light::Lkc = ".kc";
 std::string  Light::Lkl = ".kl";
 std::string  Light::Lkq = ".kq";
 std::string  Light::Lexponent = ".spotexponent";


Light::Light(glm::vec3 color, std::string name, float intensity, float ambiental, float specular) :
	transform(glm::mat4()),  intensity(intensity), ambient_intensity(ambiental), light_color(color), specular_intensity(specular)
{
	this->name = name;
}

void Light::set_position(glm::vec3 value)
{
	Position = value;
}

void Light::set_view_position(glm::vec3 value)
{
	Camera_Position = value;
}


void Light::set_ambient_intensity(float value)
{
	ambient_intensity = value;
}


void Light::set_intensity(float value)
{
	intensity = value;
}


void Light::set_specular_intensity(float value)
{
	specular_intensity = value;
}




void Light::render(unsigned int shaderID)
{
	glUseProgram(shaderID);

	int index = 0;


	index = glGetUniformLocation(shaderID, (name + Lposition).c_str());
	glUniform3fv(index, 1, &Position[0]);


	index = glGetUniformLocation(shaderID, "eye");
	glUniform3fv(index, 1, &Camera_Position[0]);


	index = glGetUniformLocation(shaderID, (name + Lkc).c_str());
	glUniform1f(index, attenuation.x);

	index = glGetUniformLocation(shaderID, (name + Lkl).c_str());
	glUniform1f(index, attenuation.y);

	index = glGetUniformLocation(shaderID, (name + Lkq).c_str());
	glUniform1f(index, attenuation.z);

	index = glGetUniformLocation(shaderID, (name + Lspecular).c_str());
	glUniform1f(index, specular_intensity);

	index = glGetUniformLocation(shaderID, (name + Ldiffuse).c_str());
	glUniform1f(index, intensity);

	index = glGetUniformLocation(shaderID, (name + LColor).c_str() );
	glUniform3fv(index, 1, &light_color[0]);

	index = glGetUniformLocation(shaderID, (name + Lambient).c_str() );
	glUniform1f(index, ambient_intensity);


}
