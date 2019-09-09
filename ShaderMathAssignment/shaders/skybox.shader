#SHADER VERTEX
#version 330 core

in vec3 a_Position;

out vec3 f_TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_ViewProjection;

void main()
{
	mat4 view = u_View;
	view[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	f_TexCoords = normalize(a_Position);
	gl_Position = u_Projection * view * vec4(a_Position, 1.0f);
	gl_Position.z = gl_Position.w - 0.00001f;
}

#SHADER FRAGMENT

#version 330 core

in vec3 f_TexCoords;
out vec4 o_Color;
uniform samplerCube skybox;

void main()
{
	o_Color = texture(skybox, f_TexCoords);
	//o_Color = vec4(0.0f, 1.0f, 1.0f, 1.0f);
}