#ifndef __COMMON_MATH_VEC_HPP__
#define __COMMON_MATH_VEC_HPP__

namespace math
{
	//--------------------------------------------------------------------------------------------
	// Wektor 2D.
	//--------------------------------------------------------------------------------------------	
	struct TFNCOMMON_EXPORT vec2
	{
		static const vec2 ZERO_VEC2;
		static const vec2 ONE_VEC2;
		static const vec2 POSITIVE_X;
		static const vec2 POSITIVE_Y;		
		static const vec2 NEGATIVE_X;
		static const vec2 NEGATIVE_Y;	
	
		vec2() {}
		vec2(scalar nx, scalar ny): x(nx), y(ny) {}
		vec2(const scalar* xy): x(xy[0]), y(xy[1]) {}
		vec2(const vec2& u): x(u.x), y(u.y) {}		
		vec2(const vec3& u);
		
		bool operator==(const vec2& u) const
		{
			return (u.x == x) && (u.y == y) ? true : false;
		}
		
		bool operator!=(const vec2& u) const
		{
			return !(*this == u);
		}
		
		bool operator<(const vec2& u) const
		{
			return ((x < u.x) && (y < u.y));
		}

		bool operator<=(const vec2& u) const
		{
			return ((x <= u.x) && (y <= u.y));
		}
		
		vec2 operator-() const
		{
			return vec2(-x, -y);
		}		
		
		scalar& operator[](utl::int8_t i)
		{
			return v[i];
		}
		
		const scalar operator[](utl::int8_t i) const
		{
			return v[i];
		}
		
		vec2 operator + (const vec2& v) const	{ return vec2(x + v.x, y + v.y); }
		vec2 operator - (const vec2& v) const	{ return vec2(x - v.x, y - v.y); }
		
		vec2& operator +=(const vec2& v)	{ x += v.x; y += v.y; return (*this); }
		vec2& operator -=(const vec2& v)	{ x -= v.x; y -= v.y; return (*this); }
		vec2& operator /=(const scalar& a)	{ x /= a; y /= a; return (*this); }
		vec2& operator *=(const scalar& a)	{ x *= a; y *= a; return (*this); }
		
		scalar dot_product(const vec2& v) const;
		scalar cross_product(const vec2& v) const;
		
		vec2 perpendicular() const;
		vec2 reflect(const vec2& v, const vec2& N) const;
		vec2 project(const vec2& v, const vec2& N) const;
		
		scalar sqr_length() const	{ return x*x + y*y;				}
		scalar length() const;
		scalar normalize();				
		
		union
		{
			struct	{	scalar x, y;	};			
			struct	{	scalar s, t;	};
			scalar v[2];	
		};
	};
	
	TFNCOMMON_EXPORT vec2 lerp(const scalar& t, const vec2& a, const vec2& b);
	TFNCOMMON_EXPORT void swap(vec2& v1, vec2& v2);
	
	inline vec2 operator* (const scalar& a, const vec2& v)	{ return vec2(v.x * a, v.y * a); }
	inline vec2 operator/ (const scalar& a, const vec2& v)	{ return vec2(v.x / a, v.y / a); }
	inline vec2 operator* (const vec2& v, const scalar& a)	{ return vec2(v.x * a, v.y * a); }
	inline vec2 operator/ (const vec2& v, const scalar& a)	{ return vec2(v.x / a, v.y / a); }
		
	//--------------------------------------------------------------------------------------------
	// Wektor 3D.
	//--------------------------------------------------------------------------------------------
	struct TFNCOMMON_EXPORT vec3
	{
		static const vec3 ZERO_VEC3;
		static const vec3 ONE_VEC3;
		static const vec3 POSITIVE_X;
		static const vec3 POSITIVE_Y;
		static const vec3 POSITIVE_Z;
		static const vec3 NEGATIVE_X;
		static const vec3 NEGATIVE_Y;
		static const vec3 NEGATIVE_Z;
			
		vec3() {}
		vec3(scalar nx, scalar ny, scalar nz): x(nx), y(ny), z(nz) {}
		vec3(const scalar* xyz): x(xyz[0]), y(xyz[1]), z(xyz[2]) {}
		vec3(const vec3& u): x(u.x), y(u.y), z(u.z) {}		
		vec3(const vec4& u);
		
		bool operator==(const vec3& u) const
		{
			return (u.x == x) && (u.y == y) && (u.z == z) ? true : false;
		}
		
		bool operator!=(const vec3& u) const
		{
			return !(*this == u);
		}
		
		bool operator<(const vec3& u) const
		{
			return ((x < u.x) && (y < u.y) && (z < u.z));
		}

		bool operator<=(const vec3& u) const
		{
			return ((x <= u.x) && (y <= u.y) && (z <= u.z));
		}
		
		vec3 operator-() const
		{
			return vec3(-x, -y, -z);
		}		
		
		scalar& operator[](utl::int8_t i)
		{
			return v[i];
		}
		
		const scalar operator[](utl::int8_t i) const
		{
			return v[i];
		}
		
		vec3 operator + (const vec3& v) const	{ return vec3(x + v.x, y + v.y, z + v.z); }
		vec3 operator - (const vec3& v) const	{ return vec3(x - v.x, y - v.y, z - v.z); }
		
		vec3& operator +=(const vec3& v)	{ x += v.x; y += v.y; z += v.z; return (*this); }
		vec3& operator -=(const vec3& v)	{ x -= v.x; y -= v.y; z -= v.z; return (*this); }
		vec3& operator /=(const scalar& a)	{ x /= a; y /= a; z /= a; return (*this); }
		vec3& operator *=(const scalar& a)	{ x *= a; y *= a; z *= a; return (*this); }
		
