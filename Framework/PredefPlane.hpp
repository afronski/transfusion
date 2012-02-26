#ifndef __FRAMEWORK_PREDEFPLANE_HPP__
#define __FRAMEWORK_PREDEFPLANE_HPP__

#include "../Common/MathColor.hpp"
#include "../Common/MathVec.hpp"
#include "Mesh.hpp"

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// PredefinedPlane.
	//--------------------------------------------------------------------------------------------	
	class TFNFRAMEWORK_EXPORT predef_plane
	{
		public:
			void fill(math::scalar A, math::vec3 pos = math::vec3::ZERO_VEC3, 
					  math::scalar density = math::ONE, 
					  math::color4 col = math::color4::WHITE4);
			
			void draw();
			void draw(const cg_program& prog);
			
		private:
			mesh _plane;
									
			math::color4 _color;
			math::vec3 _pos;
			
			math::scalar _density;
			math::scalar _A;			
	};
	
} // end namespace g3d

#endif