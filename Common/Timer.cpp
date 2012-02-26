#include "pchCommon.hpp"

#include "Timer.hpp"
#include "Exception.hpp"

using namespace utl;
 
//--------------------------------------------------------------------------------------------
// Singleton - deklaracja pola static.
//--------------------------------------------------------------------------------------------
hires_timer* singleton<hires_timer>::_singleton = 0;

//--------------------------------------------------------------------------------------------
// Implementacja Timera.
//--------------------------------------------------------------------------------------------
struct hires_timer::_hires_timerpimpl
{
	LARGE_INTEGER _start_time;
	LARGE_INTEGER _freq;
};

hires_timer::hires_timer(): _pimpl(new _hires_timerpimpl())
{}

hires_timer::~hires_timer()
{}

void hires_timer::create()
{
	if (!QueryPerformanceFrequency(&(_pimpl->_freq)))
		throw err::framework_exception("Cannot create high resolution timer (QPF Failed).");
	else	
		QueryPerformanceCounter(&(_pimpl->_start_time));
}

math::scalar hires_timer::get_miliseconds() const
{
	LARGE_INTEGER _curr;
	QueryPerformanceCounter(&_curr);	
	math::scalar milisec =	static_cast<math::scalar>(_curr.QuadPart - _pimpl->_start_time.QuadPart) / 
							static_cast<math::scalar>(_pimpl->_freq.QuadPart);
	milisec *= math::scalar(1000);	
	return milisec;
}

math::scalar hires_timer::get_microseconds() const
{
	LARGE_INTEGER _curr;
	QueryPerformanceCounter(&_curr);
	math::scalar usec = static_cast<math::scalar>(_curr.QuadPart - _pimpl->_start_time.QuadPart) / 
						static_cast<math::scalar>(_pimpl->_freq.QuadPart);
	usec *= math::scalar(1000000);	
	return usec;
}

math::scalar hires_timer::fps(utl::int32_t frames) const
{
	static LARGE_INTEGER _last = _pimpl->_start_time;
	LARGE_INTEGER _curr;
	QueryPerformanceCounter(&_curr);
	math::scalar fps =	static_cast<math::scalar>(frames) * static_cast<math::scalar>(_pimpl->_freq.QuadPart) / 
						static_cast<math::scalar>(_curr.QuadPart - _last.QuadPart);
	_last = _curr;	
	return fps;
}