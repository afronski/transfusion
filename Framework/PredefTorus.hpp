#ifndef __FRAMEWORK_PREDEFTORUS_HPP__
#define __FRAMEWORK_PREDEFTORUS_HPP__

#include "../Common/MathColor.hpp"
#include "../Common/MathVec.hpp"
#include "CgProgram.hpp"
#include "Mesh.hpp"

namespace g3d
{
	//--------------------------------------------------------------------------------------------
	// PredefinedTorus.
	//--------------------------------------------------------------------------------------------	
	class TFNFRAMEWORK_EXPORT predef_torus
	{
		public:
			void fill(math::scalar majr, math::scalar minr, math::vec3 pos = math::vec3::ZERO_VEC3,
					  utl::uint32_t prec = 360,
					  math::color4 col = math::color4::WHITE4); 
		
			void draw();
			void draw(const cg_program& prog);
		
		private:
			mesh _torus;
		
			math::vec3 _pos;
			utl::uint32_t _prec;
			math::scalar _minorR;
			math::scalar _majorR;
			math::color4 _color;
	};

} // end namespace g3d

#endif