/// \file HardwareArrayManager.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 02.03.2009
///
/// \brief Interface hardware array manager.
///
/// Contains, operates and maintains every vertex/index array in framework.

#ifndef __FRAMEWORK_HARDWAREARRAYMANAGER_HPP__
#define __FRAMEWORK_HARDWAREARRAYMANAGER_HPP__

#include "../Common/Singleton.hpp"

#define HWARRAYMANAGER (g3d::hardware_array_manager::get())

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// HardwareArrayManager.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT hardware_array_manager : public utl::singleton<hardware_array_manager>
	{
		public:
			hardware_array_manager();
			~hardware_array_manager();
			
			static DWORD internal_usage(RW_HW_ARRAY_USAGE rwhau);
					
		private:	
			struct _hardware_array_managerpimpl;
			boost::scoped_ptr<_hardware_array_managerpimpl> _pimpl;
			
			// Noncopyable idiom.
			hardware_array_manager(const hardware_array_manager&);
			hardware_array_manager& operator= (const hardware_array_manager&);
	};

} // end namespace g3d

#endif