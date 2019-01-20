#include "Real.h"

#include <Spacy/Vector.h>

#include "RealSpace.h"

#include <cmath>

namespace Spacy
{
    Real::Real( double x, const VectorSpace& space )
        : VectorBase( space ), Mixin::Get< double >( x )
    {
    }

    Real::Real( const VectorSpace& space ) : Real( 0., space )
    {
    }

    Real::Real( double x ) : Real( x, Space::R )
    {
    }

    Real& Real::operator=( double y )
    {
        get() = y;
        return *this;
    }

    Real Real::operator()( const Real& y ) const
    {
        checkDualPairing( *this, y );
        return *this * y;
    }

    Real& Real::operator+=( const Real& y )
    {
        checkSpaceCompatibility( this->space(), y.space() );
        get() += y.get();
        return *this;
    }

    Real& Real::operator-=( const Real& y )
    {
        checkSpaceCompatibility( this->space(), y.space() );
        get() -= y.get();
        return *this;
    }

    Real& Real::operator*=( const Real& y )
    {
        get() *= y.get();
        return *this;
    }

    Real& Real::operator*=( double a )
    {
        get() *= a;
        return *this;
    }

    Real& Real::operator/=( const Real& y )
    {
        get() /= y.get();
        return *this;
    }

    Real Real::operator-() const
    {
        auto result = *this;
        result.get() *= -1;
        return result;
    }

    bool Real::operator==( const Real& y ) const
    {
        checkSpaceCompatibility( space(), y.space() );
        return std::abs( get() - Mixin::get( y ) ) < space().eps();
    }

    ContiguousIterator< double > Real::begin()
    {
        return ContiguousIterator< double >( &get() );
    }

    ContiguousIterator< double > Real::end()
    {
        return ContiguousIterator< double >( &get() + 1 );
    }

    ContiguousIterator< const double > Real::begin() const
    {
        return ContiguousIterator< const double >( &get() );
    }

    ContiguousIterator< const double > Real::end() const
    {
        return ContiguousIterator< const double >( &get() + 1 );
    }

    Real abs( Real x )
    {
        return ( x > 0 ) ? x : -x;
    }

    Real pow( Real x, double y )
    {
        return Real( std::pow( x.get(), y ), x.space() );
    }

    Real sqrt( Real x )
    {
        return Real( std::sqrt( x.get() ), x.space() );
    }

    Real cbrt( Real x )
    {
        return Real( std::cbrt( x.get() ), x.space() );
    }

    Real operator*( const Mixin::Get< Real >& x, Real y )
    {
        return y *= get( x );
    }

    Real operator*( Real x, const Mixin::Get< Real >& y )
    {
        return x *= get( y );
    }
}
