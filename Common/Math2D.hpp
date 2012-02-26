/// \file Math2D.hpp
/// \author Wojtek "afronski" Gawroñski
/// \date 02.03.2009
///
/// \brief 2D Math objects.
/// 
/// File contains 2D objects, operations on this objects and geometry algorithms.

#ifndef __COMMON_MATH_2D_HPP__
#define __COMMON_MATH_2D_HPP__

namespace math
{
	//--------------------------------------------------------------------------------------------
	// Prostok¹t.
	//--------------------------------------------------------------------------------------------
	struct TFNCOMMON_EXPORT rect
	{
		rect(): left(ZERO), top(ZERO), right(ZERO), bottom(ZERO) {}
		rect(scalar a, scalar b, scalar c, scalar d): left(a), top(b), right(c), bottom(d) {}
		rect(const scalar* t): left(t[0]), top(t[1]), right(t[2]), bottom(t[3]) {}
		
		bool operator==(const rect& r) const
		{
			return (left == r.left) && (top == r.top) && (right == r.right) && (bottom == r.bottom) ? true : false;
		}
		
		bool operator!=(const rect& r) const
		{
			return !(*this == r);
		}
		
		bool operator<(const rect& r) const
		{
			return (left < r.left) && (top < r.top) && (right < r.right) && (bottom < r.bottom);
		}
		
		bool operator<=(const rect& r) const
		{
			return (left <= r.left) && (top <= r.top) && (right <= r.right) && (bottom <= r.bottom);
		}
		
		void clear();
		void move(const vec2& translate);
		void move_to(const vec2& pos);
		void scale(scalar scale);
		bool is_valid() const;
		
		scalar left, top, bottom, right;
	};
	
	TFNCOMMON_EXPORT void swap(rect& r1, rect& r2);	
	TFNCOMMON_EXPORT rect union_rectangle(const rect& r1, const rect& r2);
	TFNCOMMON_EXPORT rect intersection_rectangle(const rect& r1, const rect& r2);
	TFNCOMMON_EXPORT bool collision(const rect& r1, const rect& r2);
	TFNCOMMON_EXPORT bool collision(const rect& r, const vec2& p);	
	
	//--------------------------------------------------------------------------------------------
	// Okr¹g.
	//--------------------------------------------------------------------------------------------
	struct TFNCOMMON_EXPORT circle
	{
		circle(): center(ZERO, ZERO), R(ZERO) {}
		circle(const vec2& v, scalar r): center(v), R(r) {}
		circle(const scalar* t): center(t[0], t[1]), R(t[2]) {}

		bool operator==(const circle& c) const
		{
			return (R == c.R) && (center == c.center) ? true : false;
		}
		
		bool operator!=(const circle& c) const
		{
			return !(*this == c);
		}
		
		bool operator<(const circle& c) const
		{
			return ((R < c.R) && (center < c.center));
		}
		
		bool operator<=(const circle& c) const
		{
			return ((R <= c.R) && (center <= c.center));
		}
		
		void clear();
		void move(const vec2& translate);	
		void move_to(const vec2& pos);	
		void scale(scalar scale);
	
		vec2	center;
		scalar	R;
	};

	TFNCOMMON_EXPORT void swap(circle& c1, circle& c2);	
	TFNCOMMON_EXPORT bool collision(const circle& c1, const circle& c2);
	TFNCOMMON_EXPORT bool collision(const circle& c, const vec2& p);		
	
	//--------------------------------------------------------------------------------------------
	// Procedury i funkcje dotycz¹ce powy¿szych klas.	
	//--------------------------------------------------------------------------------------------	
	// Pole trójk¹ta.	 
	TFNCOMMON_EXPORT inline scalar triangle_area(const vec2& a, const vec2& b, const vec2& c);
	TFNCOMMON_EXPORT inline scalar triangle_area(const vec3& a, const vec3& b, const vec3& c);
	
	// Obwód trójk¹ta.
	TFNCOMMON_EXPORT inline scalar triangle_perimeter(const vec2& a, const vec2& b, const vec2& c);
	TFNCOMMON_EXPORT inline scalar triangle_perimeter(const vec3& a, const vec3& b, const vec3& c);	
	
	// Odleg³oœæ punktów.
	TFNCOMMON_EXPORT inline scalar distance(const vec2& a, const vec2& b);
	TFNCOMMON_EXPORT inline scalar distance(const vec3& a, const vec3& b);
	TFNCOMMON_EXPORT inline scalar distance(const vec4& a, const vec4& b);
	
	// Okr¹g wpisany w trójk¹t.
	TFNCOMMON_EXPORT inline void triangle_inscribed_circle(circle& cir, const vec2& a, const vec2& b, const vec2& c);
	TFNCOMMON_EXPORT inline void triangle_inscribed_circle(circle& cir, const vec3& a, const vec3& b, const vec3& c);
	
	// Okr¹g opisany na trójk¹cie.
	TFNCOMMON_EXPORT inline void triangle_circumscribed_circle(circle& cir, const vec2& a, const vec2& b, const vec2& c);
	TFNCOMMON_EXPORT inline void triangle_circumscribed_circle(circle& cir, const vec3& a, const vec3& b, const vec3& c);	

} // end namespace math

#endif