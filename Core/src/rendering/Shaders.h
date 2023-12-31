#pragma once

#include <string>

std::string vertex_shader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 Normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TBN = TBN;

    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
}

)";
std::string fragment_shader = R"(
#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
    sampler2D texture_normal1;
	float shininess;
};
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 Normal;
} fs_in;  

uniform vec4 objectColor;
uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
//#define NR_POINT_LIGHTS 4
uniform int number_of_point_lights;
uniform PointLight pointLights[100];

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.texture_normal1, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    // phase 1: Directional lighting
    vec4 result = CalcDirLight(dirLight, normalize(fs_in.Normal), normalize(viewPos - fs_in.FragPos));
    
    // phase 2: Point lights
    for (int i = 0; i < number_of_point_lights; i++){
        result += CalcPointLight(pointLights[i], normal, fs_in.TangentFragPos, viewDir);
    }

    // phase 3: Spot light
	//result += CalcSpotLight(spotLight, norm, fs_in.FragPos, viewDir);

    vec4 color = result * objectColor;
	if(color.a < 0.5)
        discard;

	FragColor = color;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    //vec3 lightDir = normalize(fs_in.TBN*-light.direction * fs_in.TangentFragPos - fs_in.TangentFragPos);
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 halfwayDir  = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// combine results
	vec4 ambient  = vec4(light.ambient, 1.0)  * texture(material.texture_diffuse1, fs_in.TexCoords);
	vec4 diffuse  = vec4(light.diffuse, 1.0)  * diff * texture(material.texture_diffuse1, fs_in.TexCoords);
	vec4 specular = vec4(0.0,0.0,0.0, 0.0);//vec4(light.specular, 1.0) * spec * texture(material.texture_specular1, fs_in.TexCoords);

	return (ambient + diffuse + specular);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightPos = fs_in.TBN * light.position;
	vec3 lightDir = normalize(lightPos - fs_in.TangentFragPos);

	// diffuse shading
	float diff = max(dot(lightDir, normal), 0.0);

	// specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	//attenuation
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec4 ambient  = vec4(light.ambient, 1.0) * texture(material.texture_diffuse1, fs_in.TexCoords);
	vec4 diffuse  = vec4(light.diffuse, 1.0) * diff * texture(material.texture_diffuse1, fs_in.TexCoords);
	vec4 specular = vec4(light.specular, 1.0) * spec * vec4(1,1,1,1);// * texture(material.texture_specular1, fs_in.TexCoords);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
)";

std::string vertex_shader_skybox = R"(

#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}  

)";
std::string fragment_shader_skybox = R"(

#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}

)";

std::string vertex_shader_instanced = R"(

#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

layout (std430, binding = 0) buffer Model
{
	mat4 models[100000];
};

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 TangentFragPos;
    vec3 TangentViewPos;
    vec3 Normal;
} vs_out;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

void main()
{
    mat4 model = models[gl_InstanceID];

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TBN = TBN;

    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
}

)";