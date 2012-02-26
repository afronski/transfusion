#ifndef __FRAMEWORK_MESH_HPP__
#define __FRAMEWORK_MESH_HPP__

#include "CgProgram.hpp"

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// Mesh.
	//--------------------------------------------------------------------------------------------
	class TFNFRAMEWORK_EXPORT mesh
	{
		public:
			mesh();
			~mesh();									
			
			void write_colors(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, RW_HW_ARRAY_ELEMENT_SIZE nsize, const void* data);
			void write_texcoords(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, RW_HW_ARRAY_ELEMENT_SIZE nsize, const void* data);
			void write_vertices(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, RW_HW_ARRAY_ELEMENT_SIZE nsize, const void* data);		
			void write_normals(utl::uint32_t count, RW_HW_ARRAY_ELEMENT_TYPE esize, const void* data);
			void write_indices(utl::uint32_t count, RW_HW_INDEX_ARRAY_ELEMENT_TYPE isize, const void* data); 
			
			utl::uint32_t vertices_count() const;
			utl::uint32_t indices_count() const;
			utl::uint32_t normals_count() const;
			
			void draw() const;
			void draw(const cg_program& prog) const;
			
			void set_draw_mode(GLenum mode);
		
		private:		
			struct _meshpimpl;
			boost::scoped_ptr<_meshpimpl> _pimpl;
		
			// Noncopyable idiom.
			mesh(const mesh&);
			mesh& operator=(const mesh&);
	};

} // end namespace g3d

#endif