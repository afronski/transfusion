#ifndef __FRAMEWORK_PREDEFSPHERE_HPP__
#define __FRAMEWORK_PREDEFSPHERE_HPP__

#include "../Common/MathColor.hpp"
#include "../Common/MathVec.hpp"
#include "CgProgram.hpp"
#include "Mesh.hpp"

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// PredefinedSphere.
	//--------------------------------------------------------------------------------------------	
	class TFNFRAMEWORK_EXPORT predef_sphere
	{
		public:
			void fill(math::scalar r, math::vec3 pos = math::vec3::ZERO_VEC3, 
					  utl::uint32_t slices = 10, utl::uint32_t stack = 10,
					  math::color4 col = math::color4::WHITE4);
		
			void draw();
			void draw(const cg_program& prog);
			
		private:			
			mesh _sphere;
			
			math::vec3 _pos;
			
			utl::uint32_t _slices;
			utl::uint32_t _stacks;
			math::scalar _radius;
			math::color4 _color;
	};
	
} // end namespace g3d

#endif