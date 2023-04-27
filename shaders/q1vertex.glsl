#version 400 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;

uniform mat4 projection;
uniform mat4 view;

out vec3 Normal;
out vec2 TexCoords;
out vec4 ModelPosition;


void main()
{
	vec4 modelPosition =  view * model * vec4(vertex, 1.0);

	ModelPosition = model * vec4(vertex, 1.0);
	Normal = vec4(transpose(inverse(model)) * vec4(normals, 1.0f)).xyz;

	TexCoords = texCoords.xy;

	gl_Position = projection * modelPosition;
}

