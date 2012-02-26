#pragma warning (disable : 4275)
#pragma warning (disable : 4251)
#pragma warning (disable : 4661)

#include <cmath>
#include <cassert>

#include <utility>

#include <sstream>
#include <fstream>
#include <string>

#include <vector>
#include <map>

#include <boost/smart_ptr.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/glew.h>

#include <CG/cg.h>
#include <CG/cgGL.h>

#include <freeimage.h>

#ifdef TFNFRAMEWORK_EXPORTS
	#define TFNFRAMEWORK_EXPORT __declspec(dllexport)
#else
	#define TFNFRAMEWORK_EXPORT __declspec(dllimport)
#endif

#include "RendererWrapper.hpp"

#include "../Common/pchCommon.hpp"
#include "../Common/MathScalar.hpp"