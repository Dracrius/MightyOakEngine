#define NUM_LIGHTS 9 

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

uniform vec3 u_CamPos;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;

vec3 DirectLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = vec3(u_LightColors[index].x,u_LightColors[index].y,u_LightColors[index].z);
    vec3 dirToLight = u_LightPositions[index];
    vec3 normalizedDirToLight = normalize(dirToLight);

    float diffusePerc = max(0, dot(normalizeNormal, normalizedDirToLight));

    // specular shading
    //vec3 reflectDir = reflect(-normalizedDirToLight, normalizeNormal);
    //vec3 viewDir = normalize(u_CamPos - v_SurfacePos);
    vec3 dirToCam = u_CamPos - v_SurfacePos;
    vec3 normalizedDirToCam = normalize(dirToCam);
    vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);

    float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = lightColor * 0.1 * materialColor;
    vec3 diffuse  = lightColor * diffusePerc * materialColor;
    vec3 specular = lightColor * specPerc;
    vec3 finalColor = ambient + diffuse; // + specular;

    return finalColor;
}

vec3 PointLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = vec3(u_LightColors[index].x,u_LightColors[index].y,u_LightColors[index].z);
    vec3 dirToLight = u_LightPositions[index] - v_SurfacePos;
    float distanceFromLight = length(dirToLight);
    vec3 normalizedDirToLight = normalize(dirToLight);
    
    float attenuation = pow( max(0, 1 - distanceFromLight / u_LightRadii[index]), u_LightPowerFactors[index]); //Falloff

    float diffusePerc = max(0, dot(normalizedDirToLight, normalizeNormal));

    // specular shading
    //vec3 reflectDir = reflect(-normalizedDirToLight, normalizeNormal);
    //vec3 viewDir = normalize(u_CamPos - v_SurfacePos);
    vec3 dirToCam = u_CamPos - v_SurfacePos;
    vec3 normalizedDirToCam = normalize(dirToCam);
    vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);

    float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = lightColor * 0.1 * materialColor * attenuation;
    vec3 diffuse  = lightColor * diffusePerc * materialColor * attenuation;
    vec3 specular = lightColor * specPerc * attenuation;
    //vec3 specular = vec3(1.0, 1.0, 1.0) * spec * materialColor * attenuation

    vec3 finalColor = ambient + diffuse + specular;

    return finalColor;
}

vec3 SpotLight(int index, vec3 normalizeNormal, vec3 materialColor, float spotCosCutoff)
{
    vec3 lightColor = vec3(u_LightColors[index].x,u_LightColors[index].y,u_LightColors[index].z);
    vec3 dirToLight = u_LightPositions[index] - v_SurfacePos;
    float distanceFromLight = length(dirToLight);
    vec3 normalizedDirToLight = normalize(dirToLight);

    float attenuation = pow( max(0, 1 - distanceFromLight / u_LightRadii[index]), u_LightPowerFactors[index]);

    float diffusePerc = max(0, dot(normalizeNormal, normalizedDirToLight));

    // specular shading
    //vec3 reflectDir = reflect(-normalizedDirToLight, normalizeNormal);
    //vec3 viewDir = normalize(u_CamPos - v_SurfacePos);
    vec3 dirToCam = u_CamPos - v_SurfacePos;
    vec3 normalizedDirToCam = normalize(dirToCam);
    vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);

    float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // See if point on surface is inside cone of illumination
    float spotDot = dot(-normalizedDirToLight, normalize(u_lightRotations[index]));
    float spotAttenuation;

    if (spotDot < spotCosCutoff)
        spotAttenuation = 0.0; // light adds no contribution
    else
        spotAttenuation = pow(spotDot, u_LightPowerFactors[index]);

    // Combine the spotlight and distance attenuation.
    attenuation *= spotAttenuation;

    vec3 ambient  = lightColor  * 0.1 * materialColor * attenuation;
    vec3 diffuse  = lightColor  * diffusePerc * materialColor * attenuation;
    vec3 specular = lightColor * specPerc * attenuation;
    vec3 finalColor = ambient + diffuse + specular;

    return finalColor;
}

void main()
{
    vec3 materialColor = vec3(u_MaterialColor.x,u_MaterialColor.y,u_MaterialColor.z);
    vec3 normalizeNormal = normalize(v_Normal);

    vec3 litColor = vec3(0.0);

    for(int i = 0; i < NUM_LIGHTS; i++)
    {
        if(i == 0)
        {
            //litColor += DirectLight(i, normalizeNormal, materialColor);
        }
        else if(i < 5)
        {
            litColor += PointLight(i, normalizeNormal, materialColor);
        }
        else
        {
            //litColor += SpotLight(i, normalizeNormal, materialColor, 30.0);
        }
    }

    gl_FragColor = vec4(litColor,1);

    // DEBUGGING:
    //gl_FragColor = vec4(v_SurfacePos,1);
    //gl_FragColor = vec4(v_Normal,1);
}
