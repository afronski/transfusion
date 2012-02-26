/// \file CgProgramManager.cpp
/// \author Wojtek "afronski" Gawroñski
/// \date 03.03.2009
///
/// \brief Implementation of Cg program manager.
/// 
/// File contains implementation singleton of Cg program manager.

#include "pchFramework.hpp"

#include "CgContext.hpp"
#include "CgProgramManager.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Exceptions.
//--------------------------------------------------------------------------------------------
err::cg_exception::cg_exception(const std::string &message, CGerror err):
	err::framework_exception(message), _err(err)
{}

const char* err::cg_exception::what() const
{
	if (_cause.empty()) // Lazy initialization message string.
	{
		try
		{
			_cause = this->framework_exception::what();			
			if (!_cause.empty()) _cause += ": ";
			
			_cause += cgGetErrorString(_err);		 
		}
		catch(...) { return framework_exception::what(); }
	}
	return _cause.c_str();
}

void err::cg_exception::cg_check_error()
{
	CGerror err = cgGetError();
	if (err != CG_NO_ERROR) throw err::cg_exception("Internal Cg Exception", err);
}

//--------------------------------------------------------------------------------------------
// Implementation.
//--------------------------------------------------------------------------------------------
cg_program_manager* utl::singleton<cg_program_manager>::_singleton = 0;

struct cg_program_manager::_cg_program_managerpimpl
{
	typedef std::map<std::string, boost::shared_ptr<cg_program> >	program_map;
	typedef program_map::iterator									program_map_it;
	typedef std::pair<std::string, boost::shared_ptr<cg_program> >	program_pair;
	
	program_map _programs;
	cg_context _context;
};

cg_program_manager::cg_program_manager(): _pimpl(new _cg_program_managerpimpl())
{}

cg_program_manager::~cg_program_manager()
{
	this->deinitialise();
}

void cg_program_manager::initialise()
{
	_pimpl->_context.create();		
}

void cg_program_manager::deinitialise()
{
	_pimpl->_context.destroy();
}

void cg_program_manager::open_program(const std::string& filename, const std::string& name, 
									  RW_CG_PROGRAM rwcp,
									  const std::string& entry, const std::string& args)
{
	boost::shared_ptr<cg_program> temp(new cg_program());
	CGprofile prof = cgGLGetLatestProfile(static_cast<CGGLenum>(rwcp));		
	cgGLSetOptimalOptions(prof);
	
	temp->set_profile(prof);
	temp->load_from_file(filename, _pimpl->_context.context(), rwcp, RWCPT_SOURCE, entry, args);
	
	if (_pimpl->_programs.insert(std::make_pair(name, temp)).second == false)
		throw err::cg_exception(std::string("Cannot insert cg program: ") + name + std::string(" "), CG_NO_ERROR);
}

void cg_program_manager::close_program(const std::string& name)
{
	_cg_program_managerpimpl::program_map_it temp = _pimpl->_programs.find(name);
	if (temp != _pimpl->_programs.end())
	{
		temp->second->destroy();
		_pimpl->_programs.erase(temp);
	}
	else
		throw err::cg_exception(std::string("Cannot find and erase cg program: ") + name + std::string(" "), CG_NO_ERROR);		
}
	
cg_program& cg_program_manager::operator [] (const std::string& name)
{
	_cg_program_managerpimpl::program_map_it ret = _pimpl->_programs.find(name);
	if (ret != _pimpl->_programs.end())
		return (*(ret->second.get()));
	else
		throw err::cg_exception(std::string("Cannot find and operate cg program: ") + name + std::string(" "), CG_NO_ERROR);
}