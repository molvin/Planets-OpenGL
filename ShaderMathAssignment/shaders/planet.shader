#SHADER VERTEX
#version 330 core
in vec3 a_Position;
in vec3 a_Normal;
out vec3 f_Normal;
out float f_Elevation;
out vec3 f_World;

uniform mat4 u_World;
uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0f);
	float l = length(a_Position);
	f_Elevation = l;
	f_Normal = a_Normal;
	f_World = (u_World * vec4(a_Position, 1.0f)).xyz;
}

#SHADER FRAGMENT

#version 330 core

uniform vec2 u_ElevationMinMax;
uniform sampler2D u_Sampler;
uniform vec3 u_LightDirection = vec3(0.0f, 1.0f, 0.0f);
uniform vec3 u_EyePosition;
uniform vec3 u_DiffuseColor = vec3(0.8, 0.9, 1.0);
uniform vec3 u_AmbientColor = vec3(1.0f, 0.8, 0.2);
uniform vec3 u_SpecularColor = vec3(1.0f, 0.9, 0.7);;
uniform float u_SpecularIntensity;

out vec4 o_Color;
in vec3 f_Normal;
in vec3 f_World;
in float f_Elevation;

const float SpecExponent = 30.0f;

void main()
{
	//Difuse lighting
	float diffuse = max(-dot(u_LightDirection, f_Normal), 0.0);
	//Specular
	vec3 worldEye = normalize(u_EyePosition - f_World);
	vec3 halfWayVector = normalize(worldEye - u_LightDirection);
	float specular = max(dot(halfWayVector, f_Normal), 0.0f);
	specular = pow(specular, SpecExponent) * u_SpecularIntensity;
	//Ambient
	float ambient = 0.2f;
	//Color
	float value = (f_Elevation - u_ElevationMinMax.x) / (u_ElevationMinMax.y - u_ElevationMinMax.x);
	o_Color = vec4(texture(u_Sampler, vec2(clamp(value, 0.0f,1.0f), 0.0f)).xyz, 1.0f);
	//Lighting
	o_Color.xyz *= (u_DiffuseColor * diffuse) + (u_AmbientColor * ambient);
	o_Color.xyz += (u_SpecularColor * specular);

	o_Color.xyz = f_Normal;//f_Normal * 0.5f + 0.5f;
}