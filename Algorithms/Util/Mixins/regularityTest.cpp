#include "regularityTest.hh"

namespace Algorithm
{
  Mixin::RegularityTest::RegularityTest(double lowerBound) noexcept
    : lowerBound_(lowerBound)
  {}

  void Mixin::RegularityTest::setLowerBound(double lowerBound) noexcept
  {
    lowerBound_ = lowerBound;
    notify();
  }

  double Mixin::RegularityTest::lowerBound() const noexcept
  {
    return lowerBound_;
  }

  bool Mixin::RegularityTest::regularityTestPassed(double nu) const noexcept
  {
    return nu > lowerBound_;
  }

  bool Mixin::RegularityTest::regularityTestFailed(double nu) const noexcept
  {
    return !regularityTestPassed(nu);
  }

  void Mixin::RegularityTest::update(DesignPattern::Observer::Subject *changedSubject)
  {
    if( changedSubject == this ) return;
    setLowerBound( dynamic_cast<RegularityTest*>(changedSubject)->lowerBound() );
  }

  void Mixin::RegularityTest::attachRegularityTest(RegularityTest* other)
  {
    attach(other);
  }

  void Mixin::RegularityTest::detachRegularityTest(RegularityTest* other)
  {
    detach(other);
  }
}
