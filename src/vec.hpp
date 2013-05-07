#ifndef MPB_VEC_HPP_INCLUDED
#define MPB_VEC_HPP_INCLUDED

#include "float.hpp"
#include "sqrt.hpp"

#include <boost/mpl/vector.hpp>

namespace mpl = boost::mpl;

namespace mpb {

namespace vec3d_detail
{
	struct vec3d_tag : mpl::int_<10> {};
}

template
<typename X, typename Y, typename Z>
struct Vec3D
{
	typedef X x;
	typedef Y y;
	typedef Z z;
	typedef Vec3D<X, Y, Z> type;
	typedef vec3d_detail::vec3d_tag tag;
};

namespace dot_product_impl
{
	template< typename V1, typename V2 >
	struct partial_dot_x :
		mpb::float_detail::rational<
			V1::x::numerator * V2::x::numerator,
			V1::x::denominator * V2::x::denominator
		>::type
	{};

	template< typename V1, typename V2 >
	struct partial_dot_y :
		mpb::float_detail::rational<
			V1::y::numerator * V2::y::numerator,
			V1::y::denominator * V2::y::denominator
		>::type
	{};

	template< typename V1, typename V2 >
	struct partial_dot_x_y :
		mpb::float_detail::rational<
			partial_dot_x<V1, V2>::numerator * partial_dot_y<V1, V2>::denominator + partial_dot_y<V1, V2>::numerator * partial_dot_x<V1, V2>::denominator,
			partial_dot_x<V1, V2>::denominator * partial_dot_y<V1, V2>::denominator
		>::type
	{};

	template< typename V1, typename V2 >
	struct partial_dot_z :
		mpb::float_detail::rational<
			V1::z::numerator * V2::z::numerator,
			V1::z::denominator * V2::z::denominator
		>::type
	{};
}
}

// go to boost::mpl namespace to implement mpl operators like mpl::plus, mpl::times, etc.
namespace boost
{ 
	namespace mpl
	{
		// plus< Vec3D<>, Vec3D<> >
		template<>
		struct plus_impl< mpb::vec3d_detail::vec3d_tag, mpb::vec3d_detail::vec3d_tag >
		{
			template< typename V1, typename V2 >
			struct apply : 
				mpb::Vec3D<
					mpl::plus<typename V1::x, typename V2::x>,
					mpl::plus<typename V1::y, typename V2::y>,
					mpl::plus<typename V1::z, typename V2::z>
				>::type
			{};
		};

		// minus< Vec3D<>, Vec3D<> >
		template<>
		struct minus_impl< mpb::vec3d_detail::vec3d_tag, mpb::vec3d_detail::vec3d_tag >
		{
			template< typename V1, typename V2 >
			struct apply : 
				mpb::Vec3D<
					mpl::minus<typename V1::x, typename V2::x>,
					mpl::minus<typename V1::y, typename V2::y>,
					mpl::minus<typename V1::z, typename V2::z>
				>::type
			{};
		};

		// times< Vec3D<>, Vec3D<> >
		template<>
		struct times_impl< mpb::vec3d_detail::vec3d_tag, mpb::vec3d_detail::vec3d_tag >
		{
			template< typename V1, typename V2 >
			struct apply : 
				mpl::plus<
					mpb::dot_product_impl::partial_dot_x_y<typename V1, typename V2>,
					mpb::dot_product_impl::partial_dot_z<typename V1, typename V2>
				>::type
			{};
		};

		// times< Vec3D<>, float_<> >
		template<>
		struct times_impl< mpb::vec3d_detail::vec3d_tag, mpb::float_detail::rational_tag >
		{
			template< typename V1, typename F>
			struct apply : 
				mpb::Vec3D<
					mpl::times<typename V1::x, F>,
					mpl::times<typename V1::y, F>,
					mpl::times<typename V1::z, F>
				>::type
			{};
		};

		// times< Vec3D<>, int_<> >
		template<>
		struct times_impl< mpb::vec3d_detail::vec3d_tag, integral_c_tag >
		{
			template< typename V1, typename I>
			struct apply : 
				mpb::Vec3D<
					mpl::times<typename V1::x, I>,
					mpl::times<typename V1::y, I>,
					mpl::times<typename V1::z, I>
				>::type
			{};
		};
	}
}

#endif // #define MPB_VEC_HPP_INCLUDED