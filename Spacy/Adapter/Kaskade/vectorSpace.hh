// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_KASKADE_VECTOR_SPACE_HH
#define SPACY_ADAPTER_KASKADE_VECTOR_SPACE_HH

#include <type_traits>

#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/Util/Mixins/impl.hh"

#include "l2Product.hh"
#include "vector.hh"
#include "util.hh"

namespace Spacy
{
  /** @addtogroup Kaskade
   * @{
   */
  namespace Kaskade
  {
    /// Creator for vector space elements for %Kaskade 7
    template <class Description>
    class VectorCreator :
        public Mixin::Impl<Description>
    {
    public:
      /**
       * @brief Create from %Kaskade 7 function space.
       * @param space single %Kaskade 7 function space (no product space)
       */
      template <class... Args,
                class = std::enable_if_t<std::is_constructible<Description,Args...>::value> >
      VectorCreator(Args&&... args)
        : Mixin::Impl<Description>( std::forward<Args>(args)... )
      {}

      /// Generate vector for %Kaskade 7.
      Vector<Description> operator()(const VectorSpace* space) const
      {
        return Vector<Description>{*space};
      }
    };

    /**
     * @brief Create single space with hilbert space structure for %Kaskade 7.
     * @param space single %Kaskade 7 function space (no product space)
     */
    template <class Description>
    auto makeHilbertSpace(const Description& description)
    {
      return ::Spacy::makeHilbertSpace( Kaskade::VectorCreator<Description>{description} , l2Product<Description>{} );
    }

    /**
     * @brief Create product space with hilbert space structure for %Kaskade 7.
     * @param spaces boost fusion forward sequence of const pointers to %Kaskade 7 function spaces
     * @param primalIds ids of primal variables
     * @param dualIds ids of dual variables
     */
    template <class Description>
    auto makeHilbertSpace(const Description& description, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds = {})
    {
      constexpr int n = boost::fusion::result_of::size<typename Description::Variables>::value;
      std::vector<std::shared_ptr< VectorSpace > > newSpaces( n );

      Detail::MakeSpaces<Description,0,n>::apply(description,newSpaces);

      if( dualIds.size() > 0)
        return ::Spacy::ProductSpace::makeHilbertSpace( newSpaces , primalIds , dualIds );

      return ::Spacy::ProductSpace::makeHilbertSpace( newSpaces , primalIds );
    }
  }
  /** @} */
}

#endif // SPACY_ADAPTER_KASKADE_VECTOR_SPACE_HH
