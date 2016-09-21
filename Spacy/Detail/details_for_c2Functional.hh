// This file was automatically generated by friendly type erasure.
// Please do not modify.

#pragma once

#include <memory>
#include <functional>
#include <Spacy/Spaces/RealSpace/real.hh>
#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>

namespace Spacy
{
    namespace C2FunctionalDetail
    {
        template < class Interface, class Buffer >
        struct Functions
        {
            using clone_function = void ( * )( void*, std::shared_ptr< void >& );
            using clone_into_function = void ( * )( void*, Buffer&, std::shared_ptr< void >& );
            using call_const_Vector__ref__function = Real ( * )( const Interface&, void*, const Vector& x );
            using d1_function = Vector ( * )( const Interface&, void*, const Vector& x );
            using d2_function = Vector ( * )( const Interface&, void*, const Vector& x, const Vector& dx );
            using hessian_function = LinearOperator ( * )( const Interface&, void*, const Vector& x );
            using domain_function = const VectorSpace& (*)( const Interface&, void* );

            clone_function clone;
            clone_into_function clone_into;
            call_const_Vector__ref__function call_const_Vector__ref_;
            d1_function d1;
            d2_function d2;
            hessian_function hessian;
            domain_function domain;
        };

        template < class Interface, class Impl >
        struct execution_wrapper
        {
            static Real call_const_Vector__ref_( const Interface& interface, void* impl, const Vector& x )
            {
                return static_cast< const Impl* >( impl )->operator()( x );
            }

            static Vector d1( const Interface& interface, void* impl, const Vector& x )
            {
                return static_cast< const Impl* >( impl )->d1( x );
            }

            static Vector d2( const Interface& interface, void* impl, const Vector& x, const Vector& dx )
            {
                return static_cast< const Impl* >( impl )->d2( x, dx );
            }

            static LinearOperator hessian( const Interface& interface, void* impl, const Vector& x )
            {
                return static_cast< const Impl* >( impl )->hessian( x );
            }

            static const VectorSpace& domain( const Interface& interface, void* impl )
            {
                return static_cast< const Impl* >( impl )->domain();
            }
        };

        template < class Interface, class Impl >
        struct execution_wrapper< Interface, std::reference_wrapper< Impl > >
        {
            static Real call_const_Vector__ref_( const Interface& interface, void* impl, const Vector& x )
            {
                return static_cast< std::reference_wrapper< Impl >* >( impl )->get().operator()( x );
            }

            static Vector d1( const Interface& interface, void* impl, const Vector& x )
            {
                return static_cast< std::reference_wrapper< Impl >* >( impl )->get().d1( x );
            }

            static Vector d2( const Interface& interface, void* impl, const Vector& x, const Vector& dx )
            {
                return static_cast< std::reference_wrapper< Impl >* >( impl )->get().d2( x, dx );
            }

            static LinearOperator hessian( const Interface& interface, void* impl, const Vector& x )
            {
                return static_cast< std::reference_wrapper< Impl >* >( impl )->get().hessian( x );
            }

            static const VectorSpace& domain( const Interface& interface, void* impl )
            {
                return static_cast< std::reference_wrapper< Impl >* >( impl )->get().domain();
            }
        };
    }
}