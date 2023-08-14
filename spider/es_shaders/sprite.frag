#version 300 es
precision mediump float;

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
    color = spriteColor * texture(image, TexCoords);
}