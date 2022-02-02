#include "Framework.h"

#include "DataTypes.h"
#include "Shapes.h"

const std::vector<fw::VertexFormat> g_SpriteVerts =
{
    { vec3(-0.5f,-0.5f,0.f),  255,255,255,255,  vec2(0.0f,0.0f) }, // bottom left
    { vec3(-0.5f,0.5f,0.f),  255,255,255,255,  vec2(0.0f,1.0f) }, // top left
    { vec3(0.5f,-0.5f,0.f),  255,255,255,255,  vec2(1.0f,0.0f) }, // bottom right
    { vec3(0.5f,0.5f,0.f),  255,255,255,255,  vec2(1.0f,1.0f) }, // top right
};

const std::vector<unsigned int> g_SpriteIndices =
{
    0, 1, 2, 2, 1, 3,
};

const std::vector<fw::VertexFormat> g_CubeVerts =
{   
    //Side 1
    { vec3(-1.0f,-1.0f,-1.f),  255,255,255,255,     vec2(0.f,       0.f) }, // Point 1
    { vec3(1.0f,1.0f,-1.f),  255,255,255,255,       vec2(1.f / 6.f, 1.f) }, // Point 2
    { vec3(1.0f,-1.0f,-1.f),  255,255,255,255,      vec2(1.f / 6.f, 0.f) }, // Point 3
    { vec3(-1.0f,-1.0f,-1.f),  255,255,255,255,     vec2(0.f,       0.f) }, // Point 4
    { vec3(-1.0f,1.0f,-1.f),  255,255,255,255,      vec2(0.f,       1.f) }, // Point 5
    { vec3(1.0f,1.0f,-1.f),  255,255,255,255,       vec2(1.f / 6.f, 1.f) }, // Point 6
    //Side 2
    { vec3(-1.0f,1.0f,-1.f),  255,255,255,255,      vec2(1.f / 6.f, 0.f) }, // Point 1
    { vec3(1.0f,1.0f,1.f),  255,255,255,255,        vec2(2.f / 6.f, 1.f) }, // Point 2
    { vec3(1.0f,1.0f,-1.f),  255,255,255,255,       vec2(2.f / 6.f, 0.f) }, // Point 3
    { vec3(-1.0f,1.0f,-1.f),  255,255,255,255,      vec2(1.f / 6.f, 0.f) }, // Point 4
    { vec3(-1.0f,1.0f,1.f),  255,255,255,255,       vec2(1.f / 6.f, 1.f) }, // Point 5
    { vec3(1.0f,1.0f,1.f),  255,255,255,255,        vec2(2.f / 6.f, 1.f) }, // Point 6
    //Side 3
    { vec3(-1.0f,1.0f,1.f),  255,255,255,255,       vec2(3.f / 6.f, 1.f) }, // Point 1
    { vec3(1.0f,-1.0f,1.f),  255,255,255,255,       vec2(2.f / 6.f, 0.f) }, // Point 2
    { vec3(1.0f,1.0f,1.f),  255,255,255,255,        vec2(2.f / 6.f, 1.f) }, // Point 3
    { vec3(-1.0f,1.0f,1.f),  255,255,255,255,       vec2(3.f / 6.f, 1.f) }, // Point 4
    { vec3(-1.0f,-1.0f,1.f),  255,255,255,255,      vec2(3.f / 6.f, 0.f) }, // Point 5
    { vec3(1.0f,-1.0f,1.f),  255,255,255,255,       vec2(2.f / 6.f, 0.f) }, // Point 6
    //Side 4
    { vec3(-1.0f,-1.0f,1.f),  255,255,255,255,      vec2(3.f / 6.f, 0.f) }, // Point 1
    { vec3(1.0f,-1.0f,-1.f),  255,255,255,255,      vec2(4.f / 6.f, 1.f) }, // Point 2
    { vec3(1.0f,-1.0f,1.f),  255,255,255,255,       vec2(4.f / 6.f, 0.f) }, // Point 3
    { vec3(-1.0f,-1.0f,1.f),  255,255,255,255,      vec2(3.f / 6.f, 0.f) }, // Point 4
    { vec3(-1.0f,-1.0f,-1.f),  255,255,255,255,     vec2(3.f / 6.f, 1.f) }, // Point 5
    { vec3(1.0f,-1.0f,-1.f),  255,255,255,255,      vec2(4.f / 6.f, 1.f) }, // Point 6
    //Side 5
    { vec3(1.0f,-1.0f,-1.f),  255,255,255,255,      vec2(4.f / 6.f, 0.f) }, // Point 1
    { vec3(1.0f,1.0f,1.f),  255,255,255,255,        vec2(5.f / 6.f, 1.f) }, // Point 2
    { vec3(1.0f,-1.0f,1.f),  255,255,255,255,       vec2(5.f / 6.f, 0.f) }, // Point 3
    { vec3(1.0f,-1.0f,-1.f),  255,255,255,255,      vec2(4.f / 6.f, 0.f) }, // Point 4
    { vec3(1.0f,1.0f,-1.f),  255,255,255,255,       vec2(4.f / 6.f, 1.f) }, // Point 5
    { vec3(1.0f,1.0f,1.f),  255,255,255,255,        vec2(5.f / 6.f, 1.f) }, // Point 6
    //Side 6
    { vec3(-1.0f,-1.0f,1.f),  255,255,255,255,      vec2(5.f / 6.f, 0.f) }, // Point 1
    { vec3(-1.0f,1.0f,-1.f),  255,255,255,255,      vec2(1.f,       1.f) }, // Point 2
    { vec3(-1.0f,-1.0f,-1.f),  255,255,255,255,     vec2(1.f,       0.f) }, // Point 3
    { vec3(-1.0f,-1.0f,1.f),  255,255,255,255,      vec2(5.f / 6.f, 0.f) }, // Point 4
    { vec3(-1.0f,1.0f,1.f),  255,255,255,255,       vec2(5.f / 6.f, 1.f) }, // Point 5
    { vec3(-1.0f,1.0f,-1.f),  255,255,255,255,      vec2(1.f,       1.f) }, // Point 6
};

