#ifndef __COMMON_MATH_COLOR_HPP__
#define __COMMON_MATH_COLOR_HPP__

namespace math
{
	//--------------------------------------------------------------------------------------------
	// Kolor 3 elementowy zmiennoprzecinkowy.
	//--------------------------------------------------------------------------------------------
    struct TFNCOMMON_EXPORT color3
    {        
		static const color3 WHITE3;
		static const color3 BLACK3;
		static const color3 RED3;
		static const color3 GREEN3;
		static const color3 BLUE3;
		static const color3 GRAY3;         

		color3(): r(ZERO), g(ZERO), b(ZERO) {}
        color3(scalar _r, scalar _g, scalar _b): r(_r), g(_g), b(_b) {}

        bool operator ==(const color3& rhs) const
        {   return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b));  }

        bool operator !=(const color3& rhs) const 
        {   return ((r != rhs.r) || (g != rhs.g) || (b != rhs.b));  }

        bool operator <= (const color3& rhs) const
        {   return ((r <= rhs.r) && (g <= rhs.g) && (b <= rhs.b));  }

        bool operator < (const color3& rhs) const
        {   return ((r < rhs.r) && (g < rhs.g) && (b < rhs.b));     }

        utl::int32_t XRGB() const;
        utl::int32_t XBGR() const;
        void XRGB(const utl::int32_t val);
        void XBGR(const utl::int32_t val);
        
        union
        {
            struct
            {
                scalar r, g, b;
            };

            scalar C[3];
        };                
    };
    
    TFNCOMMON_EXPORT void swap(color3& c1, color3& c2);
    TFNCOMMON_EXPORT color3 cubic_interpolation(const color3&, const color3&, scalar);
    TFNCOMMON_EXPORT color3 linear_interpolation(const color3&, const color3&, scalar);    

	//--------------------------------------------------------------------------------------------
	// Kolor 4 elementowy zmiennoprzecinkowy.
	//--------------------------------------------------------------------------------------------
    struct TFNCOMMON_EXPORT color4
    {        
		static const color4 WHITE4;
		static const color4 BLACK4;
		static const color4 RED4;
		static const color4 GREEN4;
		static const color4 BLUE4;
		static const color4 GRAY4;         
            
        color4(): r(ZERO), g(ZERO), b(ZERO), a(ZERO) {}
        color4(scalar _r, scalar _g, scalar _b, scalar _a): r(_r), g(_g), b(_b), a(_a) {}

        bool operator == (const color4& rhs) const
        {   return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a));  }

        bool operator != (const color4& rhs) const
        {   return ((r != rhs.r) || (g != rhs.g) || (b != rhs.b) || (a != rhs.a));  }

        bool operator <= (const color4& rhs) const
        {   return ((r <= rhs.r) && (g <= rhs.g) && (b <= rhs.b) && (a <= rhs.a));  }

        bool operator < (const color4& rhs) const
        {   return ((r < rhs.r) && (g < rhs.g) && (b < rhs.b) && (a < rhs.a));      }

        utl::int32_t ARGB() const;
        utl::int32_t RGBA() const;
        utl::int32_t ABGR() const;
        void ARGB(const utl::int32_t val);
        void RGBA(const utl::int32_t val);
        void ABGR(const utl::int32_t val);
        
        union
        {
            struct
            {
                scalar r, g, b, a;
            };
                  
            scalar C[4];
        };                
    };
       
    TFNCOMMON_EXPORT void swap(color4& c1, color4& c2);
    TFNCOMMON_EXPORT color4 linear_interpolation(const color4&, const color4&, scalar);    
    TFNCOMMON_EXPORT color4 cubic_interpolation(const color4&, const color4&, scalar);	

} // end namespace math

#endif