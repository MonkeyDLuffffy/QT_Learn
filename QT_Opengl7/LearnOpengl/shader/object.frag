#version 330 core


//受到不同的光照的影响程度
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

//一个光源提供的不同的光照强度与光源的位置
struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lithtPos;
uniform vec3 viewPos;



void main()
{
//冯氏光照模型 环境光照ambient,漫反射光照Diffuse,镜面光照Specular


    vec3 diffuseTexColor=vec3(texture(material.diffuse,TexCoords));
    vec3 specularTexColor=vec3(texture(material.specular,TexCoords));
    vec3 emissionTexColor=vec3(texture(material.emission,TexCoords));

//环境光照ambient
    vec3 ambient=light.ambient*diffuseTexColor;

//漫反射光照Diffuse,
    vec3 norm=normalize(Normal);
    vec3 lithtDir=normalize(light.position-FragPos);
    float diff = max(dot(norm,lithtDir),0.0);
    vec3 diffuse=light.diffuse*diff*diffuseTexColor;

//镜面光照Specular
    vec3 viewDir=normalize(viewPos-FragPos);
    vec3 reflectDir=reflect(-lithtDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0),material.shininess);
    vec3 specular=light.specular*spec*specularTexColor;

//求和
    vec3 result=(ambient+diffuse+specular)+emissionTexColor;
    FragColor = vec4(result, 1.0);
}
