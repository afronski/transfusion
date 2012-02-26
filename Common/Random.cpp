#include "pchCommon.hpp"

#include "Random.hpp"

using namespace math;

//--------------------------------------------------------------------------------------------
// Implementacja random.
//--------------------------------------------------------------------------------------------
utl::int32_t random::_seed = 0;

void random::init(utl::int32_t seed)
{
	_seed = seed;
	srand(_seed);
}
			
utl::int32_t random::range_rand(utl::int32_t low, utl::int32_t high)
{
	return (rand () % (high - low + 1)) + low;
}

utl::int32_t random::rand()
{
	return ::rand();
}

scalar random::uniform_rand(scalar f)
{
 scalar l1 = static_cast<scalar>(rand()) / static_cast<scalar>(RAND_MAX - 1);
 scalar l2 = static_cast<scalar>(rand()) / static_cast<scalar>(RAND_MAX - 1);
 return f * sqrtf(-TWO * log(l1)) * cos(TWO * PI * l2);
}

scalar random::randf()
{
	return static_cast<scalar>(rand()) / static_cast<scalar>(RAND_MAX - 1);
}

scalar random::perlin_noise_1d(utl::int32_t x)
{
   x = (x<<13) ^ x;
   scalar res = static_cast<scalar>( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff ) / 1073741824.0);
   return res;
}

scalar random::perlin_noise_2d(utl::int32_t x, utl::int32_t y)
{
   utl::int32_t n;
   n = x + y * 57;
   n = (n<<13) ^ n;
   scalar res = static_cast<scalar>( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff ) / 1073741824.0);
   return res;
}