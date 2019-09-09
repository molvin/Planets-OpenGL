#SHADER VERTEX
#version 330 core
in vec3 a_Position;
in vec3 a_Normal;
out vec3 f_Color;
out float f_Elevation;

uniform mat4 u_World;
uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0f);
	float l = length(a_Position);
	f_Elevation = l;
	f_Color = a_Normal;
}

#SHADER FRAGMENT

#version 330 core

uniform vec2 u_ElevationMinMax;
uniform sampler2D u_Sampler;

out vec4 o_Color;
in vec3 f_Color;
in float f_Elevation;

void main()
{
	float value = (f_Elevation - u_ElevationMinMax.x) / (u_ElevationMinMax.y - u_ElevationMinMax.x);
	o_Color = vec4(texture(u_Sampler, vec2(clamp(value, 0.0f,1.0f), 0.0f)).xyz, 1.0f);
	o_Color = vec4(f_Color, 1.0f);
}