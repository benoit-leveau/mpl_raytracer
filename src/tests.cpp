#include "stdafx.h"

#include <math.h>
#include <algorithm>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>

#include <boost/mpl/math/rational_c.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/for_each.hpp>

#include "vec.hpp"
#include "raytrace.hpp"

namespace mpl = boost::mpl;

void print_test_divisors()
{
	std::cout << "--- Tests divisors ---" << std::endl;

	typedef mpb::float_detail::LargestDivisor_Unordered<7,55>::type d;
	std::cout << "largest divisor of 7 and 55 is " << d::value << std::endl;

	typedef mpb::float_detail::LargestDivisor_Unordered<18,12>::type d1;
	std::cout << "largest divisor of 18 and 12 is " << d1::value << std::endl;

	typedef mpb::float_detail::LargestDivisor_Unordered<12,18>::type d1_b;
	std::cout << "largest divisor of 12 and 18 is " << d1_b::value << std::endl;

	typedef mpb::float_detail::LargestDivisor_Unordered<248,6760>::type d2;
	std::cout << "largest divisor of 248 and 6760 is " << d2::value << std::endl;

	typedef mpb::float_detail::LargestDivisor_Unordered<55279,71073>::type d3;
	std::cout << "largest divisor of 55279 and 71073 is " << d3::value << std::endl;

	std::cout << std::endl;
}

void print_test_float()
{
	std::cout << "--- Tests float_<> ---" << std::endl;

	typedef mpb::float_<0,0>::type f0;
	std::cout << "input (0.0) => stored as " << f0::numerator << "/" << f0::denominator << " which equals " << float_value(f0) << std::endl;

	typedef mpb::float_<1,54>::type f1;
	std::cout << "input (1.54) => stored as " << f1::numerator << "/" << f1::denominator << " which equals " << float_value(f1) << std::endl;

	typedef mpb::float_<3,12>::type f2;
	std::cout << "input (3.12) => stored as " << f2::numerator << "/" << f2::denominator << " which equals " << float_value(f2) << std::endl;

	typedef mpl::plus<f1, f2>::type f3;
	std::cout << "input (1.54+3.12=4.66) => stored as " << f3::numerator << "/" << f3::denominator << " which equals " << float_value(f3) << std::endl;

	typedef mpl::times<f1, f2>::type f4;
	std::cout << "input (1.54*3.12=4.8048) => stored as " << f4::numerator << "/" << f4::denominator << " which equals " << float_value(f4) << std::endl;

	typedef mpb::float_<0,33>::type f5;
	std::cout << "input (0.33) => stored as " << f5::numerator << "/" << f5::denominator << " which equals " << float_value(f5) << std::endl;

	std::cout << std::endl;
}

template 
<typename U>
void display_sqrt_int(U x)
{
	std::cout << "sqrt(" << U::value << ")=" << mpb::sqrt_int<U>() << " (real value=" << sqrt(float(U::value)) << ")" << std::endl;
}

template 
<typename U>
void display_sqrt_float(U x)
{
	typedef mpb::sqrt<U, 3> sqrt_value;
	std::cout << "sqrt(" << float_value(U) << ")=" << float_value(sqrt_value) << " stored as " << sqrt_value::numerator << "/" << sqrt_value::denominator << " (real value=" << sqrt(float_value(U)) << ")" << std::endl;
}

void print_test_sqrt()
{
	std::cout << "--- Tests sqrt<int> ---" << std::endl;

	display_sqrt_int(mpl::int_< 81 > ());
	display_sqrt_int(mpl::int_< 121 > ());
	display_sqrt_int(mpl::int_< 101 > ());

	std::cout << std::endl;

	std::cout << "--- Tests sqrt<float> ---" << std::endl;

	display_sqrt_float(mpb::float_< 2, 3 > ());
	display_sqrt_float(mpb::float_< 0, 81 > ());
	display_sqrt_float(mpb::float_< 1, 3 > ());
	display_sqrt_float(mpb::float_< 2, 457 > ());
	display_sqrt_float(mpb::float_< 3, 3 > ());

	std::cout << std::endl;
}

