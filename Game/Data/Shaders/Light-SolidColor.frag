uniform vec4 u_MaterialColor;
uniform vec4 u_LightColor;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;

void main()
{
    vec3 materialColor = vec3(u_MaterialColor.x,u_MaterialColor.y,u_MaterialColor.z);
    vec3 lightColor = vec3(u_LightColor.x,u_LightColor.y,u_LightColor.z);

    vec3 lightPos = vec3(9.5,12,0);
    vec3 normal = vec3(0,1,0);

    vec3 dirToLight = lightPos - v_SurfacePos;
    vec3 normalizedDirToLight = normalize(dirToLight);
    vec3 normalizeNormal = normalize(v_Normal); 

    float diffusePerc = dot(normalizedDirToLight, normalizeNormal);
    vec3 finalColor = materialColor * lightColor * diffusePerc;

    gl_FragColor = vec4(finalColor,1);

    // DEBUGGING:
    //gl_FragColor = vec4(v_SurfacePos,1);
    //gl_FragColor = vec4(normal,1);
}
