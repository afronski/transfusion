#include "pchFramework.hpp"

#include "../Common/MathVec.hpp"
#include "PredefPlane.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
void predef_plane::fill(math::scalar A, math::vec3 pos, math::scalar density, math::color4 col)	
{
	_pos = pos;
	_color = col; 
	_density = density;
	_A = A; 	
	
	if (_density >= (_A / 2.0f)) _density = _A / 2.0f;
	
	utl::uint32_t _pieces = static_cast<utl::uint32_t>(_A / _density);	
	
	math::vec3* plane_v = new math::vec3[_pieces * _pieces];
	math::vec2* plane_t  = new math::vec2[_pieces * _pieces];
	math::color4* plane_c = new math::color4[_pieces * _pieces];
	math::vec3* plane_n = new math::vec3[_pieces * _pieces]; 
	
	for(utl::uint32_t i = 0; i < (_pieces * _pieces); ++i)
	{
		plane_c[i] = _color;
		plane_n[i] = math::vec3::POSITIVE_Y;				
	}
	
	math::scalar half = (_A / 2.0f) - (_density / 2.0f);	
	
	for (utl::uint32_t y = 0; y < _pieces; ++y)
		for(utl::uint32_t x = 0; x < _pieces; ++x)
		{
			plane_t[y * _pieces + x] = math::vec2(static_cast<math::scalar>(x) / (_pieces - 1), 
												  1.0f - static_cast<math::scalar>(y) / (_pieces - 1)); 
			plane_v[y * _pieces + x] = math::vec3((x * _density) - half, 0.0f, (y * _density) - half) + pos;
		}
	
	_plane.write_vertices(_pieces * _pieces, RWHAET_FLOAT, RWHAES_3, plane_v);	
	_plane.write_texcoords(_pieces * _pieces, RWHAET_FLOAT, RWHAES_2, plane_t);			
	_plane.write_colors(_pieces * _pieces, RWHAET_FLOAT, RWHAES_4, plane_c);
	_plane.write_normals(_pieces * _pieces, RWHAET_FLOAT, plane_n);			
	
	utl::uint32_t *plane_i = new utl::uint32_t [ 6 * _pieces * _pieces ];
	
	for(utl::uint32_t y = 0; y < (_pieces - 1); ++y)
		for(utl::uint32_t x = 0; x < (_pieces - 1); ++x)
		{
			plane_i[6 * (y * _pieces + x) + 0] = y * _pieces + x;
			plane_i[6 * (y * _pieces + x) + 1] = y * _pieces + x + 1;
			plane_i[6 * (y * _pieces + x) + 2] = (y + 1) * _pieces + x + 1;						
			
			plane_i[6 * (y * _pieces + x) + 3] = y * _pieces + x;
			plane_i[6 * (y * _pieces + x) + 4] = (y + 1) * _pieces + x;
			plane_i[6 * (y * _pieces + x) + 5] = (y + 1) * _pieces + x + 1;
		}
	
	_plane.write_indices(6 * _pieces * _pieces, RWHIAET_UINT, plane_i);								
	_plane.set_draw_mode(GL_TRIANGLES);
	
	delete [] plane_v;
	delete [] plane_t;
	delete [] plane_c;
	delete [] plane_n;
	delete [] plane_i;
}

void predef_plane::draw()
{
	_plane.draw();
}

void predef_plane::draw(const cg_program& prog)
{
	_plane.draw(prog);
}