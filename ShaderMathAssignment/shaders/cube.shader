#SHADER VERTEX
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_World;
uniform mat4 u_ViewProjection;

out vec3 f_Color;
out vec2 f_uv;
out vec3 f_Normal;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0f);
	f_Color = a_normal;
	f_Normal = (u_World * vec4(a_normal, 0.0f)).xyz;
	f_uv = a_uv;
}

#SHADER FRAGMENT

#version 330 core

uniform sampler2D u_Sampler0;
uniform vec3 u_LightDirection;

out vec4 o_Color;
in vec3 f_Color;
in vec2 f_uv;
in vec3 f_Normal;

const vec3 DiffuseColor = vec3(0.8, 0.9, 1.0);
const vec3 AmbientColor = vec3(1.0f, 0.8, 0.2);

void main()
{
	//Difuse lighting
	float diffuse = max(-dot(u_LightDirection, f_Normal), 0.0);
	//Ambient
	float ambient = 0.2f;

	o_Color = texture(u_Sampler0, f_uv);
	o_Color.xyz *= (DiffuseColor * diffuse) + (AmbientColor * ambient);
	//o_Color = vec4(f_Normal, 1.0f);
	//o_Color = vec4(diffuse, diffuse, diffuse, 1.0f);
}