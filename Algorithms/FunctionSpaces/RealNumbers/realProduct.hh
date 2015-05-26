#ifndef ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
#define ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Interface/abstractScalarProduct.hh"

namespace Algorithm
{
  /// Scalar product for RealSpace.
  class RealProduct : public AbstractScalarProduct
  {
    /// Compute x*y.
    double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
