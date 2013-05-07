
#ifndef BOOST_MPL_LLONG_FWD_HPP_INCLUDED
#define BOOST_MPL_LLONG_FWD_HPP_INCLUDED

#include <boost/mpl/aux_/adl_barrier.hpp>
#include <boost/mpl/aux_/nttp_decl.hpp>

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

typedef long long llong;

template< BOOST_MPL_AUX_NTTP_DECL(llong, N) > struct llong_;

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE
BOOST_MPL_AUX_ADL_BARRIER_DECL(llong_)

#endif // BOOST_MPL_LLONG_FWD_HPP_INCLUDED
