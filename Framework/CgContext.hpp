/// \file CgContext.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 03.03.2009
///
/// \brief Interface of Cg context object.
/// 
/// File contains interface of cg context class, operations and some usability
/// tricks for better use.

#ifndef __FRAMEWORK_CGCONTEXT_HPP__
#define __FRAMEWORK_CGCONTEXT_HPP__

namespace g3d
{
	/// \brief cg_context class.
	///
	/// Contains cg context. Class have the methods operating on them, 
	/// creating, destroying, activating. It can't be copied because, 
	/// context is an elementary object, and it has many internal states
	/// which should't be shared.
	class TFNFRAMEWORK_EXPORT cg_context
	{
		public:
			cg_context();
			~cg_context();
			
			void create();
			void destroy();
		
			bool is_valid() const;
			
			CGcontext context() const;
		
		private:
			struct _cg_contextpimpl;
			boost::scoped_ptr<_cg_contextpimpl> _pimpl;
			
			// Noncopyable idiom.
			cg_context(const cg_context&);
			cg_context& operator=(const cg_context&);
	};

} // end namespace g3d

#endif