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