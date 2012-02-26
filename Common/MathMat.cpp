#include "pchCommon.hpp"

#include "MathVec.hpp"
#include "MathQuat.hpp"
#include "MathMat.hpp"

using namespace math;

//--------------------------------------------------------------------------------------------
// Macierz 3x3.
//--------------------------------------------------------------------------------------------
const mat3 mat3::ZERO_MAT3		= mat3(ZERO, ZERO, ZERO,
									   ZERO, ZERO, ZERO,									
									   ZERO, ZERO, ZERO);
const mat3 mat3::IDENTITY		= mat3( ONE, ZERO, ZERO,
									   ZERO,  ONE, ZERO,									
									   ZERO, ZERO,  ONE);

bool mat3::operator ==(const math::mat3 &M) const
{
	return (a00 == M.a00 && a01 == M.a01 && a02 == M.a02 &&
			a10 == M.a10 && a11 == M.a11 && a12 == M.a12 &&
			a20 == M.a20 && a21 == M.a21 && a22 == M.a22);
}

bool mat3::operator !=(const math::mat3 &M) const
{
	return !(*this == M);
}

void mat3::set_rotation(const math::scalar &theta, const math::vec3 &v)
{
    scalar ct = scalar(cos(theta));
    scalar st = scalar(sin(theta));

    scalar xx = v.x * v.x;
    scalar yy = v.y * v.y;
    scalar zz = v.z * v.z;
    scalar xy = v.x * v.y;
    scalar xz = v.x * v.z;
    scalar yz = v.y * v.z;

    a00 = xx + ct * (1 - xx);
    a01 = xy + ct * (  - xy) + st * -v.z;
    a02 = xz + ct * (  - xz) + st *  v.y;

    a10 = xy + ct * (  - xy) + st *  v.z;
    a11 = yy + ct * (1 - yy);
    a12 = yz + ct * (  - yz) + st * -v.x;

    a20 = xz + ct * (  - xz) + st * -v.y;
    a21 = yz + ct * (  - yz) + st *  v.x;
    a22 = zz + ct * (1 - zz);
}

void mat3::set_rotation(const math::vec3 &u, const math::vec3 &v)
{
	scalar phi = u.dot_product(v);        
    vec3 w = u.cross_product(v);
    scalar lambda = u.dot_product(u);
    scalar h;
    
    if (lambda > EPSILON)
        h = (ONE - phi) / lambda;
    else
        h = lambda;
    
    scalar hxy = w.x * w.y * h;
    scalar hxz = w.x * w.z * h;
    scalar hyz = w.y * w.z * h;

    a00 = phi + w.x * w.x * h;
    a01 = hxy - w.z;
    a02 = hxz + w.y;

    a10 = hxy + w.z;
    a11 = phi + w.y * w.y * h;
    a12 = hyz - w.x;

    a20 = hxz - w.y;
    a21 = hyz + w.x;
    a22 = phi + w.z * w.z * h;
}

mat3 mat3::operator - () const
{	
	return mat3(-a00, -a01, -a02,
				-a10, -a11, -a12,
				-a20, -a21, -a22);
}

mat3 mat3::operator + (const mat3& m) const
{
	return mat3(a00 + m.a00, a01 + m.a01, a02 + m.a02,
				a10 + m.a10, a11 + m.a11, a12 + m.a12,
				a20 + m.a20, a21 + m.a21, a22 + m.a22);
}

mat3 mat3::operator - (const mat3& m) const
{
	return mat3(a00 - m.a00, a01 - m.a01, a02 - m.a02,
				a10 - m.a10, a11 - m.a11, a12 - m.a12,
				a20 - m.a20, a21 - m.a21, a22 - m.a22);
}

mat3& mat3::operator += (const mat3& m)
{
	a00 += m.a00; a01 += m.a01; a02 += m.a02;
	a10 += m.a10; a11 += m.a11; a12 += m.a12;
	a20 += m.a20; a21 += m.a21; a22 += m.a22;
	return (*this);
}

mat3& mat3::operator -= (const mat3& m)
{
	a00 -= m.a00; a01 -= m.a01; a02 -= m.a02;
	a10 -= m.a10; a11 -= m.a11; a12 -= m.a12;
	a20 -= m.a20; a21 -= m.a21; a22 -= m.a22;
	return (*this);
}

mat3& mat3::operator *= (const mat3& m)
{
	(*this) = (*this) * m;
	return (*this);
}

