#include "pchFramework.hpp"

#include "../Common/MathVec.hpp"
#include "PredefBox.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
void predef_box::fill(math::scalar a, math::vec3 pos, math::color4 col)
{	
	_size = a;
	_color = col;
	_pos = pos;
	
	math::vec3 box_v[24];
	math::color4 box_c[24];		
	math::vec3 box_n[24];
	math::vec2 box_t[24];	
	
	box_v[0] = math::vec3(	-_size / 2,		-_size / 2,		-_size / 2) + _pos;
	box_v[1] = math::vec3(	 _size / 2,		-_size / 2,		-_size / 2) + _pos;
	box_v[2] = math::vec3(	-_size / 2,		-_size / 2,		 _size / 2) + _pos;
	box_v[3] = math::vec3(	 _size / 2,		-_size / 2,		 _size / 2) + _pos;
	
	box_v[4] = math::vec3(	-_size / 2,		 _size / 2,		-_size / 2) + _pos;
	box_v[5] = math::vec3(	 _size / 2,		 _size / 2,		-_size / 2) + _pos;
	box_v[6] = math::vec3(	-_size / 2,		 _size / 2,		 _size / 2) + _pos;
	box_v[7] = math::vec3(	 _size / 2,		 _size / 2,		 _size / 2) + _pos;

	box_v[8] = math::vec3(	-_size / 2,		-_size / 2,		-_size / 2) + _pos;
	box_v[9] = math::vec3(	 _size / 2,		-_size / 2,		-_size / 2) + _pos;
	box_v[10] = math::vec3(	-_size / 2,		-_size / 2,		 _size / 2) + _pos;
	box_v[11] = math::vec3(	 _size / 2,		-_size / 2,		 _size / 2) + _pos;

	box_v[12] = math::vec3(	-_size / 2,		 _size / 2,		-_size / 2) + _pos;
	box_v[13] = math::vec3(	 _size / 2,		 _size / 2,		-_size / 2) + _pos;
	box_v[14] = math::vec3(	-_size / 2,		 _size / 2,		 _size / 2) + _pos;
	box_v[15] = math::vec3(	 _size / 2,		 _size / 2,		 _size / 2) + _pos;

	box_v[16] = math::vec3(	-_size / 2,		-_size / 2,		-_size / 2) + _pos;
	box_v[17] = math::vec3(	 _size / 2,		-_size / 2,		-_size / 2) + _pos;
	box_v[18] = math::vec3(	-_size / 2,		-_size / 2,		 _size / 2) + _pos;
	box_v[19] = math::vec3(	 _size / 2,		-_size / 2,		 _size / 2) + _pos;

	box_v[20] = math::vec3(	-_size / 2,		 _size / 2,		-_size / 2) + _pos;
	box_v[21] = math::vec3(	 _size / 2,		 _size / 2,		-_size / 2) + _pos;
	box_v[22] = math::vec3(	-_size / 2,		 _size / 2,		 _size / 2) + _pos;
	box_v[23] = math::vec3(	 _size / 2,		 _size / 2,		 _size / 2) + _pos;
	
	box_n[0] = box_n[1] = box_n[2] = box_n[3] = math::vec3::NEGATIVE_Y;
	box_n[4] = box_n[5] = box_n[6] = box_n[7] = math::vec3::POSITIVE_Y;
	box_n[9] = box_n[11] = box_n[13] = box_n[15] = math::vec3::POSITIVE_X;
	box_n[8] = box_n[10] = box_n[12] = box_n[14] = math::vec3::NEGATIVE_X;
	box_n[16] = box_n[17] = box_n[20] = box_n[21] = math::vec3::NEGATIVE_Z;	
	box_n[18] = box_n[19] = box_n[22] = box_n[23] = math::vec3::POSITIVE_Z;	
				
	box_c[0] = box_c[4] = box_c[8] = box_c[12] = box_c[16] = box_c[20] = 
	box_c[1] = box_c[5] = box_c[9] = box_c[13] = box_c[17] = box_c[21] = 
	box_c[2] = box_c[6] = box_c[10] = box_c[14] = box_c[18] = box_c[22] = 
	box_c[3] = box_c[7] = box_c[11] = box_c[15] = box_c[19] = box_c[23] = _color;
			
	box_t[0] = box_t[4] = box_t[8] = box_t[9] = box_t[16] = box_t[18] = math::vec2(0.0f, 0.0f);
	box_t[1] = box_t[5] = box_t[10] = box_t[11] = box_t[17] = box_t[19] = math::vec2(1.0f, 0.0f);
	box_t[2] = box_t[6] = box_t[12] = box_t[13] = box_t[20] = box_t[22] = math::vec2(0.0f, 1.0f);
	box_t[3] = box_t[7] = box_t[14] = box_t[15] = box_t[21] = box_t[23] = math::vec2(1.0f, 1.0f);	
	
	_box.write_vertices(24, RWHAET_FLOAT, RWHAES_3, box_v);	
	_box.write_colors(24, RWHAET_FLOAT, RWHAES_4, box_c);		
	_box.write_normals(24, RWHAET_FLOAT, box_n);	
	_box.write_texcoords(24, RWHAET_FLOAT, RWHAES_2, box_t);
	
	utl::uint32_t box_i[36] = {3, 1, 0, 0, 2, 3,
						      4, 5, 7, 7, 6, 4,
							  9, 11, 15, 15, 13, 9,
							  14, 10, 8, 8, 12, 14,
							  16, 17, 21, 21, 20, 16,
							  23, 19, 18, 18, 22, 23 };							  
	_box.write_indices(36, RWHIAET_UINT, box_i);
	
	_box.set_draw_mode(GL_TRIANGLES);
}

void predef_box::draw()
{
	_box.draw();
}

void predef_box::draw(const cg_program& prog)
{
	_box.draw(prog);
}