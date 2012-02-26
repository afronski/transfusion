#include "pchCommon.hpp"

#include "MathVec.hpp"

using namespace math;

//--------------------------------------------------------------------------------------------
// Dodatki.
//--------------------------------------------------------------------------------------------
void is_valid(const vec2& v)
{
	assert(!_isnan(v.x) && !_isnan(v.y) && 
			_finite(v.x) && _finite(v.y));
}

void is_valid(const vec3& v)
{
	assert(!_isnan(v.x) && !_isnan(v.y) && !_isnan(v.z) && 
			_finite(v.x) && _finite(v.y) && _finite(v.z));
}

void is_valid(const vec4& v)
{
	assert(!_isnan(v.x) && !_isnan(v.y) && !_isnan(v.z) && !_isnan(v.w) && 
			_finite(v.x) && _finite(v.y) && _finite(v.z) && _finite(v.w));
}

//--------------------------------------------------------------------------------------------
// Wektor 2D.
//--------------------------------------------------------------------------------------------
const vec2 vec2::ZERO_VEC2		= vec2(ZERO, ZERO);
const vec2 vec2::ONE_VEC2		= vec2( ONE,  ONE);
const vec2 vec2::POSITIVE_X		= vec2( ONE, ZERO);
const vec2 vec2::POSITIVE_Y		= vec2(ZERO,  ONE);
const vec2 vec2::NEGATIVE_X		= vec2(-ONE, ZERO);
const vec2 vec2::NEGATIVE_Y		= vec2(ZERO, -ONE);

vec2::vec2(const vec3& u)
{
	assert(!equal(u.z, ZERO));
	
	scalar k = ONE / u.z;
	x = k * u.x;
	y = k * u.y;
}	

scalar vec2::dot_product(const vec2& v) const
{
	return v.x * x + v.y * y;
}

scalar vec2::cross_product(const vec2& v) const 
{
	return x * v.y - y * v.x;
}

vec2 vec2::perpendicular() const
{
	return vec2(-y, x);
}

vec2 vec2::reflect(const vec2& v, const vec2& N) const
{
	scalar f = TWO * v.dot_product(N);
	vec2 out;
	out.x = v.x - f * N.x;
	out.y = v.y - f * N.y;
	return out;
}

vec2 vec2::project(const vec2& v, const vec2& N) const
{
	scalar T = (v.x * N.x + v.y * N.y) / (N.x * N.x + N.y * N.y);
	vec2 out;
	out.x = N.x * T;
	out.y = N.y * T;
	return out;
}

scalar vec2::length() const		
{ 
	return sqrtf(sqr_length());	
}

scalar vec2::normalize()
{
	scalar norm = sqrtf(x*x + y*y);
	if (norm > EPSILON)
		norm = ONE / norm;
	else
		norm = ZERO;
		
	x *= norm;
	y *= norm;
	return norm;
}

vec2 math::lerp(const scalar& t, const vec2& a, const vec2& b)
{
	return vec2(a.x * (ONE - t) + t * b.x, 
				a.y * (ONE - t) + t * b.y);
}

void math::swap(vec2& v1, vec2& v2)
{
	std::swap(v1.x, v2.x);
	std::swap(v1.y, v2.y);
}

//--------------------------------------------------------------------------------------------
// Wektor 3D.
//--------------------------------------------------------------------------------------------
const vec3 vec3::ZERO_VEC3		= vec3(ZERO, ZERO, ZERO);
const vec3 vec3::ONE_VEC3		= vec3( ONE,  ONE,  ONE);
const vec3 vec3::POSITIVE_X		= vec3( ONE, ZERO, ZERO);
const vec3 vec3::POSITIVE_Y		= vec3(ZERO,  ONE, ZERO);
const vec3 vec3::POSITIVE_Z		= vec3(ZERO, ZERO,  ONE);
const vec3 vec3::NEGATIVE_X		= vec3(-ONE, ZERO, ZERO);
const vec3 vec3::NEGATIVE_Y		= vec3(ZERO, -ONE, ZERO);
const vec3 vec3::NEGATIVE_Z		= vec3(ZERO, ZERO, -ONE);

vec3::vec3(const vec4& u)
{
	assert(!equal(u.w, ZERO));
	
	scalar k = ONE / u.w;
	x = k * u.x;
	y =	k * u.y;
	z = k * u.z;
}			

