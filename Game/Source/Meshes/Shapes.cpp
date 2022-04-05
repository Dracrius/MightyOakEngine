#include "Framework.h"

#include "DataTypes.h"
#include "Shapes.h"

const std::vector<fw::VertexFormat> g_SpriteVerts =
{
    { vec3(-0.5f,-0.5f,0.f),  255,255,255,255,  vec2(0.0f,0.0f), vec3(0.f,1.f,0.f) }, // bottom left
    { vec3(-0.5f,0.5f,0.f),  255,255,255,255,  vec2(0.0f,1.0f), vec3(0.f,1.f,0.f) }, // top left
    { vec3(0.5f,-0.5f,0.f),  255,255,255,255,  vec2(1.0f,0.0f), vec3(0.f,1.f,0.f) }, // bottom right
    { vec3(0.5f,0.5f,0.f),  255,255,255,255,  vec2(1.0f,1.0f), vec3(0.f,1.f,0.f) }, // top right
};

const std::vector<unsigned int> g_SpriteIndices =
{
    0, 1, 2, 2, 1, 3,
};

const std::vector<fw::VertexFormat> g_BackgroundVerts =
{
	{ vec3(-0.5f,-0.5f,0.f),  255,255,255,255,  vec2(0.0f,0.0f), vec3(0.f,0.f,1.f) }, // bottom left
	{ vec3(-0.5f,0.5f,0.f),  255,255,255,255,  vec2(0.0f,1.0f), vec3(0.f,0.f,1.f) }, // top left
	{ vec3(0.5f,-0.5f,0.f),  255,255,255,255,  vec2(10.0f,0.0f), vec3(0.f,0.f,1.f) }, // bottom right
	{ vec3(0.5f,0.5f,0.f),  255,255,255,255,  vec2(10.0f,1.0f), vec3(0.f,0.f,1.f) }, // top right
};

const std::vector<unsigned int> g_BackgroundIndices =
{
	0, 1, 2, 2, 1, 3,
};

