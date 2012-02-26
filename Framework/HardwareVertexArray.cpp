#include "pchFramework.hpp"

#include "../Common/Exception.hpp"
#include "Renderer.hpp"
#include "HardwareArrayManager.hpp"
#include "HardwareVertexArray.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja hardware_vertex_array.
//--------------------------------------------------------------------------------------------
struct hardware_vertex_array::_hardware_vertex_arraypimpl
{
	void* _vertices;
	utl::uint32_t _vertices_count;
	utl::uint32_t _vertex_size;
			
	RW_HW_ARRAY_TYPE _type;
	RW_HW_ARRAY_ELEMENT_TYPE _elementtype;
	RW_HW_ARRAY_ELEMENT_SIZE _elementsize;
	RW_HW_ARRAY_USAGE _rwhau;
	
	bool _discard;
	
	GLuint _id;
	
	_hardware_vertex_arraypimpl(): _vertices(0), _vertices_count(0), _vertex_size(3), _discard(false)
	{}
};

hardware_vertex_array::hardware_vertex_array(RW_HW_ARRAY_TYPE rwhat, 
											 RW_HW_ARRAY_ELEMENT_TYPE rwhaet, 
											 RW_HW_ARRAY_ELEMENT_SIZE rwhaes,
											 utl::uint32_t count, 
											 RW_HW_ARRAY_USAGE rwhau,
											 bool discardable): 
	_pimpl(new _hardware_vertex_arraypimpl())
{
	_pimpl->_vertex_size = static_cast<utl::uint32_t>(rwhaet) * static_cast<utl::uint32_t>(rwhaes);
	_pimpl->_vertices_count = count;
	_pimpl->_elementtype = rwhaet;
	_pimpl->_elementsize = rwhaes;
	_pimpl->_type = rwhat;
	_pimpl->_rwhau = rwhau;
	_pimpl->_discard = discardable;

	glGenBuffersARB(1, &(_pimpl->_id));
	if (glIsBufferARB(_pimpl->_id) == GL_FALSE)
		throw err::framework_exception("Cannot generate ID for VBO!");
	
	// Allocate buffer, for specified size but with null data.
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _pimpl->_id);      
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
					_pimpl->_vertex_size * _pimpl->_vertices_count, 
				    0, 
				    hardware_array_manager::internal_usage(_pimpl->_rwhau));
}

hardware_vertex_array::~hardware_vertex_array()
{
	glDeleteBuffersARB(1, &(_pimpl->_id));
}

void hardware_vertex_array::read(utl::uint32_t offset, utl::uint32_t count, void* data)
{	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _pimpl->_id);
	glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, count, data);
}

void hardware_vertex_array::write(utl::uint32_t offset, utl::uint32_t count, const void* data)
{
	if ((offset != 0) && (count != _pimpl->_vertex_size * _pimpl->_vertices_count))
	{
		discard_array();	
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _pimpl->_id);	
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, count, data); 	
	}
	else
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _pimpl->_id);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
						_pimpl->_vertex_size * _pimpl->_vertices_count, 
						data, 
						hardware_array_manager::internal_usage(_pimpl->_rwhau));
	}
}

void hardware_vertex_array::bind_buffer() const
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _pimpl->_id);
}

RW_HW_ARRAY_TYPE hardware_vertex_array::type() const
{
	return _pimpl->_type;
}

utl::uint32_t hardware_vertex_array::element_size() const
{
	return _pimpl->_vertex_size;
}

utl::uint32_t hardware_vertex_array::element_count() const
{
	return _pimpl->_vertices_count;
}

utl::uint32_t hardware_vertex_array::element_num() const
{
	return static_cast<utl::uint32_t>(_pimpl->_elementsize);
}

RW_HW_INTERNAL_ELEMENT_TYPE hardware_vertex_array::element_type() const
{
	switch(_pimpl->_elementtype)
	{	
		case RWHAET_FLOAT:	return RWHIET_FLOAT;			
		case RWHAET_DOUBLE:	return RWHIET_DOUBLE;
		case RWHAET_UBYTE:	return RWHIET_UBYTE;		
		default:			return RWHIET_FLOAT;
	}
}

void hardware_vertex_array::discard_array()
{
	if (_pimpl->_discard)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _pimpl->_id);      
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
						_pimpl->_vertex_size * _pimpl->_vertices_count, 
					    0, 
						hardware_array_manager::internal_usage(_pimpl->_rwhau));
    }
}