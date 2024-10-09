#version 330 core
out vec4 FragColor;
  

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


uniform sampler2D ourTexture;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;


void main()
{
    vec4 tColor = texture(ourTexture, TexCoord);

    float ambientStrength = 0.7;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
       // specular
   
    vec3 result = (ambient + diffuse);

    FragColor = vec4(result, 1.0f) * tColor;
}