Compile-time Raytracer, implemented entirely using MPL, boost's meta-programming library.

Rationale
=========

Written in a few days in September 2009 mostly to learn about MPL.

Overview
========

The program performs raytracing operations during compilation, so when the program has finished compiling, the final result is stored in the compiled executable.
Running the executable just transfers the image to a file, there is no additional computation done at running-time.

Because of compiler limitations (and I found a lot of limitations), the "image" computed is very small (32x32), and the algorithm is stopping at the first impact (it is therefore more a raycaster than a raytracer).

The code defines the following template classes:
* Ray
* Light
* Color
* Pixel
* Sphere
* HitSphere (5 classes)
* ImageSequence_tile
* ImageSequence
* Vec3D (including operations between vectors: addition, substraction, multiplication by another vector or a scalar)

These classes are used to compute the projection of a sphere onto an image plane. Because of memory limitations, I didn't go farther to implement a proper perspective projection. 

Interesting Bits
================

mpl::llong_
-----------

MPL already provides a class storing <b>long integers</b> (mpl::long_). I added a mpl::llong_, which stores <b>long long integers</b>.

mpl::float_
-----------

The file <i>src/float.hpp</i> provides an extension for MPL, the <i>boost::mpl::float_</i> class.

It is internally stored (as a type, in the compiler) as a fraction of two <i>boost::mpl::llong</i>.

Usage:
<pre>
run-time:       float f = 1.67f;                                                                                               
compile-time:   typedef mpl::float_&lt;1,67>::type f;                                                                                  
</pre>
Internal implementation will actually use <i>rational_impl&lt;167,100></i>.

Using the Euclidian algorithm, the class rational_impl is converted to an irreducible fraction.

For instance, <i>float_&lt;15,10></i> will convert to <i>rational_impl&lt;3,2></i>.

Why creating more classes in the compiler memory? When performing operations like additions, multiplications and divisions, you want to use the irreducible fraction so that the compiler can re-use existing classes.

mpb::sqrt and mpb::sqrt_int
---------------------------

The file <i>src/sqrt.hpp</i> provides the implementation for a compile-time sqrt algorithm on integers of floats.

Example:
<pre>
run-time:      float a = sqrtf(9.0);
compile-time:  typedef mpb::sqrt&lt;mpl::float_&lt;9,0>>::type f; // evaluated to mpl::float_&lt;3,0>;
</pre>

The square root is computed using the Babylonian Algorithm, with a default number of iterations set to 3, which leads to pretty good results on average.

Other functions
---------------

The file <i>src/compute.hpp</i> provides implementations for the following maths functions:
* factorial: <pre>typedef factor&lt;4>::type f; // evaluated to mpl::int_&lt;24></pre>
* power: <pre>typedef pow&lt;2, 3>::type f; // evaluated to mpl::int_&lt;8></pre>
* odd: <pre>typedef odd&lt;3>::type f; // evaluated to mpl::bool_&lt;true></pre>
* even: <pre>typedef even&lt;3>::type f; // evaluated to mpl::bool_&lt;false></pre>

Compilers Tested
================

Originally developed with MSVC8 (VS2005) and whatever boost distribution I was using at the time.

Now being tested with gcc 4.2.1 and Boost 1.53.

Performances
============

Tests run with gcc 4.2.1 on a 2.4GHz Core i7
<table>
<tr><th>Image Size</th><th>Compilation Time</th><th>Memory Used</th><th>Executable Size</th></tr>
<tr><td>16x16</td><td>6s</td><td>200MB</td><td>1.2MB</td></tr>
<tr><td>32x32</td><td>1m</td><td>690MB</td><td>5MB</td></tr>
<tr><td>64x64</td><td>18m</td><td>2.1GB</td><td>19.9MB</td></tr>
</table>

As you can see, the performances degrade very quickly.

Links
=====

* Boost MPL: www.boost.org/libs/mpl/‎
* Wikipedia artcile about the Babylonian algorithm to compute square roots: http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Babylonian_method
* Wikipedia article about the Euclidian algorithm to compute GCD: http://en.wikipedia.org/wiki/Euclidean_algorithm‎
* Wikipedia article irreducible fractions: http://en.wikipedia.org/wiki/Irreducible_fraction
