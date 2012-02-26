/// \file HardwareArrayManager.cpp
/// \author Wojtek "afronski" Gawroñski
/// \date 02.03.2009
///
/// \brief Implementation of hardware array manager.
///
/// Specified for OpenGL in version 1.5. Creates, maintans and operates 
/// on hardware vertex/index arrays.

#include "pchFramework.hpp"

#include "HardwareArrayManager.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementation hardware_array_manager.
//--------------------------------------------------------------------------------------------
hardware_array_manager* utl::singleton<hardware_array_manager>::_singleton = 0;

struct hardware_array_manager::_hardware_array_managerpimpl
{
};

hardware_array_manager::hardware_array_manager(): _pimpl(new _hardware_array_managerpimpl())
{}

hardware_array_manager::~hardware_array_manager()
{}

DWORD hardware_array_manager::internal_usage(RW_HW_ARRAY_USAGE hau)
{
	switch(hau)
	{
		case RWHAU_STATIC_ARRAY:	return GL_STATIC_DRAW_ARB;
		case RWHAU_DYNAMIC_ARRAY:	return GL_DYNAMIC_DRAW_ARB;
		case RWHAU_STREAM_ARRAY:	return GL_STREAM_DRAW_ARB;			
		default:					return GL_STATIC_DRAW_ARB;
	};
}