#ifndef __FRAMEWORK_HARDWAREVERTEXARRAY_HPP__
#define __FRAMEWORK_HARDWAREVERTEXARRAY_HPP__

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// HardwareVertexArray.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT hardware_vertex_array
	{
		public:			
			hardware_vertex_array(RW_HW_ARRAY_TYPE rwhat, 
								  RW_HW_ARRAY_ELEMENT_TYPE rwhet, 
								  RW_HW_ARRAY_ELEMENT_SIZE rwhaes,
								  utl::uint32_t count, 
								  RW_HW_ARRAY_USAGE rwhau = RWHAU_STATIC_ARRAY,
								  bool discardable = false);
			~hardware_vertex_array();
			
			void read(utl::uint32_t offset, utl::uint32_t count, void* data);
			void write(utl::uint32_t offset, utl::uint32_t count, const void* data);
			
			void bind_buffer() const;
			
			RW_HW_ARRAY_TYPE type() const;			
			utl::uint32_t element_size() const;
			utl::uint32_t element_count() const;			
			utl::uint32_t element_num() const;
			RW_HW_INTERNAL_ELEMENT_TYPE element_type() const;

			void discard_array();
			
		private:
			struct _hardware_vertex_arraypimpl;
			boost::scoped_ptr<_hardware_vertex_arraypimpl> _pimpl;
				
			// Noncopyable idiom.
			hardware_vertex_array(const hardware_vertex_array&);
			hardware_vertex_array& operator=(const hardware_vertex_array&);
	};
	
} // end namespace g3d

#endif