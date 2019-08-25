#SHADER VERTEX
#version 330 core

in vec3 a_Position;
in vec3 a_Color;

uniform mat4 u_Transform;

out vec3 f_Color;

void main()
{
	gl_Position = u_Transform * vec4(a_Position, 1.0f);
	f_Color = a_Color;
}

#SHADER FRAGMENT

#version 330 core

out vec4 o_Color;
in vec3 f_Color;

void main()
{
	o_Color = vec4(f_Color, 1.0);
}