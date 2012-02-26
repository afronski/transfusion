#ifndef __FRAMEWORK_PREDEFBOX_HPP__
#define __FRAMEWORK_PREDEFBOX_HPP__

#include "../Common/MathColor.hpp"
#include "../Common/MathVec.hpp"
#include "Mesh.hpp"

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// PredefinedBox.
	//--------------------------------------------------------------------------------------------	
	class TFNFRAMEWORK_EXPORT predef_box
	{
		public:
			void fill(math::scalar a, math::vec3 pos = math::vec3::ZERO_VEC3, math::color4 col = math::color4::WHITE4);
			
			void draw();
			void draw(const cg_program& prog);
			
		private:
			mesh _box;	
			
			math::vec3 _pos;
			math::color4 _color;
			math::scalar _size;			
	};
	
} // end namespace g3d

#endif