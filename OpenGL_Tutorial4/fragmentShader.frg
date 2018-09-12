#version 330 core
in vec3 colour; // same name and type as out variable from vrt shader

out vec4 FragColor; //The output colour variable - can be named anything
 
void main()
{
    FragColor = vec4(colour, 1.0f); // sets alpha channel to 1
} 