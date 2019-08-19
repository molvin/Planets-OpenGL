#version 330 core

in vec2 a_Position;
in vec3 a_Color;
out vec3 f_Color;

void main()
{
	gl_Position = vec4(a_Position, 0.0f, 1.0f);
	f_Color = a_Color;
}