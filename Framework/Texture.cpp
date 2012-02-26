#include "pchFramework.hpp"

#include "Texture.hpp"
#include "../Common/FileOperations.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Wyj¹tki.
//--------------------------------------------------------------------------------------------
err::texture_exception::texture_exception(const std::string &file): _file(file)
{}

err::texture_exception::texture_exception(const std::string &message, const std::string &file):
	err::framework_exception(message), _file(file)
{}

const char* err::texture_exception::what() const
{
	if (_cause.empty()) // Leniwa inicjalizacja ³añcucha z b³êdem.
	{
		try
		{
			_cause = this->framework_exception::what();
			if (!_cause.empty()) _cause += ": ";
			_cause += _file;
		}
		catch(...) { return framework_exception::what(); }
	}
	return _cause.c_str();
}

const char* err::texture_exception::file() const
{
	return _file.c_str();
}

//--------------------------------------------------------------------------------------------
// Implementacja texture_settings.
//--------------------------------------------------------------------------------------------
const texture_settings texture_settings::DEFAULT			= { RWWM_CLAMP, RWWM_CLAMP, RWWM_CLAMP,
																RWMAGFT_NEAREST, RWMINFT_NEAREST, 
																RWTIF_CHOOSE, RWTOF_CHOOSE };
													 
const texture_settings texture_settings::DEFAULT_MIPMAPPING	= { RWWM_CLAMP, RWWM_CLAMP, RWWM_CLAMP,
																RWMAGFT_LINEAR, RWMINFT_MIPPMAPED_LINEAR_LINEAR, 
																RWTIF_CHOOSE, RWTOF_CHOOSE };

//--------------------------------------------------------------------------------------------
// Implementacja tex1D.
//--------------------------------------------------------------------------------------------
struct tex1D::_pimpltex1D
{
	utl::uint32_t _width;
	utl::uint32_t _bpp;
	utl::uint32_t _id;
	bool _mipmaps;
	std::string	_filename;
	FIBITMAP* _struct;
	utl::uint8_t* _data;
	
	_pimpltex1D(): _struct(0), _data(0), _mipmaps(false) {}
};

tex1D::tex1D(): _pimpl(new _pimpltex1D())
{}

tex1D::tex1D(const std::string& str): _pimpl(new _pimpltex1D())
{
	_pimpl->_filename = str;
	load(_pimpl->_filename);
}

tex1D::~tex1D()
{
	FreeImage_Unload(_pimpl->_struct);
	_pimpl->_data = 0;
}

void tex1D::load(const std::string &str)
{
	_pimpl->_filename = str;
	
	FREE_IMAGE_FORMAT ff = FIF_UNKNOWN;
	std::string ext = utl::extract_extension(str);	
	if (ext == "jpg"  || ext == "jpeg" || ext == "JPEG" || ext == "JPG")
		ff = FIF_JPEG;
	if (ext == "tga"  || ext == "TGA")	
		ff = FIF_TARGA;
	if (ext == "bmp"  || ext == "BMP")	
		ff = FIF_BMP;
	if (ext == "png"  || ext == "PNG")	
		ff = FIF_PNG;	
	if (ext == "dds"  || ext == "DDS")	
		ff = FIF_DDS;		
		
	if (ff == FIF_UNKNOWN) 
		throw err::texture_exception(_pimpl->_filename, "Unrecognized image file extension.");
	
	_pimpl->_struct = FreeImage_Load(ff, _pimpl->_filename.c_str());
	if (_pimpl->_struct == 0) 
		throw err::texture_exception(_pimpl->_filename, "Image file corrupted or does not exist.");
	
	_pimpl->_data = FreeImage_GetBits(_pimpl->_struct);
	_pimpl->_width = FreeImage_GetWidth(_pimpl->_struct);
	_pimpl->_bpp = FreeImage_GetBPP(_pimpl->_struct);
	
	if ((_pimpl->_bpp / 8) > 2)
		for(utl::uint32_t i = 0; i < (_pimpl->_width * 1 * (_pimpl->_bpp / 8)); i += (_pimpl->_bpp / 8))
			_pimpl->_data[i + 2] ^= _pimpl->_data[i] ^= _pimpl->_data[i + 2] ^= _pimpl->_data[i];
}

