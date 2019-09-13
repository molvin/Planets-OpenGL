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
out vec3 f_World;

void main()
{
	gl_Position = u_ViewProjection * u_World * vec4(a_Position, 1.0f);
	f_Color = a_normal;
	f_Normal = normalize((u_World * vec4(a_normal, 0.0f))).xyz;
	f_uv = a_uv;
	f_World = (u_World * vec4(a_Position, 1.0f)).xyz;
}

#SHADER FRAGMENT

#version 330 core

uniform sampler2D u_Sampler0;
uniform sampler2D u_LightBuffer;
uniform vec3 u_LightDirection;
uniform vec3 u_EyePosition;
uniform float u_SpecularIntensity;
uniform mat4 u_LightViewProjection;
uniform vec3 u_DiffuseColor = vec3(0.8, 0.9, 1.0);
uniform float u_DiffuseIntensity = 1.0f;

out vec4 o_Color;
in vec3 f_Color;
in vec2 f_uv;
in vec3 f_Normal;
in vec3 f_World;

const vec3 AmbientColor = vec3(1.0f, 0.8, 0.2);
const vec3 SpecularColor = vec3(1.0f, 0.9, 0.7);
const float SpecExponent = 30.0f;
const float SpecIntensity = 0.4f;
const float ShadowBias = 0.01;

struct PointLight
{
	vec3 Position;
	float Radius;
	vec3 Color;
};

vec3 CalculatePointLight(PointLight light, vec3 albedo)
{
	vec3 lightDirection = normalize(f_World - light.Position);
	float intensity = 1.0f - length(f_World - light.Position) / light.Radius;
	vec3 diffuse = albedo * light.Color * intensity * max(-dot(lightDirection, f_Normal), 0.0f);
	vec3 worldEye = normalize(u_EyePosition - lightDirection);
	vec3 halfwayVector = normalize(worldEye - lightDirection);
	float spec = max(dot(halfwayVector, f_Normal), 0.0f);
	spec = pow(spec, SpecExponent) * u_SpecularIntensity;

	//TODO: add spec, also use this method
	return diffuse + (light.Color * spec);
}

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
	//Shadows
	vec4 lightNDC = u_LightViewProjection * vec4(f_World, 1.0f);
	lightNDC = lightNDC * 0.5f + 0.5f;
	float lightDepth = texture(u_LightBuffer, lightNDC.xy).x;
	float ourDepth = lightNDC.z;
	float shadow = step(ourDepth, lightDepth + ShadowBias);
	diffuse *= shadow;

	o_Color = texture(u_Sampler0, f_uv);
	o_Color.xyz *= (u_DiffuseColor * diffuse * u_DiffuseIntensity) + (AmbientColor * ambient);
	o_Color.xyz += (SpecularColor * specular);
	//o_Color.xyz = vec3(shadow);
	//o_Color = vec4(f_Normal, 1.0f);
	//o_Color = vec4(diffuse, diffuse, diffuse, 1.0f);
	//o_Color = vec4(f_World, 1.0f);
}
