#include "pchCommon.hpp"

#include "StringOperations.hpp"

//--------------------------------------------------------------------------------------------
// Konwersja daty na string.
//--------------------------------------------------------------------------------------------
std::string utl::date_to_string()
{
	std::time_t t = time(NULL);
    std::tm res;
	localtime_s(&res, &t);

    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << res.tm_year + 1900 << "-"
       << std::setw(2) << std::setfill('0') << res.tm_mon + 1 << "-"
       << std::setw(2) << std::setfill('0') << res.tm_mday;
         
	return ss.str();
}

//--------------------------------------------------------------------------------------------
// Konwersja czasu na string.
//--------------------------------------------------------------------------------------------
std::string utl::time_to_string()
{
	std::time_t t = time(NULL);
    std::tm res;
	localtime_s(&res, &t);

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << res.tm_hour << ":"
       << std::setw(2) << std::setfill('0') << res.tm_min << ":"
       << std::setw(2) << std::setfill('0') << res.tm_sec;
         
	return ss.str();
}

//--------------------------------------------------------------------------------------------
// Zmiany wielkoœci stringa.
//--------------------------------------------------------------------------------------------
void utl::upper_case(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void utl::lower_case(std::string& str)
{	
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);	
}

//--------------------------------------------------------------------------------------------
// Formatowanie stringa.
//--------------------------------------------------------------------------------------------
boost::format utl::format_string(const std::string& str)
{
	const boost::format fmt(str);
	return fmt;
}

//--------------------------------------------------------------------------------------------
// Szybkie formatowanie stringa.
//--------------------------------------------------------------------------------------------
utl::int32_t utl::fast_format_string(std::string& outstr, const char* fmtstring, ...)
{
    va_list vargs;
    
    va_start (vargs, fmtstring);    
    
		utl::int32_t len = _vscprintf(fmtstring, vargs) + 1;
		char* buff = new char [ len ];
			utl::int32_t ires = vsprintf_s(buff, len, fmtstring, vargs);
			outstr = buff;
		delete [] buff;
		
    va_end (vargs);
    
    return ires;	
}