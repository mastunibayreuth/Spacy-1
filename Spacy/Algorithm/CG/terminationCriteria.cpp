// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "terminationCriteria.hh"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

#include <iostream>

namespace Spacy
{
  namespace CG
  {
    namespace Termination
    {
      bool StrakosTichyEnergyError::operator()() const
      {
        using std::max;
        auto tol = max( relativeAccuracy() , eps() );
        if( verbosityLevel() > 1 ) std::cout << "      termination criterion (relative error): " << sqrt(squaredRelativeError()) << "\n      tolerance: " << tol << std::endl;
        return scaledGamma2.size() > maxSteps() || ( scaledGamma2.size() > lookAhead_ && squaredRelativeError() < tol*tol );
      }


      void StrakosTichyEnergyError::update(double alpha, double qAq, double, double rPINVr)
      {
        using std::abs;
        scaledGamma2.push_back( alpha * rPINVr );
        energyNorm2 += alpha * rPINVr;
        stepLength2 = abs(qAq);
      }

      bool StrakosTichyEnergyError::vanishingStep() const noexcept
      {
        using std::min;
        double tol = absoluteAccuracy()*absoluteAccuracy();
        if( energyNorm2 > tol) tol = min(tol,eps()*eps()*energyNorm2);
        return stepLength2 < tol;
      }

      void StrakosTichyEnergyError::clear() noexcept
      {
        scaledGamma2.clear();
        energyNorm2 = 0;
      }

      void StrakosTichyEnergyError::setLookAhead(unsigned lookAhead) noexcept
      {
        lookAhead_ = lookAhead;
      }

      bool StrakosTichyEnergyError::minimalDecreaseAchieved() const noexcept
      {
        return squaredRelativeError() < minimalAccuracy()*minimalAccuracy();
      }

      double StrakosTichyEnergyError::squaredRelativeError() const noexcept
      {
        if( scaledGamma2.size() < lookAhead_ ) return std::numeric_limits<double>::max();
        return std::accumulate(scaledGamma2.end() - lookAhead_, scaledGamma2.end(), 0.) / energyNorm2;
      }
    }
  }
}
