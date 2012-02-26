#include "pchFramework.hpp"

#include "Window.hpp"
#include "RendererContext.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja struktury wewnêtrznej - _contextpimp.
//--------------------------------------------------------------------------------------------
struct renderer_context::_contextpimpl 
{
	HDC			_dc;
	HGLRC		_glrc;	
	
	_contextpimpl(): _dc(0), _glrc(0) {}
	
	void reset() 
	{	_dc = 0; _glrc = 0;	}
};

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
renderer_context::renderer_context(): _pimpl(new _contextpimpl())
{}

renderer_context::~renderer_context()
{}

void renderer_context::activate()
{
	wglMakeCurrent(_pimpl->_dc, _pimpl->_glrc);	
}

void renderer_context::deactivate()
{
	wglMakeCurrent(_pimpl->_dc, 0);	
}

void renderer_context::create(HWND hwnd, utl::uint8_t depthbits, utl::uint8_t stencilbits)
{
	if (hwnd != 0)
	{
		_pimpl->_dc = GetDC(hwnd);
		
		// Building PixelFormat.
		utl::int32_t PixFormat = 0;
		static PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 
			32,
			0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 
			depthbits,			// Depth bits.
			stencilbits,		// Stencil bits.
			0,
			PFD_MAIN_PLANE,
			0, 0, 0, 0
		};		
		
		PixFormat = ChoosePixelFormat(_pimpl->_dc, &pfd);
		if (PixFormat == 0) 
			throw err::window_exception("Cannot choose pixel format.");
			
		SetPixelFormat(_pimpl->_dc, PixFormat, &pfd);		
				
		// Create HGLRC.
		_pimpl->_glrc = wglCreateContext(_pimpl->_dc);
		if (!_pimpl->_glrc)
			throw err::window_exception("Cannot create HGLRC.");
	}
}

void renderer_context::destroy(HWND hwnd)
{
	if (hwnd != 0)
	{
		wglDeleteContext(_pimpl->_glrc);
		ReleaseDC(hwnd, _pimpl->_dc);
		_pimpl->reset();
	}
}

void renderer_context::swap_buffers()
{
	SwapBuffers(_pimpl->_dc);
}