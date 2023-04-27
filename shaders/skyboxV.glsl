#version 330 core
layout(location = 0)in vec4 position;

out vec3 texDir; //texture direction
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 vertex = vec4(projection *  view * vec4(position.xyz, 1.0f));
	gl_Position = vertex.xyww;

	texDir = vec3(position.xyz) * 1;
}