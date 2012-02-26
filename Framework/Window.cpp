#include "pchFramework.hpp"

#include "RendererContext.hpp"
#include "Renderer.hpp"
#include "WindowClass.hpp"
#include "Window.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Wyj¹tki.
//--------------------------------------------------------------------------------------------
err::window_exception::window_exception(const std::string &message):
	err::framework_exception(message), _err_nr(GetLastError())
{}

const char* err::window_exception::what() const
{
	if (_cause.empty()) // Leniwa inicjalizacja ³añcucha z b³êdem.
	{
		try
		{
			_cause = this->framework_exception::what();			
			if (!_cause.empty()) _cause += ": ";
			
			// Kod b³êdu Win32 API
			LPVOID lpMsgBuf;			
		    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						  FORMAT_MESSAGE_FROM_SYSTEM |
						  FORMAT_MESSAGE_IGNORE_INSERTS,
						  NULL,
						  _err_nr,
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					      (LPTSTR) &lpMsgBuf,
				          0, NULL );

			_cause += static_cast<char*>(lpMsgBuf);

			LocalFree(lpMsgBuf);			 
		}
		catch(...) { return framework_exception::what(); }
	}
	return _cause.c_str();
}

//--------------------------------------------------------------------------------------------
// Implementacja klasy okna - private.
//--------------------------------------------------------------------------------------------
struct window::_windowpimpl
{
	HINSTANCE	_instance;
	HWND		_handle;
	
	renderer_context _context;
	window_class _wndclass;	

	std::string _title;
	std::string _nclass;
	
	bool _appactive;
	
	bool Keys[256];
	
	_windowpimpl(): _instance(0), _handle(0), _appactive(true) {}
};

LRESULT CALLBACK window::wnd_proc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam)
{
	window* window_ptr = reinterpret_cast<window*>(GetWindowLongPtrA(hwnd, GWL_USERDATA));
	if (window_ptr)
	{
		window::MESSAGE_RESULT res_pair = window_ptr->message_proc(hwnd, message, wparam, lparam);
		if (res_pair.first)
			return 0;
		else
		{
			if (res_pair.second != 0)
				return res_pair.second;
			else
				return DefWindowProc(hwnd, message, wparam, lparam);
		}
	}
	else
		return DefWindowProc(hwnd, message, wparam, lparam);
}

//--------------------------------------------------------------------------------------------
// Implementacja klasy okna - public.
//--------------------------------------------------------------------------------------------
window::window(const std::string &title, const std::string& nclass): _pimpl(new _windowpimpl())
{
	_pimpl->_title = title;
	_pimpl->_nclass = nclass;
	memset(_pimpl->Keys, false, sizeof(bool) * 256);
}

window::~window()
{}

window::MESSAGE_RESULT window::message_proc(HWND hwnd, utl::uint32_t message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
	{
		case WM_ACTIVATEAPP: 
			if (wparam == TRUE)
				_pimpl->_appactive = true;
			else
				_pimpl->_appactive = false;
			return std::make_pair(true, 0);
	
		case WM_DESTROY:
			destroy();
			PostQuitMessage(0);
			return std::make_pair(false, 0);
		
		case WM_SIZE:	
			resize(LOWORD(lparam), HIWORD(lparam));		
			return std::make_pair(true, 0);
		
		case WM_NCHITTEST:
			if (RENDERER.get_fullscreen()) return std::make_pair(false, HTCLIENT);
			return std::make_pair(false, 0);
		
		case WM_POWERBROADCAST:
			if (wparam == PBT_APMQUERYSUSPEND) return std::make_pair(false, BROADCAST_QUERY_DENY);
			return std::make_pair(false, 0);		
		
		case WM_SYSCOMMAND:				
			if (RENDERER.get_fullscreen()) 
			{
				switch(wparam)
				{
					case SC_MOVE:
					case SC_SIZE:
					case SC_MAXIMIZE:
					case SC_KEYMENU:
					case SC_MONITORPOWER:
					case SC_SCREENSAVE:
						return std::make_pair(false, 1);
				}
			}
			return std::make_pair(false, 0);			
		
		case WM_KEYDOWN:
			_pimpl->Keys[wparam] = true;
			return std::make_pair(true, 0);

		case WM_KEYUP:
			_pimpl->Keys[wparam] = false;
			return std::make_pair(true, 0);	
		
		default:	
			return std::make_pair(false, 0);
	}	
}