fw::Mesh* CreatePlane(vec2 size, ivec2 vertRes)
{
    vec2 stepSize = vec2(size.x / (vertRes.x - 1), size.y / (vertRes.y - 1));

    std::vector<fw::VertexFormat> verts;
    std::vector<unsigned int> indices;
    
    //Centered
    for (int y = 0; y < vertRes.y; y++)
    {
        for (int x = 0; x < vertRes.x; x++)
        {
            vec3 pos = vec3((float(x) - ((vertRes.x / 2) - 0.5f)) * stepSize.x, (float(y) - ((vertRes.y / 2) - 0.5f)) * stepSize.y, 0);

            verts.push_back({ pos,  255,255,255,255,  vec2(float(x) / float(vertRes.x - 1.f), float(y) / float(vertRes.y - 1.f)) });
        }
    }

    //Bottom Left
    /*for (int y = 0; y < vertRes.y; y++)
    {
        for (int x = 0; x < vertRes.x; x++)
        {
            vec3 pos = vec3(x * stepSize.x, y * stepSize.y, 0);

            verts.push_back({ pos,  255,255,255,255,  vec2(float(x) / float(vertRes.x - 1.f), float(y) / float(vertRes.y - 1.f)) });
        }
    }*/

    for (int iy = 0; iy < (vertRes.y - 1); iy++)
    {
        for (int ix = 0; ix < (vertRes.x - 1); ix++)
        {
            indices.push_back((iy * vertRes.x) + ix);
            indices.push_back((vertRes.x * (iy + 1)) + ix);
            indices.push_back((vertRes.x * (iy + 1) + 1) + ix);
            indices.push_back((iy * vertRes.x) + ix);
            indices.push_back((vertRes.x * (iy + 1) + 1) + ix);
            indices.push_back((iy * vertRes.x) + 1 + ix);
        }
    }

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    fw::Mesh* pMesh = new fw::Mesh(GL_TRIANGLES, verts, indices);
    return pMesh;
};
