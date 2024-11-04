

#type Vertex
#version 450 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TextureCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in int a_TextureSlot;
layout(location = 4) in mat4 a_Transform;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat int v_TextureSlot;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TextureCoord;
	v_TextureSlot = a_TextureSlot;

	mat4 transform = a_Transform;
	
	gl_Position = u_ViewProjection * transform * a_Position;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in flat int v_TextureSlot;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = Input.Color;
	texColor *= texture(u_Textures[v_TextureSlot], Input.TexCoord);
	o_Color = texColor;
}
