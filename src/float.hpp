#ifndef MPB_FLOAT_HPP_INCLUDED
#define MPB_FLOAT_HPP_INCLUDED

#include "llong.hpp"
#include "compute.hpp"
#include <boost/mpl/times.hpp>
#include <boost/mpl/divides.hpp>

namespace mpl = boost::mpl;

namespace mpb{

// hiding detail implementation
namespace float_detail
{
	struct rational_tag : mpl::int_<10> {};

	// base class for floating point values
	template< long long N, long long D>
	struct rational_impl
	{
		BOOST_STATIC_CONSTANT(long long, numerator = N);
		BOOST_STATIC_CONSTANT(long long, denominator = D);
		typedef rational_impl<N,D> type;
		typedef rational_tag tag;
	};
	
	// Euclidian Algorithm:
	// Suppose we want the largest divisor of 625 and 7850.
	// We will call 7850 the big number and 625 the small number.
	// We do 7850 ÷ 625 and find the remainder which is 350.
	// We now make 625 the new big number and 350 the new small number 
	// and we keep repeating the steps until the remainder is zero.
	// Big number	Small number 	Remainder
	//	7850		625				350
	//	625			350				275
	//	350			275				75
	//	275			75				50
	//	75			50				25
	//	50			25				0
	// => largest divisor is 25

	// recursively find largest divisor
	template<long long Big, long long Small>
	struct LargestDivisor :
		LargestDivisor<
			Small, 
			Big - (Big / Small) * Small
		>::type
	{};

	// specialization to stop recursion
	// of LargestDivisor
	template<long long Big>
	struct LargestDivisor<Big, 0> :
		mpl::llong_<Big>::type
	{};

	// find the largest divisor of N and D
	// not knowing which is bigger
	template<long long N, long long D>
	struct LargestDivisor_Unordered :
		mpl::if_<
			mpl::less< mpl::llong_<N>, mpl::llong_<D> >,
			LargestDivisor<D, N>,
			LargestDivisor<N, D>
		>::type
	{};

	// class used to reduce a rational N/D
	// depending on the largest divisor of N and D
	template<long long N, long long D, long long divisor>
	struct rational_reduce_euclidian :
		mpl::if_<
			mpl::equal_to<
				mpl::llong_<divisor>,
				mpl::llong_<1>
			>,
			rational_impl<N, D>, // no divisor found, store rational as is
			rational_impl< N/divisor, D/divisor >
		>::type
	{};

	// class used to create a rational N/D
	// storing rational<N/D,1> if N%D==0
	// else reverting to rational_reduce_mod
	template<long long N, long long D, bool storeInteger>
	struct rational_reduce :
		mpl::if_<
			mpl::bool_<storeInteger>,
			rational_impl<N/D, 1>,
			rational_reduce_euclidian<
				N, 
				D, 
				LargestDivisor_Unordered<N, D>::value
			>
		>::type
	{
	};						

	// class used to create a rational N/D
	template<long long N, long long D>
	struct rational :
		rational_reduce<
			N, 
			D, 
			mpl::equal_to<
				mpl::llong_<N%D>,
				mpl::llong_<0>
			>::value
		>::type
	{  
		typedef rational<N,D> type;
		rational() {}
	};

	// specialization of rational
	// for integer values
	template<int N>
	struct rational<N, 1> :
		rational_impl<N, 1>::type
	{};

	// helper class used to convert float_<> to rational_<>
	// A is integer value
	// B is floating value
	// D is 10^n (with n being the number of digits of B)
	template<long long A, long long B, long long D>
	struct float_to_rational : 
		rational<
			mpl::plus<
				mpl::times<
					mpl::llong_< A >,
					mpl::llong_< D >
				>,
				mpl::llong_< B >
			>::value,
			D
		>::type
	{};
}

// user-friendly class to store a floating point value
// run-time:       float f = 1.67f;
// compile-time:   typedef float_<1,67>::type f;
//
// - internal implementation will actually use rational_impl<167,100>
// - float_<15,10> will convert to rational_impl<3,2> through reduction

template<long long A, long long B>
struct float_ : 
	float_detail::float_to_rational<
		A, 
		B, 
		mpb::pow< 
			mpl::llong_<10>, 
			mpb::nb_digits< mpl::llong_< B > > 
		>::value
	>::type
{};

} // ns mpb

