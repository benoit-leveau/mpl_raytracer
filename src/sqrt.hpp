#ifndef MPB_SQRT_HPP_INCLUDED
#define MPB_SQRT_HPP_INCLUDED

#include <boost/mpl/divides.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/range_c.hpp>

namespace mpl = boost::mpl;

namespace mpb {

// implementation for sqrt
namespace sqrt_impl
{
	// initial coefficient for sqrt algorithm
	// 2 if D is odd, 6 if D is even
	// D being the number of digits of N
	template < typename D >
	struct init_coeff : 
		mpl::if_< 
			odd<D>, 
			mpl::int_<2>, 
			mpl::int_<6>
		>::type
	{};

	// initial value for sqrt algorithm
	// init_coeff * pow(10, n)
	// with D=2*n+2 if D is even, D=2*n+1 if D is even
	// D being the number of digits of N
	template < typename D >
	struct init_value : 
		mpl::times< 
			init_coeff<D>, 
			pow<
				mpl::int_<10>, 
				mpl::int_< (D::value-1)/2 > 
			>
		>::type
	{};
}

// computes A/B
template < typename A, typename B>
struct ratio : 
	mpl::int_< A::value / B::value >::type
{};

// Babylonian implementation of sqrt
// with customizable number of iterations (defaults to 3)
// for each iteration:
//  new_value = 0.5 * (previous_value + N/previous_value)
template < typename N, int Iterations = 3 >
struct sqrt :
	mpl::fold<
		mpl::range_c<int, 0, Iterations>,
		sqrt_impl::init_value< 
			nb_digits<
				mpl::int_<N::numerator / N::denominator>
			>
		>,
		mpl::times<
			mpl::plus<
				mpl::_1, 
				mpl::divides< N, mpl::_1 >
			>,
			mpb::float_<0,5>
		>
	>::type
{};

// integer version of sqrt
template < typename N, int Iterations = 3 >
struct sqrt_int :
	mpl::fold<
		mpl::range_c<int, 0, Iterations>,
		sqrt_impl::init_value< nb_digits<N> >,
		ratio<
			mpl::plus<
				mpl::_1, 
				ratio< N, mpl::_1 >
			>,
			mpl::int_<2>
		>
	>::type
{};

} // ns mpb

#endif // MPB_COMPUTE_HPP_INCLUDED