scalar vec3::dot_product(const vec3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

vec3 vec3::cross_product(const vec3& v) const
{
	return vec3(y * v.z - z * v.y, 
				z * v.x - x * v.z,				
				x * v.y - y * v.x);	
}
		
vec3 vec3::reflect(const vec3& v, const vec3& N) const
{
	scalar f = TWO * v.dot_product(N);
	vec3 out;
	out.x = v.x - f * N.x;
	out.y = v.y - f * N.y;
	out.z = v.z - f * N.z;
	return out;
}

vec3 vec3::project(const vec3& v, const vec3& N) const
{
	scalar T = (v.x * N.x + v.y * N.y + v.z * N.z) / (N.x * N.x + N.y * N.y + N.z * N.z);
	vec3 out;
	out.x = N.x * T;
	out.y = N.y * T;
	out.z = N.z * T;
	return out;
}

scalar vec3::length() const		
{ 
	return sqrtf(sqr_length());	
}

scalar vec3::normalize()
{
	scalar norm = sqrtf(x*x + y*y + z*z);
	if (norm > EPSILON)
		norm = ONE / norm;
	else
		norm = ZERO;
		
	x *= norm;
	y *= norm;
	z *= norm;
	return norm;
}

vec3 vec3::rotate_x(math::scalar ang) const
{
	math::scalar sinang = sinf(ang * TO_RAD);
	math::scalar cosang = cosf(ang * TO_RAD);
	return vec3(x, y * cosang - z * sinang, y * sinang + z * cosang);	
}

vec3 vec3::rotate_y(math::scalar ang) const
{
	math::scalar sinang = sinf(ang * TO_RAD);
	math::scalar cosang = cosf(ang * TO_RAD);
	return vec3(x * cosang + z * sinang, y, -x * sinang + z * cosang);
}

vec3 vec3::rotate_z(math::scalar ang) const
{
	math::scalar sinang = sinf(ang * TO_RAD);
	math::scalar cosang = cosf(ang * TO_RAD);
	return vec3(x * cosang - y * sinang, x * sinang + y * cosang, z);	
}
		
vec3 math::lerp(const scalar& t, const vec3& a, const vec3& b)
{
	return vec3(a.x * (ONE - t) + t * b.x,
				a.y * (ONE - t) + t * b.y,
				a.z * (ONE - t) + t * b.z);
}

void math::swap(vec3& v1, vec3& v2)
{
	std::swap(v1.x, v2.x);
	std::swap(v1.y, v2.y);
	std::swap(v1.z, v2.z);
}

//--------------------------------------------------------------------------------------------
// Wektor 4D.
//--------------------------------------------------------------------------------------------
const vec4 vec4::ZERO_VEC4		= vec4(ZERO, ZERO, ZERO, ZERO);
const vec4 vec4::ONE_VEC4		= vec4( ONE,  ONE,  ONE,  ONE);
const vec4 vec4::POSITIVE_X		= vec4( ONE, ZERO, ZERO, ZERO);
const vec4 vec4::POSITIVE_Y		= vec4(ZERO,  ONE, ZERO, ZERO);
const vec4 vec4::POSITIVE_Z		= vec4(ZERO, ZERO,  ONE, ZERO);
const vec4 vec4::POSITIVE_W		= vec4(ZERO, ZERO, ZERO,  ONE);
const vec4 vec4::NEGATIVE_X		= vec4(-ONE, ZERO, ZERO, ZERO);
const vec4 vec4::NEGATIVE_Y		= vec4(ZERO, -ONE, ZERO, ZERO);
const vec4 vec4::NEGATIVE_Z		= vec4(ZERO, ZERO, -ONE, ZERO);
const vec4 vec4::NEGATIVE_W		= vec4(ZERO, ZERO, ZERO, -ONE);

scalar vec4::length() const		
{ 
	return sqrtf(sqr_length());		
}

scalar vec4::normalize()
{
	scalar norm = sqrtf(x*x + y*y + z*z + w*w);
	if (norm > EPSILON)
		norm = ONE / norm;
	else
		norm = ZERO;
		
	x *= norm;
	y *= norm;
	z *= norm;
	w *= norm;
	return norm;
}

scalar vec4::dot_product(const vec4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

vec4 math::lerp(const scalar& t, const vec4& a, const vec4& b)
{
	return vec4(a.x * (ONE - t) + t * b.x,
				a.y * (ONE - t) + t * b.y,
				a.z * (ONE - t) + t * b.z,
				a.w * (ONE - t) + t * b.w);
}

void math::swap(vec4& v1, vec4& v2)
{
	std::swap(v1.x, v2.x);
	std::swap(v1.y, v2.y);
	std::swap(v1.z, v2.z);
	std::swap(v1.w, v2.w);
}