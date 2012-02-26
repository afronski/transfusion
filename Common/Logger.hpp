#ifndef __COMMON_LOGGER_HPP__
#define __COMMON_LOGGER_HPP__

#include "Exception.hpp"
#include "Singleton.hpp"

//--------------------------------------------------------------------------------------------
// Pomocnicza makrodefinicja.
//--------------------------------------------------------------------------------------------
#define	 LOG	(::utl::log_manager::get())

namespace utl
{	
	//--------------------------------------------------------------------------------------------
	// Typ wyliczeniowy typu priorytetów.
	//--------------------------------------------------------------------------------------------
	enum PRIO_TYPE
	{
		PT_ERR	= 2 << 0,
		PT_WARN	= 2 << 1,
		PT_INFO	= 2 << 2
	};
	
	//--------------------------------------------------------------------------------------------
	// Klasa Ilog_output. 
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT Ilog_output
	{
		public:					
			Ilog_output();
			virtual ~Ilog_output();
						
			virtual bool create() = 0;
			virtual bool destroy() = 0;
									
			friend Ilog_output& operator << (Ilog_output& output, const std::string& str)
			{
				output.write(str);
				return output;
			}

			friend Ilog_output& operator << (Ilog_output& output, Ilog_output& (*logmanip)(Ilog_output&))			
			{
				logmanip(output);
				return output;
			}
			
			friend class prio;
			
		protected:
			virtual void write(const std::string& str) = 0;		
			void set_priority(PRIO_TYPE prio);
			
			PRIO_TYPE _lastpriority;					
	};
	
	//--------------------------------------------------------------------------------------------
	// Klasa textfile_log.
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT textfile_output : public Ilog_output
	{
		public:
			textfile_output(const std::string& fn);
			~textfile_output();

			virtual bool create();
			virtual bool destroy();
	
		protected:
			virtual void write(const std::string& str);
			
		private:
			std::string _filename;
			std::ofstream _file;
	};	
	
	//--------------------------------------------------------------------------------------------
	// LogManager.
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT log_manager : public singleton<log_manager>
	{
		public:		
			log_manager();
			~log_manager();
			
			// Metoda link(...) przejmuje kontrolê nad wskaŸnikiem.
			void link(const std::string& name, Ilog_output* out);
			void unlink(const std::string& name);
			
			Ilog_output& operator [] (const std::string& name);
			
		private:
			struct _log_managerpimpl;
			boost::scoped_ptr<_log_managerpimpl> _pimpl;
			
			// Noncopyable idiom.
			log_manager(const log_manager&);
			log_manager& operator=(const log_manager&);
	};
	
	//--------------------------------------------------------------------------------------------
	// Manipulator umo¿liwiajacy wpisanie do wyjœcia priorytetu.
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT prio
	{
		public:		
			prio(PRIO_TYPE p): _prio(p) {}								
			
			friend Ilog_output& operator << (Ilog_output& out, const prio& p)
			{	
				p.change_priority(out);
				return out;
			}
			
		private:
			void change_priority(Ilog_output& out) const
			{
				out.set_priority(_prio);
			}
		
			PRIO_TYPE _prio;								
	};		
	
	//--------------------------------------------------------------------------------------------
	// Manipulator koñca linii.
	//--------------------------------------------------------------------------------------------
	TFNCOMMON_EXPORT Ilog_output& eol(Ilog_output& out);

} // end namespace utl

namespace err
{
	class TFNCOMMON_EXPORT logger_exception : public framework_exception
	{
		public:
			logger_exception(const std::string& message);
			
			virtual ~logger_exception() throw() {}
			
			const char* what() const throw();
			
		private:		
			mutable std::string _cause;
	};

} // end namespace err

#endif