void tex1D::save(const std::string &str, utl::uint32_t w, utl::uint32_t b, utl::uint8_t *d)
{
	if ((_pimpl->_data != 0) && (_pimpl->_struct != 0))
	{
		FreeImage_Unload(_pimpl->_struct);
		_pimpl->_data = 0;
	}
	
	_pimpl->_filename = str;

	FREE_IMAGE_FORMAT ff = FIF_UNKNOWN;
	std::string ext = utl::extract_extension(str);	
	if (ext == "jpg"  || ext == "jpeg" || ext == "JPEG" || ext == "JPG")
		ff = FIF_JPEG;
	if (ext == "tga"  || ext == "TGA")	
		ff = FIF_TARGA;
	if (ext == "bmp"  || ext == "BMP")	
		ff = FIF_BMP;
	if (ext == "png"  || ext == "PNG")	
		ff = FIF_PNG;	
	if (ext == "dds"  || ext == "DDS")	
		ff = FIF_DDS;				
		
	if (ff == FIF_UNKNOWN) 
		throw err::texture_exception(_pimpl->_filename, "Unrecognized image file extension.");	
	
	_pimpl->_width = w;
	_pimpl->_bpp = b;
	_pimpl->_data = new utl::uint8_t [_pimpl->_width * 1 * (_pimpl->_bpp / 8)];
	
	::memcpy(_pimpl->_data, d, _pimpl->_width * 1 * (_pimpl->_bpp /8));
	if ((_pimpl->_bpp / 8) > 2)
		for(utl::uint32_t i = 0; i < (_pimpl->_width * 1 * (_pimpl->_bpp / 8)); i += (_pimpl->_bpp / 8))
			_pimpl->_data[i + 2] ^= _pimpl->_data[i] ^= _pimpl->_data[i + 2] ^= _pimpl->_data[i];
		
	_pimpl->_struct = FreeImage_Allocate(_pimpl->_width, 1, _pimpl->_bpp);
	if (_pimpl->_struct == 0)
		throw err::texture_exception(_pimpl->_filename, "Cannot allocate memory for a image data to save.");
	
	utl::uint8_t* ptr = FreeImage_GetBits(_pimpl->_struct);
	::memcpy(ptr, _pimpl->_data, _pimpl->_width * 1 * (_pimpl->_bpp / 8));
	
	// Sprz¹tanie (jeden obraz jest ju¿ w pamiêci).
	delete [] _pimpl->_data;
	_pimpl->_data = ptr;
	
	FreeImage_Save(ff, _pimpl->_struct, _pimpl->_filename.c_str());
}

void tex1D::save(const std::string& str)
{
	if ((_pimpl->_data != 0) && (_pimpl->_struct != 0))
	{
		FreeImage_Unload(_pimpl->_struct);
		_pimpl->_data = 0;
	}
	
	_pimpl->_filename = str;

	FREE_IMAGE_FORMAT ff = FIF_UNKNOWN;
	std::string ext = utl::extract_extension(str);	
	if (ext == "jpg"  || ext == "jpeg" || ext == "JPEG" || ext == "JPG")
		ff = FIF_JPEG;
	if (ext == "tga"  || ext == "TGA")	
		ff = FIF_TARGA;
	if (ext == "bmp"  || ext == "BMP")	
		ff = FIF_BMP;
	if (ext == "png"  || ext == "PNG")	
		ff = FIF_PNG;	
	if (ext == "dds"  || ext == "DDS")	
		ff = FIF_DDS;				
		
	if (ff == FIF_UNKNOWN) 
		throw err::texture_exception(_pimpl->_filename, "Unrecognized image file extension.");	

	_pimpl->_struct = FreeImage_Allocate(_pimpl->_width, 1, _pimpl->_bpp);
	if (_pimpl->_struct == 0)
		throw err::texture_exception(_pimpl->_filename, "Cannot allocate memory for a image data to save.");
	
	utl::uint8_t* ptr = FreeImage_GetBits(_pimpl->_struct);
	::memcpy(ptr, _pimpl->_data, _pimpl->_width * 1 * (_pimpl->_bpp / 8));
	
	// Sprz¹tanie (jeden obraz jest ju¿ w pamiêci).
	delete [] _pimpl->_data;
	_pimpl->_data = ptr;
	
	FreeImage_Save(ff, _pimpl->_struct, _pimpl->_filename.c_str());
}

