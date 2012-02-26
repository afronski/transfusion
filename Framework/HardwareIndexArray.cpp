#include "pchFramework.hpp"

#include "../Common/Exception.hpp"
#include "Renderer.hpp"
#include "HardwareArrayManager.hpp"
#include "HardwareIndexArray.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja hardware_index_array.
//--------------------------------------------------------------------------------------------
struct hardware_index_array::_hardware_index_arraypimpl
{
	void* _idx;
	
	utl::uint32_t _idx_count;
	utl::uint32_t _idx_size;			
	
	RW_HW_INDEX_ARRAY_ELEMENT_TYPE _elementtype;
	RW_HW_ARRAY_USAGE _rwhau;

	bool _discard;
	
	GLuint _id;
	 
	_hardware_index_arraypimpl(): _idx(0), _idx_count(0), _idx_size(sizeof(utl::uint16_t)), _discard(false)
	{}
};

hardware_index_array::hardware_index_array(RW_HW_INDEX_ARRAY_ELEMENT_TYPE rwhiaet, utl::uint32_t count, RW_HW_ARRAY_USAGE rwhau, bool discardable): 
	_pimpl(new _hardware_index_arraypimpl())
{
	_pimpl->_idx_size = static_cast<utl::uint32_t>(rwhiaet);
	_pimpl->_elementtype = rwhiaet;
	_pimpl->_idx_count = count;
	_pimpl->_rwhau = rwhau;
	_pimpl->_discard = discardable;

	glGenBuffersARB(1, &(_pimpl->_id));
	if (glIsBufferARB(_pimpl->_id) == GL_FALSE)
		throw err::framework_exception("Cannot generate ID for IBO!");
	
	// Allocate buffer, for specified size but with null data.
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _pimpl->_id);      
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
					_pimpl->_idx_size * _pimpl->_idx_count, 
    				0, 
    				hardware_array_manager::internal_usage(_pimpl->_rwhau));
}

hardware_index_array::~hardware_index_array()
{
	glDeleteBuffersARB(1, &(_pimpl->_id));
}

void hardware_index_array::read(utl::uint32_t offset, utl::uint32_t count, void* data)
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _pimpl->_id);
	glGetBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, count, data);
}

void hardware_index_array::write(utl::uint32_t offset, utl::uint32_t count, const void* data)
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _pimpl->_id);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
					_pimpl->_idx_size * _pimpl->_idx_count, 
					data, 
				    hardware_array_manager::internal_usage(_pimpl->_rwhau));
}

void hardware_index_array::bind_buffer() const
{
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _pimpl->_id);
}

utl::uint32_t hardware_index_array::element_size() const
{
	return _pimpl->_idx_size;
}

utl::uint32_t hardware_index_array::element_count() const
{
	return _pimpl->_idx_count;
}

RW_HW_INTERNAL_ELEMENT_TYPE hardware_index_array::element_type() const
{
	switch(_pimpl->_elementtype)
	{
		case RWHIAET_UINT:		return RWHIET_UINT;			
		case RWHIAET_USHORT:	return RWHIET_USHORT;			
		default:				return RWHIET_UINT;			
	};	
}

void hardware_index_array::discard_array()
{
	if (_pimpl->_discard)
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _pimpl->_id);      
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
						_pimpl->_idx_size * _pimpl->_idx_count, 
    					0, 
    					hardware_array_manager::internal_usage(_pimpl->_rwhau));
    }
}