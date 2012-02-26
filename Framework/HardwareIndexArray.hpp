#ifndef __FRAMEWORK_HARDWAREINDEXARRAY_HPP__
#define __FRAMEWORK_HARDWAREINDEXARRAY_HPP__

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// HardwareIndexArray.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT hardware_index_array
	{
		public:		
			hardware_index_array(RW_HW_INDEX_ARRAY_ELEMENT_TYPE rwhiaet, utl::uint32_t count, 
								 RW_HW_ARRAY_USAGE rwhau = RWHAU_STATIC_ARRAY,
								 bool discardable = false);
			~hardware_index_array();
			
			void read(utl::uint32_t offset, utl::uint32_t count, void* data);
			void write(utl::uint32_t offset, utl::uint32_t count, const void* data);
			
			void bind_buffer() const;
						
			utl::uint32_t element_size() const;
			utl::uint32_t element_count() const;						
			RW_HW_INTERNAL_ELEMENT_TYPE element_type() const;					
			
			void discard_array();
			
		private:
			struct _hardware_index_arraypimpl;
			boost::scoped_ptr<_hardware_index_arraypimpl> _pimpl;
			
			// Noncopyable idiom.
			hardware_index_array(const hardware_index_array&);
			hardware_index_array& operator=(const hardware_index_array&);
	};

} // end namespace g3d

#endif