void tex1D::generate_from_memory(utl::uint32_t w, utl::uint32_t b, utl::uint8_t* d, bool mipmaps, const texture_settings& texsett)
{
	_pimpl->_filename = "";
	_pimpl->_width = w;
	_pimpl->_bpp = b;
	_pimpl->_data = new utl::uint8_t [_pimpl->_width * 1 * (_pimpl->_bpp / 8)];
	
	::memcpy(_pimpl->_data, d, _pimpl->_width * 1 * (_pimpl->_bpp /8));
	if ((_pimpl->_bpp / 8) > 2)
		for(utl::uint32_t i = 0; i < (_pimpl->_width * 1 * (_pimpl->_bpp / 8)); i += (_pimpl->_bpp / 8))
			_pimpl->_data[i + 2] ^= _pimpl->_data[i] ^= _pimpl->_data[i + 2] ^= _pimpl->_data[i];
		
	_pimpl->_struct = FreeImage_Allocate(_pimpl->_width, 1, _pimpl->_bpp);
	if (_pimpl->_struct == 0)
		throw err::texture_exception(_pimpl->_filename, "Cannot allocate memory for a image data to save.");
	
	utl::uint8_t* ptr = FreeImage_GetBits(_pimpl->_struct);
	::memcpy(ptr, _pimpl->_data, _pimpl->_width * 1 * (_pimpl->_bpp / 8));
	
	// Sprz¹tanie (jeden obraz jest ju¿ w pamiêci).
	delete [] _pimpl->_data;
	_pimpl->_data = ptr;
	
	if (mipmaps) 
		this->generate_mipmaps(texsett);
	else
		this->generate(texsett);
}

void tex1D::generate(const texture_settings& texsett)
{
	_pimpl->_mipmaps = false;

	if (!math::is_pow_2(_pimpl->_width))
		throw err::texture_exception(_pimpl->_filename, "Width is not power of two.");
	
	glGenTextures(1, reinterpret_cast<GLuint*>(&_pimpl->_id));
	glBindTexture(GL_TEXTURE_1D, _pimpl->_id);
	
	this->settings(texsett);
	
	GLenum format;
	if (texsett.OutputFormat == RWTOF_CHOOSE)
	{
		format = GL_RGB;
		if (_pimpl->_bpp == 32) format = GL_RGBA;
		if (_pimpl->_bpp == 8)	format = GL_LUMINANCE;			
	}
	else format = texsett.OutputFormat;	

	GLenum formatint;	
	if (texsett.InternalFormat == RWTIF_CHOOSE)
	{		
		formatint = GL_RGB;
		if (_pimpl->_bpp == 32)	formatint = GL_RGBA;		
		if (_pimpl->_bpp == 8) formatint = GL_LUMINANCE;	
	}
	else formatint = texsett.InternalFormat;			
	
	glTexImage1D(GL_TEXTURE_1D, 0, formatint, _pimpl->_width, 0, format, GL_UNSIGNED_BYTE, _pimpl->_data);
}

void tex1D::generate_mipmaps(const texture_settings& texsett)
{
	_pimpl->_mipmaps = true;

	if (!math::is_pow_2(_pimpl->_width))
		throw err::texture_exception(_pimpl->_filename, "Width is not power of two.");

	glGenTextures(1, reinterpret_cast<GLuint*>(&_pimpl->_id));	
	glBindTexture(GL_TEXTURE_1D, _pimpl->_id);
	
	this->settings(texsett);
	
	GLenum format;
	if (texsett.OutputFormat == RWTOF_CHOOSE)
	{
		format = GL_RGB;	
		if (_pimpl->_bpp == 32) format = GL_RGBA;
		if (_pimpl->_bpp == 8) format = GL_LUMINANCE;	
	} 
	else format = texsett.OutputFormat;
	
	GLenum formatint;	
	if (texsett.InternalFormat == RWTIF_CHOOSE)
	{		
		formatint = GL_RGB;
		if (_pimpl->_bpp == 32)	formatint = GL_RGBA;		
		if (_pimpl->_bpp == 8) formatint = GL_LUMINANCE;	
	}
	else formatint = texsett.InternalFormat;			
	
	gluBuild1DMipmaps(GL_TEXTURE_1D, formatint, _pimpl->_width, format, GL_UNSIGNED_BYTE, _pimpl->_data);
}

