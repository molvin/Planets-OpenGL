#SHADER VERTEX
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_World;
uniform mat4 u_ViewProjection;

out vec3 f_Normal;
out vec3 f_World;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0f);
	f_Normal = (u_World * vec4(a_normal, 0.0f)).xyz;
	f_World = (u_World * vec4(a_Position, 1.0f)).xyz;
}

#SHADER FRAGMENT

#version 330 core

uniform sampler2D u_Sampler;
uniform vec3 u_LightDirection;
uniform vec3 u_Color;

out vec4 o_Color;
in vec3 f_Normal;
in vec3 f_World;


void main()
{
	vec2 uv;
	float intensity = max(dot(f_Normal, -u_LightDirection), 0.0f);
	uv.x = intensity;
	uv.y = 0.0f;

	//o_Color = vec4(intensity, intensity, intensity, 1.0f);
	o_Color = texture(u_Sampler, uv);
    o_Color *= vec4(u_Color, 1.0f);
}