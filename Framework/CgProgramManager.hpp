/// \file CgProgramManager.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 03.03.2009
///
/// \brief Interface of Cg program manager.
/// 
/// File contains singleton of Cg program manager.

#ifndef __FRAMEWORK_CGPROGRAMMANAGER_HPP__
#define __FRAMEWORK_CGPROGRAMMANAGER_HPP__

#include "../Common/Exception.hpp"
#include "../Common/Singleton.hpp"
#include "CgProgram.hpp"

//--------------------------------------------------------------------------------------------
// Pomocnicza makrodefinicja.
//--------------------------------------------------------------------------------------------
#define CGPROGRAMMANAGER			(g3d::cg_program_manager::get())

namespace g3d
{
	/// \brief cg_program_manager class.
	///
	/// Class which manages programs, loading, sending values for parameters,
	/// operates on cg context and settings all things included in program.
	class TFNFRAMEWORK_EXPORT cg_program_manager : public utl::singleton<cg_program_manager>
	{
		public:
			cg_program_manager();
			~cg_program_manager();			
			
			void initialise();
			void deinitialise();
			
			void open_program(const std::string& filename, const std::string& name,
							  RW_CG_PROGRAM rwcp,
							  const std::string& entry = "main", 
							  const std::string& args = "");
			void close_program(const std::string& name);
				
			cg_program& operator [] (const std::string& name);
		
		private:
			struct _cg_program_managerpimpl;
			boost::scoped_ptr<_cg_program_managerpimpl> _pimpl;
			
			// Noncopyable idiom.
			cg_program_manager(const cg_program_manager&);
			cg_program_manager& operator=(const cg_program_manager&);
	};

} // end namspace g3d

/// \brief Namespace err.
///
/// Have everything what is connected with exceptions, errors and unwanted actions
/// in framework.
namespace err
{
	/// \brief Cg exception class.
	///
	/// Indicate a cg exception in the framework. Contains a important function - cg_check_error(), 
	/// which check cg state, and inform about any problems.
	class TFNFRAMEWORK_EXPORT cg_exception : public framework_exception
	{
		public:
			cg_exception(const std::string& message, CGerror err);
			
			virtual ~cg_exception() throw() {}
			
			const char* what() const throw();
			
			/// \brief Function checking error state CG lib.
			///
			/// Check state and return it, ideal to throwing an exception eg.
			/// \code
			///	 if (err::cg_exception::cg_check_error() != CG_NO_ERROR)
			///		throw err::cg_exception("Something happened");
			/// \endcode
			static void cg_check_error();
			
		private:
			CGerror _err;
			mutable std::string _cause;
	};

} // end namespace err

#endif