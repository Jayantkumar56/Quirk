

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

	switch (v_TextureSlot){
		case 0 : o_Color  *= texture(u_Textures[0],  Input.TexCoord); break;
		case 1 : o_Color  *= texture(u_Textures[1],  Input.TexCoord); break;
		case 2 : o_Color  *= texture(u_Textures[2],  Input.TexCoord); break;
		case 3 : o_Color  *= texture(u_Textures[3],  Input.TexCoord); break;
		case 4 : o_Color  *= texture(u_Textures[4],  Input.TexCoord); break;
		case 5 : o_Color  *= texture(u_Textures[5],  Input.TexCoord); break;
		case 6 : o_Color  *= texture(u_Textures[6],  Input.TexCoord); break;
		case 7 : o_Color  *= texture(u_Textures[7],  Input.TexCoord); break;
		case 8 : o_Color  *= texture(u_Textures[8],  Input.TexCoord); break;
		case 9 : o_Color  *= texture(u_Textures[9],  Input.TexCoord); break;
		case 10 : o_Color *= texture(u_Textures[10], Input.TexCoord); break;
		case 11 : o_Color *= texture(u_Textures[11], Input.TexCoord); break;
		case 12 : o_Color *= texture(u_Textures[12], Input.TexCoord); break;
		case 13 : o_Color *= texture(u_Textures[13], Input.TexCoord); break;
		case 14 : o_Color *= texture(u_Textures[14], Input.TexCoord); break;
		case 15 : o_Color *= texture(u_Textures[15], Input.TexCoord); break;
		case 16 : o_Color *= texture(u_Textures[16], Input.TexCoord); break;
		case 17 : o_Color *= texture(u_Textures[17], Input.TexCoord); break;
		case 18 : o_Color *= texture(u_Textures[18], Input.TexCoord); break;
		case 19 : o_Color *= texture(u_Textures[19], Input.TexCoord); break;
		case 20 : o_Color *= texture(u_Textures[20], Input.TexCoord); break;
		case 21 : o_Color *= texture(u_Textures[21], Input.TexCoord); break;
		case 22 : o_Color *= texture(u_Textures[22], Input.TexCoord); break;
		case 23 : o_Color *= texture(u_Textures[23], Input.TexCoord); break;
		case 24 : o_Color *= texture(u_Textures[24], Input.TexCoord); break;
		case 25 : o_Color *= texture(u_Textures[25], Input.TexCoord); break;
		case 26 : o_Color *= texture(u_Textures[26], Input.TexCoord); break;
		case 27 : o_Color *= texture(u_Textures[27], Input.TexCoord); break;
		case 28 : o_Color *= texture(u_Textures[28], Input.TexCoord); break;
		case 29 : o_Color *= texture(u_Textures[29], Input.TexCoord); break;
		case 30 : o_Color *= texture(u_Textures[30], Input.TexCoord); break;
		case 31 : o_Color *= texture(u_Textures[31], Input.TexCoord); break;
	}

	o_EntityId = v_EntityId;
}
