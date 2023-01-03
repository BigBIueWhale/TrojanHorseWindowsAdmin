#pragma once

#include <boost/multiprecision/cpp_int.hpp>

struct PublicKey_t
{
    boost::multiprecision::cpp_int N;
    boost::multiprecision::cpp_int e;
};
