

#type Vertex
#version 330 core
			
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
uniform mat4 u_ViewProjection;

out vec4 o_Color;

void main() {
	gl_Position = u_ViewProjection * a_Position;
	o_Color = a_Color;
}


#type Fragment
#version 330 core
			
out vec4 color;
in vec4 o_Color;

void main() {
	color = vec4(0.823, 0.490, 0.176, 1.0);
	color = o_Color;
}
