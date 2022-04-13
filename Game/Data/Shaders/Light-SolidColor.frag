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
uniform float u_SpotCosCutoff[NUM_LIGHTS];

uniform vec3 u_CamPos;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;

vec3 DirectLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = vec3(u_LightColors[index].x,u_LightColors[index].y,u_LightColors[index].z);
    vec3 dirToLight = u_LightPositions[index];
    vec3 normalizedDirToLight = normalize(dirToLight);

    float diffusePerc = max(0, dot(normalizeNormal, normalizedDirToLight));

    // Phong Specular Shading
    //vec3 reflectDir = reflect(-normalizedDirToLight, normalizeNormal);
    //vec3 viewDir = normalize(u_CamPos - v_SurfacePos);
    //float specPerc = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);

    // Blinn-Phong Specular Shading
    vec3 dirToCam = u_CamPos - v_SurfacePos;
    vec3 normalizedDirToCam = normalize(dirToCam);
    vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);
    float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100);

    vec3 ambient  = lightColor * 0.1 * materialColor;
    vec3 diffuse  = lightColor * diffusePerc * materialColor;
    vec3 specular = lightColor * specPerc;

    vec3 finalColor = ambient + diffuse + specular;

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

    // Phong Specular Shading
    //vec3 reflectDir = reflect(-normalizedDirToLight, normalizeNormal);
    //vec3 viewDir = normalize(u_CamPos - v_SurfacePos);
    //float specPerc = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);

    // Blinn-Phong Specular Shading
    vec3 dirToCam = u_CamPos - v_SurfacePos;
    vec3 normalizedDirToCam = normalize(dirToCam);
    vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);
    float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100);

    vec3 ambient  = lightColor * 0.1 * materialColor * attenuation;
    vec3 diffuse  = lightColor * diffusePerc * materialColor * attenuation;
    vec3 specular = lightColor * specPerc * attenuation;

    vec3 finalColor = ambient + diffuse + specular;

    return finalColor;
}

float DistanceAtten(int index, float distanceFromLight)
{
    float attenuation = pow( max(0, 1 - distanceFromLight / u_LightRadii[index]), u_LightPowerFactors[index]); //Falloff
    return attenuation;
}

vec3 SpotLight(int index, vec3 normalizeNormal, vec3 materialColor)
{
    vec3 lightColor = vec3(u_LightColors[index].x,u_LightColors[index].y,u_LightColors[index].z);
    vec3 dirToLight = u_LightPositions[index] - v_SurfacePos;
    float distanceFromLight = length(dirToLight);
    vec3 normalizedDirToLight = normalize(dirToLight);

    float attenuation = pow( max(0, 1 - distanceFromLight / u_LightRadii[index]), u_LightPowerFactors[index]);

    float diffusePerc = max(0, dot(normalizeNormal, normalizedDirToLight));

    // Phong Specular Shading
    //vec3 reflectDir = reflect(-normalizedDirToLight, normalizeNormal);
    //vec3 viewDir = normalize(u_CamPos - v_SurfacePos);
    //float specPerc = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);
    
    // Blinn-Phong Specular Shading
    vec3 dirToCam = u_CamPos - v_SurfacePos;
    vec3 normalizedDirToCam = normalize(dirToCam);
    vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);
    float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100);

    // spot light attenuation
    float spotDot = dot(-normalizedDirToLight, normalize(u_lightRotations[index]));
    float cutoff = 1 - u_SpotCosCutoff[index];
    float spotAttenuation = clamp((spotDot - u_SpotCosCutoff[index]) / cutoff, 0.0, 1.0);

    // Combine the spotlight and distance attenuation.
    attenuation *= spotAttenuation;

    vec3 ambient  = lightColor * 0.1 * materialColor * attenuation;
    vec3 diffuse  = lightColor * diffusePerc * materialColor * attenuation;
    vec3 specular = lightColor * specPerc * attenuation;

    vec3 finalColor = ambient + diffuse + specular;

    return finalColor;
}

float SpotAtten(int index, vec3 normalizedDirToLight)
{
    float spotDot = dot(-normalizedDirToLight, normalize(u_lightRotations[index]));
    float cutoff = 1 - u_SpotCosCutoff[index];
    float spotAttenuation = clamp((spotDot - u_SpotCosCutoff[index]) / cutoff, 0.0, 1.0);
    return spotAttenuation;
}

void main()
{
    vec3 materialColor = vec3(u_MaterialColor.x,u_MaterialColor.y,u_MaterialColor.z);
    vec3 normalizeNormal = normalize(v_Normal);

    vec3 litColor = vec3(0.0);

    for(int i = 0; i < NUM_LIGHTS; i++)
    {
        vec3 lightColor = vec3(u_LightColors[i].x,u_LightColors[i].y,u_LightColors[i].z);
        vec3 dirToLight = u_LightPositions[i];
        float distanceFromLight = length(dirToLight);
        vec3 normalizedDirToLight = normalize(dirToLight);

        float diffusePerc = max(0, dot(normalizeNormal, normalizedDirToLight));

        // Phong Specular Shading
        //vec3 reflectDir = reflect(-normalizedDirToLight, normalizeNormal);
        //vec3 viewDir = normalize(u_CamPos - v_SurfacePos);
        //float specPerc = pow(max(0.0, dot(viewDir, reflectDir)), material.shininess);

        // Blinn-Phong Specular Shading
        vec3 dirToCam = u_CamPos - v_SurfacePos;
        vec3 normalizedDirToCam = normalize(dirToCam);
        vec3 halfVector = normalize(normalizedDirToCam + normalizedDirToLight);
        float specPerc = pow(max(0.0, dot(normalizeNormal, halfVector)), 100);

        float attenuation;

        if(i == 0)
        {
            vec3 ambient  = lightColor * 0.1 * materialColor;
            vec3 diffuse  = lightColor * diffusePerc * materialColor;
            vec3 specular = lightColor * specPerc;

            litColor += ambient + diffuse + specular;
        }
        else if(i < 5)
        {
            litColor += PointLight(i, normalizeNormal, materialColor);
            
            /*float attenuation = DistanceAtten(i, distanceFromLight);
            vec3 ambient  = lightColor * 0.1 * materialColor * attenuation;
            vec3 diffuse  = lightColor * diffusePerc * materialColor * attenuation;
            vec3 specular = lightColor * specPerc * attenuation;

            litColor += ambient + diffuse + specular;*/
        }
        else
        {
            litColor += SpotLight(i, normalizeNormal, materialColor);

            /*float attenuation = DistanceAtten(i, distanceFromLight) * SpotAtten(int index, normalizedDirToLight);
            vec3 ambient  = lightColor * 0.1 * materialColor * attenuation;
            vec3 diffuse  = lightColor * diffusePerc * materialColor * attenuation;
            vec3 specular = lightColor * specPerc * attenuation;

            litColor += ambient + diffuse + specular;*/
        }
    }

    gl_FragColor = vec4(litColor,1);

    // DEBUGGING:
    //gl_FragColor = vec4(v_SurfacePos,1);
    //gl_FragColor = vec4(v_Normal,1);
}
