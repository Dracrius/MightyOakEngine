uniform samplerCube u_CubemapTexture;

uniform vec3 u_CamPos;

varying vec3 v_Normal;
varying vec3 v_SurfacePos;
varying vec3 v_ReflectedDir;

void main()
{
    //vec3 dirToSurface = v_SurfacePos - u_CamPos;
    //vec3 reflectedDir = reflect(dirToSurface, normalize(v_Normal));

    gl_FragColor = textureCube(u_CubemapTexture, v_ReflectedDir);

    //gl_FragColor = vec4(v_Normal, 1);
}
