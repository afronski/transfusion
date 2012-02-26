#include "pchCommon.hpp"

#include "StringOperations.hpp"
#include "Logger.hpp"

using namespace utl;

//--------------------------------------------------------------------------------------------
// Wyj¹tki.
//--------------------------------------------------------------------------------------------
err::logger_exception::logger_exception(const std::string &message):
	err::framework_exception(message)
{}

const char* err::logger_exception::what() const
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
// Implementacja - Ilog_output.
//--------------------------------------------------------------------------------------------
Ilog_output::Ilog_output(): _lastpriority(PT_INFO)
{}
			
Ilog_output::~Ilog_output()
{}

void Ilog_output::set_priority(PRIO_TYPE prio)
{
	_lastpriority = prio;
	
	std::string prefix("[ ");
	switch(_lastpriority)
	{
		case PT_ERR:		prefix += "!!"; break;
		case PT_WARN:		prefix += "**"; break;
		case PT_INFO:		prefix += "ii"; break;
		default:			prefix += "ii"; break;
	}
	prefix += " :: ";
    prefix += date_to_string();
    prefix += " ";
    prefix += time_to_string();
	prefix += " ] ";	
	
	this->write(prefix);	
}

//--------------------------------------------------------------------------------------------
// Implementacja - textfile_output.
//--------------------------------------------------------------------------------------------
textfile_output::textfile_output(const std::string& fn): _filename(fn)
{}

textfile_output::~textfile_output()
{
	this->destroy();
}

bool textfile_output::create()
{
	_file.open(_filename.c_str());
	if (!_file.good())
		throw err::logger_exception(std::string("Cannot open text file log: ") + _filename);
	return true;
}

bool textfile_output::destroy()
{
	_file.flush();
	_file.close();
	return true;
}

void textfile_output::write(const std::string& str)
{	
	_file << str;
	_file.flush();
	if (!_file.good()) 
		throw err::logger_exception(std::string("Cannot write to text file log: ") + _filename);
}

//--------------------------------------------------------------------------------------------
// Implementacja - log_manager.
//--------------------------------------------------------------------------------------------
log_manager* singleton<log_manager>::_singleton = 0;

struct log_manager::_log_managerpimpl
{
	typedef std::pair<std::string, boost::shared_ptr<Ilog_output> > log_outpair;
	typedef std::map<std::string, boost::shared_ptr<Ilog_output> >::iterator log_iter;
	typedef std::map<std::string, boost::shared_ptr<Ilog_output> > log_container;
	
	std::map<std::string, boost::shared_ptr<Ilog_output> > _outputs;
};

log_manager::log_manager(): _pimpl(new _log_managerpimpl())
{}

log_manager::~log_manager()
{}
			
void log_manager::link(const std::string& name, Ilog_output* out)
{
	if (!out->create()) 
		throw err::logger_exception(std::string("Cannot insert logger output named: ") + name);	
		
	boost::shared_ptr<Ilog_output> smartptr(out);
	if (_pimpl->_outputs.insert(std::make_pair(name, smartptr)).second == false)
		throw err::logger_exception(std::string("Cannot insert logger output named: ") + name);	
}

void log_manager::unlink(const std::string& name)
{
	_log_managerpimpl::log_iter ret = _pimpl->_outputs.find(name);
	if (ret != _pimpl->_outputs.end())
	{
		if (!ret->second->destroy()) 
			throw err::logger_exception(std::string("Cannot erase logger output named: ") + name);	
				
		_pimpl->_outputs.erase(ret);
	}
	else
		throw err::logger_exception(std::string("Cannot find and erase logger output named: ") + name);
}
			
Ilog_output& log_manager::operator [] (const std::string& name)
{
	_log_managerpimpl::log_iter ret = _pimpl->_outputs.find(name);
	if (ret != _pimpl->_outputs.end())
		return (*(ret->second.get()));
	else
		throw err::logger_exception(std::string("Cannot find and operate logger output named: ") + name);
}

//--------------------------------------------------------------------------------------------
// Manipulator koñca linii.
//--------------------------------------------------------------------------------------------
Ilog_output& utl::eol(Ilog_output& out)
{
	out << "\n";
	return out;
}