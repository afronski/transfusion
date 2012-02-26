#include "pchCommon.hpp"

#include "FileOperations.hpp"

using namespace utl;

//--------------------------------------------------------------------------------------------
// Wyci¹gniêcie rozszerzenia.
//--------------------------------------------------------------------------------------------
std::string utl::extract_extension(const std::string& str)
{
	if (str.empty()) return std::string("");
	
	std::string::size_type idx = str.find_last_of(".");
	if (idx != std::string::npos)
		return str.substr(idx + 1);
	else
		return std::string("");
}

//--------------------------------------------------------------------------------------------
// Wyci¹gniêcie nazwy pliku.
//--------------------------------------------------------------------------------------------
std::string utl::extract_filename(const std::string& str)
{
	if (str.empty()) return std::string("");
	
	std::string::size_type idx = str.find_last_of(".");
	return str.substr(0, idx);
}

//--------------------------------------------------------------------------------------------
// Implementacja fast_textfile.
//--------------------------------------------------------------------------------------------
struct fast_textfile::_fast_textfilepimpl
{
	FILE* _file;
	std::string _filename;
};

fast_textfile::fast_textfile(): _pimpl(new _fast_textfilepimpl())
{}

fast_textfile::~fast_textfile()
{
	this->close();
}
			
std::string fast_textfile::read_all()
{
	fseek(_pimpl->_file, 0, SEEK_END);
		utl::int32_t end = ftell(_pimpl->_file);
	fseek(_pimpl->_file, 0, SEEK_SET);
	
	char* buff = new char [ end + 1];
		fread(buff, sizeof(char), end, _pimpl->_file);
		std::string ret(buff, end + 1);
	delete [] buff;
	
	return ret;
}
			
void fast_textfile::open(const std::string& filename)
{
	_pimpl->_filename = filename;
	fopen_s(&_pimpl->_file, _pimpl->_filename.c_str(), "rt");	
}

void fast_textfile::close()
{
	fclose(_pimpl->_file);	
}

std::string fast_textfile::filename() const
{
	return _pimpl->_filename;
}

//--------------------------------------------------------------------------------------------
// Implementacja fast_binfile.
//--------------------------------------------------------------------------------------------
struct fast_binfile::_fast_binfilepimpl
{
	FILE* _file;
	std::string _filename;
};

fast_binfile::fast_binfile(): _pimpl(new _fast_binfilepimpl())
{}

fast_binfile::~fast_binfile()
{
	this->close();
}

utl::int32_t fast_binfile::size()
{
	fseek(_pimpl->_file, 0, SEEK_END);
		utl::int32_t end = ftell(_pimpl->_file);
	fseek(_pimpl->_file, 0, SEEK_SET);
	
	return end;
}
			
void fast_binfile::read_all(utl::int32_t size, utl::int8_t* data)
{	
	utl::int8_t* buff = new utl::int8_t [ size ];
		fread(buff, sizeof(utl::int8_t), size, _pimpl->_file);
		memcpy(data, buff, size * sizeof(utl::int8_t));
	delete [] buff;
}
			
void fast_binfile::open(const std::string& filename)
{
	_pimpl->_filename = filename;
	fopen_s(&_pimpl->_file, _pimpl->_filename.c_str(), "rb");	
}

void fast_binfile::close()
{
	fclose(_pimpl->_file);	
}

std::string fast_binfile::filename() const
{
	return _pimpl->_filename;
}