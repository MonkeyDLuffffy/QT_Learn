#version 420 core


in VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
}fs_in;
//受到不同的光照的影响程度
struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflection1;

    float shininess;
};
uniform Material material;
uniform sampler2D depthMap;

float DirectShadowCalculation(vec4 fragPosLightSpace);


//一个光源提供的不同的光照强度与光源的位置
struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float cutOn;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotlight;
vec3 CalcSpotLightColor(SpotLight light);


struct DirectLight
{

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectLight directlight;
vec3 CalcDirectLightColor(DirectLight light);


struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


#define NR_POINT_LIGHTS 4
uniform PointLight[NR_POINT_LIGHTS] pointlight;
vec3 CalcPointLightColor(PointLight light);
float PointShadowCalculation(PointLight light);

out vec4 FragColor;


uniform vec3 objectColor;
uniform vec3 viewPos;
//uniform samplerCube skybox;

uniform float far_plane;
uniform samplerCube depthCubeMap;

vec3 diffuseTexColor=vec3(texture(material.texture_diffuse1,fs_in.TexCoords));
vec3 specularTexColor=vec3(texture(material.texture_specular1,fs_in.TexCoords));
vec3 reflectionTexColor=vec3(texture(material.texture_reflection1,fs_in.TexCoords));
//vec3 emissionTexColor=vec3(texture(material.emission,fs_in.TexCoords));

vec3 norm=normalize(fs_in.Normal);
vec3 viewDir=normalize(viewPos-fs_in.FragPos);


uniform sampler2D textureTest;
void main()
{

//冯氏光照模型 环境光照ambient,漫反射光照Diffuse,镜面光照Specular
    vec3 result=vec3(0,0,0);
    //result += CalcSpotLightColor(spotlight);
   //result += CalcDirectLightColor(directlight);

//    for(int i=0; i<NR_POINT_LIGHTS;++i)
//    {
        result+=CalcPointLightColor(pointlight[0]);
//    }


        //result=texture(textureTest,fs_in.TexCoords).rgb;
    FragColor = vec4(result, 1.0);

    //gl_FragDepth = gl_FragCoord.z + 0.1;
}

vec3 CalcSpotLightColor(SpotLight light)
{
    //点光源会根据距离进行衰减，衰减的值用一个二次方程表示，并让环境光，漫反射光，镜面反射光都*=衰减因子
    float distance=length(light.position-fs_in.FragPos);
    float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);

    //环境光照ambient
    vec3 ambient=light.ambient*diffuseTexColor;
    ambient*=attenuation;

    //漫反射光照
    vec3 lightDir=normalize(light.position-fs_in.FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse=light.diffuse*diff*diffuseTexColor;
    diffuse*=attenuation;

    //Blinn-Phong
    vec3 halfwayDir=normalize(lightDir+viewDir);
    //镜面光照
    //vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(norm,halfwayDir),0),material.shininess);
    vec3 specular=light.specular*spec*specularTexColor;
    specular*=attenuation;

    float theta=dot(lightDir,normalize(-light.direction));
    float alpha=smoothstep(light.cutOff,light.cutOn ,theta);

    diffuse*=alpha;
    specular*=alpha;
    //return vec3(1,1,1);
    return ambient+(diffuse+specular);
    //return light.ambient+light.diffuse+light.specular;
}

vec3 CalcDirectLightColor(DirectLight light)
{
   float directShadow=DirectShadowCalculation(fs_in.FragPosLightSpace);
    //环境光照ambient
    vec3 ambient=light.ambient*diffuseTexColor;

    //漫反射光照
     vec3 lightDir=normalize(-light.direction);
     float diff = max(dot(norm,lightDir),0.0);
     vec3 diffuse=light.diffuse*diff*diffuseTexColor;

        vec3 halfwayDir=normalize(lightDir+viewDir);
     //镜面光照
     //vec3 reflectDir=reflect(-lightDir,norm);
     float spec=pow(max(dot(norm,halfwayDir),0),material.shininess);
     vec3 specular=light.specular*spec*specularTexColor;

      return ambient+/*(1.0-directShadow)**/(diffuse+specular);
}

vec3 CalcPointLightColor(PointLight light)
{

    float pointShadow=PointShadowCalculation(light);
//    //点光源会根据距离进行衰减，衰减的值用一个二次方程表示，并让环境光，漫反射光，镜面反射光都*=衰减因子
//    float distance=length(light.position-fs_in.FragPos);
//    float attenuation=1.0/*/(light.constant+light.linear*distance+light.quadratic*distance*distance)*/;

//    //环境光照ambient
//    vec3 ambient=light.ambient*diffuseTexColor;
//    ambient*=attenuation;

//    //漫反射光照
//    vec3 lightDir=normalize(light.position-fs_in.FragPos);
//    float diff = max(dot(norm,lightDir),0.0);
//    vec3 diffuse=light.diffuse*diff*diffuseTexColor;
//    diffuse*=attenuation;

//    //镜面光照
//    vec3 halfwayDir=normalize(lightDir+viewDir);
//    vec3 reflectDir=reflect(-lightDir,norm);
//    float spec=pow(max(dot(norm,halfwayDir),0),material.shininess);
//    vec3 specular=light.specular*spec*specularTexColor;
//    specular*=attenuation;
//    //return vec3(0,0,0);
//    //return vec3(0.1,0.1,0.1);

//    return ambient+(1-pointShadow)*(diffuse+specular);

    vec3 color = diffuseTexColor;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.4);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // calculate shadow
   // float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;
    vec3 lighting = (ambient + (1.0 - pointShadow) * (diffuse + specular)) * color;

    return lighting;
}

float DirectShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias=0;
          bias =max(0.05*(1.0-dot(norm,directlight.direction)),0.005);
    float shadow =0;

    vec2 texelSize=1.0/textureSize(depthMap,0);

    for(int x=-1;x<=1;++x)
    {
        for(int y=-1;y<=1;++y)
        {
            float pcfDepth=texture(depthMap,projCoords.xy+vec2(x,y)*texelSize).r;

            shadow+=currentDepth-bias>pcfDepth?1.0:0.0;
        }

    }
        shadow/=9.0f;



            //currentDepth-bias > closestDepth &&projCoords.z < 1.0 ? 1.0 : 0.0;

    if(projCoords.z > 1.0/* || projCoords.x < 0.0 || projCoords.x > 1.0*/)
            shadow = 0.0;

 return shadow;
}

float PointShadowCalculation(PointLight light)
{
    vec3 fragToLight= fs_in.FragPos - light.position;

    float closestDepth = texture(depthCubeMap, fragToLight).r;

    closestDepth *= far_plane;
    float currentDepth=length(fragToLight);

    float bias=0.05;
    float shadow=currentDepth-bias>closestDepth?1.0:0.0;


    return shadow;
}
