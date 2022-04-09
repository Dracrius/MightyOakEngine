/*struct Light {    
    vec3 position;
    vec3 diffuse;
};  
uniform int numLights; 
uniform Light lights[];*/

uniform vec4 u_MaterialColor;
uniform vec4 u_LightColor;
uniform vec3 u_LightPos;
uniform float u_LightRadius;
uniform float u_LightPowerFactor;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;

void main()
{
    vec3 materialColor = vec3(u_MaterialColor.x,u_MaterialColor.y,u_MaterialColor.z);
    vec3 normalizeNormal = normalize(v_Normal);

    vec3 lightColor = vec3(u_LightColor.x,u_LightColor.y,u_LightColor.z);
    vec3 dirToLight = u_LightPos - v_SurfacePos;
    float distanceFromLight = length(dirToLight);
    vec3 normalizedDirToLight = normalize(dirToLight);
    
    float attenuation = pow( max(0, 1 - distanceFromLight / u_LightRadius), u_LightPowerFactor); //Falloff

    float diffusePerc = max(0, dot(normalizedDirToLight, normalizeNormal));
    vec3 finalColor = materialColor * lightColor * diffusePerc * attenuation;

    gl_FragColor = vec4(finalColor,1);

    // DEBUGGING:
    //gl_FragColor = vec4(v_SurfacePos,1);
    //gl_FragColor = vec4(v_Normal,1);
}