void window::resize(utl::int32_t width, utl::int32_t height)
{
	if (height == 0) height = 1;

	RENDERER.set_viewport(0, 0, width, height);	
	RENDERER.set_perspective(math::scalar(45.0), 
							 static_cast<math::scalar>(width) / static_cast<math::scalar>(height), 
							 math::scalar(1.0), math::scalar(1000.0));	
}

void window::init()
{			
	RENDERER.shade_model(RWSM_SMOOTH);			
	RENDERER.set_clear_color();
				 
	user_init();
}

void window::create(HINSTANCE hInst)
{				
	renderer::settings currsettings = 
	{
		1024, 768, 32, 60,
		24, 0,
		false,
		true,
		math::color4(0.0f, 0.0f, 1.0f, 0.5f),
		renderer::settings::EX_STYLE_WINDOWED, 
		renderer::settings::STYLE_WINDOWED
	};		

	_pimpl->_instance = hInst;	
	if (_pimpl->_nclass == "") _pimpl->_nclass = "TransfusionWindowClass";
	if (!_pimpl->_wndclass.register_class(&window::wnd_proc, 
										  hInst, 
			  							  RGB(currsettings.ClearColor.r * 255,
											  currsettings.ClearColor.g * 255,
											  currsettings.ClearColor.b * 255),
										  _pimpl->_nclass, 
										  0))
		throw err::window_exception("Window class cannot be registered!");
	
	RENDERER.adjust_window();
		
	_pimpl->_handle = CreateWindowExA(currsettings.ExWindowStyle, 
									  _pimpl->_wndclass.classname().c_str(), 
									  _pimpl->_title.c_str(), 
									  currsettings.WindowStyle,									  
									  (GetSystemMetrics(SM_CXSCREEN) - (currsettings.Width)) / 2, 
									  (GetSystemMetrics(SM_CYSCREEN) - (currsettings.Height)) / 2, 									  
									  currsettings.Width, 
									  currsettings.Height,
									  0, 0, 
									  _pimpl->_instance, 0);
		
	if (!_pimpl->_handle)
		throw err::window_exception("Window cannot be created!");		
	SetWindowLongPtrA(_pimpl->_handle, GWL_USERDATA, reinterpret_cast<LONG>(this));
		
	ShowWindow(_pimpl->_handle, SW_SHOW);
	UpdateWindow(_pimpl->_handle);
	
	_pimpl->_context.create(_pimpl->_handle, currsettings.DepthBits, currsettings.StencilBits);
	_pimpl->_context.activate();
	
	// Inicjalizacja renderera.
	RENDERER.init();		
	RENDERER.change_settings(currsettings, window_handle());	
	
	init();		
	resize(RENDERER.get_width(), RENDERER.get_height());
}

void window::destroy()
{
	user_destroy();
	
	_pimpl->_context.deactivate();
	_pimpl->_context.destroy(_pimpl->_handle);
}

utl::int32_t window::run()
{
	MSG msg = {0};
	msg.message = WM_NULL;
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{			
			TranslateMessage(&msg);
			DispatchMessage(&msg);			
		}
		else
		{
			if (_pimpl->_appactive)
			{
				update(math::ZERO);
				draw();						
				_pimpl->_context.swap_buffers();
			}
			else
				WaitMessage();
		}
	}
	
	if (!_pimpl->_wndclass.unregister_class())
		throw err::window_exception("Cannot unregister window class.");
	return static_cast<utl::int32_t>(msg.wParam);
}

HWND window::window_handle() const
{
	return _pimpl->_handle;
}

bool window::key_state(utl::uint32_t key) const
{
	return _pimpl->Keys[key];
}