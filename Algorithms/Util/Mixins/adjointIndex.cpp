#include "adjointIndex.hh"

#include <cmath>

Algorithm::Mixin::AdjointIndex::AdjointIndex(unsigned index) noexcept
  : index_(index)
{}

void Algorithm::Mixin::AdjointIndex::setAdjointIndex(unsigned index) noexcept
{
  index_ = index;
  notify();
}

double Algorithm::Mixin::AdjointIndex::adjointIndex() const noexcept
{
  return index_;
}

void Algorithm::Mixin::AdjointIndex::update(DesignPattern::Observer::Subject *changedSubject)
{
  if( changedSubject == this ) return;
  setAdjointIndex( dynamic_cast<AdjointIndex*>(changedSubject)->adjointIndex() );
}

void Algorithm::Mixin::AdjointIndex::attachAdjointIndex(AdjointIndex* other)
{
  attach(other);
}

void Algorithm::Mixin::AdjointIndex::detachAdjointIndex(AdjointIndex* other)
{
  detach(other);
}