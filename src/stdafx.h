// stdafx.h : include file for standard system include files

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include <cmath>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "format.h"

#define GETTER(Type, X) Type X() const { return _##X; }
#define SETTER(Type, X) void Set##X(const Type &value) { _##X = value; }
#define GETSET(Type, X) GETTER(Type, X) SETTER(Type, X)

#define TIC_DURATION 1.0 / 60