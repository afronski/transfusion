#pragma warning (disable : 4275)
#pragma warning (disable : 4251)
#pragma warning (disable : 4661)

#include <cstdlib>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <ctime>

#include <algorithm>
#include <limits>
#include <utility>
#include <exception>

#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#include <map>
#include <vector>

#include <boost/smart_ptr.hpp>
#include <boost/format.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef TFNCOMMON_EXPORTS
	#define TFNCOMMON_EXPORT __declspec(dllexport)
#else
	#define TFNCOMMON_EXPORT __declspec(dllimport)
#endif

#include "StdInt.hpp"
#include "MathScalar.hpp"