#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform float roughness;
uniform float metallic;
uniform vec3 camPos;

void main()
{
    vec3 lightPos = vec3(10,20,10);
    vec3 lightColor = vec3(1.0);
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(camPos - FragPos);
    vec3 H = normalize(V+L);
    float diff = max(dot(N,L),0.0);
    float spec = pow(max(dot(N,H),0.0),16.0);
    vec3 color = objectColor * diff + spec * metallic;
    FragColor = vec4(color,1.0);
}
