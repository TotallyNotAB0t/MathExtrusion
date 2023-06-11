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
layout(location = 3) in mat3 TBN;
layout(location = 6) in flat int imaterial;

layout(location = 0) out vec4 outColor;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float ShadowCalculation(vec4 fragPosLightSpace,vec3 N,int id)
{
    vec3 L = normalize(ubs[id].position - WorldPos);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowSampler[id], projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);  
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;     

    return shadow;
}  

void main()
{	
	vec3 color = texture(texSampler[ubm[imaterial].albedoMap], fragTexCoord).rgb * ubm[imaterial].albedo * Color; 
    vec3 ambient = vec3(0.003) * color * ubm[imaterial].ao * texture(texSampler[ubm[imaterial].aoMap], fragTexCoord).rgb;
    vec3 metallic = texture(texSampler[ubm[imaterial].metallicMap], fragTexCoord).rgb * ubm[imaterial].metallic;
    float roughness = texture(texSampler[ubm[imaterial].roughnessMap], fragTexCoord).r * ubm[imaterial].roughness;
    vec3 normal = texture(texSampler[ubm[imaterial].normalMap], fragTexCoord).rgb * 2.0;
    normal = mix(vec3(0.5, 0.5, 1.0), normal, ubm[imaterial].normal);
    normal = normalize(normal * 2.0 - 1.0);    

    vec3 N = normalize(TBN * normal);
    vec3 V = normalize(ubc.camPos - WorldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, color, metallic);    

    vec3 reflectedSkyboxColor = texture(samplerCubeMap, reflect(-V, N)).rgb;

    float shadow = 1.0f;
    for(int i = 0; i < ubd.maxShadow;i++)
    {
        shadow += ShadowCalculation((ubs[i].proj * ubs[i].view) * vec4(WorldPos,1.0),N,i);
    }

    // Reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < ubd.maxLight; i++)
    {
        vec3 L = normalize(ubl[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(ubl[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = ubl[i].color * attenuation * ubl[i].range;
        if(ubl[i].status == 0)
        {
           L = -ubl[i].direction;
           H = normalize(V + L);
        }

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);        
        float G = GeometrySmith(N, V, L, roughness);      
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);       

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;      

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = (numerator / denominator);  
            
        // Add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);

        if (ubl[i].status == 0) // DirLight
        {
            Lo += (kD * color + specular) * ubl[i].color * ubl[i].range/10.0f * NdotL;
        }
        else if (ubl[i].status == 1) // PointLight
        {
            Lo += (kD * color / PI + specular) * radiance * NdotL;
        }
        else if (ubl[i].status == 2) // SpotLight
        {
            vec3 lightDir = normalize(ubl[i].direction);
            float spotAngle = radians(ubl[i].spotAngle);
            float spotEffect = dot(lightDir, -L);

            if (spotEffect > cos(spotAngle / 2.0))
            {
                float transitionAngle = radians(4.0);
                float edge0 = cos(spotAngle / 2.0 - transitionAngle);
                float edge1 = cos(spotAngle / 2.0);
                float smoothFactor = smoothstep(edge1, edge0, spotEffect);

                Lo += (kD * color / PI + specular) * radiance * NdotL * pow(smoothFactor, 2.0);
            }
        }
    }    
        
    color = ambient*(reflectedSkyboxColor*5) + Lo*shadow;
        
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    outColor = vec4(color, 1.0);
}