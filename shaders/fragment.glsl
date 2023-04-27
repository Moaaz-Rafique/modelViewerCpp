#version 400 core

struct Light_struct
{
	vec3 position;
	
	vec3 color;
	
	vec3 spotLightDirection;

	float spotexponent;
	
	float cutoff;

	float innercutoff;
	
	float intensity;

	float kc; 
	
	float kl;
	
	float kq;

	float ambientIntensity;

	float specularIntensity;
};

uniform Light_struct light;

uniform vec3 eye;

in vec3 Normal;

in vec2 TexCoords;

in vec4 ModelPosition;

out vec4 Color;

uniform sampler2D texture;
uniform sampler2D spec_map;

uniform samplerCube skybox;


vec3 Point_light(vec3 normal, int index);


void main()
{
	eye - ModelPosition.xyz;
	Color =  vec4(Point_light(Normal, 1) , 1.0f);
}


vec3 Point_light(vec3 normal, int index)
{
	vec3 ambient = light.ambientIntensity * vec3(0.3f);

	float d = length(light.position - ModelPosition.xyz);

	float attenuation =	1.0f;// /	(light.kc +   light.kl * d +  light.kq * d * d);

	vec3 N = normalize(normal.xyz);
	vec3 L = normalize(light.position - ModelPosition.xyz);

	vec3 diffuse = max(dot(N, L), 0.0) * light.intensity * light.color * attenuation;

	vec3 V = normalize(eye - ModelPosition.xyz);
	vec3 R = reflect(-L, N);

	vec3 environMentReflection = reflect(V, N);

	vec3 relfectionColor =  texture(skybox, environMentReflection).rgb;

	vec3 Half = normalize(L + V);

	vec3 specular = pow(max(dot(N, Half), 0.0), 64.0f) * vec3(0.0f, 0.0f, 1.0f) * attenuation * light.specularIntensity * texture(spec_map, TexCoords.st);
	
	vec3 result = (ambient + diffuse + specular) * relfectionColor;
	return result;
}

