#version 330 core
layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec2 texCoords; // Texture coordinates

out vec2 TexCoords; // Pass texture coordinates to fragment shader
out vec3 FragPos;   // Pass vertex position to fragment shader

uniform mat4 model;     // Model matrix
uniform mat4 view;      // View matrix
uniform mat4 projection; // Projection matrix

void main() {
    FragPos = vec3(model * vec4(position, 1.0)); // Convert position to world coordinates
    gl_Position = projection * view * vec4(FragPos, 1.0); // Transform position to clip space
    TexCoords = texCoords; // Pass texture coordinates
}
