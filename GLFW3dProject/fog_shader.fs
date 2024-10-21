#version 330 core
out vec4 color;

in vec2 TexCoords; // Texture coordinates from vertex shader
in vec3 FragPos;   // Fragment position from vertex shader

uniform sampler2D ourTexture;
uniform vec3 cameraPos; // Camera position for fog calculations
uniform vec3 fogColor;   // Fog color
uniform float fogDensity; // Fog density

void main() {
    // Calculate distance from the fragment position to the camera position
    float distance = length(cameraPos - FragPos);
    
    // Calculate fog factor using an exponential function
    float fogFactor = exp(-fogDensity * distance * distance);

    // For demonstration, assume you have a texture to sample
    vec4 sceneColor = texture(textureSampler, TexCoords);
    
    // Mix fog color and scene color based on fog factor
    color = mix(vec4(fogColor, 1.0), sceneColor, fogFactor);
}
