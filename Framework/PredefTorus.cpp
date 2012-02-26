#include "pchFramework.hpp"

#include "../Common/MathVec.hpp"
#include "PredefTorus.hpp"

using namespace g3d;

//--------------------------------------------------------------------------------------------
// Implementacja.
//--------------------------------------------------------------------------------------------
void predef_torus::fill(math::scalar majr, math::scalar minr, math::vec3 pos, utl::uint32_t prec, math::color4 col)
{
	_majorR = majr;
	_minorR = minr;
	_prec = prec;
	_color = col;
	_pos = pos;

	if (prec < 0) prec = 1;
	
	math::scalar nprec = 360.0f / prec;
	
	utl::uint32_t vertexnum = (prec + 1) * (prec + 1);
	utl::uint32_t indicesnum = 3 * prec * prec * 2;	
   
	math::vec3* torus_v = new math::vec3[vertexnum];
	math::color4* torus_c = new math::color4[vertexnum];
	math::vec2* torus_t = new math::vec2[vertexnum];
	math::vec3* torus_n = new math::vec3[vertexnum];
	utl::uint32_t* torus_i = new utl::uint32_t[indicesnum];		
   
	for(utl::uint32_t i = 0; i < vertexnum; ++i) torus_c[i] = _color;
   
	math::scalar amount = math::ZERO;
	math::vec3 binormal;
	for (utl::uint32_t i = 0; i < prec + 1; ++i)
	{		
        amount = i * nprec;
        torus_v[i] = math::vec3(_minorR, 0.0f, 0.0f).rotate_z(amount) + math::vec3(_majorR, 0.0f, 0.0f);        
        
        torus_t[i] = math::vec2(0.0f, static_cast<math::scalar>(i) / prec);
         
        binormal = math::vec3::NEGATIVE_Y.rotate_z(amount);         
        torus_n[i] = binormal.cross_product(math::vec3::NEGATIVE_Z);
	}
	
	utl::uint32_t index = 0;
	for(utl::uint32_t ring = 1; ring < prec + 1; ++ring)
    {
		for(utl::uint32_t i = 0; i < prec + 1; ++i)
        {			
			amount = ring * nprec;
			index = ring * (prec + 1) + i;

            torus_v[index] = torus_v[i].rotate_y(amount);               
            torus_t[index] = math::vec2(static_cast<math::scalar>(ring) / prec, torus_t[i].y);               
            torus_n[index] = torus_n[i].rotate_y(amount);
            torus_v[index] += _pos;
        }
    }
    
    for (utl::uint32_t i = 0; i < prec + 1; ++i) torus_v[i] += pos;			
	
	_torus.write_vertices(vertexnum, RWHAET_FLOAT, RWHAES_3, torus_v);	
	_torus.write_colors(vertexnum, RWHAET_FLOAT, RWHAES_4, torus_c);		
	_torus.write_normals(vertexnum, RWHAET_FLOAT, torus_n);	
	_torus.write_texcoords(vertexnum, RWHAET_FLOAT, RWHAES_2, torus_t);	
	
	for(utl::uint32_t ring = 0; ring < prec; ++ring)
    {
		for(utl::uint32_t i = 0; i < prec; ++i)
        {			
            torus_i[((ring * prec + i) * 2) * 3 + 0] = ring * (prec + 1) + i;
            torus_i[((ring * prec + i) * 2) * 3 + 1] = (ring + 1) * (prec + 1) + i;
            torus_i[((ring * prec + i) * 2) * 3 + 2] = ring * (prec + 1) + i + 1;
            torus_i[((ring * prec + i) * 2 + 1) * 3 + 0] = ring * (prec + 1) + i + 1;
            torus_i[((ring * prec + i) * 2 + 1) * 3 + 1] = (ring + 1) * (prec + 1) + i;
            torus_i[((ring * prec + i) * 2 + 1) * 3 + 2] = (ring + 1) * (prec + 1) + i + 1;
        }
    }
    
    _torus.write_indices(indicesnum, RWHIAET_UINT, torus_i);
   
	delete [] torus_v;
	delete [] torus_c;
	delete [] torus_t;
	delete [] torus_n;
	delete [] torus_i;
}
		
void predef_torus::draw()
{
	_torus.draw();	
}

void predef_torus::draw(const cg_program& prog)
{
	_torus.draw(prog);
}