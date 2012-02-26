#ifndef __COMMON_MATH_QUAT_HPP__
#define __COMMON_MATH_QUAT_HPP__

namespace math
{
	//--------------------------------------------------------------------------------------------
	// Kwaternion.
	//--------------------------------------------------------------------------------------------
	struct TFNCOMMON_EXPORT quat
	{
		static const quat ZERO_QUAT;
		static const quat IDENTITY;
		
		quat() { zero_quat(); }
		quat(scalar a, scalar b, scalar c, scalar d): x(a), y(b), z(c), w(d) {}
		quat(const scalar* q): x(q[0]), y(q[1]), z(q[2]), w(q[3]) {}		
	
		bool operator==(const quat& q) const	
		{ 
			return ((x == q.x) && (y == q.y) && (z == q.z) && (w == q.w)); 
		}
		
		bool operator!=(const quat& q) const	
		{ 
			return !(*this == q);
		}
	
		bool operator<(const quat& u) const
		{
			return ((x < u.x) && (y < u.y) && (z < u.z) && (w < u.w));
		}

		bool operator<=(const quat u) const
		{
			return ((x <= u.x) && (y <= u.y) && (z <= u.z) && (w <= u.w));
		}
		
		quat operator-() const
		{
			return quat(-x, -y, -z, -w);
		}	
	
		scalar& operator[] (utl::int8_t i)				
		{ 
			return Q[i]; 
		}
		
		const scalar& operator[] (utl::int8_t i) const	
		{
			return Q[i];
		}	
		
		quat operator + (const quat& q) const { return quat(x + q.x, y + q.y, z + q.z, w + q.w); }
		quat operator - (const quat& q) const { return quat(x - q.x, y - q.y, z - q.z, w - q.w); }
		inline quat operator * (const quat& q) const;
		
		quat& operator += (const quat& q) { x += q.x; y += q.y; z += q.z; w += q.w; return (*this); }
		quat& operator -= (const quat& q) { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return (*this); }
		quat& operator *= (const quat& q) { (*this) = (*this) * q; return (*this); }
		
		quat& operator *= (const scalar& q) { x *= q; y *= q; z *= q; w *= q; return (*this); }
		quat& operator /= (const scalar& q) { x /= q; y /= q; z /= q; w /= q; return (*this); }
	
		inline scalar angle() const;
		inline vec3 axis() const;
		
		inline void zero_quat();
		inline void identity_quat();
		
		void from_axis_and_angle(const vec3& axis, const scalar& angle);
		void from_matrix(const mat4& m);
		mat4 to_matrix() const;
		
		inline void rot_x(const scalar& a);
		inline void rot_y(const scalar& a);
		inline void rot_z(const scalar& a);
		
		void inverse();
		inline void conjugate();
		inline void normalize();
		
		scalar dot_product(const quat& q) const	{ return x * q.x + y * q.y + z * q.z + w * q.w; }		
		inline scalar length() const;	
		inline scalar sqr_length() const		{ return x * x + y * y + z * z + w * w;			}
		
		union 
		{
			struct 
			{
				scalar x, y, z, w;
			};
			
			scalar Q[4];
		};
	};
	
	TFNCOMMON_EXPORT quat lerp(const scalar& t, const quat& a, const quat& b);
	TFNCOMMON_EXPORT quat slerp(const scalar& t, const quat& a, const quat& b);
	TFNCOMMON_EXPORT void swap(quat& q1, quat& q2);
	
	TFNCOMMON_EXPORT quat	operator * (const quat& q, const scalar& a);
	TFNCOMMON_EXPORT quat	operator / (const quat& q, const scalar& a);
	TFNCOMMON_EXPORT quat	operator * (const scalar& a, const quat& q);
	TFNCOMMON_EXPORT quat	operator / (const scalar& a, const quat& q);

} // end namespace math

#endif