mat3& mat3::operator *= (const scalar& a)
{
	a00 *= a; a01 *= a; a02 *= a;
	a10 *= a; a11 *= a; a12 *= a;
	a20 *= a; a21 *= a; a22 *= a;
	return (*this);
}

mat3& mat3::operator /= (const scalar& a)
{
	a00 /= a; a01 /= a; a02 /= a;
	a10 /= a; a11 /= a; a12 /= a;
	a20 /= a; a21 /= a; a22 /= a;
	return (*this);
}

inline void mat3::zero_matrix()
{
	a00 = ZERO; a01 = ZERO; a02 = ZERO;
	a10 = ZERO; a11 = ZERO; a12 = ZERO;
	a20 = ZERO; a21 = ZERO; a22 = ZERO;
}

inline void mat3::identity_matrix()
{
	a00 =  ONE; a01 = ZERO; a02 = ZERO;
	a10 = ZERO; a11 =  ONE; a12 = ZERO;
	a20 = ZERO; a21 = ZERO; a22 =  ONE;
}

void mat3::inverse()
{
	mat3 B;
    B.a00 =  (a11 * a22 - a21 * a12);
    B.a10 = -(a10 * a22 - a20 * a12);
    B.a20 =  (a10 * a21 - a20 * a11);
    B.a01 = -(a01 * a22 - a21 * a02);
    B.a11 =  (a00 * a22 - a20 * a02);
    B.a21 = -(a00 * a21 - a20 * a01);
    B.a02 =  (a01 * a12 - a11 * a02);
    B.a12 = -(a00 * a12 - a10 * a02);
    B.a22 =  (a00 * a11 - a10 * a01);

    scalar det = (a00 * B.a00) + (a01 * B.a10) + (a02 * B.a20);
    assert(det != ZERO);
    
    scalar oodet = ONE / det;

    B.a00 *= oodet; B.a01 *= oodet; B.a02 *= oodet;
    B.a10 *= oodet; B.a11 *= oodet; B.a12 *= oodet;
    B.a20 *= oodet; B.a21 *= oodet; B.a22 *= oodet;
    (*this) = B;
}

inline scalar mat3::det() const
{
	return det3x3(a00, a01, a02,
				  a10, a11, a12,
				  a20, a21, a22);
}

inline void mat3::transpose()
{
    scalar tmp;
    tmp = a01;
    a01 = a10;
    a10 = tmp;

    tmp = a02;
    a02 = a20;
    a20 = tmp;

    tmp = a12;
    a12 = a21;
    a21 = tmp;
}

inline void swap(mat3& m1, mat3& m2)
{
	std::swap(m1.a00, m2.a00); std::swap(m1.a01, m2.a01); std::swap(m1.a02, m2.a02);
	std::swap(m1.a10, m2.a10); std::swap(m1.a11, m2.a11); std::swap(m1.a12, m2.a12);
	std::swap(m1.a20, m2.a20); std::swap(m1.a21, m2.a21); std::swap(m1.a22, m2.a22);
}

inline mat3 operator * (const scalar& a, const mat3& m)
{
	return mat3(m.a00 * a, m.a01 * a, m.a02 * a,
				m.a10 * a, m.a11 * a, m.a12 * a,
				m.a20 * a, m.a21 * a, m.a22 * a);
}

inline mat3 operator / (const scalar& a, const mat3& m)
{
	return mat3(m.a00 / a, m.a01 / a, m.a02 / a,
				m.a10 / a, m.a11 / a, m.a12 / a,
				m.a20 / a, m.a21 / a, m.a22 / a);
}

inline mat3 operator * (const mat3& m, const scalar& a)
{
	return mat3(m.a00 * a, m.a01 * a, m.a02 * a,
				m.a10 * a, m.a11 * a, m.a12 * a,
				m.a20 * a, m.a21 * a, m.a22 * a);
}

inline mat3 operator / (const mat3& m, const scalar& a)
{
	return mat3(m.a00 / a, m.a01 / a, m.a02 / a,
				m.a10 / a, m.a11 / a, m.a12 / a,
				m.a20 / a, m.a21 / a, m.a22 / a);
}
	
inline vec3 operator * (const vec3& v, const mat3& m)
{
	return vec3(m.a00 * v.x + m.a01 * v.y + m.a02 * v.z,
				m.a10 * v.x + m.a11 * v.y + m.a12 * v.z,
				m.a20 * v.x + m.a21 * v.y + m.a22 * v.z);
}

