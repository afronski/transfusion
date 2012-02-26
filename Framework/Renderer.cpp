#include "pchFramework.hpp"

#include "Renderer.hpp"
#include <gl/wglew.h>

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Wyj¹tki.
//--------------------------------------------------------------------------------------------
err::renderer_exception::renderer_exception(const std::string &message):
	err::framework_exception(message)
{}

const char* err::renderer_exception::what() const
{
	if (_cause.empty()) // Leniwa inicjalizacja ³añcucha z b³êdem.
	{
		try
		{
			_cause = this->framework_exception::what();												
		}
		catch(...) { return framework_exception::what(); }
	}
	return _cause.c_str();
}

//--------------------------------------------------------------------------------------------
// Singleton - deklaracja pola static.
//--------------------------------------------------------------------------------------------
renderer* utl::singleton<renderer>::_singleton = 0;

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
renderer::renderer()
{}

renderer::~renderer()
{}

void renderer::init()
{
	if (glewInit() != GLEW_OK) throw err::framework_exception("Cannot initialize GLEW!");
	if (!GLEW_VERSION_1_5) throw err::framework_exception("Version OpenGL less than 1.5!");	
	if (!wglewIsSupported("WGL_EXT_swap_control")) throw err::framework_exception("WGL_EXT_swap_control extension not present.");
	
	// Internal renderer settings.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
		
void renderer::change_settings(const settings& sett, HWND hwnd)
{
	_actualsettings = sett;
	
	if (_actualsettings.Fullscreen)
	{
		_actualsettings.ExWindowStyle = renderer::settings::EX_STYLE_FULLSCREEN;
		_actualsettings.WindowStyle = renderer::settings::STYLE_FULLSCREEN;
		
		LONG_PTR exstyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
		LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
				
		style &= ~renderer::settings::STYLE_WINDOWED;
		style |= renderer::settings::STYLE_FULLSCREEN;
		
		exstyle &= ~renderer::settings::EX_STYLE_WINDOWED;
		exstyle |= renderer::settings::EX_STYLE_FULLSCREEN;
	
		SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle);
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		
		DEVMODE dms = {0};
		memset(&dms, 0, sizeof(DEVMODE));
		dms.dmSize = sizeof(DEVMODE);
		dms.dmPelsWidth = _actualsettings.Width;
		dms.dmPelsHeight = _actualsettings.Height;
		dms.dmBitsPerPel = _actualsettings.BPP;
		dms.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;
		
		if (ChangeDisplaySettings(&dms, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			throw err::renderer_exception("Cannot switch display settings!");				
		
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
					 SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		
		while(ShowCursor(false) >= 0);		
	}
	else
	{	
		_actualsettings.ExWindowStyle = renderer::settings::EX_STYLE_WINDOWED;
		_actualsettings.WindowStyle = renderer::settings::STYLE_WINDOWED;
	
		ChangeDisplaySettings(0, 0);
	
		LONG_PTR exstyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
		LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
				
		style &= ~renderer::settings::STYLE_FULLSCREEN;
		style |= renderer::settings::STYLE_WINDOWED;
		
		exstyle &= ~renderer::settings::EX_STYLE_FULLSCREEN;
		exstyle |= renderer::settings::EX_STYLE_WINDOWED;		
	
		SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle);
		SetWindowLongPtr(hwnd, GWL_STYLE, style);
		
		SetWindowPos(hwnd, HWND_NOTOPMOST, 
					 (GetSystemMetrics(SM_CXSCREEN) - _actualsettings.Width) / 2, 
					 (GetSystemMetrics(SM_CYSCREEN) - _actualsettings.Height) / 2,
					 _actualsettings.Width, _actualsettings.Height, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
					 
		while(ShowCursor(true) <= 0);
	}	
	
	set_clear_color();
	set_vsync();
}

void renderer::adjust_window()
{
	RECT wndrect = { 0, 0, _actualsettings.Width, _actualsettings.Height };
	AdjustWindowRectEx(&wndrect, _actualsettings.WindowStyle, false, _actualsettings.ExWindowStyle);
}

std::string renderer::version() const
{	
	std::string ret;
	ret += reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	ret += " ";
	ret += reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	ret += " OpenGL ";
	ret += reinterpret_cast<const char*>(glGetString(GL_VERSION));
	return ret;
}

bool renderer::check_extension(const std::string& ext) const
{
	if (!glewIsSupported(ext.c_str()))
		return false;		
	else
		return true;
}

void renderer::set_vsync() const
{
	if (_actualsettings.VSync)		
		wglSwapIntervalEXT(1);
	else
		wglSwapIntervalEXT(0);
}

void renderer::set_clear_color() const
{
	glClearColor(_actualsettings.ClearColor.r,
				 _actualsettings.ClearColor.g,
				 _actualsettings.ClearColor.b,
				 _actualsettings.ClearColor.a);	
}

void renderer::set_ortho() const
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();	
	gluOrtho2D(-(static_cast<GLdouble>(_actualsettings.Width)/2.0), static_cast<GLdouble>(_actualsettings.Width)/2.0, 
			   -(static_cast<GLdouble>(_actualsettings.Height)/2.0), static_cast<GLdouble>(_actualsettings.Height)/2.0);
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();
	glLoadIdentity();
}

void renderer::return_from_ortho() const
{	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);	
	glPopMatrix();	
	glMatrixMode(GL_MODELVIEW);		
}

void renderer::set_perspective(math::scalar fov, math::scalar aspect, math::scalar nearf, math::scalar farf) const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearf, farf);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}
			
void renderer::polygon_draw_mode(RW_POLYGON_SIDE eglps, RW_POLYGON_DRAW_MODE eglpdm) const
{
	glPolygonMode(eglps, eglpdm);
}

void renderer::alpha_function(RW_ALPHA_FUNCTION eglaf, math::scalar val) const
{
	glAlphaFunc(eglaf, val);
}

void renderer::enable(RW_STATES egls) const
{
	glEnable(egls);
}

void renderer::disable(RW_STATES egls) const
{
	glDisable(egls);
}
			
void renderer::push_all_states() const
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

void renderer::pop_all_states() const
{
	glPopAttrib();
}

void renderer::bind_texture(RW_TEXTURE_TARGET egltt, utl::uint32_t id) const
{
	glBindTexture(egltt, id);
}

void renderer::set_viewport(utl::uint32_t x1, utl::uint32_t y1, utl::uint32_t x2, utl::uint32_t y2) const
{
	glViewport(x1, y1, x2, y2);
}

void renderer::matrix_mode(RW_MATRIX_MODE eglmm) const
{
	glMatrixMode(eglmm);
}

void renderer::identity_matrix() const
{
	glLoadIdentity();
}

void renderer::shade_model(RW_SMOOTH_MODE eglsm) const
{
	glShadeModel(eglsm);
}