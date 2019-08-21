#SHADER VERTEX
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

#SHADER FRAGMENT

#version 330 core

uniform sampler2D u_Sampler;

out vec4 o_Color;

in vec2 f_TexCoord;
in vec3 f_Color;

void main()
{
	o_Color = vec4(f_TexCoord, 0.0f, 1.0f);
	//o_Color = vec4(f_Color, 1.0f);
	o_Color = texture(u_Sampler, f_TexCoord);
}