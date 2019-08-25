#SHADER VERTEX
#version 330 core

in vec3 a_Position;
in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec3 f_Color;
out vec2 f_TexCoord;

uniform mat4 u_Transform;

void main()
{
	gl_Position = u_Transform * vec4(a_Position, 1.0f);
	f_Color = vec3(a_TexCoord, 0.0f);
	f_TexCoord = a_TexCoord;
}

#SHADER FRAGMENT

#version 330 core

uniform sampler2D u_Sampler0;
uniform sampler2D u_Sampler1;
uniform float u_Time;

out vec4 o_Color;

in vec2 f_TexCoord;
in vec3 f_Color;

void main()
{
	vec4 texture0 = texture(u_Sampler0, f_TexCoord);
	vec4 texture1 = texture(u_Sampler1, f_TexCoord);

	o_Color = mix(texture0, texture1, sin(u_Time) * 0.5 + 0.5);
}