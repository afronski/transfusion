#include "pchCommon.hpp"

using namespace math;

//--------------------------------------------------------------------------------------------
// Sta³e.
//--------------------------------------------------------------------------------------------
const scalar POS_INF =		 std::numeric_limits<scalar>::infinity();
const scalar NEG_INF =		-std::numeric_limits<scalar>::infinity();

//--------------------------------------------------------------------------------------------
// Dodatki.
//--------------------------------------------------------------------------------------------
void is_valid(const scalar& a)
{
	assert(!_isnan(a) && _finite(a));
}

//--------------------------------------------------------------------------------------------
// Funkcje dodatkowe dotycz¹ce powy¿szych klas.
//--------------------------------------------------------------------------------------------
// Wyznacznik macierzy:
// | a1 a2 |
// | b1 b2 |
inline scalar math::det2x2(scalar a1, scalar a2, scalar b1, scalar b2)
{
	return a1 * b2 - b1 * a2;
}

// Wyznacznik macierzy:
// | a1 a2 a3 | 
// | b1 b2 b3 |
// | c1 c2 c3 |
inline scalar math::det3x3(scalar a1, scalar a2, scalar a3, 
						   scalar b1, scalar b2, scalar b3,
						   scalar c1, scalar c2, scalar c3)
{
	return a1 * det2x2(b2, b3, c2, c3) - b1 * det2x2(a2, a3, c2, c3) + c1 * det2x2(a2, a3, b2, b3);
}