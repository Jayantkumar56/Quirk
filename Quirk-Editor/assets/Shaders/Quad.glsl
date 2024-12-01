

#type Vertex
#version 450 core

layout(location = 0) in vec4  a_Position;
layout(location = 1) in float a_VertexNo;
layout(location = 2) in vec4  a_Color;
layout(location = 3) in int   a_TextureSlot;
layout(location = 4) in vec4  a_TexCoordX;
layout(location = 5) in vec4  a_TexCoordY;
layout(location = 6) in mat4  a_Transform;
layout(location = 10) in int  a_EntityId;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 2) out flat int v_TextureSlot;
layout (location = 3) out flat int v_EntityId;

void main()
{
	Output.Color    = a_Color;
	Output.TexCoord = vec2(a_TexCoordX[int(a_VertexNo)], a_TexCoordY[int(a_VertexNo)]);

	v_TextureSlot = a_TextureSlot;
	v_EntityId    = a_EntityId;

	mat4 transform = a_Transform;
	gl_Position    = u_ViewProjection * transform * a_Position;
}

#type Fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_EntityId;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 2) in flat int v_TextureSlot;
layout (location = 3) in flat int v_EntityId;

uniform sampler2D u_Textures[32];

void main()
{
	o_Color = Input.Color;
	o_Color *= texture(u_Textures[v_TextureSlot], Input.TexCoord);
	o_EntityId = v_EntityId;
}