utl::uint32_t tex1D::width() const
{
	return _pimpl->_width;
}

utl::uint32_t tex1D::bpp() const
{
	return _pimpl->_bpp;
}

utl::uint32_t tex1D::ID() const
{
	return _pimpl->_id;
}

std::string tex1D::filename() const
{
	return _pimpl->_filename;
}

bool tex1D::is_mipmapped() const
{
	return _pimpl->_mipmaps;
}

void tex1D::settings(const texture_settings& texsett)
{	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, texsett.SWrap);	
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, texsett.MagFilter);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, texsett.MagFilter);	
}

//--------------------------------------------------------------------------------------------
// Implementacja tex2D.
//--------------------------------------------------------------------------------------------
struct tex2D::_pimpltex2D
{
	utl::uint32_t _width;
	utl::uint32_t _height;
	utl::uint32_t _bpp;
	utl::uint32_t _id;
	bool _mipmaps;
	std::string	_filename;
	FIBITMAP* _struct;
	utl::uint8_t* _data;
	
	_pimpltex2D(): _struct(0), _data(0), _mipmaps(false) {}	
};

tex2D::tex2D(): _pimpl(new _pimpltex2D())
{}

tex2D::tex2D(const std::string& str): _pimpl(new _pimpltex2D())
{
	_pimpl->_filename = str;
	load(_pimpl->_filename);
}

tex2D::~tex2D()
{
	FreeImage_Unload(_pimpl->_struct);
	_pimpl->_data = 0;
}

void tex2D::load(const std::string& str)
{
	_pimpl->_filename = str;
	
	FREE_IMAGE_FORMAT ff = FIF_UNKNOWN;
	std::string ext = utl::extract_extension(str);	
	if (ext == "jpg"  || ext == "jpeg" || ext == "JPEG" || ext == "JPG")
		ff = FIF_JPEG;
	if (ext == "tga"  || ext == "TGA")	
		ff = FIF_TARGA;
	if (ext == "bmp"  || ext == "BMP")	
		ff = FIF_BMP;
	if (ext == "png"  || ext == "PNG")	
		ff = FIF_PNG;	
	if (ext == "dds"  || ext == "DDS")	
		ff = FIF_DDS;				
		
	if (ff == FIF_UNKNOWN) 
		throw err::texture_exception(_pimpl->_filename, "Unrecognized image file extension.");
	
	_pimpl->_struct = FreeImage_Load(ff, _pimpl->_filename.c_str());
	if (_pimpl->_struct == 0) 
		throw err::texture_exception(_pimpl->_filename, "Image file corrupted or does not exist.");
	
	_pimpl->_data = FreeImage_GetBits(_pimpl->_struct);
	_pimpl->_width = FreeImage_GetWidth(_pimpl->_struct);
	_pimpl->_height = FreeImage_GetHeight(_pimpl->_struct);
	_pimpl->_bpp = FreeImage_GetBPP(_pimpl->_struct);
	
	if ((_pimpl->_bpp / 8) > 2)
		for(utl::uint32_t i = 0; i < (_pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8)); i += (_pimpl->_bpp / 8))
			_pimpl->_data[i + 2] ^= _pimpl->_data[i] ^= _pimpl->_data[i + 2] ^= _pimpl->_data[i];
}

