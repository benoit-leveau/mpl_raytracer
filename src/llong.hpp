
#ifndef BOOST_MPL_LLONG_HPP_INCLUDED
#define BOOST_MPL_LLONG_HPP_INCLUDED

#include "llong_fwd.hpp"

typedef long long llong;

#define AUX_WRAPPER_VALUE_TYPE llong
#include <boost/mpl/aux_/integral_wrapper.hpp>

#endif // BOOST_MPL_LLONG_HPP_INCLUDED
