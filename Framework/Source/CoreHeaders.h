#pragma once

// This file includes many static headers and must be included by all files
//   inside this Framework project.
// Any changes to any of these headers will cause a full rebuild of the
//   Framework project, which isn't desirable.

// Has to be included before Windows.h otherwise has issues with min/max defines.
#include "../Libraries/pcg-cpp/include/pcg_random.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <vector>
#include <queue>
#include <string>

#include <GL/GL.h>
#include "GL/glext.h"
#include "GL/wglext.h"

#include "GL/GLExtensions.h"
#include "GL/WGLExtensions.h"
