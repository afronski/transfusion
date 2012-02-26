#include "pchFramework.hpp"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include "Renderer.hpp"

#include "HardwareVertexArray.hpp"
#include "HardwareIndexArray.hpp"

#include "FontManager.hpp"

#include "Texture.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Wyj¹tki.
//--------------------------------------------------------------------------------------------
err::font_manager_exception::font_manager_exception(const std::string &message):
	err::framework_exception(message)
{}

const char* err::font_manager_exception::what() const
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
// Implementacja Font.
//--------------------------------------------------------------------------------------------
struct font::coord
{	
	math::scalar x1;
	math::scalar y1;
	math::scalar x2;	
	utl::uint32_t width;
	
	coord(math::scalar a = math::ZERO, math::scalar b = math::ZERO, math::scalar c = math::ZERO, utl::uint32_t w = 0): 
		x1(a), y1(b), x2(c), width(w)
	{}
};

struct font::_fontpimpl
{
	FT_Library* ptrlib;
	FT_Face _face;
	tex2D _texture;	
			
	std::map<char, font::coord> _coords;
	
	hardware_vertex_array *vertices;
	hardware_vertex_array *uvs;
	hardware_vertex_array *colors;
	hardware_index_array *indices;
	
	std::string _name;
			
	utl::uint32_t _width;
	utl::uint32_t _height;
	
	math::scalar _lineheight;
	utl::uint32_t _pixlineheight;
			
	utl::uint32_t _size;		
};			

font::font(void* ft): _pimpl(new _fontpimpl())
{
	_pimpl->ptrlib = reinterpret_cast<FT_Library*>(ft);
}

font::~font()
{
	this->destroy();
}

void font::create(const std::string& name, utl::uint32_t size)
{
	// Czyszczenie poprzedniej textury.
	this->destroy();
	
	_pimpl->_size = size;
	_pimpl->_name = name;

	// Znaki które bêd¹ renderowane	i pomocnicze typy.
	const size_t margin = 3;
	utl::int32_t image_height = 0, image_width = 256;
	const std::string chars("abcdefghijklmnopqrstuvwxyz"
							"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							"1234567890~!@#$%^&*()-=+;:"
							"'\",./?[]|\\ <>`\xFF");
								
	// £adowanie nowego fontu.
	if (FT_New_Face((*(_pimpl->ptrlib)), name.c_str(), 0, &_pimpl->_face))
		throw err::font_manager_exception("Cannot create font face.");
	
	// Sprawdzenie czy font jest true-type, i ustawienie wielkoœci.
	if (!(_pimpl->_face->face_flags & FT_FACE_FLAG_SCALABLE) ||
		!(_pimpl->_face->face_flags & FT_FACE_FLAG_HORIZONTAL))
		throw err::font_manager_exception("Error with sizing font face.");	
	FT_Set_Pixel_Sizes(_pimpl->_face, _pimpl->_size, 0);
	
	// Obliczenie maksymalnych ogonków i g³ówek (ascent and descent).
	utl::int32_t max_descent = 0, max_ascent = 0;
	utl::int32_t space_on_line = image_width - margin, lines = 1;
	utl::int32_t idx = 0, advance = 0;
	for(utl::uint32_t i = 0; i < chars.size(); ++i)
	{
		idx = FT_Get_Char_Index(_pimpl->_face, static_cast<utl::uint32_t>(chars[i]));
		if (chars[i] == '\xFF') idx = 0;		// Nieznany znak.
		
		// Rendering aktualnego glifu.
		FT_Load_Glyph(_pimpl->_face, idx, FT_LOAD_DEFAULT);
	    FT_Render_Glyph(_pimpl->_face->glyph, FT_RENDER_MODE_NORMAL);
		
	    advance = (_pimpl->_face->glyph->metrics.horiAdvance >> 6) + margin;
	    
		// Jeœli linia jest pe³na, idŸ do nastêpnej...
		if (advance > space_on_line)
		{
			space_on_line = image_width - margin;
			++lines;
		}		

	    space_on_line -= advance;
 
		max_ascent = max(_pimpl->_face->glyph->bitmap_top, max_ascent);
		max_descent = max(_pimpl->_face->glyph->bitmap.rows - _pimpl->_face->glyph->bitmap_top, max_descent);		
	}
	
	// Obliczenie jak wysok¹ texturê potrzeba.	
	image_height = math::next_pow_2((max_ascent + max_descent + margin) * lines + margin);

	utl::uint8_t* data = new utl::uint8_t[image_height * image_width];
	memset(data, 0, image_height * image_width * sizeof(utl::uint8_t));
	
	// WskaŸniki, gdzie renderowaæ glif
	utl::int32_t x = margin, y = margin + max_ascent;
 
	for(utl::uint32_t i = 0; i < chars.size(); ++i)
	{
		idx = FT_Get_Char_Index(_pimpl->_face, static_cast<utl::uint32_t>(chars[i]));
		if (chars[i] == '\xFF') idx = 0;
 
		// Rysuj glif.
		FT_Load_Glyph(_pimpl->_face, idx, FT_LOAD_DEFAULT);
		FT_Render_Glyph(_pimpl->_face->glyph, FT_RENDER_MODE_NORMAL);
 
		// Obliczamy pozycjê renederowanego znaku
		advance = (_pimpl->_face->glyph->metrics.horiAdvance >> 6) + margin;
		if (advance > (image_width - x))
		{
			x = margin;
			y += (max_ascent + max_descent + margin);
		}
 
		// Wype³niamy koordynaty
		_pimpl->_coords.insert(std::make_pair(chars[i], font::coord(static_cast<math::scalar>(x) / image_width, 
																	static_cast<math::scalar>(y - max_ascent) / image_height, 
																	static_cast<math::scalar>(x + advance - margin) / image_width,
																    advance - margin)));
 
		// Kopiujemy na nasz¹ texturê, glif freetype.
		for (utl::int32_t row = 0; row < _pimpl->_face->glyph->bitmap.rows; ++row)
		{
			for (utl::int32_t pixel = 0; pixel < _pimpl->_face->glyph->bitmap.width; ++pixel)
			{
				data[(x + _pimpl->_face->glyph->bitmap_left + pixel) +
                     (y - _pimpl->_face->glyph->bitmap_top + row) * image_width] =
					_pimpl->_face->glyph->bitmap.buffer[pixel + row * _pimpl->_face->glyph->bitmap.pitch];
			}
		}
    
		x += advance;    
	}
	
	FT_Done_Face(_pimpl->_face);
	
	_pimpl->_width = image_width;
	_pimpl->_height = image_height;
	_pimpl->_pixlineheight = (max_ascent + max_descent);
	_pimpl->_lineheight = static_cast<math::scalar>(max_ascent + max_descent) / static_cast<math::scalar>(image_height);
	
	texture_settings currsett = { RWWM_CLAMP, RWWM_CLAMP, RWWM_CLAMP, 
								  RWMAGFT_NEAREST, RWMINFT_NEAREST,
								  RWTIF_ALPHA, RWTOF_ALPHA };
    _pimpl->_texture.generate_from_memory(image_width, image_height, 8, data, false, currsett);
    delete [] data;          
}

