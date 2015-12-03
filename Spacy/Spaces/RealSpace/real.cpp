// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "real.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/invalidArgumentException.hh"

#include "Spacy/vector.hh"
#include "vectorSpace.hh"

#include <cmath>

namespace
{
  double computePow(double x, double y)
  {
    return pow(x,y);
  }

  double computeSqrt(double x)
  {
    return sqrt(x);
  }

  double computeCbrt(double x)
  {
    return cbrt(x);
  }
}

namespace Spacy
{
  Real::Real(double x, const VectorSpace& space)
    : VectorBase(space),
      Mixin::Get<double>(x)
  {}

  Real::Real(const VectorSpace& space)
    : Real(0.,space)
  {}

  Real::Real(double x)
    : Real(x,Space::R)
  {}

  Real& Real::operator=(double y)
  {
    get() = y;
    return *this;
  }

  Real& Real::operator=(const Vector& y)
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::operator=(const ::Spacy::Vector&)");
    get() = cast_ref<Real>(y).get();
    return *this;
  }

  Real::Real(const Vector& y)
    : VectorBase(y.space()),
      Mixin::Get<double>(0)
  {
    if( !is<Real>(y) ) throw InvalidArgumentException("Real::operator=(const ::Spacy::Vector&)");
    *this = cast_ref<Real>(y);
  }

  Real Real::operator()(const Real& y) const
  {
    checkDualPairing(*this,y);
    return *this * y;
  }

  Real& Real::operator+=(const Real& y)
  {
    checkSpaceCompatibility(this->space(),y.space());
    get() += y.get();
    return *this;
  }

  Real& Real::operator-=(const Real& y)
  {
    checkSpaceCompatibility(this->space(),y.space());
    get() -= y.get();
    return *this;
  }

  Real& Real::operator*=(const Real& y)
  {
    get() *= y.get();
    return *this;
  }

  Real Real::operator-() const
  {
    auto result = *this;
    result.get() *= -1;
    return result;
  }

  bool Real::operator==(const Real& y) const
  {
    checkSpaceCompatibility(this->space(),y.space());
    return get() == y.get();
  }


  double& toDouble(Real &x)
  {
    return x.get();
  }

  double toDouble(const Real &x)
  {
    return x.get();
  }

  double toDouble(const Vector& x)
  {
    if( !is<Real>(x) ) throw InvalidArgumentException("Spacy::toDouble(const Vector& v)");
    return toDouble(cast_ref<Real>(x));
  }

  double& toDouble(Vector& x)
  {
    if( !is<Real>(x) ) throw InvalidArgumentException("Spacy::toDouble(Vector& v)");
    return toDouble(cast_ref<Real>(x));
  }

  Real operator/(Real x, const Real& y)
  {
    return x *= 1./toDouble(y);
  }

  Real operator/(double x, const Real& y)
  {
    return Real(x,y.space())/y;
  }

  Real operator/(const Real& x, double y)
  {
    return x/Real(y,x.space());
  }


  Real operator*(Real x, const Real& y)
  {
    toDouble(x) *= toDouble(y);
    return x;
  }

//  Real operator*(double x, Real y)
//  {
//    toDouble(y) *= x;
//    return y;
//  }

//  Real operator*(Real x, double y)
//  {
//    toDouble(x) *= y;
//    return x;
//  }


  Real operator+(Real x, const Real& y)
  {
    toDouble(x) += toDouble(y);
    return x;
  }

  Real operator+(Real x, double y)
  {
    toDouble(x) += y;
    return x;
  }

  Real operator+(double x, Real y)
  {
    toDouble(y) += x;
    return y;
  }

  Real operator+(Real x, const Vector& y)
  {
    return x += cast_ref<Real>(y);
  }

  Real operator+(const Vector& x, Real y)
  {
    return y += cast_ref<Real>(x);
  }


  Real operator-(Real x, const Real& y)
  {
    toDouble(x) -= toDouble(y);
    return x;
  }

  Real operator-(Real x, double y)
  {
    toDouble(x) -= y;
    return x;
  }

  Real operator-(double x, Real y)
  {
    y *= -1;
    toDouble(y) += x;
    return y;
  }


  Vector operator*(Real a, Vector x)
  {
    return x *= toDouble(a);
  }

  Vector operator*(Vector x, Real a)
  {
    return x *= toDouble(a);
  }


  bool operator<(const Real& x, const Real& y)
  {
    return toDouble(x) < toDouble(y);
  }

  bool operator>(const Real& x, const Real& y)
  {
    return toDouble(x) > toDouble(y);
  }

  bool operator<(const Real& x, double y)
  {
    return toDouble(x) < y;
  }

  bool operator<(double x, const Real& y)
  {
    return x < toDouble(y);
  }

  bool operator>(const Real& x, double y)
  {
    return toDouble(x) > y;
  }

  bool operator>(double x, const Real& y)
  {
    return x > toDouble(y);
  }

  bool operator>=(double x, const Real& y)
  {
    return x >= toDouble(y);
  }

  bool operator>=(const Real& x, double y)
  {
    return toDouble(x) >= y;
  }

  bool operator>=(const Real& x, const Real& y)
  {
    return toDouble(x) >= toDouble(y);
  }

  bool operator<=(const Real& x, const Real& y)
  {
    return toDouble(x) <= toDouble(y);
  }

  bool operator<=(double x, const Real& y)
  {
    return x <= toDouble(y);
  }

  bool operator<=(const Real& x, double y)
  {
    return toDouble(x) <= y;
  }

  bool operator==(const Real& x, double y)
  {
    return toDouble(x) == y;
  }

  bool operator==(double x, const Real& y)
  {
    return x == toDouble(y);
  }

  std::ostream& operator<<(std::ostream& os, const Real& x)
  {
    os << toDouble(x);
    return os;
  }

  Real abs(Real x)
  {
    return ( x > 0 ) ? x : -x;
  }


  Real max(const Real& x, const Real& y)
  {
    return ( toDouble(x) > toDouble(y) ) ? x : y;
  }

  Real min(const Real& x, const Real& y)
  {
    return ( toDouble(x) < toDouble(y) ) ? x : y;
  }

  Real pow(Real x, double y)
  {
    return Real(computePow(toDouble(x),y),x.space());
  }

  Real sqrt(Real x)
  {
    return Real(computeSqrt(toDouble(x)),x.space());
  }

  Real cbrt(Real x)
  {
    return Real(computeCbrt(toDouble(x)),x.space());
  }
}
