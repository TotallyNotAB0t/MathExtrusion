#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout(set = 0, binding = 0) uniform UniformBufferModel
{
    mat4 model;
} ubo[];

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inLightPos;

layout (location = 0) out float outFragColor;

void main() 
{
    outFragColor = length(inPos - inLightPos);
}