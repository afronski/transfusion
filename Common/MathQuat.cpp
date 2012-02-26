#include "pchCommon.hpp"

#include "MathVec.hpp"
#include "MathMat.hpp"
#include "MathQuat.hpp"

using namespace math;

//--------------------------------------------------------------------------------------------
// Kwaternion.
//--------------------------------------------------------------------------------------------
const quat quat::ZERO_QUAT	= quat(ZERO, ZERO, ZERO, ZERO);
const quat quat::IDENTITY	= quat(ZERO, ZERO, ZERO,  ONE);

inline quat quat::operator * (const quat& q) const
{
	return quat(w*q.x + x*q.w + y*q.z - z*q.y,
				w*q.y + y*q.w + z*q.x - x*q.z,
				w*q.z + z*q.w + x*q.y - y*q.x,
				w*q.w - x*q.x - y*q.y - z*q.z);
}

inline scalar quat::angle() const
{
	return acosf(w) * TWO;
}

inline vec3 quat::axis() const
{
	scalar f = ONE - w * w;
	if (f <= ZERO)
		return vec3(ZERO, ZERO, ZERO);
		
	scalar oneoversinthetaover2 = ONE / sqrtf(f);
	return vec3(x * oneoversinthetaover2,
				y * oneoversinthetaover2,
				z * oneoversinthetaover2);
}

inline void quat::zero_quat()
{
	x = ZERO; y = ZERO; z = ZERO; w = ZERO;
}
inline void quat::identity_quat()
{
	x = ZERO; y = ZERO; z = ZERO; w = ONE;
}

void quat::from_axis_and_angle(const vec3& axis, const scalar& angle)
{
	scalar Sin = sinf(angle);
	x = Sin * axis.x;
	y = Sin * axis.y;
	z = Sin * axis.z;
	w = cosf(angle * ZERO_5);
}

void quat::from_matrix(const mat4& m)
{
	m.get_rotation(*this);
}

mat4 quat::to_matrix() const
{
	scalar	xx = x * x, yy = y * y,
			zz = z * z, xy = x * y,
			xz = x * z, yz = y * z,
			wx = w * x,	wy = w * y,
			wz = w * z;
	
	return mat4(ONE - TWO * (yy + zz),		TWO * (xy + wz),			TWO * ( xz - wy ),			ZERO,
				TWO * (xy - wz),			ONE - TWO * ( xx + zz ),	TWO * ( yz + wx ),			ZERO,
				TWO * (xz + wy),			TWO * ( yz - wx ),			ONE - TWO * ( xx + yy ),	ZERO,
				ZERO,                       ZERO,						ZERO,						ONE);
}

inline void quat::rot_x(const scalar& a)
{
	x = sinf(a * ZERO_5);
	y = ZERO;
	z = ZERO;
	w = cosf(a * ZERO_5);
}

inline void quat::rot_y(const scalar& a)
{
	x = ZERO;
	y = sinf(a * ZERO_5);
	z = ZERO;
	w = cosf(a * ZERO_5);
}

inline void quat::rot_z(const scalar& a)
{
	x = ZERO;
	y = ZERO;
	z = sinf(a * ZERO_5);
	w = cosf(a * ZERO_5);
}

void quat::inverse()
{
	scalar norm = ONE / sqrtf(x * x + y * y + z * z + w * w);
	w *= norm; 
	x *= -norm; 
	y *= -norm; 
	z *= -norm; 
}

inline void quat::conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

inline scalar quat::length() const			
{ 
	return sqrtf(x * x + y * y + z * z + w * w);
}	

inline void quat::normalize()
{
	scalar n = ONE / sqrtf(x * x + y * y + z * z + w * w);
	x *= n;
	y *= n;
	z *= n;
	w *= n;
}

inline quat lerp(const scalar& t, const quat& a, const quat& b)
{
	scalar w1 = t;
	scalar w2 = ONE - t;

	return quat((a.x * w1) + (b.x * w2),
				(a.y * w1) + (b.y * w2),
				(a.z * w1) + (b.z * w2),
				(a.w * w1) + (b.w * w2));
}

inline quat slerp(const scalar& t, const quat& a, const quat& b)
{	
	scalar cosomega = a.dot_product(b);	
	quat new_b = b;
	if (cosomega < ZERO)
	{
		new_b.x = -new_b.x;
		new_b.y = -new_b.y;
		new_b.z = -new_b.z;
		new_b.w = -new_b.w;
	}

	scalar k0, k1;
	if (cosomega > 0.9999f)
	{
		k0 = ONE - t;
		k1 = t;
	}
	else
	{
		scalar sinomega = sqrtf(ONE - cosomega*cosomega);
		scalar omega = atan2f(sinomega, cosomega);
		scalar oneoversinomega = ONE / sinomega;
		
		k0 = sinf((ONE - t) * omega) * oneoversinomega;
		k1 = sinf(t * omega) * oneoversinomega;
	}
	
	return quat(a.x * k0 + new_b.x * k1,
				a.y * k0 + new_b.y * k1,
				a.z * k0 + new_b.z * k1,	
				a.w * k0 + new_b.w * k1);
}

inline void swap(quat& q1, quat& q2)
{
	std::swap(q1.x, q2.x);
	std::swap(q1.y, q2.y);
	std::swap(q1.z, q2.z);
	std::swap(q1.w, q2.w);
}

inline quat	operator * (const quat& q, const scalar& a)
{
	return quat(q.x * a, q.y * a, q.z * a, q.w * a);
}

inline quat	operator / (const quat& q, const scalar& a)
{
	return quat(q.x / a, q.y / a, q.z / a, q.w / a);
}

inline quat	operator * (const scalar& a, const quat& q)
{
	return quat(q.x * a, q.y * a, q.z * a, q.w * a);
}

inline quat	operator / (const scalar& a, const quat& q)
{
	return quat(q.x * a, q.y * a, q.z * a, q.w * a);
}