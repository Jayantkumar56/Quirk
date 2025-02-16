


#type Vertex
#version 450 core

layout(location = 0) in vec4 v_Position;
layout(location = 1) in vec4 v_Normal;
layout(location = 2) in vec4 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
	gl_Position = u_ViewProjection * u_Transform * v_Position;
}








#type Fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_EntityId;

uniform int  u_EntityId;
uniform vec3 u_Color; 

void main() {
	o_Color    = vec4(u_Color, 1.0f);
	o_EntityId = u_EntityId;
}

