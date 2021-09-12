#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoord;

out vec2 v_textureCoord;

void main()
{
   gl_Position = position;
   v_textureCoord = textureCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_textureCoord;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{ 
	vec4 textureColor = texture(u_texture, v_textureCoord);
	color = textureColor;
};
