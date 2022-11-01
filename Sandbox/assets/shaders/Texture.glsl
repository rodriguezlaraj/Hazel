// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//color = texture(u_Texture, v_TexCoord);
	//color = texture(u_Texture, v_TexCoord * 10.0) * vec4(1.0, 0.8, 0.8, 1.0);
	//The 10 scaling and the vec4(1.0, 0.8, 0.8, 1.0) could be set as uniforms to edit them dynamically. The colour is set dynamically below
	color = texture(u_Texture, v_TexCoord * 10.0) * u_Color;
	//Used to debug the textures. We should see a gradient. Black to Red ->   and Black to Green ^ and mixture in between
	//color = vec4(v_TexCoord, 0.0, 1.0);
}