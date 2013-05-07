#ifndef MPB_COMPUTE_HPP_INCLUDED
#define MPB_COMPUTE_HPP_INCLUDED

#include <boost/mpl/times.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/equal_to.hpp>

namespace mpl = boost::mpl;

namespace mpb {

// recursive using specialization for factor<0>
template < typename N>
struct factor : mpl::times<
					N, 
					factor< mpl::int_< N::value - 1 > >
				>::type
{};

// specialization
template<>
struct factor< mpl::int_<0> > : 
	mpl::int_<1>::type
{};

// number of digits of N
// recursive but using mpl::if_ instead of specialization
template <typename N>
struct nb_digits : 
	mpl::if_<
		mpl::less< N, mpl::int_<10> >, 
		mpl::int_<1>, 
		mpl::plus< 
			mpl::int_<1>,
			nb_digits< mpl::int_< N::value / 10> >
		>
	>::type
{};

// pow(Value, Exponent)
template <typename Value, typename Exponent>
struct pow : 
	mpl::times<
		Value, 
		pow< Value, mpl::int_< Exponent::value - 1 >>
	>::type
{};

// specialization for 0
template <typename Value>
struct pow<Value, mpl::int_<0>> : 
	mpl::int_<1>::type
{};

// is N odd?
template < typename N >
struct odd : 
	mpl::equal_to<
		mpl::int_<N::value % 2>, 
		mpl::int_<1>
	>::type
{};

// is N even?
template < typename N >
struct even : 
	mpl::equal_to<
		mpl::int_<N::value % 2>, 
		mpl::int_<0>
	>::type
{};

} // ns mpb

#endif // MPB_COMPUTE_HPP_INCLUDED