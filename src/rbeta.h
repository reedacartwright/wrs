#pragma once
#ifndef RBETA_H
#define RBETA_H

#include <cstdint>
#include <cmath>
#include <cfloat>

#include "xorshift64.h"

#include "rgamma.h"

inline double rand_beta(xorshift64 &rng, double a, double b) {
  double x1 = rand_gamma (rng, a, 1.0);
  double x2 = rand_gamma (rng, b, 1.0);
  return x1 / (x1 + x2);
}

#endif // RBETA_H
