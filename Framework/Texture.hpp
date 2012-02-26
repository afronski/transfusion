#ifndef __FRAMEWORK_TEXTURE_HPP__
#define __FRAMEWORK_TEXTURE_HPP__

#include "../Common/Exception.hpp"

namespace g3d
{
	//--------------------------------------------------------------------------------------------	
	// Ustawienia dla textur.
	//--------------------------------------------------------------------------------------------	
	struct TFNFRAMEWORK_EXPORT texture_settings
	{
		static const texture_settings DEFAULT;
		static const texture_settings DEFAULT_MIPMAPPING;		
		
		RW_WRAPING_MODE SWrap;
		RW_WRAPING_MODE TWrap;
		RW_WRAPING_MODE RWrap;
		
		RW_MAGFILTER_MODE MagFilter;
		RW_MINFILTER_MODE MinFilter;		
		
		RW_TEXTURE_INTERNAL_FORMAT InternalFormat;		
		RW_TEXTURE_OUTPUT_FORMAT OutputFormat;		
	};	

	//--------------------------------------------------------------------------------------------
	// Tekstury Jednowymiarowe.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT tex1D
	{
		public:
			tex1D();
			tex1D(const std::string& str);
			~tex1D();
			
			void load(const std::string& str);			
			void save(const std::string& str, utl::uint32_t w, utl::uint32_t b, utl::uint8_t* d);
			void save(const std::string& str);
						
			void generate_from_memory(utl::uint32_t w, utl::uint32_t b, utl::uint8_t* d, bool mipmaps = false, 
									  const texture_settings& texsett = texture_settings::DEFAULT);
			void generate(const texture_settings& texsett = texture_settings::DEFAULT);
			void generate_mipmaps(const texture_settings& texsett = texture_settings::DEFAULT_MIPMAPPING);
			
			utl::uint32_t width() const;
			utl::uint32_t bpp() const;
			utl::uint32_t ID() const;
			std::string filename() const;
			bool is_mipmapped() const;
			
		private:
			struct _pimpltex1D;
			boost::scoped_ptr<_pimpltex1D> _pimpl;
			
			void settings(const texture_settings& texsett);
			
			// Noncopyable idiom.
			tex1D(const tex1D&);
			tex1D& operator=(const tex1D&);
	};

	//--------------------------------------------------------------------------------------------
	// Tekstury Dwuwymiarowe.
	//--------------------------------------------------------------------------------------------	
	class TFNFRAMEWORK_EXPORT tex2D
	{
		public:
			tex2D();
			tex2D(const std::string& str);
			~tex2D();
			
			void load(const std::string& str);
			void save(const std::string& str, utl::uint32_t w, utl::uint32_t h, utl::uint32_t b, utl::uint8_t* d);
			void save(const std::string& str);
			
			void generate_from_memory(utl::uint32_t w, utl::uint32_t h, utl::uint32_t b, utl::uint8_t* d, bool mipmaps = false,
									  const texture_settings& texsett = texture_settings::DEFAULT);
			void generate(const texture_settings& texsett = texture_settings::DEFAULT);		
			void generate_mipmaps(const texture_settings& texsett = texture_settings::DEFAULT_MIPMAPPING);	
						
			utl::uint32_t width() const;
			utl::uint32_t height() const;			
			utl::uint32_t bpp() const;			
			utl::uint32_t ID() const;
			std::string filename() const;
			bool is_mipmapped() const;
			
		private:
			struct _pimpltex2D;
			boost::scoped_ptr<_pimpltex2D> _pimpl;	
			
			void settings(const texture_settings& texsett);
			
			// Noncopyable idiom.
			tex2D(const tex2D&);
			tex2D& operator=(const tex2D&);
	};

	namespace tex
	{
		//--------------------------------------------------------------------------------------------
		// Funkcje globalne inicjalizuj¹ce/deinicjalizuj¹ce tekstury.
		//--------------------------------------------------------------------------------------------
		TFNFRAMEWORK_EXPORT void initialise();
		TFNFRAMEWORK_EXPORT void deinitialise();
		
	} // end namespace tex

} // end namespace g3d

namespace err
{
	class TFNFRAMEWORK_EXPORT texture_exception : public framework_exception
	{
		public:
			texture_exception(const std::string& file);
			texture_exception(const std::string& message, const std::string& file);
			
			virtual ~texture_exception() throw() {}
			
			const char* file() const throw();
			const char* what() const throw();
			
		private:
			std::string _file;
			mutable std::string _cause;
	};
		
} // end namespace err

#endif