inline vec3 operator * (const mat3& m, const vec3& v)
{
	return vec3(m.a00 * v.x + m.a01 * v.y + m.a02 * v.z,
				m.a10 * v.x + m.a11 * v.y + m.a12 * v.z,
				m.a20 * v.x + m.a21 * v.y + m.a22 * v.z);
}

//--------------------------------------------------------------------------------------------
// Macierz 4x4.
//--------------------------------------------------------------------------------------------
const mat4 mat4::ZERO_MAT4		= mat4(ZERO, ZERO, ZERO, ZERO,
									   ZERO, ZERO, ZERO, ZERO,									
									   ZERO, ZERO, ZERO, ZERO,
									   ZERO, ZERO, ZERO, ZERO);
const mat4 mat4::IDENTITY		= mat4( ONE, ZERO, ZERO, ZERO,
									   ZERO,  ONE, ZERO, ZERO,									
									   ZERO, ZERO,  ONE, ZERO,
									   ZERO, ZERO, ZERO,  ONE);

bool mat4::operator ==(const math::mat4 &M) const
{
	return (a00 == M.a00 && a01 == M.a01 && a02 == M.a02 && a03 == M.a03 &&
			a01 == M.a01 && a11 == M.a11 && a12 == M.a12 && a13 == M.a13 &&
			a02 == M.a02 && a21 == M.a21 && a22 == M.a22 && a23 == M.a23 &&
			a03 == M.a03 && a31 == M.a31 && a32 == M.a32 && a33 == M.a33);
}

bool mat4::operator !=(const math::mat4 &M) const
{
	return !(*this == M);
}

mat3& mat4::get_rotation(math::mat3 &rot) const
{
    rot.a00 = a00; rot.a10 = a10; rot.a20 = a20;
    rot.a01 = a01; rot.a11 = a11; rot.a21 = a21;
    rot.a02 = a02; rot.a12 = a12; rot.a22 = a22;
    return rot;
}

quat& mat4::get_rotation(math::quat &rot) const
{
	scalar tr = a00 + a11 + a22;
      
    if (tr > ZERO) 
    {
        scalar s = sqrtf(tr + ONE);
        rot.w = s * ZERO_5;
        s = ZERO_5 / s;
        rot.x = (a12 - a21) * s;
        rot.y = (a20 - a02) * s;
        rot.z = (a01 - a10) * s;
    } 
    else
    {		
        utl::int8_t nxt[3] = {1, 2, 0};
   
        // Find the largest diagonal.
        utl::int8_t i = 0;
        
        if (a11 > a00)
            i = 1;
            
        if (a22 > (*this)(i, i))
            i = 2;
            
        utl::int8_t j = nxt[i];
        utl::int8_t k = nxt[j];
      
        scalar s = sqrtf(((*this)(i, i) - ( (*this)(j, j) + (*this)(k, k))) + ONE);
      
        rot.Q[i] = s * ZERO_5;
        s = ZERO_5 / s;
      
        rot.w	 = ((*this)(j, k) - (*this)(k, j)) * s;
        rot.Q[j] = ((*this)(i, j) + (*this)(j, i)) * s;
        rot.Q[k] = ((*this)(i, k) + (*this)(k, i)) * s;
    } 
    
    return rot;
}

void mat4::set_rotation(const quat& q)
{
	scalar w2 = q.w * q.w * TWO;
	scalar x2 = q.x * q.x * TWO;
	scalar y2 = q.y * q.y * TWO;
	scalar z2 = q.z * q.z * TWO;

	a00 = ONE - y2 - z2;
	a11 = ONE - x2 - z2;
	a22 = ONE - x2 - y2;

	scalar  wz = q.w * q.z, 
			xy = q.x * q.y;        
	a01 = TWO * (xy + wz);
	a10 = TWO * (xy - wz);

	scalar	wx = q.w * q.x, 
			yz = q.y * q.z;
	a12 = TWO * (yz + wx);
	a21 = TWO * (yz - wx);

	scalar	wy = q.w * q.y, 
			xz = q.x * q.z;
	a02 = TWO * (xz - wy);
	a20 = TWO * (xz + wy);
}

void mat4::set_rotation(const mat3& M)
{
    a00 = M.a00; a10 = M.a10; a20 = M.a20;
    a01 = M.a01; a11 = M.a11; a21 = M.a21;
    a02 = M.a02; a12 = M.a12; a22 = M.a22;
}

