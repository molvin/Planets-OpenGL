#version 330 core

in vec2 a_Position;
in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec3 f_Color;
out vec2 f_TexCoord;

uniform float u_Time;
uniform vec2 u_Offset;
uniform float u_Scale;

void main()
{
	vec2 Pos = a_Position;
	Pos = vec2(
		Pos.x * cos(u_Time) - Pos.y * sin(u_Time),
		Pos.y * cos(u_Time) + Pos.x * sin(u_Time)
	) * u_Scale;
	Pos += u_Offset;

	gl_Position = vec4(Pos, 0.0f, 1.0f);
	f_Color = vec3(a_TexCoord, 0.0f);
	f_TexCoord = a_TexCoord;
}