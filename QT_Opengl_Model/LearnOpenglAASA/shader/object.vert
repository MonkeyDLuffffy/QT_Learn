#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;


layout(std140) uniform Matrices
{
  mat4 projection;
  mat4 view;
};

out VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
}vs_out;

uniform mat4 model;

void main()
{
   vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
   vs_out.FragPos=vec3(model*vec4(aPos,1));
   vs_out.TexCoords=aTexCoords;

    gl_Position =projection* view* model*vec4(aPos, 1.0);
    //gl_PointSize=gl_Position.z;
}
