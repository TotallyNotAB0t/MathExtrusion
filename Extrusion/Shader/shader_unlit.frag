#version 450
#extension GL_EXT_nonuniform_qualifier : enable

const float PI = 3.14159265359;

layout(set = 0, binding = 0) uniform UniformBufferCamera 
{
    vec3 camPos;
    mat4 view;
    mat4 proj;
} ubc;

layout(set = 1, binding = 0) uniform sampler2D texSampler[];

layout(set = 2, binding = 0) uniform UniformBufferModel
{
    mat4 model;
} ubo[];

layout(set = 3, binding = 0) uniform UniformBufferMaterial
{
    vec3  albedo;
    vec2 offset;
    vec2 tilling;
    float metallic;
    float roughness;    
    float normal;
    float ao;
    uint albedoMap;
    uint normalMap;
    uint metallicMap;
    uint roughnessMap;
    uint aoMap;
    uint castShadow;
} ubm[];

layout(set = 4, binding = 0) uniform UniformBufferLight
{
	vec3 position;
    vec3 color;
	vec3 direction;
	float range;
	float spotAngle;
	uint status;//DirLight = 0 ; PointLight = 1 ; SpotLight = 2
} ubl[];

layout(set = 5, binding = 0) uniform UniformBufferDiver
{
    uint maxLight;
    uint maxShadow;
    float u_time;
    float gamma;
} ubd;

layout(set = 6, binding = 0) uniform samplerCube samplerCubeMap;

layout(set = 7, binding = 0) uniform sampler2D shadowSampler[];

layout(set = 8, binding = 0) uniform UniformBufferShadow
{
    mat4 view;
    mat4 proj;
    vec3 position;
} ubs[];

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 WorldPos;
layout(location = 3) in flat int imaterial;

layout(location = 0) out vec4 outColor;


void main()
{	
	vec3 color = texture(texSampler[ubm[imaterial].albedoMap], fragTexCoord).rgb * ubm[imaterial].albedo * Color; 
    
     outColor = vec4(color, 1.0);
}