void mat4::set_rotation(const scalar& theta, const vec3& v)
{
    scalar ct = scalar(cos(theta));
    scalar st = scalar(sin(theta));

    scalar xx = v.x * v.x;
    scalar yy = v.y * v.y;
    scalar zz = v.z * v.z;
    scalar xy = v.x * v.y;
    scalar xz = v.x * v.z;
    scalar yz = v.y * v.z;

    a00 = xx + ct * (ONE - xx);
    a01 = xy + ct * (    - xy) + st * -v.z;
    a02 = xz + ct * (    - xz) + st *  v.y;

    a10 = xy + ct * (    - xy) + st *  v.z;
    a11 = yy + ct * (ONE - yy);
    a12 = yz + ct * (    - yz) + st * -v.x;

    a20 = xz + ct * (    - xz) + st * -v.y;
    a21 = yz + ct * (    - yz) + st *  v.x;
    a22 = zz + ct * (ONE - zz);
}

void mat4::set_rotation(const vec3& u, const vec3& v)
{
	scalar phi = u.dot_product(v);        
    vec3 w = u.cross_product(v);
    scalar lambda = u.dot_product(u);
    scalar h;
    
    if (lambda > EPSILON)
        h = (ONE - phi) / lambda;
    else
        h = lambda;
    
    scalar hxy = w.x * w.y * h;
    scalar hxz = w.x * w.z * h;
    scalar hyz = w.y * w.z * h;

    a00 = phi + w.x * w.x * h;
    a01 = hxy - w.z;
    a02 = hxz + w.y;

    a10 = hxy + w.z;
    a11 = phi + w.y * w.y * h;
    a12 = hyz - w.x;

    a20 = hxz - w.y;
    a21 = hyz + w.x;
    a22 = phi + w.z * w.z * h;
}		

vec3& mat4::get_translation(vec3& t) const
{
    t.x = a03;
    t.y = a13;
    t.z = a23;
    return t;
}

void mat4::set_translation(const vec3& t)
{
    a03 = t.x;
    a13 = t.y;
    a23 = t.z;
}

vec3& mat4::get_scale(vec3& s) const
{
	s.x = a00;
	s.y = a11;
	s.z = a22;
	return s;
}

void mat4::set_scale(const vec3& s)
{
	a00 = s.x;
	a11 = s.y;
	a22 = s.z;
}

mat4 mat4::operator - () const
{
	return mat4(-a00, -a01, -a02, -a03,
				-a10, -a11, -a12, -a13,
				-a20, -a21, -a22, -a23,
				-a30, -a31, -a32, -a33);
}

mat4 mat4::operator + (const mat4& m) const
{
	return mat4(a00 + m.a00, a01 + m.a01, a02 + m.a02, a03 + m.a03,
				a10 + m.a10, a11 + m.a11, a12 + m.a12, a13 + m.a13,
				a20 + m.a20, a21 + m.a21, a22 + m.a22, a23 + m.a23,
				a30 + m.a30, a31 + m.a31, a32 + m.a32, a33 + m.a33);
}

mat4 mat4::operator - (const mat4& m) const
{
	return mat4(a00 - m.a00, a01 - m.a01, a02 - m.a02, a03 - m.a03,
				a10 - m.a10, a11 - m.a11, a12 - m.a12, a13 - m.a13,
				a20 - m.a20, a21 - m.a21, a22 - m.a22, a23 - m.a23,
				a30 - m.a30, a31 - m.a31, a32 - m.a32, a33 - m.a33);
}
		
mat4& mat4::operator += (const mat4& m)
{
	a00 += m.a00; a01 += m.a01; a02 += m.a02; a03 += m.a03; 
	a10 += m.a10; a11 += m.a11; a12 += m.a12; a13 += m.a13; 
	a20 += m.a20; a21 += m.a21; a22 += m.a22; a23 += m.a23; 
	a30 += m.a30; a31 += m.a31; a32 += m.a32; a33 += m.a33; 
	return (*this);
}

mat4& mat4::operator -= (const mat4& m)
{
	a00 -= m.a00; a01 -= m.a01; a02 -= m.a02; a03 -= m.a03; 
	a10 -= m.a10; a11 -= m.a11; a12 -= m.a12; a13 -= m.a13; 
	a20 -= m.a20; a21 -= m.a21; a22 -= m.a22; a23 -= m.a23; 
	a30 -= m.a30; a31 -= m.a31; a32 -= m.a32; a33 -= m.a33; 
	return (*this);
}

