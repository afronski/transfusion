#ifndef __COMMON_RANDOM_HPP__
#define __COMMON_RANDOM_HPP__

namespace math
{
	//--------------------------------------------------------------------------------------------
	// Random Static class.
	//--------------------------------------------------------------------------------------------
	class TFNCOMMON_EXPORT random
	{
		public:
			static void init(utl::int32_t seed);
			
			static utl::int32_t range_rand(utl::int32_t low, utl::int32_t high);			
			static utl::int32_t rand();		
				
			static scalar uniform_rand(scalar f);
			static scalar randf();
			
			static scalar perlin_noise_1d(utl::int32_t x);			
			static scalar perlin_noise_2d(utl::int32_t x, utl::int32_t y);
			
		private:
			static utl::int32_t _seed;
	};
	
} // end namespace math

#endif