#ifndef __COMMON_TIMER_HPP__
#define __COMMON_TIMER_HPP__

#include "Singleton.hpp"

//--------------------------------------------------------------------------------------------
// Pomocnicza makrodefinicja.
//--------------------------------------------------------------------------------------------
#define TIMER	(utl::hires_timer::get())

namespace utl
{
	//--------------------------------------------------------------------------------------------
	// Timer.
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT hires_timer : public singleton<hires_timer>
	{
		public:
			hires_timer();
			~hires_timer();	
						
			void create();
			
			math::scalar get_miliseconds() const;
			math::scalar get_microseconds() const;			
			math::scalar fps(utl::int32_t frames = 1) const;
			
		private:
			struct _hires_timerpimpl;
			boost::scoped_ptr<_hires_timerpimpl> _pimpl;
			
			// Noncopyable idiom.
			hires_timer(const hires_timer&);
			hires_timer& operator=(const hires_timer&);						
	};

} // end namespace utl

#endif