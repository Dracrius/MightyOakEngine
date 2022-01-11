#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Tilemap.h"

const int g_MainMapWidth = 10;
const int g_MainMapHeight = 10;
const TileType g_MainMap[g_MainMapWidth * g_MainMapHeight] =
{
    TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Grass, TT_Grass, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Grass, TT_Grass, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Grass, TT_Grass, TT_Grass, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Grass, TT_Dirt,  TT_Grass, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Empty, TT_Empty, TT_Stone, TT_Grass, TT_Grass, TT_Grass, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Empty, TT_Empty, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone,
    TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone,
    TT_Dirt,  TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone, TT_Stone,
};
