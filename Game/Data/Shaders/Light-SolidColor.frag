/*struct Light {    
    vec3 position;
    vec3 diffuse;
};  
uniform int numLights; 
uniform Light lights[];*/

#define NUM_LIGHTS 4 

uniform vec4 u_MaterialColor;
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform float u_LightRadius;
uniform float u_LightPowerFactor;

uniform vec4 u_LightColors[NUM_LIGHTS];
uniform vec3 u_LightPositions[NUM_LIGHTS];
uniform float u_LightRadii[NUM_LIGHTS];
uniform float u_LightPowerFactors[NUM_LIGHTS];

varying vec3 v_Normal;
varying vec3 v_SurfacePos;

vec3 PointLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = vec3(u_LightColors[index].x,u_LightColors[index].y,u_LightColors[index].z);
    vec3 dirToLight = u_LightPositions[index] - v_SurfacePos;
    float distanceFromLight = length(dirToLight);
    vec3 normalizedDirToLight = normalize(dirToLight);
    
    float attenuation = pow( max(0, 1 - distanceFromLight / u_LightRadii[index]), u_LightPowerFactors[index]); //Falloff

    float diffusePerc = max(0, dot(normalizedDirToLight, normalizeNormal));
    vec3 finalColor = materialColor * lightColor * diffusePerc * attenuation;

    return finalColor;
}

void main()
{
    vec3 materialColor = vec3(u_MaterialColor.x,u_MaterialColor.y,u_MaterialColor.z);
    vec3 normalizeNormal = normalize(v_Normal);

    vec3 litColor = vec3(0.0);

    for(int i = 0; i < NUM_LIGHTS; i++)
    {
        litColor += PointLight(i, normalizeNormal, materialColor);
    }

    gl_FragColor = vec4(litColor,1);

    // DEBUGGING:
    //gl_FragColor = vec4(v_SurfacePos,1);
    //gl_FragColor = vec4(v_Normal,1);
}
