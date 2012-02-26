#include "pchCommon.hpp"

#include "MathColor.hpp"

using namespace math;

//--------------------------------------------------------------------------------------------
// Kolor 3 elementowy.
//--------------------------------------------------------------------------------------------
const color3 color3::WHITE3 =  color3(	 ONE,	 ONE,	 ONE);
const color3 color3::BLACK3 =  color3(	ZERO,	ZERO,	ZERO);
const color3 color3::RED3   =  color3(	 ONE,	ZERO,	ZERO);
const color3 color3::GREEN3 =  color3(	ZERO,	 ONE,	ZERO);
const color3 color3::BLUE3  =  color3(	ZERO,	ZERO,	 ONE);
const color3 color3::GRAY3  =  color3(ZERO_5, ZERO_5, ZERO_5);   

utl::int32_t color3::XRGB() const
{
    utl::int8_t byte; 
    utl::int32_t ret;
    byte = static_cast<utl::int8_t>(r * 255.0f);
    ret = byte << 16;
    byte = static_cast<utl::int8_t>(g * 255.0f);
    ret += byte << 8;
    byte = static_cast<utl::int8_t>(b * 255.0f);
    ret += byte;
    return ret;
}

utl::int32_t color3::XBGR() const
{
    utl::int8_t byte; 
    utl::int32_t ret;
    byte = static_cast<utl::int8_t>(b * 255.0f);
    ret = byte << 16;
    byte = static_cast<utl::int8_t>(g * 255.0f);
    ret += byte << 8;
    byte = static_cast<utl::int8_t>(r * 255.0f);
    ret += byte;
    return ret;
}

void color3::XRGB(const utl::int32_t val)
{
    utl::int32_t v = val;
    r = static_cast<utl::int8_t>(v >> 16) / 255.0f;
    g = static_cast<utl::int8_t>(v >> 8) / 255.0f;
    b = static_cast<utl::int8_t>(v) / 255.0f;
}

void color3::XBGR(const utl::int32_t val)
{   
    utl::int32_t v = val;
    b = static_cast<utl::int8_t>(v >> 16) / 255.0f;
    g = static_cast<utl::int8_t>(v >> 8) / 255.0f;
    r = static_cast<utl::int8_t>(v) / 255.0f;
}

void math::swap(color3& c1, color3& c2)
{
	std::swap(c1.r, c2.r);
	std::swap(c1.g, c2.g);
	std::swap(c1.b, c2.b);	
}

color3 math::linear_interpolation(const color3& from, const color3& to, scalar time)
{    
    color3 ret;
    ret.r = from.r + (to.r - from.r) * time;
    ret.g = from.g + (to.g - from.g) * time;
    ret.b = from.b + (to.b - from.b) * time;
    return ret;
}

color3 math::cubic_interpolation(const color3& from, const color3& to, scalar time)
{    
    color3 ret;
    ret.r = (to.r - from.r) * ((3 - 2 * time) * time * time) + from.r;
    ret.g = (to.g - from.g) * ((3 - 2 * time) * time * time) + from.g;
    ret.b = (to.b - from.b) * ((3 - 2 * time) * time * time) + from.b;
    return ret;
}

//--------------------------------------------------------------------------------------------
// Kolor 4 elementowy.
//--------------------------------------------------------------------------------------------
const color4 color4::WHITE4 = color4(	 ONE,	 ONE,	 ONE,	ONE);
const color4 color4::BLACK4 = color4(	ZERO,	ZERO,	ZERO,	ONE);
const color4 color4::RED4   = color4(	 ONE,	ZERO,	ZERO,	ONE);
const color4 color4::GREEN4 = color4(	ZERO,	 ONE,	ZERO,	ONE);
const color4 color4::BLUE4  = color4(	ZERO,	ZERO,	 ONE,	ONE);
const color4 color4::GRAY4  = color4( ZERO_5, ZERO_5, ZERO_5,	ONE);    

utl::int32_t color4::ARGB() const
{
    utl::int8_t byte; 
    utl::int32_t ret;
    byte = static_cast<utl::int8_t>(a * 255.0f);
    ret = byte << 24;
    byte = static_cast<utl::int8_t>(r * 255.0f);
    ret += byte << 16;
    byte = static_cast<utl::int8_t>(g * 255.0f);
    ret += byte << 8;
    byte = static_cast<utl::int8_t>(b * 255.0f);
    ret += byte;
    return ret;
}

utl::int32_t color4::RGBA() const
{    
    utl::int8_t byte; 
    utl::int32_t ret;
    byte = static_cast<utl::int8_t>(r * 255.0f);
    ret = byte << 24;
    byte = static_cast<utl::int8_t>(g * 255.0f);
    ret += byte << 16;
    byte = static_cast<utl::int8_t>(b * 255.0f);
    ret += byte << 8;
    byte = static_cast<utl::int8_t>(a * 255.0f);
    ret += byte;
    return ret;
}

utl::int32_t color4::ABGR() const
{    
    utl::int8_t byte; 
    utl::int32_t ret;
    byte = static_cast<utl::int8_t>(a * 255.0f);
    ret = byte << 24;
    byte = static_cast<utl::int8_t>(b * 255.0f);
    ret += byte << 16;
    byte = static_cast<utl::int8_t>(g * 255.0f);
    ret += byte << 8;
    byte = static_cast<utl::int8_t>(r * 255.0);
    ret += byte;
    return ret;
}

void color4::ARGB(const utl::int32_t val)
{
    utl::int32_t v = val;
    a = static_cast<utl::int8_t>(v >> 24) / 255.0f;
    r = static_cast<utl::int8_t>(v >> 16) / 255.0f;
    g = static_cast<utl::int8_t>(v >> 8) / 255.0f;
    b = static_cast<utl::int8_t>(v) / 255.0f;
}

void color4::RGBA(const utl::int32_t val)
{    
    utl::int32_t v = val;
    r = static_cast<utl::int8_t>(v >> 24) / 255.0f;
    g = static_cast<utl::int8_t>(v >> 16) / 255.0f;
    b = static_cast<utl::int8_t>(v >> 8) / 255.0f;
    a = static_cast<utl::int8_t>(v) / 255.0f;
}

void color4::ABGR(const utl::int32_t val)
{    
    utl::int32_t v = val;
    a = static_cast<utl::int8_t>(v >> 24) / 255.0f;
    b = static_cast<utl::int8_t>(v >> 16) / 255.0f;
    g = static_cast<utl::int8_t>(v >> 8) / 255.0f;
    r = static_cast<utl::int8_t>(v) / 255.0f;
}

void math::swap(color4& c1, color4& c2)
{
	std::swap(c1.r, c2.r);
	std::swap(c1.g, c2.g);
	std::swap(c1.b, c2.b);
	std::swap(c1.a, c2.a);
}

color4 math::linear_interpolation(const color4& from, const color4& to, scalar time)
{
    color4 ret;
    ret.r = from.r + (to.r - from.r) * time;
    ret.g = from.g + (to.g - from.g) * time;
    ret.b = from.b + (to.b - from.b) * time;
    ret.a = from.a + (to.a - from.a) * time;
    return ret;
}

color4 math::cubic_interpolation(const color4& from, const color4& to, scalar time)
{
    color4 ret;
    ret.r = (to.r - from.r) * ((3 - 2 * time) * time * time) + from.r;
    ret.g = (to.g - from.g) * ((3 - 2 * time) * time * time) + from.g;
    ret.b = (to.b - from.b) * ((3 - 2 * time) * time * time) + from.b;
    ret.a = (to.a - from.a) * ((3 - 2 * time) * time * time) + from.a;
    return ret;
}