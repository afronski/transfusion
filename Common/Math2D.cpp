#include "pchCommon.hpp"

#include "MathVec.hpp"
#include "Math2D.hpp"

using namespace math;

//--------------------------------------------------------------------------------------------
// Prostok¹t.
//--------------------------------------------------------------------------------------------
void rect::clear()
{
	left = right = top = bottom = ZERO;
}

void rect::move(const vec2& translate)
{
	left += translate.x;
	top += translate.y;	
	right += translate.x;
	bottom += translate.y;
}

void rect::move_to(const vec2& pos)
{
	scalar width = right - left;
	scalar height = bottom - top;
	left = pos.x;
	top = pos.y;
	right = pos.x + width;
	bottom = pos.y + height;
}

void rect::scale(scalar scale)
{
	left *= scale;
	top *= scale;
	right *= scale;
	bottom *= scale;
}

bool rect::is_valid() const
{
	return ((left <= right) && (top >= bottom));
}

void math::swap(rect& r1, rect& r2)
{
	std::swap(r1.left, r2.left);
	std::swap(r1.top, r2.top);
	std::swap(r1.right, r2.right);
	std::swap(r1.bottom, r2.bottom);
}

rect math::union_rectangle(const rect& r1, const rect& r2)
{
	rect res;
	res.left = min_scalar(r1.left, r2.left);
	res.top = max_scalar(r1.top, r2.top);
	res.right = max_scalar(r1.right, r2.right);
	res.bottom = min_scalar(r1.bottom, r2.bottom);
	return res;
}

rect math::intersection_rectangle(const rect& r1, const rect& r2)
{
	rect res;
	res.left = max_scalar(r1.left, r2.left);
	res.top = min_scalar(r1.top, r2.top);
	res.right = min_scalar(r1.right, r2.right);
	res.bottom = max_scalar(r1.bottom, r2.bottom);
	if (res.is_valid())
		return res;
	else
		return rect(ZERO, ZERO, ZERO, ZERO);
}

bool math::collision(const rect& r1, const rect& r2)
{	
	return ((r1.left <= r2.right) && (r1.right >= r2.left) && (r1.top >= r2.bottom) && (r1.bottom <= r2.top));
}

bool math::collision(const rect& r, const vec2& p)
{	
	return ((p.x >= r.left) && (p.x <= r.right) && (p.y <= r.top) && (p.y >= r.bottom));
}

//--------------------------------------------------------------------------------------------
// Okr¹g.
//--------------------------------------------------------------------------------------------
void circle::clear()
{
	R = ZERO;
	center = vec2(ZERO, ZERO);
}

void circle::move(const vec2& translate)
{
	center += translate;
}

void circle::move_to(const vec2& pos)
{
	center = pos;
}

void circle::scale(scalar scale)
{
	R *= scale;
}

void math::swap(circle& c1, circle& c2)
{
	std::swap(c1.R, c2.R);
	std::swap(c1.center, c2.center);
}

bool math::collision(const circle& c1, const circle& c2)
{
	return (distance(c1.center, c2.center) <= (c1.R + c2.R));
}

bool math::collision(const circle& c, const vec2& p)
{
	return (distance(c.center, p) <= c.R);
}

//--------------------------------------------------------------------------------------------
// Funkcje dodatkowe dotycz¹ce powy¿szych klas.
//--------------------------------------------------------------------------------------------
// Pole trójk¹ta.	 
inline scalar math::triangle_area(const vec2& a, const vec2& b, const vec2& c)
{
	return ZERO_5 * ((a.y - c.y) * (b.x - c.x) + (b.y - c.y) * (c.x - a.x));
}

inline scalar math::triangle_area(const vec3& a, const vec3& b, const vec3& c)
{
	return ZERO_5 *(c - b).cross_product(a - c).length();
}

// Obwód trójk¹ta.
inline scalar math::triangle_perimeter(const vec2& a, const vec2& b, const vec2& c)
{
	return distance(a, b) + distance(b, c) + distance(c, a);
}

inline scalar math::triangle_perimeter(const vec3& a, const vec3& b, const vec3& c)
{
	return distance(a, b) + distance(b, c) + distance(c, a);
}

// Odleg³oœæ punktów.
inline scalar math::distance(const vec2& a, const vec2& b)
{
	return (b - a).length();
}

inline scalar math::distance(const vec3& a, const vec3& b)
{
	return (b - a).length();
}

inline scalar math::distance(const vec4& a, const vec4& b)
{
	return (b - a).length();
}

// Okr¹g wpisany w trójk¹t.
inline void math::triangle_inscribed_circle(circle& cir, const vec2& a, const vec2& b, const vec2& c)
{
	scalar area = triangle_area(a, b, c);
	if (area < EPSILON)
	{
		cir.center = a;
		cir.R = ZERO;
	}
	
	scalar oneoverperim = ONE / triangle_perimeter(a, b, c);
	
	cir.center = a * (b - c).length();
	cir.center += b * (c - a).length();
	cir.center += c * (a - b).length();
	cir.center *= oneoverperim;
	cir.R = TWO * area * oneoverperim;
}

inline void math::triangle_inscribed_circle(circle& cir, const vec3& a, const vec3& b, const vec3& c)
{
	scalar area = triangle_area(a, b, c);
	if (area < EPSILON)
	{
		cir.center = a;
		cir.R = ZERO;
	}
	
	scalar oneoverperim = ONE / triangle_perimeter(a, b, c);
	
	cir.center = a * (b - c).length();
	cir.center += b * (c - a).length();
	cir.center += c * (a - b).length();
	cir.center *= oneoverperim;
	cir.R = TWO * area * oneoverperim;
}

// Okr¹g opisany na trójk¹cie.
inline void math::triangle_circumscribed_circle(circle& cir, const vec2& a, const vec2& b, const vec2& c)
{
	vec2 e0, e1;
	scalar d1, d2, d3;
	scalar c1, c2, c3, oo_c;

	e0 = c - a;
	e1 = b - a;
	d1 = e0.dot_product(e1);

	e0 = c - b;
	e1 = a - b;
	d2 = e0.dot_product(e1);

	e0 = a - c;
	e1 = b - c;
	d3 = e0.dot_product(e1);

	c1 = d2 * d3;
	c2 = d3 * d1;
	c3 = d1 * d2;
	oo_c = ONE / (c1 + c2 + c3);

	cir.center  = a * (c2 + c3);
	cir.center += b * (c3 + c1);
	cir.center += c * (c1 + c2);
	cir.center *= oo_c * ZERO_5;

	cir.R = ZERO_5 * sqrtf((d1 + d2) * (d2 + d3) * (d3 + d1) * oo_c);
}

inline void math::triangle_circumscribed_circle(circle& cir, const vec3& a, const vec3& b, const vec3& c)
{
	vec2 e0, e1;
	scalar d1, d2, d3;
	scalar c1, c2, c3, oo_c;

	e0 = c - a;
	e1 = b - a;
	d1 = e0.dot_product(e1);

	e0 = c - b;
	e1 = a - b;
	d2 = e0.dot_product(e1);

	e0 = a - c;
	e1 = b - c;
	d3 = e0.dot_product(e1);

	c1 = d2 * d3;
	c2 = d3 * d1;
	c3 = d1 * d2;
	oo_c = ONE / (c1 + c2 + c3);

	cir.center  = a * (c2 + c3);
	cir.center += b * (c3 + c1);
	cir.center += c * (c1 + c2);
	cir.center *= oo_c * ZERO_5;

	cir.R = ZERO_5 * sqrtf((d1 + d2) * (d2 + d3) * (d3 + d1) * oo_c);
}