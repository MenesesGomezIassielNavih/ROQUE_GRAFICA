#version 330 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  tangent;
layout (location = 4) in vec3  bitangent;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float radius;
uniform float height;

void main()
{
    
    //--rotacion antihoraria
    /*
    vec4 PosL = vec4(aPos, 10.0f);
    PosL.x += radius * cos(time);
    PosL.y += radius * sin(time);
    PosL.z += height;
    */

    //trayectoria circular horaria
    
    vec4 PosL = vec4(aPos, 10.0f);

    float waveX = radius * sin(time) * 0.5; // Onda sinusoidal con amplitud reducida en la coordenada X
    float waveY = radius * cos(time) * 0.5; // Onda cosenoidal con amplitud reducida en la coordenada Y
    PosL.x += waveX;
    PosL.y += waveY;
    PosL.z += height;

    PosL.z += 2.0f * sin(time * 5.0f);//ondas sinusoidles

    gl_Position = projection * view * model * PosL;

    TexCoords = aTexCoords;  
}