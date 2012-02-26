/// \file CgContext.cpp
/// \author Wojtek "afronski" Gawroñski
/// \date 03.03.2009
///
/// \brief Implementation of Cg context object.
/// 
/// File contains implementation of cg context class, operations and some usability
/// tricks for better use.

#include "pchFramework.hpp"

#include "CgContext.hpp"
#include "CgProgramManager.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementation cg_context.
//--------------------------------------------------------------------------------------------
struct cg_context::_cg_contextpimpl
{
	CGcontext _cgcont;
};

cg_context::cg_context(): _pimpl(new _cg_contextpimpl())
{
	_pimpl->_cgcont = 0;
}

cg_context::~cg_context()
{
	destroy();
}

void cg_context::create()
{
	_pimpl->_cgcont = cgCreateContext();
	err::cg_exception::cg_check_error();
}

void cg_context::destroy()
{
	if (is_valid())
		cgDestroyContext(_pimpl->_cgcont);
}

bool cg_context::is_valid() const
{
	if (cgIsContext(_pimpl->_cgcont) == CG_TRUE)
		return true;
	else
		return false;
}

CGcontext cg_context::context() const
{
	return _pimpl->_cgcont;
}