#ifndef __COMMON_FILEOPERATIONS_HPP__
#define __COMMON_FILEOPERATIONS_HPP__

namespace utl
{
	//--------------------------------------------------------------------------------------------
	// Wyci¹gniêcie danych ze œcie¿ki.
	//--------------------------------------------------------------------------------------------
	TFNCOMMON_EXPORT std::string extract_extension(const std::string& str);	
	TFNCOMMON_EXPORT std::string extract_filename(const std::string& str);	
	
	//--------------------------------------------------------------------------------------------
	// FastTextFile
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT fast_textfile
	{
		public:
			fast_textfile();
			~fast_textfile();			
			
			std::string read_all();
			
			void open(const std::string& filename);
			void close();
			
			std::string filename() const;
			
		private:
			struct _fast_textfilepimpl;
			boost::scoped_ptr<_fast_textfilepimpl> _pimpl;
			
			// Noncopyable idiom.
			fast_textfile(const fast_textfile&);
			fast_textfile& operator=(const fast_textfile&);
	};
	
	//--------------------------------------------------------------------------------------------
	// FastBinFile
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT fast_binfile
	{
		public:
			fast_binfile();
			~fast_binfile();			
			
			void read_all(utl::int32_t size, utl::int8_t* data);
			utl::int32_t size();
			
			void open(const std::string& filename);
			void close();
			
			std::string filename() const;
			
		private:
			struct _fast_binfilepimpl;
			boost::scoped_ptr<_fast_binfilepimpl> _pimpl;
			
			// Noncopyable idiom.
			fast_binfile(const fast_binfile&);
			fast_binfile& operator=(const fast_binfile&);
	};
					
} // end namespace utl

#endif