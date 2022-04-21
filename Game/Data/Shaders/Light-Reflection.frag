#define NUM_LIGHTS 9 

uniform samplerCube u_CubemapTexture;

uniform vec4 u_MaterialColor;
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform float u_LightRadius;
uniform float u_LightPowerFactor;

uniform vec4 u_LightColors[NUM_LIGHTS];
uniform vec3 u_LightPositions[NUM_LIGHTS];
uniform vec3 u_lightRotations[NUM_LIGHTS];
uniform float u_LightRadii[NUM_LIGHTS];
uniform float u_LightPowerFactors[NUM_LIGHTS];
uniform float u_SpotCosCutoff[NUM_LIGHTS];

uniform vec3 u_CamPos;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;
varying vec3 v_ReflectedDir;

varying vec2 v_UVCoord;
varying vec4 v_Color;

vec3 CalcLightPerc(vec3 lightColor, vec3 materialColor, vec3 normalizeNormal, vec3 normalizedDirToLight)
{
    float diffusePerc = max(0.0, dot(normalizeNormal, normalizedDirToLight));

    // Blinn-Phong Specular Shading
    vec3 dirToCam = u_CamPos - v_SurfacePos;
    vec3 normalizedDirToCam = normalize(dirToCam);
    vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);
    float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100.0);

    vec3 ambient  = lightColor * 0.1 * materialColor;
    vec3 diffuse  = lightColor * diffusePerc * materialColor;
    vec3 specular = lightColor * specPerc;

    return ambient + diffuse + specular;
}

vec3 DirectLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = u_LightColors[index].xyz;
    vec3 dirOfLight = u_lightRotations[index];
    vec3 normalizedDirOfLight = normalize(dirOfLight);

    vec3 finalColor = CalcLightPerc(lightColor, materialColor, normalizeNormal, normalizedDirOfLight);

    return finalColor;
}

vec3 PointLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = u_LightColors[index].xyz;
    vec3 dirToLight = u_LightPositions[index] - v_SurfacePos;
    float distanceFromLight = length(dirToLight);
    vec3 normalizedDirToLight = normalize(dirToLight);
    
    float attenuation = pow( max(0.0, 1.0 - distanceFromLight / u_LightRadii[index]), u_LightPowerFactors[index]); //Falloff

    vec3 finalColor = CalcLightPerc(lightColor, materialColor, normalizeNormal, normalizedDirToLight) * attenuation;

    return finalColor;
}

vec3 SpotLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = u_LightColors[index].xyz;
    vec3 dirToLight = u_LightPositions[index] - v_SurfacePos;
    float distanceFromLight = length(dirToLight);
    vec3 normalizedDirToLight = normalize(dirToLight);

    float attenuation = pow( max(0.0, 1.0 - distanceFromLight / u_LightRadii[index]), u_LightPowerFactors[index]);

    // spot light attenuation
    float spotDot = dot(-normalizedDirToLight, normalize(u_lightRotations[index]));
    float cutoff = 1.0 - u_SpotCosCutoff[index];
    float spotAttenuation = clamp((spotDot - u_SpotCosCutoff[index]) / cutoff, 0.0, 1.0);

    // Combine the spotlight and distance attenuation.
    attenuation *= spotAttenuation;

    vec3 finalColor = CalcLightPerc(lightColor, materialColor, normalizeNormal, normalizedDirToLight) * attenuation;

    return finalColor;
}

void main()
{
    vec4 textureColor = textureCube(u_CubemapTexture, v_ReflectedDir);
    vec3 materialColor = vec3(textureColor.x,textureColor.y,textureColor.z);
    vec3 normalizeNormal = normalize(v_Normal);

    vec3 litColor = vec3(0.0);

    for(int i = 0; i < NUM_LIGHTS; i++)
    {
        if(i == 0)
        {
            litColor += DirectLight(i, normalizeNormal, materialColor);
        }
        else if(i < 5)
        {
            litColor += PointLight(i, normalizeNormal, materialColor);
        }
        else
        {
            litColor += SpotLight(i, normalizeNormal, materialColor);
        }
    }

    //float gamma = 2.2;
    //litColor = pow(litColor, vec3(1.0/gamma));

    gl_FragColor = vec4(litColor,1);

    // DEBUGGING:
    //gl_FragColor = vec4(v_SurfacePos,1);
    //gl_FragColor = vec4(v_Normal,1);
}
