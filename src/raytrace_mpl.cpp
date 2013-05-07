#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <iostream>
#include <vector>

#include <boost/mpl/for_each.hpp>

#include "raytrace.hpp"

#include "scene_description.h"

namespace mpl = boost::mpl;

std::ofstream imageFile;

struct draw_pixel
{
    template< typename Pixel > void operator()( Pixel pixel)
    {
		float red, green, blue;
		if (Pixel::hit::value)
		{
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f;
		}
		else
		{
			red = 0.0f;
			green = 0.0f;
			blue = 0.0f;
		}
		imageFile
			.put((unsigned char)std::min(blue*255.0f,255.0f))
			.put((unsigned char)std::min(green*255.0f, 255.0f))
			.put((unsigned char)std::min(red*255.0f, 255.0f));
    }
};

struct draw_tile
{
    template< typename Tile > void operator()( Tile tile)
    {
		mpl::for_each< Tile > ( draw_pixel() );
    }
};

bool raytrace_MPL(char* outputName)
{
	imageFile.open(outputName, std::ios_base::binary);
	if (!imageFile)
		return false; 
	
	// main class which will do the computation at compile-time
	typedef ImageSequence< IMAGE_SIZE_X , IMAGE_SIZE_Y >::type image;

	// Ajout du header TGA
	imageFile.put(0).put(0);
	imageFile.put(2);        /* RGB non compresse */

	imageFile.put(0).put(0);
	imageFile.put(0).put(0);
	imageFile.put(0);

	imageFile.put(0).put(0); /* origine X */ 
	imageFile.put(0).put(0); /* origine Y */

	imageFile.put((image::size_x & 0x00FF)).put((image::size_x & 0xFF00) / 256);
	imageFile.put((image::size_y & 0x00FF)).put((image::size_y & 0xFF00) / 256);
	imageFile.put(24);       /* 24 bit bitmap */ 
	imageFile.put(0); 
	// fin du header TGA

	// compile value for each pixel
	// and generates runtime code to store pixel value in the image
	mpl::for_each< image > ( draw_tile() );

	return true;	
}