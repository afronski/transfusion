/// \file FontManager.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 02.03.2009
///
/// \brief Interface font manager.
/// 
/// File contains font manager, font and exceptions of this subsystem.

#ifndef __FRAMEWORK_FONTMANAGER_HPP__
#define __FRAMEWORK_FONTMANAGER_HPP__

#include "../Common/Singleton.hpp"
#include "../Common/Exception.hpp"
#include "../Common/MathColor.hpp"

//--------------------------------------------------------------------------------------------
// Pomocnicza makrodefinicja.
//--------------------------------------------------------------------------------------------
#define FONTMANAGER			(g3d::font_manager::get())

namespace g3d
{	
	//--------------------------------------------------------------------------------------------
	// Font.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT font
	{
		public:			
			font(void* ft);
			~font();
						
			void create(const std::string& name, utl::uint32_t size);
			void destroy();
			
			void print(math::scalar x, math::scalar y, const std::string& str, math::color4 color);
			
		private:
			struct _fontpimpl;
			boost::scoped_ptr<_fontpimpl> _pimpl;
			
			struct coord;
			
			// Noncopyable idiom.
			font(const font&);
			font& operator=(const font&);
	};
	
	//--------------------------------------------------------------------------------------------
	// FontManager.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT font_manager : public utl::singleton<font_manager>
	{
		public:
			font_manager();
			~font_manager();
	
			void initialise();
			void deinitialise();
			
			void add(const std::string& name, utl::uint32_t size, const std::string& filename);
			void remove(const std::string& name);
			
			font& operator [] (const std::string& name);
			
		private:
			struct _font_managerpimpl;
			boost::scoped_ptr<_font_managerpimpl> _pimpl;
			
			// Noncopyable idiom.
			font_manager(const font_manager&);
			font_manager& operator=(const font_manager&);
	};

} // end namespace g3d

namespace err
{
	class TFNFRAMEWORK_EXPORT font_manager_exception : public framework_exception
	{
		public:
			font_manager_exception(const std::string& message);
			
			virtual ~font_manager_exception() throw() {}
			
			const char* what() const throw();
			
		private:		
			mutable std::string _cause;
	};

} // end namespace err

#endif