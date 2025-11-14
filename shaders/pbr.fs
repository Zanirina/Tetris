#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform float roughness;
uniform float metallic;
uniform vec3 camPos;

uniform float emissionStrength;
uniform vec3 emissionColor;
uniform float fade;

// Fog
uniform int useFog;
uniform vec3 fogColor;
uniform float fogNear;
uniform float fogFar;

void main()
{
    // --- Lighting ---
    vec3 lightPos = vec3(10,20,10);
    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - FragPos);
    vec3 V = normalize(camPos - FragPos);
    vec3 H = normalize(V + L);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 32.0); // ярче бликов

    // осветил базовый цвет
    vec3 litColor = objectColor * (diff * 1.5 + 0.3) + spec * metallic * 1.5;

    // --- Emission (glow) ---
    vec3 emission = emissionColor * emissionStrength;

    // глянец по краю
    float edge = pow(1.0 - max(dot(N, V), 0.0), 2.0);
    emission *= edge;

    litColor += emission;

    // Fade-in
    litColor *= fade;

    // Fog
    if(useFog == 1)
    {
        float dist = length(camPos - FragPos);
        float fogFactor = clamp((dist - fogNear) / (fogFar - fogNear), 0.0, 1.0);
        vec3 finalColor = mix(litColor, fogColor, fogFactor);
        FragColor = vec4(finalColor, 1.0);
    }
    else
    {
        FragColor = vec4(litColor, 1.0);
    }
}
