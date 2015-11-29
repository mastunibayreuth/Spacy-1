#include "linearOperator.hh"

#include "Spacy/Spaces/realSpace.hh"
#include "linearSolver.hh"

namespace Mock
{
  LinearOperator::LinearOperator(double value)
    : Spacy::Mixin::Impl<double>(value),
      Spacy::OperatorBase(Spacy::Space::R,Spacy::Space::R),
      Spacy::VectorBase(Spacy::Space::R)
  {}

  ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& dx) const
  {
    return Spacy::Real(impl());
  }

  /// Access solver representing \f$A^{-1}\f$
  ::Spacy::LinearSolver LinearOperator::solver() const
  {
    return IndefiniteLinearSolver();
  }
}