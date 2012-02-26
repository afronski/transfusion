#include "pchFramework.hpp"

#include "WindowClass.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
window_class::window_class()
{
	memset(&_wndclass, 0, sizeof(WNDCLASSEXA));
}

bool window_class::register_class(WNDPROC wndproc,
								  HINSTANCE hInst,
								  utl::uint32_t RGBbackground,
								  const std::string& classname,
							      DWORD addstyle)
{	
	LOGBRUSH logbrush = { BS_SOLID, RGBbackground, 0 };
	_classname = classname;
	
	_hInst = hInst;
	_wndclass.cbSize = sizeof(WNDCLASSEX);
	_wndclass.style = CS_HREDRAW | CS_VREDRAW | addstyle;
	_wndclass.hInstance = hInst;
	_wndclass.lpszClassName = _classname.c_str();
	_wndclass.hCursor = static_cast<HCURSOR>(LoadImage(hInst, IDC_ARROW, IMAGE_CURSOR, 0, 0, 0));
	_wndclass.hIcon = static_cast<HICON>(LoadImage(hInst, IDI_APPLICATION, IMAGE_ICON, 0, 0, 0));
	_wndclass.hbrBackground = CreateBrushIndirect(&logbrush);
	_wndclass.lpfnWndProc = wndproc;
	_wndclass.lpszMenuName = 0;
	_wndclass.cbClsExtra = 0;
	_wndclass.cbWndExtra = 0;
	_wndclass.hIconSm = 0;
	
	return (RegisterClassExA(&_wndclass) != 0);
}

bool window_class::unregister_class()
{
	return (UnregisterClassA(_classname.c_str(), _hInst) != 0);
}