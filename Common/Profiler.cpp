#include "pchCommon.hpp"

#include "Timer.hpp"
#include "Logger.hpp"
#include "Profiler.hpp"
#include "StringOperations.hpp"

using namespace utl;

//--------------------------------------------------------------------------------------------
// Wyj¹tki.
//--------------------------------------------------------------------------------------------
err::profiler_exception::profiler_exception(const std::string &message):
	err::framework_exception(message)
{}

const char* err::profiler_exception::what() const
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
// Implementacja - klasa profile_sample.
//--------------------------------------------------------------------------------------------
struct profile_manager::profile_sample
{				
	profile_sample();
								
	void start();
	void stop();

	void add_child(profile_sample sample);

	void print(utl::int32_t lvl);
		
	std::string		_name;
	math::scalar	_start;
	math::scalar	_end;
	math::scalar	_time;
	utl::int32_t		_cycle;

	boost::weak_ptr<profile_sample> _parent;
	std::vector< boost::shared_ptr<profile_sample> > _children;
};

profile_manager::profile_sample::profile_sample(): 
	_time(math::ZERO), _cycle(1)
{}

void profile_manager::profile_sample::start()
{
	_start = TIMER.get_microseconds();
}

void profile_manager::profile_sample::stop()
{
	_end = TIMER.get_microseconds();
	_time += ((_end - _start) / 1000000.0f);
}
		
void profile_manager::profile_sample::add_child(profile_sample sample)
{	
	boost::shared_ptr<profile_sample> p1(new profile_sample(sample));
	utl::int32_t found = -1;
	for(utl::uint32_t i = 0; i < _children.size(); ++i)
	{
		if (_children[i]->_name == p1->_name) 
		{
			found = i;
			break;
		}
	}
			
	if (found == -1) 
		this->_children.push_back(p1);
	else
		++this->_children[found]->_cycle;
}

// Pomocnicza funkcja wype³niaj¹ca tabulacje.
void tabs(utl::int32_t lvl)
{
	for(utl::int32_t i = 0; i < lvl; ++i) LOG["profiler"] << "\t";
}

void profile_manager::profile_sample::print(utl::int32_t lvl)
{		
	tabs(lvl); LOG["profiler"] << "-------------------------------------------------" << eol;
	tabs(lvl); LOG["profiler"] << "- " << _name << eol;	
	tabs(lvl); LOG["profiler"] << "- ===> ";
	if (_cycle > 1) LOG["profiler"] << "(average from " << type_to_string(_cycle) << " samples) ";
	LOG["profiler"] << (format_string("%.6f") % (_time / _cycle)).str() << "[s]" << eol;
	
	for(utl::uint32_t i = 0; i < _children.size(); ++i)	
		_children[i]->print(lvl + 1);				
	tabs(lvl); LOG["profiler"] << "-------------------------------------------------" << eol;
}

//--------------------------------------------------------------------------------------------
// Implementacja - profile_manager.
//--------------------------------------------------------------------------------------------
profile_manager* singleton<profile_manager>::_singleton = 0;

profile_manager::profile_manager(): _nested(0)
{
	boost::shared_ptr<profile_sample> p1(new profile_sample());
	p1->_name = "Root";
	_root = p1;
	_actual = _root;
}

profile_manager::~profile_manager()
{}

void profile_manager::save_results(const std::string& filename)
{
	if (_nested != 0) 
		throw err::profiler_exception("Illness profiler tree.");
		
	LOG.link("profiler", new textfile_output(filename));						
	for(utl::uint32_t i = 0; i < _root->_children.size(); ++i) _root->_children[i]->print(0);
}

void profile_manager::start(const std::string& name)
{	
	++_nested;
	
	boost::shared_ptr<profile_sample> p1(_actual);		

	profile_sample temp;		
	temp._name = name;
	temp._parent = p1;
	p1->add_child(temp);	
	_actual = p1->_children[p1->_children.size() - 1];
	
	if (boost::shared_ptr<profile_sample> temp = _actual.lock()) temp->start();
}

void profile_manager::stop()
{	
	boost::shared_ptr<profile_sample> p1(_actual);		
	p1->stop();			
	
	if (p1->_parent.lock()) _actual = p1->_parent;
	
	--_nested;
}