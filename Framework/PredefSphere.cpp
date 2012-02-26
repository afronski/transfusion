#include "pchFramework.hpp"

#include "../Common/MathVec.hpp"
#include "PredefSphere.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
void predef_sphere::fill(math::scalar r, math::vec3 pos, utl::uint32_t slices, utl::uint32_t stacks, math::color4 col)
{
	_slices = slices;
	_stacks = stacks;
	_radius = r;
	_color = col;
	_pos = pos;
	
	utl::uint32_t vertexnum = (_stacks + 1) * _slices;
	
	math::vec3* sphere_v = new math::vec3[vertexnum];
	math::color4* sphere_c = new math::color4[vertexnum];
	math::vec2* sphere_t = new math::vec2[vertexnum];
	math::vec3* sphere_n = new math::vec3[vertexnum];
	
	math::scalar theta = math::ZERO, sintheta = math::ZERO, costheta = math::ZERO;
	math::scalar phi = math::ZERO, sinphi = math::ZERO, cosphi = math::ZERO;
	utl::uint32_t index = 0;
	
	for(utl::uint32_t sn = 0; sn <= _stacks; ++sn)
	{
		theta = sn * math::PI / _stacks;
		sintheta = sinf(theta);
		costheta = cosf(theta);

		for(utl::uint32_t sln = 0; sln < _slices; ++sln)
		{
			index = sn * _slices + sln;
			phi = sln * math::TWO * math::PI / (_slices - 1);
			sinphi = sinf(phi);
			cosphi = cosf(phi);

			if (sln == (_slices - 1))
				sphere_v[index] = sphere_v[sn * _slices];
			else
			{
				sphere_v[index] = math::vec3(cosphi * sintheta, costheta, sinphi * sintheta);
				sphere_v[index] *= _radius;				
			}

			sphere_n[index] = sphere_v[index];
			sphere_n[index].normalize();
			sphere_c[index] = _color;
			sphere_t[index] = math::vec2(1.0f - static_cast<math::scalar>(sln) / static_cast<math::scalar>(_slices - 1),
										 1.0f - static_cast<math::scalar>(sn) / static_cast<math::scalar>(_stacks));
			if (sln != (_slices - 1)) sphere_v[index] += _pos;										 
		}
	}
	
	_sphere.write_vertices(vertexnum, RWHAET_FLOAT, RWHAES_3, sphere_v);	
	_sphere.write_colors(vertexnum, RWHAET_FLOAT, RWHAES_4, sphere_c);		
	_sphere.write_normals(vertexnum, RWHAET_FLOAT, sphere_n);	
	_sphere.write_texcoords(vertexnum, RWHAET_FLOAT, RWHAES_2, sphere_t);		
	
	std::vector<utl::uint32_t> indices;
	utl::uint32_t begin = 0;
	for(utl::uint32_t sn = 0; sn < _stacks; ++sn)
	{
		begin = sn * _slices;
		for(utl::uint32_t sln = 0; sln < _slices - 1; ++sln)
		{
			indices.push_back(begin + sln + _slices);
			indices.push_back(begin + sln + 1);
			indices.push_back(begin + sln);				

			indices.push_back(begin + sln + _slices);
			indices.push_back(begin + sln + _slices + 1);
			indices.push_back(begin + sln + 1);
		}
	}
	
	_sphere.write_indices(indices.size(), RWHIAET_UINT, &indices[0]);
	
	delete [] sphere_v;
	delete [] sphere_c;
	delete [] sphere_t;
	delete [] sphere_n;
}

void predef_sphere::draw()
{
	_sphere.draw();
}

void predef_sphere::draw(const cg_program& prog)
{
	_sphere.draw(prog);
}