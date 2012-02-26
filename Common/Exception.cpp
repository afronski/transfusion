#include "pchCommon.hpp"

#include "Exception.hpp"

using namespace err;

//--------------------------------------------------------------------------------------------
// Implementacja wyj¹tków.
//--------------------------------------------------------------------------------------------
framework_exception::framework_exception(): std::runtime_error("")
{}

framework_exception::framework_exception(const std::string &message): std::runtime_error(message)
{}

const char* framework_exception::what() const
{
	if (_cause.empty())	// Leniwa inicjalizacja ³añcucha z b³êdem.
	{
		try
		{
			_cause = this->std::runtime_error::what();
		}
		catch(...)	{	return std::runtime_error::what(); }
	}
	return _cause.c_str();
}