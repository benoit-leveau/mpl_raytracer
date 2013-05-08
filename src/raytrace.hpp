#ifndef MPB_RAYTRACE_HPP_INCLUDED
#define MPB_RAYTRACE_HPP_INCLUDED

#include <boost/mpl/pair.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/less.hpp>

#include "vec.hpp"

namespace mpl = boost::mpl;

template <typename Point, typename Direction>
struct Ray
{
	typedef Point start;
	typedef Direction direction;
	typedef Ray<Point, Direction> type;
};

template < typename Red, typename Green, typename Blue >
struct Color
{
	typedef Red reg;
	typedef Green green;
	typedef Blue blue;
	typedef Color<Red, Green, Blue> type;
};

template < typename Position, typename Size, typename Color, typename Reflection >
struct Sphere
{
	typedef Position position; 
	typedef Size size;
	typedef Color color;
	typedef Reflection reflection;
	typedef Sphere<Position, Size, Color, Reflection> type;
};

template < typename Position, typename Color >
struct Light
{
	typedef Position position;
	typedef Color color;
	typedef Light<Position, Color> type;
};

template < typename T, typename T0, typename T1>
struct HitSphere_5 :
	mpl::if_<
		mpl::and_<
			mpl::less<
				mpb::float_<0,1>,
				T0
			>,
			mpl::less<
				T0,
				T
			>
		>,
		mpl::bool_< true >,
		mpl::if_<
			mpl::and_<
				mpl::less<
					mpb::float_<0,1>,
					T1
				>,
				mpl::less<
					T1,
					T
				>
			>,
			mpl::bool_< true >,
			mpl::bool_< false >
		>
	>::type
{};

template < typename T, typename B, typename sqrt_D>
struct HitSphere_4 :
	HitSphere_5<
		T,
		mpl::minus< B, sqrt_D >,
		mpl::plus < B, sqrt_D >
	>::type
{};

template < typename T, typename B, typename D>
struct HitSphere_3 :
	mpl::if_<
		mpl::less<
			D,
			mpb::float_<0,0>
		>,
		mpl::bool_< false >,
		HitSphere_4< T, B, mpb::sqrt< D > >
	>::type
{};

template < typename Sphere, typename T, typename dist, typename B>
struct HitSphere_2 :
	HitSphere_3<
		T,
		B,
		mpl::plus<
			mpl::minus<
				mpl::times< B, B >,
				mpl::times< dist, dist >
			>,
			mpl::times<
				typename Sphere::size,
				typename Sphere::size
			>
		>
	>::type
{};

template < typename Ray, typename Sphere, typename T, typename dist>
struct HitSphere_1 :
	HitSphere_2 <
		Sphere,
		T,
		dist,
		mpl::times< typename Ray::direction, dist > 
	>::type
{
};

template < typename Ray, typename Sphere, typename T >
struct HitSphere :
	HitSphere_1<
		Ray, 
		Sphere, 
		T, 
		mpl::minus< 
			typename Sphere::position, 
			typename Ray::start 
		>
	>::type
{};

struct imagesequence_tag;

template< typename PosX, typename PosY, int start_x, int tile_size_x >
struct Pixel
{
	BOOST_STATIC_CONSTANT(int, x = PosX::value);
	BOOST_STATIC_CONSTANT(int, y = PosY::value);

	typedef Pixel< PosX, PosY, start_x, tile_size_x > type;

	typedef PosX X;
	typedef PosY Y;

	typedef Pixel<
				mpl::int_< (start_x + ((X::value-start_x+1)%tile_size_x)) >,
				mpl::int_< (Y::value + ((X::value-start_x+1)/tile_size_x)) >,
				start_x,
				tile_size_x
	> next;

	typedef HitSphere<
		Ray<
			mpb::Vec3D< mpb::float_<PosX::value, 0>, mpb::float_<PosY::value, 0>, mpb::float_<-50,0> >,
			mpb::Vec3D< mpb::float_<0,0>, mpb::float_<0,0>, mpb::float_<1,0> >
		>,
		Sphere<
			mpb::Vec3D< mpb::float_<32,0>, mpb::float_<24,0>, mpb::float_<0,0> >,
			mpb::float_<6, 0>,
			Color< mpb::float_<1,0>, mpb::float_<1,0>, mpb::float_<0,0> >,
			mpb::float_<0, 5>
		>,
		mpb::float_<100, 0>
	> hit;
};

template< int start_x, int start_y, int tile_size_x, int tile_size_y, int image_size_x >
struct ImageSequence_tile
{
	BOOST_STATIC_CONSTANT(int, x = start_x);
	BOOST_STATIC_CONSTANT(int, y = start_y);
	BOOST_STATIC_CONSTANT(int, tx = tile_size_x);
	BOOST_STATIC_CONSTANT(int, ty = tile_size_y);

	// #pragma message("computing tile...")
    typedef imagesequence_tag tag;
    typedef ImageSequence_tile type;

	typedef Pixel< mpl::int_<start_x>, mpl::int_<start_y>, start_x, tile_size_x > begin;
	typedef Pixel< mpl::int_<start_x>, mpl::int_<start_y + tile_size_y>, start_x, tile_size_x > end;
	
	typedef ImageSequence_tile<
				((start_x+tile_size_x)%image_size_x),
				(start_y + (tile_size_y*((start_x+tile_size_x)/image_size_x))),
				tile_size_x,
				tile_size_y,
				image_size_x
	> next;
};

template< int SIZE_X, int SIZE_Y >
struct ImageSequence
{
	// #pragma message("computing image...")

	BOOST_STATIC_CONSTANT(int, size_x = SIZE_X);
	BOOST_STATIC_CONSTANT(int, size_y = SIZE_Y);

	typedef ImageSequence type;
	typedef ImageSequence_tile< 0, 0, 16, 16, SIZE_X > begin;
	typedef ImageSequence_tile< 0, size_y, 16, 16, SIZE_X > end;
};

#endif // MPB_RAYTRACE_HPP_INCLUDED