// go to boost::mpl namespace to implement mpl operators like mpl::plus, mpl::times, etc.
namespace boost
{ 
	namespace mpl
	{
		// plus< float_<>, float_<> >
		template<>
		struct plus_impl< mpb::float_detail::rational_tag, mpb::float_detail::rational_tag >
		{
			template< typename N1, typename N2 >
			struct apply :
				mpb::float_detail::rational<
					N1::numerator * N2::denominator + N2::numerator * N1::denominator,
					N1::denominator * N2::denominator
				>::type
			{};
		};

		// minus< float_<>, float_<> >
		template<>
		struct minus_impl< mpb::float_detail::rational_tag, mpb::float_detail::rational_tag >
		{
			template< typename N1, typename N2 >
			struct apply :
				mpb::float_detail::rational<
					N1::numerator * N2::denominator - N2::numerator * N1::denominator,
					N1::denominator * N2::denominator
				>::type
			{};
		};

		// less< float_<>, float_<> >
		template<>
		struct less_impl< mpb::float_detail::rational_tag, mpb::float_detail::rational_tag >
		{
			template< typename N1, typename N2 >
			struct apply :
				mpl::less<
					mpl::llong_< N1::numerator * N2::denominator >,
					mpl::llong_< N2::numerator * N1::denominator >
				>::type
			{};
		};

		// plus< int_<>, float_<> >
		template<>
		struct plus_impl< integral_c_tag, mpb::float_detail::rational_tag >
		{
			template< typename I, typename F >
			struct apply
				: mpb::float_detail::rational<
					F::numerator + I::value * F::denominator,
					F::denominator
				>
			{
			};
		};

		// plus< float_<>, int_<> >
		template<>
		struct plus_impl< mpb::float_detail::rational_tag, integral_c_tag >
		{
			template< typename F, typename I >
			struct apply
				: mpb::float_detail::rational<
					F::numerator + I::value * F::denominator,
					F::denominator
				>
			{
			};
		};

		// times< float_<>, float_<> >
		template<>
		struct times_impl< mpb::float_detail::rational_tag, mpb::float_detail::rational_tag >
		{
			template< typename N1, typename N2 >
			struct apply :
				mpb::float_detail::rational<
					N1::numerator * N2::numerator,
					N1::denominator * N2::denominator
				>
			{
			};
		};

		// times< float_<> , int_<> >
		template<>
		struct times_impl< mpb::float_detail::rational_tag, integral_c_tag >
		{
			template< typename F, typename I >
			struct apply :
				mpb::float_detail::rational<
					F::numerator * I::value,
					F::denominator
				>
			{
			};
		};

		// times< int_<> , float_<> >
		template<>
		struct times_impl< integral_c_tag, mpb::float_detail::rational_tag>
		{
			template< typename I, typename F >
			struct apply :
				mpb::float_detail::rational<
					F::numerator * I::value,
					F::denominator
				>
			{
			};
		};

		// divides< float_<>, float_<> >
		template<>
		struct divides_impl< mpb::float_detail::rational_tag, mpb::float_detail::rational_tag >

		{
			template< typename N1, typename N2 >
			struct apply :
				mpb::float_detail::rational<
					N1::numerator * N2::denominator,
					N1::denominator * N2::numerator
				>
			{
			};
		};

		// divides< float_<>, int_<> >
		template<>
		struct divides_impl< mpb::float_detail::rational_tag, integral_c_tag >
		{
			template< typename F, typename I >
			struct apply :
				mpb::float_detail::rational<
					F::numerator,
					F::denominator * I::value
				>
			{
			};
		};
	}
}

#define float_value(x) (float(x::numerator) / float(x::denominator))

#endif // MPB_FLOAT_HPP_INCLUDED