		scalar dot_product(const vec3& v) const;
		vec3 cross_product(const vec3& v) const;
		
		vec3 reflect(const vec3& v, const vec3& N) const;
		vec3 project(const vec3& v, const vec3& N) const;		
		
		scalar sqr_length() const	{ return x*x + y*y + z*z;		}
		scalar length() const;
		scalar normalize();

		vec3 rotate_x(math::scalar ang) const;
		vec3 rotate_y(math::scalar ang) const;
		vec3 rotate_z(math::scalar ang) const;
		
		union
		{
			struct	{	scalar x, y, z;	};			
			struct	{	scalar s, t, r;	};
			scalar v[3];	
		};
	};	
	
	TFNCOMMON_EXPORT vec3 lerp(const scalar& t, const vec3& a, const vec3& b);
	TFNCOMMON_EXPORT void swap(vec3& v1, vec3& v2);
	
	inline vec3 operator* (const scalar& a, const vec3& v)	{ return vec3(v.x * a, v.y * a, v.z * a); }
	inline vec3 operator/ (const scalar& a, const vec3& v)	{ return vec3(v.x / a, v.y / a, v.z / a); }
	inline vec3 operator* (const vec3& v, const scalar& a)	{ return vec3(v.x * a, v.y * a, v.z * a); }
	inline vec3 operator/ (const vec3& v, const scalar& a)	{ return vec3(v.x / a, v.y / a, v.z / a); }	

	//--------------------------------------------------------------------------------------------
	// Wektor 4D.
	//--------------------------------------------------------------------------------------------
	struct TFNCOMMON_EXPORT vec4
	{
		static const vec4 ZERO_VEC4;
		static const vec4 ONE_VEC4;
		static const vec4 POSITIVE_X;
		static const vec4 POSITIVE_Y;
		static const vec4 POSITIVE_Z;
		static const vec4 POSITIVE_W;
		static const vec4 NEGATIVE_X;
		static const vec4 NEGATIVE_Y;
		static const vec4 NEGATIVE_Z;
		static const vec4 NEGATIVE_W;
	
		vec4() {}
		vec4(scalar nx, scalar ny, scalar nz, scalar nw): x(nx), y(ny), z(nz), w(nw) {}
		vec4(const scalar* xyzw): x(xyzw[0]), y(xyzw[1]), z(xyzw[2]), w(xyzw[3]) {}
		vec4(const vec4& u): x(u.x), y(u.y), z(u.z), w(u.w) {}		
		
		bool operator==(const vec4& u) const
		{
			return (u.x == x) && (u.y == y) && (u.z == z) && (u.w == w) ? true : false;
		}
		
		bool operator!=(const vec4& u) const
		{
			return !(*this == u);
		}
		
		bool operator<(const vec4& u) const
		{
			return ((x < u.x) && (y < u.y) && (z < u.z) && (w < u.w));
		}

		bool operator<=(const vec4& u) const
		{
			return ((x <= u.x) && (y <= u.y) && (z <= u.z) && (w <= u.w));
		}
		
		vec4 operator-() const
		{
			return vec4(-x, -y, -z, -w);
		}
		
		scalar& operator[](utl::int8_t i)
		{
			return v[i];
		}
		
		const scalar operator[](utl::int8_t i) const
		{
			return v[i];
		}
	
		vec4 operator + (const vec4& v) const	{ return vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
		vec4 operator - (const vec4& v) const	{ return vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
		
		vec4& operator +=(const vec4& v)	{ x += v.x; y += v.y; z += v.z; w += v.w; return (*this); }
		vec4& operator -=(const vec4& v)	{ x -= v.x; y -= v.y; z -= v.z; w -= v.w; return (*this); }
		vec4& operator /=(const scalar& a)	{ x /= a; y /= a; z /= a; w /= a; return (*this); }
		vec4& operator *=(const scalar& a)	{ x *= a; y *= a; z *= a; w *= a; return (*this); }
		
		scalar dot_product(const vec4& v) const;	
		
		scalar sqr_length() const	{ return x*x + y*y + z*z + w*w;		}
		scalar length() const;
		scalar normalize();		
		
		union
		{
			struct	{	scalar x, y, z, w;	};			
			struct	{	scalar s, t, r, q;	};
			scalar v[4];	
		};
	};
	
	TFNCOMMON_EXPORT vec4 lerp(const scalar& t, const vec4& a, const vec4& b);
	TFNCOMMON_EXPORT void swap(vec4& v1, vec4& v2);
	
	inline vec4 operator* (const scalar& a, const vec4& v)	{ return vec4(v.x * a, v.y * a, v.z * a, v.w * a); }
	inline vec4 operator/ (const scalar& a, const vec4& v)	{ return vec4(v.x / a, v.y / a, v.z / a, v.w / a); }
	inline vec4 operator* (const vec4& v, const scalar& a)	{ return vec4(v.x * a, v.y * a, v.z * a, v.w * a); }
	inline vec4 operator/ (const vec4& v, const scalar& a)	{ return vec4(v.x / a, v.y / a, v.z / a, v.w / a); }	
	
	//--------------------------------------------------------------------------------------------
	// Pomocznicze funkcje.
	//--------------------------------------------------------------------------------------------
	TFNCOMMON_EXPORT void is_valid(const vec2& v);
	TFNCOMMON_EXPORT void is_valid(const vec3& v);
	TFNCOMMON_EXPORT void is_valid(const vec4& v);	
	
} // end namespace math

#endif