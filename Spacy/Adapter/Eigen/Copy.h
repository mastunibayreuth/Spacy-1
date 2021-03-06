#pragma once

#include <Spacy/Vector.h>

#include <Eigen/Dense>

namespace Spacy
{
    /** @addtogroup EigenGroup @{ */
    namespace Rn
    {
        ///  Copy ::Spacy::Vector to flat coefficient vector of %Eigen .
        void copy( const ::Spacy::Vector& x, Eigen::VectorXd& y );

        ///  Copy flat coefficient vector of %Eigen to ::Spacy::Vector.
        void copy( const Eigen::VectorXd& x, ::Spacy::Vector& y );
    }
    /** @} */
}