void tex2D::save(const std::string& str, utl::uint32_t w, utl::uint32_t h, utl::uint32_t b, utl::uint8_t *d)
{
	if ((_pimpl->_data != 0) && (_pimpl->_struct != 0))
	{
		FreeImage_Unload(_pimpl->_struct); 
		_pimpl->_data = 0;
	}
	
	_pimpl->_filename = str;

	FREE_IMAGE_FORMAT ff = FIF_UNKNOWN;
	std::string ext = utl::extract_extension(str);	
	if (ext == "jpg"  || ext == "jpeg" || ext == "JPEG" || ext == "JPG")
		ff = FIF_JPEG;
	if (ext == "tga"  || ext == "TGA")	
		ff = FIF_TARGA;
	if (ext == "bmp"  || ext == "BMP")	
		ff = FIF_BMP;
	if (ext == "png"  || ext == "PNG")	
		ff = FIF_PNG;	
	if (ext == "dds"  || ext == "DDS")	
		ff = FIF_DDS;				
		
	if (ff == FIF_UNKNOWN) 
		throw err::texture_exception(_pimpl->_filename, "Unrecognized image file extension.");	
	
	_pimpl->_width = w;
	_pimpl->_height = h;
	_pimpl->_bpp = b;
	_pimpl->_data = new utl::uint8_t [_pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8)];
	
	::memcpy(_pimpl->_data, d, _pimpl->_width * _pimpl->_height * (_pimpl->_bpp /8));
	
	if ((_pimpl->_bpp / 8) > 2)
		for(utl::uint32_t i = 0; i < (_pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8)); i += (_pimpl->_bpp / 8))
			_pimpl->_data[i + 2] ^= _pimpl->_data[i] ^= _pimpl->_data[i + 2] ^= _pimpl->_data[i];
		
	_pimpl->_struct = FreeImage_Allocate(_pimpl->_width, _pimpl->_height, _pimpl->_bpp);
	if (_pimpl->_struct == 0)
		throw err::texture_exception(_pimpl->_filename, "Cannot allocate memory for a image data to save.");
	
	utl::uint8_t* ptr = FreeImage_GetBits(_pimpl->_struct);
	::memcpy(ptr, _pimpl->_data, _pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8));
	
	// Sprz¹tanie (jeden obraz jest ju¿ w pamiêci).
	delete [] _pimpl->_data;
	_pimpl->_data = ptr;
	
	FreeImage_Save(ff, _pimpl->_struct, _pimpl->_filename.c_str());
}

void tex2D::save(const std::string& str)
{
	if ((_pimpl->_data != 0) && (_pimpl->_struct != 0))
	{
		FreeImage_Unload(_pimpl->_struct); 
		_pimpl->_data = 0;
	}

	_pimpl->_filename = str;

	FREE_IMAGE_FORMAT ff = FIF_UNKNOWN;
	std::string ext = utl::extract_extension(str);	
	if (ext == "jpg"  || ext == "jpeg" || ext == "JPEG" || ext == "JPG")
		ff = FIF_JPEG;
	if (ext == "tga"  || ext == "TGA")	
		ff = FIF_TARGA;
	if (ext == "bmp"  || ext == "BMP")	
		ff = FIF_BMP;
	if (ext == "png"  || ext == "PNG")	
		ff = FIF_PNG;	
	if (ext == "dds"  || ext == "DDS")	
		ff = FIF_DDS;				
		
	if (ff == FIF_UNKNOWN) 
		throw err::texture_exception(_pimpl->_filename, "Unrecognized image file extension.");	

	_pimpl->_struct = FreeImage_Allocate(_pimpl->_width, _pimpl->_height, _pimpl->_bpp);
	if (_pimpl->_struct == 0)
		throw err::texture_exception(_pimpl->_filename, "Cannot allocate memory for a image data to save.");
	
	utl::uint8_t* ptr = FreeImage_GetBits(_pimpl->_struct);
	::memcpy(ptr, _pimpl->_data, _pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8));
	
	// Sprz¹tanie (jeden obraz jest ju¿ w pamiêci).
	delete [] _pimpl->_data;
	_pimpl->_data = ptr;
	
	FreeImage_Save(ff, _pimpl->_struct, _pimpl->_filename.c_str());
}

void tex2D::generate_from_memory(utl::uint32_t w, utl::uint32_t h, utl::uint32_t b, utl::uint8_t* d, bool mipmaps, const texture_settings& texsett)
{
	_pimpl->_filename = "";
	_pimpl->_width = w;
	_pimpl->_height = h;
	_pimpl->_bpp = b;
	_pimpl->_data = new utl::uint8_t [_pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8)];
	
	::memcpy(_pimpl->_data, d, _pimpl->_width * _pimpl->_height * (_pimpl->_bpp /8));
	
	if ((_pimpl->_bpp / 8) > 2)
		for(utl::uint32_t i = 0; i < (_pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8)); i += (_pimpl->_bpp / 8))
			_pimpl->_data[i + 2] ^= _pimpl->_data[i] ^= _pimpl->_data[i + 2] ^= _pimpl->_data[i];
		
	_pimpl->_struct = FreeImage_Allocate(_pimpl->_width, _pimpl->_height, _pimpl->_bpp);
	if (_pimpl->_struct == 0)
		throw err::texture_exception(_pimpl->_filename, "Cannot allocate memory for a image data to save.");
	
	utl::uint8_t* ptr = FreeImage_GetBits(_pimpl->_struct);
	::memcpy(ptr, _pimpl->_data, _pimpl->_width * _pimpl->_height * (_pimpl->_bpp / 8));
	
	// Sprz¹tanie (jeden obraz jest ju¿ w pamiêci).
	delete [] _pimpl->_data;
	_pimpl->_data = ptr;	

	if (mipmaps)
		this->generate_mipmaps(texsett);
	else
		this->generate(texsett);
}

