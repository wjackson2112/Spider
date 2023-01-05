#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform bool  chaos;
uniform bool  confuse;
uniform bool  shake;
uniform float time;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoords;

    if(chaos)
    {
        float strength = 0.3;
        TexCoords = vec2(TexCoords.x + sin(time) * strength,
                         TexCoords.y + cos(time) * strength);
    }
    else if(confuse)
    {
        TexCoords = vec2(1.0 - aTexCoords.x, 1.0 - aTexCoords.y);
    }
    else
    {
        TexCoords = aTexCoords;
    }

    if(shake)
    {
        float strength = 0.005;
        TexCoords = vec2(TexCoords.x + cos(time * 10) * strength,
                         TexCoords.y + cos(time * 15) * strength);
    }
}