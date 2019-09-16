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

struct DirectionalLight
{
	vec3 Direction;
	vec3 Color;
	sampler2D ShadowBuffer;
	mat4 ViewProjection;
};
struct PointLight
{
	vec3 Position;
	float Radius;
	vec3 Color;
};


uniform vec2 u_ElevationMinMax;
uniform sampler2D u_Sampler;
uniform vec3 u_EyePosition;
uniform float u_SpecularIntensity;
uniform vec3 u_AmbientColor = vec3(1.0f, 0.8, 0.2);
uniform DirectionalLight u_DirectionalLight;

out vec4 o_Color;
in vec3 f_Normal;
in vec3 f_World;
in float f_Elevation;

const vec3 AmbientColor = vec3(1.0f, 0.8, 0.2);
const float SpecExponent = 30.0f;
const float SpecIntensity = 0.4f;
const float ShadowBias = 0.01;

vec3 CalculatePointLight(PointLight light, vec3 albedo)
{
	vec3 lightDirection = normalize(f_World - light.Position);
	float intensity = 1.0f - length(f_World - light.Position) / light.Radius;
	vec3 diffuse = albedo * light.Color * intensity * max(-dot(lightDirection, f_Normal), 0.0f);
	vec3 worldEye = normalize(u_EyePosition - lightDirection);
	vec3 halfwayVector = normalize(worldEye - lightDirection);
	float spec = max(dot(halfwayVector, f_Normal), 0.0f);
	spec = pow(spec, SpecExponent) * u_SpecularIntensity;
	vec3 specular = light.Color * intensity * spec;
	return diffuse + specular;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 albedo)
{
	vec3 diffuse = albedo * light.Color * max(-dot(light.Direction, f_Normal), 0.0f);
	vec3 worldEye = normalize(u_EyePosition - light.Direction);
	vec3 halfwayVector = normalize(worldEye - light.Direction);
	float spec = max(dot(halfwayVector, f_Normal), 0.0f);
	spec = pow(spec, SpecExponent) * u_SpecularIntensity;
	vec3 specular = light.Color * spec;

	vec4 lightNDC = light.ViewProjection * vec4(f_World, 1.0f);
	lightNDC = lightNDC * 0.5f + 0.5f;
	float lightDepth = texture(light.ShadowBuffer, lightNDC.xy).x;
	float ourDepth = lightNDC.z;
	float shadow = step(ourDepth, lightDepth + ShadowBias);
	diffuse *= shadow;
	specular *= shadow;

	return diffuse + specular;
}


void main()
{
	//Ambient
	float ambient = 0.02f;
	//Color
	float value = (f_Elevation - u_ElevationMinMax.x) / (u_ElevationMinMax.y - u_ElevationMinMax.x);
	vec3 albedo= texture(u_Sampler, vec2(clamp(value, 0.0f,1.0f), 0.0f)).xyz;

	o_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	o_Color.xyz += CalculateDirectionalLight(u_DirectionalLight, albedo);
	o_Color.xyz += ambient;
}