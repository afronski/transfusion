#ifndef __COMMON_MATH_MAT_HPP__
#define __COMMON_MATH_MAT_HPP__

namespace math
{	
	//--------------------------------------------------------------------------------------------
	// Macierz 3x3.
	//--------------------------------------------------------------------------------------------
	struct TFNCOMMON_EXPORT mat3
	{
		static const mat3 IDENTITY;
		static const mat3 ZERO_MAT3;
	
		mat3() { zero_matrix(); }
		
		mat3(const scalar* mat):
			a00(mat[0]), a01(mat[1]), a02(mat[2]),
			a10(mat[3]), a11(mat[4]), a12(mat[5]),
			a20(mat[6]), a21(mat[7]), a22(mat[8])	{}
			
		mat3(const mat3& m): 
			a00(m.a00), a01(m.a01), a02(m.a02),
			a10(m.a10), a11(m.a11), a12(m.a12),
			a20(m.a20), a21(m.a21), a22(m.a22)	{}			
		
		mat3(const scalar& f0, const scalar& f1, const scalar& f2, 
			 const scalar& f3, const scalar& f4, const scalar& f5, 
			 const scalar& f6, const scalar& f7, const scalar& f8):
			 a00(f0), a01(f1), a02(f2),
			 a10(f3), a11(f4), a12(f5),
			 a20(f6), a21(f7), a22(f8)	{}
		
		const vec3 col(const utl::int8_t i) const
		{
			return vec3(&M[(i * 3)]);			
		}
		
		const vec3 row(const utl::int8_t i) const
		{
			return vec3(M[i], M[i + 3], M[i + 6]);
		}
		
		const scalar& operator()(const utl::int8_t& i, const utl::int8_t& j) const
		{
			return M[j * 3 + i];
		}
		
		scalar& operator()(const utl::int8_t& i, const utl::int8_t& j)
		{
			return M[j * 3 + i];
		}
		
		void set_row(utl::int8_t i, const vec3& v)
		{
			M[i]		= v.x;
			M[i + 3]	= v.y;
			M[i + 6]	= v.z;
		}
		
		void set_col(utl::int8_t i, const vec3& v)
		{
			M[i * 3]		= v.x;
			M[i * 3 + 1]	= v.y;
			M[i * 3 + 2] 	= v.z;
		}
		
		bool operator==(const mat3& M) const;
		bool operator!=(const mat3& M) const;
		
		void set_rotation(const scalar& theta, const vec3& v);
		void set_rotation(const vec3& u, const vec3& v);
		
		mat3 operator - () const;
		mat3 operator + (const mat3& m) const;
		mat3 operator - (const mat3& m) const;
		mat3 operator * (const mat3& m) const
		{
			return mat3(a00 * m.a00 + a01 * m.a10 + a02 * m.a20,
						a10 * m.a00 + a11 * m.a10 + a12 * m.a20,
						a20 * m.a00 + a21 * m.a10 + a22 * m.a20,
						
						a00 * m.a01 + a01 * m.a11 + a02 * m.a21,
						a10 * m.a01 + a11 * m.a11 + a12 * m.a21,
						a20 * m.a01 + a21 * m.a11 + a22 * m.a21,
						
						a00 * m.a02 + a01 * m.a12 + a02 * m.a22,
						a10 * m.a02 + a11 * m.a12 + a12 * m.a22,
						a20 * m.a02 + a21 * m.a12 + a22 * m.a22);
		}		
		
		mat3& operator += (const mat3& m);
		mat3& operator -= (const mat3& m);
		mat3& operator *= (const mat3& m);
		
		mat3& operator *= (const scalar& a);
		mat3& operator /= (const scalar& a);
		
		inline void zero_matrix();
		inline void identity_matrix();
		
		void inverse();
		inline scalar det() const;
		inline void transpose();
		
		union
		{
			struct
			{
				scalar a00, a01, a02;
				scalar a10, a11, a12;
				scalar a20, a21, a22;
			};
			
			struct
			{
				scalar _11, _12, _13;
				scalar _21, _22, _23;
				scalar _31, _32, _33;
			};		
			
			scalar M[9];
		};
	};
	
	TFNCOMMON_EXPORT void swap(mat3& m1, mat3& m2);

	TFNCOMMON_EXPORT mat3 operator * (const scalar& a, const mat3& m);
	TFNCOMMON_EXPORT mat3 operator / (const scalar& a, const mat3& m);
	TFNCOMMON_EXPORT mat3 operator * (const mat3& m, const scalar& a);
	TFNCOMMON_EXPORT mat3 operator / (const mat3& m, const scalar& a);
	
	TFNCOMMON_EXPORT vec3 operator * (const vec3& v, const mat3& m);
	TFNCOMMON_EXPORT vec3 operator * (const mat3& m, const vec3& v);

	//--------------------------------------------------------------------------------------------
	// Macierz 4x4.
	//--------------------------------------------------------------------------------------------
	struct TFNCOMMON_EXPORT mat4
	{
		static const mat4 IDENTITY;
		static const mat4 ZERO_MAT4;
	
		mat4() { zero_matrix(); }
		
