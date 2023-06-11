#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout(set = 0, binding = 0) uniform UniformBufferModel
{
    mat4 model;
} ubo[];

layout(set = 1, binding = 0) uniform UniformBufferCamShadow
{
    mat4 view;
    mat4 proj;
    vec3 position;
} ubS[];

layout(push_constant) uniform PushConstants
{    
    uint is;
} index;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in int index_ubo;

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outLightPos;

void main() 
{
	gl_Position = ubS[index.is].proj * ubS[index.is].view * ubo[index_ubo].model * vec4(inPosition, 1.0);
    outPos = inPosition;
	outLightPos = ubS[index.is].position; 
}