mat4& mat4::operator *= (const mat4& m)
{
	(*this) = (*this) * m;
	return (*this);
}
		
mat4& mat4::operator *= (const scalar& a)
{
	a00 *= a; a01 *= a; a02 *= a; a03 *= a; 
	a10 *= a; a11 *= a; a12 *= a; a13 *= a; 
	a20 *= a; a21 *= a; a22 *= a; a23 *= a; 
	a30 *= a; a31 *= a; a32 *= a; a33 *= a; 
	return (*this);
}

mat4& mat4::operator /= (const scalar& a)
{
	a00 /= a; a01 /= a; a02 /= a; a03 /= a; 
	a10 /= a; a11 /= a; a12 /= a; a13 /= a; 
	a20 /= a; a21 /= a; a22 /= a; a23 /= a; 
	a30 /= a; a31 /= a; a32 /= a; a33 /= a; 
	return (*this);
}
		
inline void mat4::zero_matrix()
{
	a00 = ZERO; a01 = ZERO; a02 = ZERO; a03 = ZERO;
	a10 = ZERO; a11 = ZERO; a12 = ZERO; a13 = ZERO;
	a20 = ZERO; a21 = ZERO; a22 = ZERO; a23 = ZERO;
	a30 = ZERO; a31 = ZERO; a32 = ZERO; a33 = ZERO;
}

inline void mat4::identity_matrix()
{
	a00 =  ONE; a01 = ZERO; a02 = ZERO; a03 = ZERO;
	a10 = ZERO; a11 =  ONE; a12 = ZERO; a13 = ZERO;
	a20 = ZERO; a21 = ZERO; a22 =  ONE; a23 = ZERO;
	a30 = ZERO; a31 = ZERO; a32 = ZERO; a33 =  ONE;
}
		
void mat4::inverse()
{
	mat4 B;
    B.a00 =  det3x3(a11, a21, a31, a12, a22, a32, a13, a23, a33);
    B.a10 = -det3x3(a10, a20, a30, a12, a22, a32, a13, a23, a33);
    B.a20 =  det3x3(a10, a20, a30, a11, a21, a31, a13, a23, a33);
    B.a30 = -det3x3(a10, a20, a30, a11, a21, a31, a12, a22, a32);

    B.a01 = -det3x3(a01, a21, a31, a02, a22, a32, a03, a23, a33);
    B.a11 =  det3x3(a00, a20, a30, a02, a22, a32, a03, a23, a33);
    B.a21 = -det3x3(a00, a20, a30, a01, a21, a31, a03, a23, a33);
    B.a31 =  det3x3(a00, a20, a30, a01, a21, a31, a02, a22, a32);

    B.a02 =  det3x3(a01, a11, a31, a02, a12, a32, a03, a13, a33);
    B.a12 = -det3x3(a00, a10, a30, a02, a12, a32, a03, a13, a33);
    B.a22 =  det3x3(a00, a10, a30, a01, a11, a31, a03, a13, a33);
    B.a32 = -det3x3(a00, a10, a30, a01, a11, a31, a02, a12, a32);

    B.a03 = -det3x3(a01, a11, a21, a02, a12, a22, a03, a13, a23);
    B.a13 =  det3x3(a00, a10, a20, a02, a12, a22, a03, a13, a23);
    B.a23 = -det3x3(a00, a10, a20, a01, a11, a21, a03, a13, a23);
    B.a33 =  det3x3(a00, a10, a20, a01, a11, a21, a02, a12, a22);

    scalar det = (a00 * B.a00) + (a01 * B.a10) + (a02 * B.a20) + (a03 * B.a30);
    assert(det != ZERO);

    scalar oodet = ONE / det;

    B.a00 *= oodet; B.a01 *= oodet; B.a02 *= oodet; B.a03 *= oodet;
	B.a10 *= oodet; B.a11 *= oodet; B.a12 *= oodet; B.a13 *= oodet;
    B.a20 *= oodet; B.a21 *= oodet; B.a22 *= oodet; B.a23 *= oodet;
    B.a30 *= oodet; B.a31 *= oodet; B.a32 *= oodet; B.a33 *= oodet;
	(*this) = B;
}

