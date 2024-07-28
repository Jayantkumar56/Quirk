

#type Vertex
#version 330 core
			
layout(location = 0) in vec4 a_Position;
uniform mat4 u_ViewProjection;

void main() {
	gl_Position = u_ViewProjection * a_Position;
}


#type Fragment
#version 330 core
			
out vec4 color;

void main() {
	color = vec4(0.823, 0.490, 0.176, 1.0);
}
