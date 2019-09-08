#SHADER VERTEX
#version 330 core

const vec2 QuadVertices[] = vec2[](
	vec2(-1.0f, -1.0f),
	vec2(1.0f, -1.0f),
	vec2(1.0f, 1.0f),
	vec2(-1.0f, 1.0f)
);
const vec2 QuadTexCoords[] = vec2[](
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f)
);

out vec2 f_TexCoord;

void main()
{
	gl_Position = vec4(QuadVertices[gl_VertexID], 0.0f, 1.0f);
	f_TexCoord = QuadTexCoords[gl_VertexID];
}

#SHADER FRAGMENT

#version 330 core

uniform sampler2D u_FrameColor;

in vec2 f_TexCoord;
out vec4 o_Color;

void main()
{
	o_Color = texture(u_FrameColor, f_TexCoord);
}