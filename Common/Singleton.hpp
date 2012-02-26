#ifndef __COMMON_SINGLETON_HPP__
#define __COMMON_SINGLETON_HPP__

/// \brief Utl namespace.
///
/// Main Common module namespace, contains many usefull objects, which aren't
/// connected only with a framework, they are like a concepts or ideas, which
/// can be used more than only in one case.
namespace utl
{
	/// \brief Singleton pattern.
	///
	/// Template class for singleton pattern, it can be used in dll libraries.
	/// Every class must declare static field \a _singleton, because each class
	/// have unique \a T type.
	template < typename T >
		class singleton
		{
			protected:
				static T* _singleton;
				
			public:
				singleton()
				{
					assert(!_singleton);
					_singleton = static_cast<T*>(this);
				}
				
				~singleton()
				{
					assert(_singleton);
					_singleton = 0;
				}
				
				static T& get()
				{	
					assert(_singleton); 
					return (*_singleton);	
				}
				
				static T* get_ptr()
				{	
					return (_singleton);	
				}
		};

} // end namespace utl

#endif