		mat4(const scalar* mat):
			a00(mat[0]),  a01(mat[1]),  a02(mat[2]),  a03(mat[3]),
			a10(mat[4]),  a11(mat[5]),  a12(mat[6]),  a13(mat[7]),
			a20(mat[8]),  a21(mat[9]),  a22(mat[10]), a23(mat[11]),
			a30(mat[12]), a31(mat[13]), a32(mat[14]), a33(mat[15])	{}
			
		mat4(const mat4& m):
			a00(m.a00), a01(m.a01), a02(m.a02), a03(m.a03),
			a10(m.a10), a11(m.a11), a12(m.a12), a13(m.a13),
			a20(m.a20), a21(m.a21), a22(m.a22), a23(m.a23),
			a30(m.a30), a31(m.a31), a32(m.a32), a33(m.a33) {}
		
		mat4(const scalar& f0,  const scalar& f1,  const scalar& f2,  const scalar& f3,
			 const scalar& f4,  const scalar& f5,  const scalar& f6,  const scalar& f7,
			 const scalar& f8,  const scalar& f9,  const scalar& f10, const scalar& f11,
			 const scalar& f12, const scalar& f13, const scalar& f14, const scalar& f15):
			 a00(f0),  a01(f1),  a02(f2),  a03(f3),
			 a10(f4),  a11(f5),  a12(f6),  a13(f7),
			 a20(f8),  a21(f9),  a22(f10), a23(f11),
			 a30(f12), a31(f13), a32(f14), a33(f15)	{}	 
		
		const vec4 col(const utl::int8_t i) const
		{
			return vec4(&M[(i * 4)]);			
		}
		
		const vec4 row(const utl::int8_t i) const
		{
			return vec4(M[i], M[i + 4], M[i + 8], M[i + 12]);
		}
		
		const scalar& operator()(const utl::int8_t& i, const utl::int8_t& j) const
		{
			return M[j * 4 + i];
		}
		
		scalar& operator()(const utl::int8_t& i, const utl::int8_t& j)
		{
			return M[j * 4 + i];
		}
		
		void set_row(utl::int8_t i, const vec4& v)
		{
			M[i]		= v.x;
			M[i + 4]	= v.y;
			M[i + 8]	= v.z;
			M[i + 12]	= v.w;
		}
		
		void set_col(utl::int8_t i, const vec4& v)
		{
			M[i * 4]		= v.x;
			M[i * 4 + 1]	= v.y;
			M[i * 4 + 2] 	= v.z;
			M[i * 4 + 3]	= v.w;
		}		

		bool operator==(const mat4& M) const;
		bool operator!=(const mat4& M) const;
		
		mat3& get_rotation(mat3& rot) const;
		quat& get_rotation(quat& rot) const;
		
		void set_rotation(const quat& q);
		void set_rotation(const mat3& M);
		void set_rotation(const scalar& theta, const vec3& v);
		void set_rotation(const vec3& u, const vec3& v);
		
		vec3& get_translation(vec3& t) const;
		void set_translation(const vec3& t);
		
		vec3& get_scale(vec3& s) const;
		void set_scale(const vec3& s);

		mat4 operator - () const;
		mat4 operator + (const mat4& m) const;
		mat4 operator - (const mat4& m) const;
		mat4 operator * (const mat4& m) const
		{
			return mat4(_11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41,
						_11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42,
						_11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43,
						_11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44,

						_21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41,
						_21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42,
						_21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43,
						_21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44,

						_31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41,
						_31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42,
						_31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43,
						_31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44,

						_41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41,
						_41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42,
						_41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43,
						_41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44);
		}
		
		mat4& operator += (const mat4& m);
		mat4& operator -= (const mat4& m);
		mat4& operator *= (const mat4& m);
		
		mat4& operator *= (const scalar& a);
		mat4& operator /= (const scalar& a);
		
		inline void zero_matrix();
		inline void identity_matrix();
		
		void inverse();
		inline scalar det() const;
		inline void transpose();
					
		union
		{
			struct
			{
				scalar a00, a01, a02, a03;
				scalar a10, a11, a12, a13;
				scalar a20, a21, a22, a23;
				scalar a30, a31, a32, a33;
			};

			struct
			{
				scalar _11, _12, _13, _14;
				scalar _21, _22, _23, _24;			
				scalar _31, _32, _33, _34;
				scalar _41, _42, _43, _44;				
			};			
			
			scalar M[16];
		};
	};
	
	TFNCOMMON_EXPORT void swap(mat4& m1, mat4& m2);	
	
	TFNCOMMON_EXPORT mat4 operator * (const scalar& a, const mat4& m);
	TFNCOMMON_EXPORT mat4 operator / (const scalar& a, const mat4& m);
	TFNCOMMON_EXPORT mat4 operator * (const mat4& m, const scalar& a);
	TFNCOMMON_EXPORT mat4 operator / (const mat4& m, const scalar& a);
	
	TFNCOMMON_EXPORT vec4 operator * (const vec4& v, const mat4& m);
	TFNCOMMON_EXPORT vec4 operator * (const mat4& m, const vec4& v);		

} // end namespace math

#endif