#SHADER VERTEX
#version 330 core
in vec3 a_Position;
out vec3 f_Color;

uniform mat4 u_World;
uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0f);
	float l = length(a_Position);
	f_Color = vec3(l - 1, 0.0f, 0.0f);
}

#SHADER FRAGMENT

#version 330 core

out vec4 o_Color;
in vec3 f_Color;

void main()
{
	o_Color = vec4(f_Color, 1.0f);
}