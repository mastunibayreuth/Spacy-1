#ifndef ALGORITHM_C2_OPERATOR_HH
#define ALGORITHM_C2_OPERATOR_HH

#include "c1Operator.hh"

namespace Algorithm
{
  class AbstractC2Operator;

  class C2Operator : public C1Operator
  {
  public:
    C2Operator(std::shared_ptr<AbstractC2Operator> impl);

//    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy);

    FunctionSpaceElement d2(const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

    AbstractC2Operator& impl();

    const AbstractC2Operator& impl() const;

  private:
    std::shared_ptr<AbstractC2Operator> impl_;
  };

  template <class Implementation, class... Args>
  C2Operator makeC2Operator(Args&&... args)
  {
    return C2Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C2_OPERATOR_HH