void tex2D::generate(const texture_settings& texsett)
{
	_pimpl->_mipmaps = false;

	if (!math::is_pow_2(_pimpl->_width) ||
		!math::is_pow_2(_pimpl->_height))
		throw err::texture_exception(_pimpl->_filename, "Width or Height is not power of two.");
	
	glGenTextures(1, reinterpret_cast<GLuint*>(&_pimpl->_id));
	glBindTexture(GL_TEXTURE_2D, _pimpl->_id);
		
	this->settings(texsett);
	
	GLenum formatout;
	if (texsett.OutputFormat == RWTOF_CHOOSE)
	{
		formatout = GL_RGB;
		if (_pimpl->_bpp == 32) formatout = GL_RGBA;
		if (_pimpl->_bpp == 8)	formatout = GL_LUMINANCE;
	}
	else formatout = texsett.OutputFormat;

	GLenum formatint;	
	if (texsett.InternalFormat == RWTIF_CHOOSE)
	{		
		formatint = GL_RGB;
		if (_pimpl->_bpp == 32)	formatint = GL_RGBA;		
		if (_pimpl->_bpp == 8) formatint = GL_LUMINANCE;	
	}
	else formatint = texsett.InternalFormat;				
	
	glTexImage2D(GL_TEXTURE_2D, 0, formatint, _pimpl->_width, _pimpl->_height, 0, formatout, GL_UNSIGNED_BYTE, _pimpl->_data);
}

void tex2D::generate_mipmaps(const texture_settings& texsett)
{
	_pimpl->_mipmaps = true;

	if (!math::is_pow_2(_pimpl->_width) ||
		!math::is_pow_2(_pimpl->_height))
		throw err::texture_exception(_pimpl->_filename, "Width or Height is not power of two.");

	glGenTextures(1, reinterpret_cast<GLuint*>(&_pimpl->_id));	
	glBindTexture(GL_TEXTURE_2D, _pimpl->_id);		
	
	this->settings(texsett);
	
	GLenum formatout;		
	if (texsett.OutputFormat == RWTOF_CHOOSE)
	{		
		formatout = GL_RGB;
		if (_pimpl->_bpp == 32)	formatout = GL_RGBA;		
		if (_pimpl->_bpp == 8) formatout = GL_LUMINANCE;	
	}
	else formatout = texsett.OutputFormat;		

	GLenum formatint;	
	if (texsett.InternalFormat == RWTIF_CHOOSE)
	{		
		formatint = GL_RGB;
		if (_pimpl->_bpp == 32)	formatint = GL_RGBA;		
		if (_pimpl->_bpp == 8) formatint = GL_LUMINANCE;	
	}
	else formatint = texsett.InternalFormat;		
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, formatint, _pimpl->_width, _pimpl->_height, formatout, GL_UNSIGNED_BYTE, _pimpl->_data);
}

utl::uint32_t tex2D::bpp() const
{
	return _pimpl->_bpp;
}

utl::uint32_t tex2D::height() const
{
	return _pimpl->_height;
}

utl::uint32_t tex2D::ID() const
{
	return _pimpl->_id;
}

utl::uint32_t tex2D::width() const
{	
	return _pimpl->_width;
}

std::string tex2D::filename() const
{
	return _pimpl->_filename;
}

bool tex2D::is_mipmapped() const
{ 
	return _pimpl->_mipmaps;
}

void tex2D::settings(const texture_settings& texsett)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texsett.SWrap);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texsett.TWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texsett.MagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texsett.MinFilter);	
}

//--------------------------------------------------------------------------------------------
// Funkcje globalne.
//--------------------------------------------------------------------------------------------
void tex::initialise()
{
	FreeImage_Initialise(true);
}

void tex::deinitialise()
{
	FreeImage_DeInitialise();
}