void print_test_vec3d()
{
	typedef mpb::Vec3D< mpb::float_<0,2>, mpb::float_<2,0>, mpb::float_<3,6> >::type vec1;
	typedef mpb::Vec3D< mpb::float_<0,4>, mpb::float_<3,0>, mpb::float_<0,1> >::type vec2;
	
	typedef mpl::plus< vec1, vec2 > result;
	std::cout << "(0.2 2.0 3.6) + (0.4 3.0 0.1) = (" << float_value(result::x) << "," << float_value(result::y) << "," << float_value(result::z) << ")" << std::endl;

	typedef mpl::minus< vec1, vec2 > result1;
	std::cout << "(0.2 2.0 3.6) - (0.4 3.0 0.1) = (" << float_value(result1::x) << "," << float_value(result1::y) << "," << float_value(result1::z) << ")" << std::endl;

	typedef mpl::times< vec1, mpl::int_<2> > result2;
	std::cout << "2 * (0.2 2.0 3.6) = (" << float_value(result2::x) << "," << float_value(result2::y) << "," << float_value(result2::z) << ")" << std::endl;

	typedef mpl::times< vec1, mpb::float_<1,7> > result3;
	std::cout << "1.7 * (0.2 2.0 3.6) = (" << float_value(result3::x) << "," << float_value(result3::y) << "," << float_value(result3::z) << ")" << std::endl;

	typedef mpl::times< vec1, vec2 > result4;
	std::cout << "(0.2 2.0 3.6) * (0.4 3.0 0.1) = " << float_value(result4) << std::endl;
}

void print_test_raytrace()
{
	// ray viewRay = { {float(x), float(y), -10000.0f}, { 0.0f, 0.0f, 1.0f}};

	typedef Ray< 
			mpb::Vec3D< 
				mpb::float_<0,0>,
				mpb::float_<0,0>,
				mpb::float_<0,0>
			>,
			mpb::Vec3D< 
				mpb::float_<1,0>,
				mpb::float_<0,0>,
				mpb::float_<0,0>
			>
	>::type ray;

	typedef Sphere<
			mpb::Vec3D< 
				mpb::float_<15,0>,
				mpb::float_<4,0>,
				mpb::float_<0,0>
			>,
			mpb::float_<5,0>,
			mpb::Vec3D< 
				mpb::float_<1,0>,
				mpb::float_<1,0>,
				mpb::float_<1,0>
			>,
			mpb::float_<0,0>
	>::type sphere1;

	typedef Sphere<
			mpb::Vec3D< 
				mpb::float_<5,0>,
				mpb::float_<2,0>,
				mpb::float_<3,0>
			>,
			mpb::float_<2,0>,
			mpb::Vec3D< 
				mpb::float_<1,0>,
				mpb::float_<1,0>,
				mpb::float_<1,0>
			>,
			mpb::float_<0,0>
	>::type sphere2;

	typedef HitSphere<
		ray,
		sphere1,
		mpb::float_<1000,0>
	>::type hit1;

	typedef HitSphere<
		ray,
		sphere2,
		mpb::float_<1000,0>
	>::type hit2;

	std::cout << std::endl;

	std::cout << "Launching a ray start=(0,0,0) direction=(1,0,0)" << std::endl;
	std::cout << "=> sphere 1 at (15,4,0) radius=5 " << std::endl;
	std::cout << "=> sphere 2 at (5,2,3) radius=2 " << std::endl;
	
	if (hit1::value)
		std::cout << "sphere 1 hit" << std::endl;
	else
		std::cout << "sphere 1 not hit" << std::endl;

	if (hit2::value)
		std::cout << "sphere 2 hit" << std::endl;
	else
		std::cout << "sphere 2 not hit" << std::endl;

	std::cout << std::endl;
}

int print_tests()
{
	print_test_divisors();	
	print_test_float();
	print_test_sqrt();
	print_test_vec3d();
	print_test_raytrace();

	return 0;
}

