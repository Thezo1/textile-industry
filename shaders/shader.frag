#version 330 core
out vec4 FragColor;

in vec3 our_color;
in vec2 text_coord;

uniform sampler2D tex0;
void main()
{
    FragColor = texture(tex0, text_coord);
}
