#ifndef __FRAMEWORK_WINDOW_HPP__
#define __FRAMEWORK_WINDOW_HPP__

#include "../Common/Exception.hpp"

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// Klasa okna.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT window
	{
		public:
			typedef std::pair<bool, utl::int32_t> MESSAGE_RESULT;
			
			window(const std::string& title = "Transfusion", const std::string& nclass = "");
			virtual ~window();											
					
			virtual MESSAGE_RESULT message_proc(HWND hwnd, utl::uint32_t message, WPARAM wparam, LPARAM lparam);
			
			virtual void resize(utl::int32_t width, utl::int32_t height);			
			void init();
			
			virtual void user_init() = 0;
			virtual void user_destroy() = 0;
			
			virtual void update(math::scalar dt) = 0;		
			virtual void draw() = 0;		
			
			void create(HINSTANCE hInst);
			void destroy();
			
			utl::int32_t run();
						
			bool key_state(utl::uint32_t key) const;
			HWND window_handle() const;
					
		private:			
			struct _windowpimpl;
			boost::scoped_ptr<_windowpimpl> _pimpl;					
			
			// Procedura wewnêtrzna - do obs³ugi komunikatów.
			static LRESULT CALLBACK wnd_proc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam);
			
			// Noncopyable idiom.			
			window(const window&);
			window& operator=(const window&);
	};
		
} // end namespace g3d

namespace err
{
	class TFNFRAMEWORK_EXPORT window_exception : public framework_exception
	{
		public:
			window_exception(const std::string& message);
			
			virtual ~window_exception() throw() {}
			
			const char* what() const throw();
			
		private:
			utl::uint32_t _err_nr;
			mutable std::string _cause;
	};

} // end namespace err

#endif