const std::vector<fw::VertexFormat> g_CubeVerts =
{   
    //Side 1
    { vec3(-0.5f,-0.5f,-0.5f),  255,255,255,255,     vec2(0.f,       0.f), vec3(0.f,0.f,-1.f) }, // Point 1
    { vec3(0.5f,0.5f,-0.5f),  255,255,255,255,       vec2(1.f / 6.f, 1.f), vec3(0.f,0.f,-1.f) }, // Point 2
    { vec3(0.5f,-0.5f,-0.5f),  255,255,255,255,      vec2(1.f / 6.f, 0.f), vec3(0.f,0.f,-1.f) }, // Point 3
    { vec3(-0.5f,-0.5f,-0.5f),  255,255,255,255,     vec2(0.f,       0.f), vec3(0.f,0.f,-1.f) }, // Point 4
    { vec3(-0.5f,0.5f,-0.5f),  255,255,255,255,      vec2(0.f,       1.f), vec3(0.f,0.f,-1.f) }, // Point 5
    { vec3(0.5f,0.5f,-0.5f),  255,255,255,255,       vec2(1.f / 6.f, 1.f), vec3(0.f,0.f,-1.f) }, // Point 6
    //Side 2
    { vec3(-0.5f,0.5f,-0.5f),  255,255,255,255,      vec2(1.f / 6.f, 0.f), vec3(0.f,1.f,0.f) }, // Point 1
    { vec3(0.5f,0.5f,0.5f),  255,255,255,255,        vec2(2.f / 6.f, 1.f), vec3(0.f,1.f,0.f) }, // Point 2
    { vec3(0.5f,0.5f,-0.5f),  255,255,255,255,       vec2(2.f / 6.f, 0.f), vec3(0.f,1.f,0.f) }, // Point 3
    { vec3(-0.5f,0.5f,-0.5f),  255,255,255,255,      vec2(1.f / 6.f, 0.f), vec3(0.f,1.f,0.f) }, // Point 4
    { vec3(-0.5f,0.5f,0.5f),  255,255,255,255,       vec2(1.f / 6.f, 1.f), vec3(0.f,1.f,0.f) }, // Point 5
    { vec3(0.5f,0.5f,0.5f),  255,255,255,255,        vec2(2.f / 6.f, 1.f), vec3(0.f,1.f,0.f) }, // Point 6
    //Side 3
    { vec3(-0.5f,0.5f,0.5f),  255,255,255,255,       vec2(3.f / 6.f, 1.f), vec3(0.f,0.f,1.f) }, // Point 1
    { vec3(0.5f,-0.5f,0.5f),  255,255,255,255,       vec2(2.f / 6.f, 0.f), vec3(0.f,0.f,1.f) }, // Point 2
    { vec3(0.5f,0.5f,0.5f),  255,255,255,255,        vec2(2.f / 6.f, 1.f), vec3(0.f,0.f,1.f) }, // Point 3
    { vec3(-0.5f,0.5f,0.5f),  255,255,255,255,       vec2(3.f / 6.f, 1.f), vec3(0.f,0.f,1.f) }, // Point 4
    { vec3(-0.5f,-0.5f,0.5f),  255,255,255,255,      vec2(3.f / 6.f, 0.f), vec3(0.f,0.f,1.f) }, // Point 5
    { vec3(0.5f,-0.5f,0.5f),  255,255,255,255,       vec2(2.f / 6.f, 0.f), vec3(0.f,0.f,1.f) }, // Point 6
    //Side 4
    { vec3(-0.5f,-0.5f,0.5f),  255,255,255,255,      vec2(3.f / 6.f, 0.f), vec3(0.f,-1.f,0.f) }, // Point 1
    { vec3(0.5f,-0.5f,-0.5f),  255,255,255,255,      vec2(4.f / 6.f, 1.f), vec3(0.f,-1.f,0.f) }, // Point 2
    { vec3(0.5f,-0.5f,0.5f),  255,255,255,255,       vec2(4.f / 6.f, 0.f), vec3(0.f,-1.f,0.f) }, // Point 3
    { vec3(-0.5f,-0.5f,0.5f),  255,255,255,255,      vec2(3.f / 6.f, 0.f), vec3(0.f,-1.f,0.f) }, // Point 4
    { vec3(-0.5f,-0.5f,-0.5f),  255,255,255,255,     vec2(3.f / 6.f, 1.f), vec3(0.f,-1.f,0.f) }, // Point 5
    { vec3(0.5f,-0.5f,-0.5f),  255,255,255,255,      vec2(4.f / 6.f, 1.f), vec3(0.f,-1.f,0.f) }, // Point 6
    //Side 5
    { vec3(0.5f,-0.5f,-0.5f),  255,255,255,255,      vec2(4.f / 6.f, 0.f), vec3(1.f,0.f,0.f) }, // Point 1
    { vec3(0.5f,0.5f,0.5f),  255,255,255,255,        vec2(5.f / 6.f, 1.f), vec3(1.f,0.f,0.f) }, // Point 2
    { vec3(0.5f,-0.5f,0.5f),  255,255,255,255,       vec2(5.f / 6.f, 0.f), vec3(1.f,0.f,0.f) }, // Point 3
    { vec3(0.5f,-0.5f,-0.5f),  255,255,255,255,      vec2(4.f / 6.f, 0.f), vec3(1.f,0.f,0.f) }, // Point 4
    { vec3(0.5f,0.5f,-0.5f),  255,255,255,255,       vec2(4.f / 6.f, 1.f), vec3(1.f,0.f,0.f) }, // Point 5
    { vec3(0.5f,0.5f,0.5f),  255,255,255,255,        vec2(5.f / 6.f, 1.f), vec3(1.f,0.f,0.f) }, // Point 6
    //Side 6
    { vec3(-0.5f,-0.5f,0.5f),  255,255,255,255,      vec2(5.f / 6.f, 0.f), vec3(-1.f,0.f,0.f) }, // Point 1
    { vec3(-0.5f,0.5f,-0.5f),  255,255,255,255,      vec2(1.f,       1.f), vec3(-1.f,0.f,0.f) }, // Point 2
    { vec3(-0.5f,-0.5f,-0.5f),  255,255,255,255,     vec2(1.f,       0.f), vec3(-1.f,0.f,0.f) }, // Point 3
    { vec3(-0.5f,-0.5f,0.5f),  255,255,255,255,      vec2(5.f / 6.f, 0.f), vec3(-1.f,0.f,0.f) }, // Point 4
    { vec3(-0.5f,0.5f,0.5f),  255,255,255,255,       vec2(5.f / 6.f, 1.f), vec3(-1.f,0.f,0.f) }, // Point 5
    { vec3(-0.5f,0.5f,-0.5f),  255,255,255,255,      vec2(1.f,       1.f), vec3(-1.f,0.f,0.f) }, // Point 6
};