void font::destroy()
{
	_pimpl->_size = 0;
	_pimpl->_name = "";
	_pimpl->_width = _pimpl->_height = 0;
}

void font::print(math::scalar x, math::scalar y, const std::string& str, math::color4 color)
{	
	RENDERER.set_ortho();						

	RENDERER.push_all_states();
		RENDERER.polygon_draw_mode(RWPS_FRONT_AND_BACK, RWPDM_FILL);	
		RENDERER.disable(RWS_LIGHTING);	
		RENDERER.enable(RWS_ALPHA_TEST);
		RENDERER.alpha_function(RWAF_GREATER, 0.0625f);
	
		glColor4fv(color.C);
		RENDERER.bind_texture(RWTT_TEXTURE_2D, _pimpl->_texture.ID());
	
		font::coord c;			
		glBegin(GL_QUADS);	
			for(std::string::size_type i = 0; i < str.size(); ++i)
			{
				c = _pimpl->_coords[str[i]];
			
				glTexCoord2f(c.x1, c.y1);			
				glVertex2f(x, y);			
			
				glTexCoord2f(c.x2,  c.y1);			
				glVertex2f(x + c.width, y);			
			
				glTexCoord2f(c.x2, c.y1 + _pimpl->_lineheight);			
				glVertex2f(x + c.width, y - _pimpl->_pixlineheight);			
			
				glTexCoord2f(c.x1, c.y1 + _pimpl->_lineheight);
				glVertex2f(x, y - _pimpl->_pixlineheight);						
									
				x += static_cast<math::scalar>(c.width);
			}
		glEnd();
	
		RENDERER.enable(RWS_LIGHTING);	
		RENDERER.disable(RWS_ALPHA_TEST);		
	RENDERER.pop_all_states();
	
	RENDERER.return_from_ortho();
}

//--------------------------------------------------------------------------------------------
// Implementacja FontManager.
//--------------------------------------------------------------------------------------------
font_manager* utl::singleton<font_manager>::_singleton = 0;

struct font_manager::_font_managerpimpl
{
	typedef std::map< std::string, boost::shared_ptr<font> >			font_map;
	typedef font_map::iterator											font_map_it;
	typedef std::pair< std::string, boost::shared_ptr<font> >			font_pair;
	
	FT_Library _ftlib;
	font_map	_fontmap;	
};

font_manager::font_manager(): _pimpl(new _font_managerpimpl())
{}

font_manager::~font_manager()
{}

void font_manager::initialise()
{
	if (FT_Init_FreeType(&_pimpl->_ftlib)) 
		throw err::font_manager_exception("Cannot initialise FreeType Library");
}

void font_manager::deinitialise()
{
	if (FT_Done_FreeType(_pimpl->_ftlib))
		throw err::font_manager_exception("Cannot deinitialise FreeType Library");
}

void font_manager::add(const std::string& name, utl::uint32_t size, const std::string& filename)
{
	boost::shared_ptr<font> temp(new font(&_pimpl->_ftlib));
	temp->create(filename, size);
	
	if (_pimpl->_fontmap.insert(std::make_pair(name, temp)).second == false)
		throw err::font_manager_exception(std::string("Cannot insert font: ") + name);
}

void font_manager::remove(const std::string& name)
{
	_font_managerpimpl::font_map_it temp = _pimpl->_fontmap.find(name);
	if (temp != _pimpl->_fontmap.end())
	{
		temp->second->destroy();
		_pimpl->_fontmap.erase(temp);
	}
	else
		throw err::font_manager_exception(std::string("Cannot find and erase font: ") + name);		
}

font& font_manager::operator [] (const std::string& name)
{
	_font_managerpimpl::font_map_it ret = _pimpl->_fontmap.find(name);
	if (ret != _pimpl->_fontmap.end())
		return (*(ret->second.get()));
	else
		throw err::font_manager_exception(std::string("Cannot find and operate font: ") + name);		
}