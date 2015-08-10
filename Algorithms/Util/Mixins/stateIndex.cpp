#include "stateIndex.hh"

#include <cmath>

Algorithm::Mixin::StateIndex::StateIndex(unsigned index) noexcept
  : index_(index)
{}

void Algorithm::Mixin::StateIndex::setStateIndex(unsigned index) noexcept
{
  index_ = index;
  notify();
}

double Algorithm::Mixin::StateIndex::stateIndex() const noexcept
{
  return index_;
}

void Algorithm::Mixin::StateIndex::update(DesignPattern::Observer::Subject *changedSubject)
{
  if( changedSubject == this ) return;
  setStateIndex( dynamic_cast<StateIndex*>(changedSubject)->stateIndex() );
}

void Algorithm::Mixin::StateIndex::attachStateIndex(StateIndex* other)
{
  attach(other);
}

void Algorithm::Mixin::StateIndex::detachStateIndex(StateIndex* other)
{
  detach(other);
}