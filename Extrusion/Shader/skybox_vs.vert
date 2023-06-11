#version 450
#extension GL_EXT_nonuniform_qualifier : enable

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
    vec3 lightColor;
	float range;
	uint status;//DirLight = 0 ; PointLight = 1 ; SpotLight = 2
} ubl[];

layout(set = 5, binding = 0) uniform UniformBufferDiver
{
	uint maxLight;
	float u_time;
	float gamma;
}ubd;

layout (set = 6, binding = 0) uniform samplerCube samplerCubeMap;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec3 inTangent;

//Instanced
layout(location = 5) in int index_ubo;
layout(location = 6) in int index_material;

layout (location = 0) out vec3 outUVW;

void main() 
{
	outUVW = inPosition;
	mat4 modelView = ubc.view;
	modelView[3] = vec4(0.0, 0.0, 0.0, 1.0);
	gl_Position = ubc.proj * modelView * vec4(inPosition.xyz, 1.0);
}
