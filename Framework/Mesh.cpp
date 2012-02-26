#include "pchFramework.hpp"

#include "../Common/Exception.hpp"
#include "HardwareVertexArray.hpp"
#include "HardwareIndexArray.hpp"
#include "Mesh.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
struct mesh::_meshpimpl
{
	hardware_vertex_array* V;
	hardware_vertex_array* N;
	hardware_vertex_array* T;
	hardware_vertex_array* C;
	
	hardware_index_array* I;
	
	GLenum _drawmode;
	
	utl::uint32_t _v_count;
	utl::uint32_t _c_count;
	utl::uint32_t _t_count;
	utl::uint32_t _n_count;
	utl::uint32_t _i_count;	
	
	RW_HW_ARRAY_ELEMENT_TYPE _v_esize;
	RW_HW_ARRAY_ELEMENT_TYPE _c_esize;
	RW_HW_ARRAY_ELEMENT_TYPE _t_esize;
	RW_HW_ARRAY_ELEMENT_TYPE _n_esize;
	RW_HW_INDEX_ARRAY_ELEMENT_TYPE _i_esize; 
	
	_meshpimpl(): V(0), N(0), T(0), C(0), I(0), _drawmode(GL_TRIANGLES) {}
	
	~_meshpimpl() 
	{
		delete V;
		delete N;
		delete T;
		delete C;
		delete I;
	}
};

mesh::mesh(): _pimpl(new _meshpimpl())
{}

mesh::~mesh()
{}

void mesh::write_colors(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, RW_HW_ARRAY_ELEMENT_SIZE nsize, const void* data)
{
	if (_pimpl->C != 0) delete _pimpl->C;

	_pimpl->_c_count = count;
	_pimpl->_c_esize = esize;
	
	_pimpl->C = new hardware_vertex_array(RWHAT_COLOR, esize, nsize, count);
	_pimpl->C->write(0, _pimpl->_c_count * _pimpl->_c_esize, data);
}

void mesh::write_texcoords(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, RW_HW_ARRAY_ELEMENT_SIZE nsize, const void* data)
{
	if (_pimpl->T != 0) delete _pimpl->T;

	_pimpl->_t_count = count;
	_pimpl->_t_esize = esize;
	
	_pimpl->T = new hardware_vertex_array(RWHAT_TEXCOORD, esize, nsize, count);
	_pimpl->T->write(0, _pimpl->_t_count * _pimpl->_t_esize, data);
}
			
void mesh::write_vertices(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, RW_HW_ARRAY_ELEMENT_SIZE nsize, const void* data)
{
	if (_pimpl->V != 0) delete _pimpl->V;

	_pimpl->_v_count = count;
	_pimpl->_v_esize = esize;
	
	_pimpl->V = new hardware_vertex_array(RWHAT_POSITION, esize, nsize, count);
	_pimpl->V->write(0, _pimpl->_v_count * _pimpl->_v_esize, data);
}

void mesh::write_normals(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, const void* data)
{
	if (_pimpl->N != 0) delete _pimpl->N;

	_pimpl->_n_count = count;
	_pimpl->_n_esize = esize;	

	_pimpl->N = new hardware_vertex_array(RWHAT_NORMAL, esize, RWHAES_3, count);
	_pimpl->N->write(0, _pimpl->_n_count * _pimpl->_n_esize, data);
}

void mesh::write_indices(utl::uint32_t count, RW_HW_INDEX_ARRAY_ELEMENT_TYPE isize, const void* data)
{
	if (_pimpl->I != 0) delete _pimpl->I;

	_pimpl->_i_count = count;
	_pimpl->_i_esize = isize;
	
	_pimpl->I = new hardware_index_array(isize, count);
	_pimpl->I->write(0, _pimpl->_i_count * _pimpl->_i_esize, data);
}
			
utl::uint32_t mesh::vertices_count() const
{
	return _pimpl->_v_count;
}

utl::uint32_t mesh::indices_count() const
{
	return _pimpl->_i_count;
}

utl::uint32_t mesh::normals_count() const
{
	return _pimpl->_n_count;
}

void mesh::draw(const cg_program& prog) const
{
	CGparameter color, tex, normal, pos;
	if (_pimpl->C)
	{
		color = cgGetNamedParameter(prog.program(), "color");
		cgGLEnableClientState(color);				
		_pimpl->C->bind_buffer(); 
		cgGLSetParameterPointer(color, _pimpl->C->element_num(), _pimpl->C->element_type(), 0, 0);		
	}	
	if (_pimpl->T)
	{
		tex = cgGetNamedParameter(prog.program(), "texCoord");
		cgGLEnableClientState(tex);				
		_pimpl->T->bind_buffer();
		cgGLSetParameterPointer(tex, _pimpl->T->element_num(), _pimpl->T->element_type(), 0, 0);
	}	
	if (_pimpl->N)
	{
		normal = cgGetNamedParameter(prog.program(), "normal");
		cgGLEnableClientState(normal);
		_pimpl->N->bind_buffer();
		cgGLSetParameterPointer(normal, 3, _pimpl->N->element_type(), 0, 0);
	}
	if (_pimpl->V)
	{
		pos = cgGetNamedParameter(prog.program(), "position");
		cgGLEnableClientState(pos);
		_pimpl->V->bind_buffer();
		cgGLSetParameterPointer(pos, _pimpl->V->element_num(), _pimpl->V->element_type(), 0, 0);
	
	}
	if (_pimpl->I)
	{
		_pimpl->I->bind_buffer();
		glDrawElements(_pimpl->_drawmode, _pimpl->_i_count, _pimpl->I->element_type(), 0);				
	}
		
	if (_pimpl->C) cgGLDisableClientState(color);
	if (_pimpl->T) cgGLDisableClientState(tex);
	if (_pimpl->N) cgGLDisableClientState(normal);
	if (_pimpl->V) cgGLDisableClientState(pos);
}	
			
void mesh::draw() const
{	
	if (_pimpl->C)
	{
		glEnableClientState(GL_COLOR_ARRAY);				
		_pimpl->C->bind_buffer();
		glColorPointer(_pimpl->C->element_num(), _pimpl->C->element_type(), 0, 0);
	}	
	if (_pimpl->T)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		_pimpl->T->bind_buffer();
		glTexCoordPointer(_pimpl->T->element_num(), _pimpl->T->element_type(), 0, 0);
	}	
	if (_pimpl->N)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		_pimpl->N->bind_buffer();
		glNormalPointer(_pimpl->N->element_type(), 0, 0);
	}
	if (_pimpl->V)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		_pimpl->V->bind_buffer();
		glVertexPointer(_pimpl->V->element_num(), _pimpl->V->element_type(), 0, 0);
	}
	if (_pimpl->I)
	{
		_pimpl->I->bind_buffer();
		glDrawElements(_pimpl->_drawmode, _pimpl->_i_count, _pimpl->I->element_type(), 0);				
	}
		
	if (_pimpl->C) glDisableClientState(GL_COLOR_ARRAY);
	if (_pimpl->T) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (_pimpl->N) glDisableClientState(GL_NORMAL_ARRAY);
	if (_pimpl->V) glDisableClientState(GL_VERTEX_ARRAY);
}

void mesh::set_draw_mode(GLenum mode)
{
	_pimpl->_drawmode = mode;
}