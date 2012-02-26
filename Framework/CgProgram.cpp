/// \file CgProgram.cpp
/// \author Wojtek "afronski" Gawroñski
/// \date 03.03.2009
///
/// \brief Implementation of Cg program.
/// 
/// File contains implementation of cg program, operations and some usability
/// tricks for better use.

#include "pchFramework.hpp"

#include "CgProgram.hpp"
#include "CgProgramManager.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementation.
//--------------------------------------------------------------------------------------------
cg_program::cg_program()
{
	_profile = CG_PROFILE_UNKNOWN;
	_program = 0;
}

cg_program::~cg_program()
{
	this->destroy();
}
			
void cg_program::load_from_file(const std::string& filename, CGcontext context, RW_CG_PROGRAM rwcp,
								RW_CG_PROGRAM_TYPE rwcpt, const std::string& entry, const std::string& args)
{
	_filename = filename;
	_rwcpt = rwcpt;
	_type = rwcp;	
	
	char* str = 0;
	if (!args.empty())		
	{
		str = new char [ args.size() ];
		strncpy_s(str, args.size() * sizeof(char), args.c_str(), args.size() * sizeof(char));
	}	
			
	_program = cgCreateProgramFromFile(context, static_cast<CGenum>(rwcpt), filename.c_str(), _profile, entry.c_str(), const_cast<const char**>(&str));
	err::cg_exception::cg_check_error();		
	cgGLLoadProgram(_program);		
	err::cg_exception::cg_check_error();	
	
	if (!args.size()) delete [] str;
}

void cg_program::destroy()
{
	cgDestroyProgram(_program);
}
			
void cg_program::compile_program() const
{
	cgCompileProgram(_program);
	err::cg_exception::cg_check_error();
}

bool cg_program::is_program_compiled() const
{
	if (cgIsProgramCompiled(_program) == CG_TRUE)
		return true;
	else	
		return false;
}
			
std::string cg_program::get_listing(CGcontext context) const
{
	return std::string(cgGetLastListing(context));
}

void cg_program::set_parameter(const std::string& paramname, const math::vec4& values)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetParameter4fv(param, values.v);
}

void cg_program::set_parameter(const std::string& paramname, const math::vec3& values)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetParameter3fv(param, values.v);
}

void cg_program::set_parameter(const std::string& paramname, const math::color4& values)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetParameter4fv(param, values.C);
}

void cg_program::set_parameter(const std::string& paramname, const math::color3& values)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetParameter3fv(param, values.C);
}

void cg_program::set_parameter(const std::string& paramname, const math::scalar& value)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetParameter1fv(param, &value);
}

void cg_program::set_parameter(const std::string& paramname, const math::mat4& values)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetMatrixParameterfc(param, values.M);
}

void cg_program::set_texture(const std::string& paramname, utl::uint32_t id)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetTextureParameter(param, id);
}

void cg_program::set_renderer_matrix(const std::string& paramname)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetStateMatrixParameter(param, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void cg_program::set_renderer_transposed_matrix(const std::string& paramname)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetStateMatrixParameter(param, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_TRANSPOSE);
}

void cg_program::set_world_matrix(const std::string& paramname)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetStateMatrixParameter(param, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void cg_program::set_world_transposed_matrix(const std::string& paramname)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetStateMatrixParameter(param, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_TRANSPOSE);
}

void cg_program::set_world_inverted_transposed_matrix(const std::string& paramname)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetStateMatrixParameter(param, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);
}

void cg_program::set_world_inverted_matrix(const std::string& paramname)
{
	CGparameter param = cgGetNamedParameter(_program, paramname.c_str());
	cgGLSetStateMatrixParameter(param, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE);
}

void cg_program::enable_profile() const
{
	cgGLEnableProfile(_profile);
}

void cg_program::disable_profile() const
{
	cgGLDisableProfile(_profile);
}
			
void cg_program::bind_program() const
{
	cgGLBindProgram(_program);
}