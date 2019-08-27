#SHADER VERTEX
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_Transform;

out vec3 f_Color;
out vec2 f_uv;

void main()
{
	gl_Position = u_Transform * vec4(a_Position, 1.0f);
	f_Color = a_normal;
	f_uv = a_uv;
}

#SHADER FRAGMENT

#version 330 core

uniform sampler2D u_Sampler0;
out vec4 o_Color;
in vec3 f_Color;
in vec2 f_uv;

void main()
{

	o_Color = texture(u_Sampler0, f_uv);
}