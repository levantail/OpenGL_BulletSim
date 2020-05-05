#shader vertex
#version 330 core
		
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textCoord;
	
out vec2 v_TextCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TextCoord = textCoord;
};


#shader fragment
#version 330 core
		
layout(location = 0) out vec4 color; 

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_TextCoord;

void main()
{
	color = texture(u_Texture, v_TextCoord);
	color = vec4(1.0);
};
