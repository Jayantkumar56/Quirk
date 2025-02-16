

#type Vertex
#version 450 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;

layout(location = 0) out vec3 o_Position;
layout(location = 1) out vec3 o_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 u_Rotation;

void main() {	
//	o_Normal    = mat3(transpose(inverse(u_Transform))) * v_Normal;
	o_Normal    = vec3(u_Rotation * vec4(v_Normal.xyz, 1.0f));
    o_Position  = vec3(u_Transform * vec4(v_Position, 1.0f));
	gl_Position = u_ViewProjection * u_Transform * vec4(v_Position, 1.0f);
}







#type Fragment
#version 450 core

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int  o_EntityId;

struct Light {
	vec3  Position;
	vec3  Color;
	float Ambient;
	float Diffuse;
	float Specular;
};

struct Material {
	vec3  Ambient;
	vec3  Diffuse;
	vec3  Specular;
	float Shininess;
};

uniform vec3     u_CameraPosition;
uniform int      u_EntityId;
uniform Light    u_Light;
uniform Material u_Material;

// forward declarations (light calculation function)
vec4 CalculateColorWithLight    (vec4 fragColor);
vec4 CalculateColorWithMaterial ();

void main() {
	vec4 color = vec4(1.0f, 0.5f, 0.31f, 1.0f);

//	o_Color    = CalculateColorWithLight(color);
	o_Color    = CalculateColorWithMaterial();
	o_EntityId = u_EntityId;
}

// calculations done in world space
// TODO: try calculations in view space (i.e. in camera relative space)
vec4 CalculateColorWithLight(vec4 fragColor) {
	vec3 lightColor     = u_Light.Color;
	vec3 lightDirection = normalize(u_Light.Position - v_Position);

	float ambientStrength  = 0.13f;
	float diffuseStrength  = max(dot(v_Normal, lightDirection), 0.0f);
	float specularStrength = 0.5;

	vec3 viewDir    = normalize(u_CameraPosition - v_Position);
	vec3 reflectDir = reflect(-lightDirection, v_Normal);
	float spec      = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient  = ambientStrength  * lightColor;
	vec3 diffuse  = diffuseStrength  * lightColor;
    vec3 specular = specularStrength * spec * lightColor;

	return fragColor * vec4(ambient.xyz + diffuse.xyz + specular.xyz, 1.0f);
}

vec4 CalculateColorWithMaterial() {
	vec3 lightDirection = normalize(u_Light.Position - v_Position);

	// ambient
	vec3 ambient = u_Light.Ambient * u_Light.Color * u_Material.Ambient;

	// diffuse
	float diffuseStrength  = max(dot(v_Normal, lightDirection), 0.0f);
	vec3  duffuse          = u_Light.Diffuse * u_Light.Color * (diffuseStrength * u_Material.Diffuse);

	// specular
	vec3  reflectDir = reflect(-lightDirection, v_Normal);
	vec3  viewDir    = normalize(u_CameraPosition - v_Position);
	float spec       = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
	vec3  specular   = u_Light.Specular * u_Light.Color * (spec * u_Material.Specular);

	return vec4(ambient.xyz + duffuse.xyz + specular.xyz, 1.0f);
}