inline scalar mat4::det() const
{
	return ((a00 * a11 - a10 * a01) * (a22 * a33 - a32 * a23) - 
			(a00 * a21 - a20 * a01) * (a12 * a33 - a32 * a13) +
			(a00 * a31 - a30 * a01) * (a12 * a23 - a22 * a13) + 
			(a10 * a21 - a20 * a11) * (a02 * a33 - a32 * a03) -
			(a10 * a31 - a30 * a11) * (a02 * a23 - a22 * a03) + 
			(a20 * a31 - a30 * a21) * (a02 * a13 - a12 * a03));
}

inline void mat4::transpose()
{
	scalar tmp;
    tmp = a01;
    a01 = a10;
    a10 = tmp;

    tmp = a02;
    a02 = a20;
    a20 = tmp;

    tmp = a03;
    a03 = a30;
    a30 = tmp;

    tmp = a12;
    a12 = a21;
    a21 = tmp;

    tmp = a13;
    a13 = a31;
    a31 = tmp;

    tmp = a23;
    a23 = a32;
    a32 = tmp;
}

inline void swap(mat4& m1, mat4& m2)
{
	std::swap(m1.a00, m2.a00); std::swap(m1.a01, m2.a01); std::swap(m1.a02, m2.a02); std::swap(m1.a03, m2.a03);
	std::swap(m1.a10, m2.a10); std::swap(m1.a11, m2.a11); std::swap(m1.a12, m2.a12); std::swap(m1.a13, m2.a13);
	std::swap(m1.a20, m2.a20); std::swap(m1.a21, m2.a21); std::swap(m1.a22, m2.a22); std::swap(m1.a23, m2.a23);
	std::swap(m1.a30, m2.a30); std::swap(m1.a31, m2.a31); std::swap(m1.a32, m2.a32); std::swap(m1.a33, m2.a33);
}

inline mat4 operator * (const scalar& a, const mat4& m)
{
	return mat4(m.a00 * a, m.a01 * a, m.a02 * a, m.a03 * a,
				m.a10 * a, m.a11 * a, m.a12 * a, m.a13 * a,
				m.a20 * a, m.a21 * a, m.a22 * a, m.a23 * a,
				m.a30 * a, m.a31 * a, m.a32 * a, m.a33 * a);
}

inline mat4 operator / (const scalar& a, const mat4& m)
{
	return mat4(m.a00 / a, m.a01 / a, m.a02 / a, m.a03 / a,
				m.a10 / a, m.a11 / a, m.a12 / a, m.a13 / a,
				m.a20 / a, m.a21 / a, m.a22 / a, m.a23 / a,
				m.a30 / a, m.a31 / a, m.a32 / a, m.a33 / a);
}

inline mat4 operator * (const mat4& m, const scalar& a)
{
	return mat4(m.a00 * a, m.a01 * a, m.a02 * a, m.a03 * a,
				m.a10 * a, m.a11 * a, m.a12 * a, m.a13 * a,
				m.a20 * a, m.a21 * a, m.a22 * a, m.a23 * a,
				m.a30 * a, m.a31 * a, m.a32 * a, m.a33 * a);
}

inline mat4 operator / (const mat4& m, const scalar& a)
{
	return mat4(m.a00 / a, m.a01 / a, m.a02 / a, m.a03 / a,
				m.a10 / a, m.a11 / a, m.a12 / a, m.a13 / a,
				m.a20 / a, m.a21 / a, m.a22 / a, m.a23 / a,
				m.a30 / a, m.a31 / a, m.a32 / a, m.a33 / a);
}

inline vec4 operator * (const vec4& v, const mat4& m)
{
	return vec4(v.x * m._11 + v.y * m._21 + v.z * m._31 + v.w * m._41,
				v.x * m._12 + v.y * m._22 + v.z * m._32 + v.w * m._42,
				v.x * m._13 + v.y * m._23 + v.z * m._33 + v.w * m._43,
				v.x * m._14 + v.y * m._24 + v.z * m._34 + v.w * m._44);
}

inline vec4 operator * (const mat4& m, const vec4& v)
{
	return vec4(v.x * m._11 + v.y * m._21 + v.z * m._31 + v.w * m._41,
				v.x * m._12 + v.y * m._22 + v.z * m._32 + v.w * m._42,
				v.x * m._13 + v.y * m._23 + v.z * m._33 + v.w * m._43,
				v.x * m._14 + v.y * m._24 + v.z * m._34 + v.w * m._44);
}