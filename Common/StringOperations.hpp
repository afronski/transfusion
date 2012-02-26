#ifndef __COMMON_STRINGOPERATIONS_HPP__
#define __COMMON_STRINGOPERATIONS_HPP__

namespace utl
{
	//--------------------------------------------------------------------------------------------
	// Konwersje daty/czasu na string.
	//--------------------------------------------------------------------------------------------
	TFNCOMMON_EXPORT std::string date_to_string();
	TFNCOMMON_EXPORT std::string time_to_string();

	//--------------------------------------------------------------------------------------------
	// Zmiany wielkoœci stringa.
	//--------------------------------------------------------------------------------------------
	TFNCOMMON_EXPORT void upper_case(std::string& str);
	TFNCOMMON_EXPORT void lower_case(std::string& str);

	//--------------------------------------------------------------------------------------------
	// Konwersje.
	//--------------------------------------------------------------------------------------------
	template < typename T >
		std::string type_to_string(const T& type)
		{
			std::ostringstream ss;
			ss << type;
			return ss.str();
		}

	template < typename T >
		T string_to_type(const std::string& str)
		{
			std::istringstream ss(str);			
			T type;
			ss >> type;
			if (!ss) 
				return T();
			else
				return type;
		}

	//--------------------------------------------------------------------------------------------
	// Formatowanie stringa.
	//--------------------------------------------------------------------------------------------
	TFNCOMMON_EXPORT boost::format format_string(const std::string& str);

	//--------------------------------------------------------------------------------------------
	// Szybkie formatowanie stringa.
	//--------------------------------------------------------------------------------------------
	TFNCOMMON_EXPORT utl::int32_t fast_format_string(std::string& outstr, const char* fmtstring, ...);

} // end namespace utl

#endif