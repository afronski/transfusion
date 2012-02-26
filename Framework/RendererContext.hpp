#ifndef __FRAMEWORK_RENDERERCONTEXT_HPP__
#define __FRAMEWORK_RENDERERCONTEXT_HPP__

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// Kontekst renderingu.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT renderer_context		
	{		
		public:			
			renderer_context();
			~renderer_context();
			
			void activate();
			void deactivate();
			
			void create(HWND hwnd, utl::uint8_t depthbits, utl::uint8_t stencilbits);
			void destroy(HWND hwnd);	
			
			void swap_buffers();	
			
		private:
			struct _contextpimpl;
			boost::scoped_ptr<_contextpimpl> _pimpl;				
			
			// Noncopyable idiom.
			renderer_context(const renderer_context&);
			renderer_context& operator=(const renderer_context&);
	};
	
} // end namespace g3d

#endif