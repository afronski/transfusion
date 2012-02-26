#ifndef __COMMON_EXCEPTION__
#define __COMMON_EXCEPTION__

namespace err
{
	//--------------------------------------------------------------------------------------------
	// Klasa abstrakcyjna rozpoczynaj¹ca rodzinê wyj¹tków.
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT framework_exception : public std::runtime_error
	{	
		public:
			framework_exception();
			framework_exception(const std::string& message);
			
			virtual ~framework_exception() throw() {}
			
			const char* what() const throw();
			
		protected:
			mutable std::string _cause;
	};

} // end namespace err

#endif