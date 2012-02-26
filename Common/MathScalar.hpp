#ifndef __COMMON_MATH_SCALAR_HPP__
#define __COMMON_MATH_SCALAR_HPP__

namespace math
{
	//--------------------------------------------------------------------------------------------
	// Sta³e i typ scalar.
	//--------------------------------------------------------------------------------------------	
	typedef float scalar;
	
	const scalar ZERO				= scalar(0);
	const scalar ONE				= scalar(1);
	const scalar ZERO_5				= scalar(0.5);
	const scalar TWO				= scalar(2.0);
	
	const scalar PI					= scalar(3.14159265358979323846264338327950288419716939937510582);
	const scalar HALF_PI			= scalar(3.14159265358979323846264338327950288419716939937510582 * 0.5);
	const scalar TWO_PI				= scalar(3.14159265358979323846264338327950288419716939937510582 * 2.0);
	
	const scalar EPSILON			= scalar(10e-6);
	
	const scalar TO_RAD				= PI / scalar(180);
	const scalar TO_DEG				= scalar(180) / PI;
	
	extern const scalar POS_INF;
	extern const scalar NEG_INF;
	
	//--------------------------------------------------------------------------------------------
	// Deklaracje.
	//--------------------------------------------------------------------------------------------	
	struct vec2;
	struct vec3;
	struct vec4;
	
	struct quat;
	
	struct mat3;
	struct mat4;
	
	struct rect;
	struct circle;
	
	struct color3;
	struct color4;
		
	//--------------------------------------------------------------------------------------------
	// Dodatki.
	//--------------------------------------------------------------------------------------------		
	void is_valid(const scalar& a);	
	
	inline bool is_pow_2(utl::int32_t a)
	{
		return ((a & (a-1)) == 0);
	}
	
	inline utl::int32_t next_pow_2(utl::int32_t a)
	{
		utl::int32_t rval = 2;
		while(rval < a) rval <<= 1;
		return rval;
	}
	
	inline scalar deg_to_rad(scalar theta)
	{
		return theta * TO_RAD;
	}
	
	inline scalar rad_to_deg(scalar theta)
	{
		return theta * TO_DEG;
	}
	
	inline bool equal(scalar a, scalar b)
	{
		return ((a < b + EPSILON) && (a > b - EPSILON)) ? true : false;
	}
	
	inline scalar max_scalar(scalar a, scalar b)
	{
		return (a > b) ? a : b;
	}
	
	inline scalar min_scalar(scalar a, scalar b)
	{
		return (a < b) ? a : b;
	}
	
	inline scalar clamp(scalar u, scalar min, scalar max)
	{
		u = (u < min) ? min : u;
		u = (u > max) ? max : u;
		return u;
	}
	
	inline scalar lerp(scalar t, scalar a, scalar b)
	{
		return a * (ONE - t) + t * b;
	}	
	
	//--------------------------------------------------------------------------------------------
	// Procedury i funkcje dotycz¹ce powy¿szych klas.	
	//--------------------------------------------------------------------------------------------		
	// Wyznacznik macierzy:
	// | a1 a2 |
	// | b1 b2 |
	TFNCOMMON_EXPORT scalar det2x2(scalar a1, scalar a2, scalar b1, scalar b2);
	
	// Wyznacznik macierzy:
	// | a1 a2 a3 | 
	// | b1 b2 b3 |
	// | c1 c2 c3 |
	TFNCOMMON_EXPORT scalar det3x3(scalar a1, scalar a2, scalar a3, 
								   scalar b1, scalar b2, scalar b3,
								   scalar c1, scalar c2, scalar c3);
						 						
} // end namespace math

#endif