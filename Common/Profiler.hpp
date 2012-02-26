#ifndef __COMMON_PROFILER_HPP__
#define __COMMON_PROFILER_HPP__

//--------------------------------------------------------------------------------------------
// Pomocnicza makrodefinicja.
//--------------------------------------------------------------------------------------------
#define PROFILER		(utl::profile_manager::get())

namespace utl
{
	//--------------------------------------------------------------------------------------------
	// ProfileManager.
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT profile_manager : public singleton<profile_manager>
	{
		public:
			profile_manager();
			~profile_manager();
			
			void save_results(const std::string& filename = "profiler.log");
			
			void start(const std::string& name);			
			void stop();						
			
		private:									
			// ProfileSample - pomocnicza struktura danych
			struct profile_sample;
			
			boost::shared_ptr<profile_sample> _root;
			boost::weak_ptr<profile_sample> _actual;
			
			utl::int32_t _nested;
			
			// Noncopyable idiom.
			profile_manager(const profile_manager&);
			profile_manager& operator=(const profile_manager&);
	};
	
} // end namespace utl

namespace err
{
	class TFNCOMMON_EXPORT profiler_exception : public framework_exception
	{
		public:
			profiler_exception(const std::string& message);
			
			virtual ~profiler_exception() throw() {}
			
			const char* what() const throw();
			
		private:
			mutable std::string _cause;
	};
	
} // end namespace err

#endif