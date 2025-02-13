

#type Vertex
#version 450 core

layout(location = 0) in vec4 i_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;


void main() {
	gl_Position = u_ViewProjection * u_Transform * i_Position;
}


#type Fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_EntityId;

uniform int u_EntityId;

void main() {
	o_Color = vec4(1.0f, 0.5f, 0.31f, 1.0f);
	o_EntityId = u_EntityId;
}

