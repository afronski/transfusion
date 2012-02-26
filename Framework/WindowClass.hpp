#ifndef __FRAMEWORK_WINDOWCLASS_HPP__
#define __FRAMEWORK_WINDOWCLASS_HPP__

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// Window class.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT window_class
	{
		public:
			window_class();
			
			bool register_class(WNDPROC wndproc,
								HINSTANCE hInst,
							    utl::uint32_t RGBbackground = RGB(0,0,0),
							    const std::string& classname = "TransfusionWindowClass",
							    DWORD addstyle = 0);
			bool unregister_class();
			
			std::string classname() const	{ return _classname;	}
			
		private:
			WNDCLASSEXA _wndclass;	
			std::string _classname;
			HINSTANCE	_hInst;
	
			// Noncopyable idiom.
			window_class(const window_class&);
			window_class& operator=(const window_class&);
	};

} // end namespace g3d

#endif