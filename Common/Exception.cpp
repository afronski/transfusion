#include "pchCommon.hpp"

#include "Exception.hpp"

using namespace err;

//--------------------------------------------------------------------------------------------
// Implementacja wyj�tk�w.
//--------------------------------------------------------------------------------------------
framework_exception::framework_exception(): std::runtime_error("")
{}

framework_exception::framework_exception(const std::string &message): std::runtime_error(message)
{}

const char* framework_exception::what() const
{
	if (_cause.empty())	// Leniwa inicjalizacja �a�cucha z b��dem.
	{
		try
		{
			_cause = this->std::runtime_error::what();
		}
		catch(...)	{	return std::runtime_error::what(); }
	}
	return _cause.c_str();
}