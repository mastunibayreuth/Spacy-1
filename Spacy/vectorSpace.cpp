#include "vectorSpace.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/cmath.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

#include "Spacy/Spaces/primalDualProductSpace.hh"
#include "Spacy/Spaces/ProductSpace/vectorSpace.hh"
#include "Spacy/vector.hh"

#include <boost/type_erasure/is_empty.hpp>

#include <stdexcept>
#include <utility>

namespace Spacy
{
  namespace
  {
    class HilbertSpaceNorm
    {
    public:
      explicit HilbertSpaceNorm(ScalarProduct sp)
        : sp_(std::move(sp))
      {}

      Real operator()(const Vector& x) const
      {
        return sqrt(sp_(x,x));
      }

    private:
      ScalarProduct sp_;
    };
  }


  VectorSpace::VectorSpace(VectorCreator impl, Norm norm, bool defaultIndex)
    : Mixin::Impl<VectorCreator>{std::move(impl)},
      norm_{norm}
  {
    if(defaultIndex) index_ = 0;
  }

  VectorSpace::VectorSpace(VectorSpace&& V)
    : Mixin::Impl<VectorCreator>{V.impl()} ,
      norm_{std::move(V.norm_)} ,
      sp_{std::move(V.sp_)} ,
      index_{std::move(V.index_)} ,
      primalSpaces_{std::move(V.primalSpaces_)} ,
      dualSpaces_{std::move(V.dualSpaces_)}
  {
    if( &V == V.dualSpace_)
      setDualSpace(this);
    else
      if( V.dualSpace_ != nullptr)
        setDualSpace(V.dualSpace_);
  }

  VectorSpace& VectorSpace::operator=(VectorSpace&& V)
  {
    Mixin::Impl<VectorCreator>::operator=(std::move(V));
    norm_ = std::move(V.norm_);
    sp_ = std::move(V.sp_);
    index_ = V.index_;
    primalSpaces_ = std::move(V.primalSpaces_);
    dualSpaces_ = std::move(V.dualSpaces_);
    if( &V == V.dualSpace_)
      setDualSpace(this);
    else
      if( V.dualSpace_ != nullptr )
        setDualSpace(V.dualSpace_);
    return *this;
  }

  void VectorSpace::setNorm(Norm norm)
  {
    norm_ = std::move(norm);
  }

  const Norm& VectorSpace::norm() const
  {
    return norm_;
  }

  Vector VectorSpace::vector() const
  {
    return impl()(this);
  }

  unsigned VectorSpace::index() const
  {
    return index_;
  }

  void VectorSpace::setScalarProduct(ScalarProduct sp)
  {
    sp_ = std::move(sp);
    setNorm( HilbertSpaceNorm{sp_} );
  }

  const ScalarProduct& VectorSpace::scalarProduct() const
  {
    if( !sp_ ) throw std::runtime_error( (std::string("No scalar product defined in space ") + std::to_string(index()) + "!").c_str());
    return sp_;
  }

  void VectorSpace::addDualSpace(const VectorSpace& Y)
  {
    dualSpaces_.push_back(Y.index());
  }

  bool VectorSpace::isPrimalWRT(const VectorSpace& Y) const
  {
    for( auto index : dualSpaces_ )
      if( index == Y.index() )
        return true;

    return false;
  }

  const VectorSpace& VectorSpace::dualSpace() const
  {
    if( dualSpace_ == nullptr ) throw InvalidArgumentException("VectorSpace::dualSpace()");
    return *dualSpace_;
  }

  void VectorSpace::setDualSpace(const VectorSpace* Y)
  {
    dualSpace_ = Y;
    addDualSpace(*dualSpace_);
  }

  bool VectorSpace::isHilbertSpace() const
  {
    if( sp_ ) return true;
    return false;
  }

  bool VectorSpace::isAdmissible(const Vector& x) const
  {
    return restriction_(x);
  }

  void VectorSpace::setRestriction(std::function<bool(const Vector&)> f)
  {
    restriction_ = std::move(f);
  }


  VectorSpace makeBanachSpace(VectorCreator creator, Norm norm)
  {
    return VectorSpace{std::move(creator),std::move(norm)};
  }

  namespace
  {
    void connectSubSpaces(PrimalDualProductSpace::VectorCreator& creator)
    {
      for( auto i = 0u; i < creator.subSpaces().size(); ++i)
      {
        auto& subSpace = creator.subSpace(i);
        subSpace.setDualSpace(&subSpace);
        subSpace.addDualSpace(subSpace);
      }
    }

    void connectSubSpaces(ProductSpace::VectorCreator& creator)
    {
      for( auto i = 0u; i < creator.subSpaces().size(); ++i)
      {
        auto& subSpace = creator.subSpace(i);
        subSpace.setDualSpace(&subSpace);
        subSpace.addDualSpace(subSpace);
      }
    }

    template <class Creator>
    void connectSubSpacesIfConsistent(VectorCreator& creator)
    {
      if( is<Creator>(creator) )
        connectSubSpaces( cast_ref<Creator>(creator) );
    }

    void connectSubSpaces(VectorCreator& creator)
    {
      connectSubSpacesIfConsistent<PrimalDualProductSpace::VectorCreator>(creator);
      connectSubSpacesIfConsistent<ProductSpace::VectorCreator>(creator);
    }
  }

  VectorSpace makeHilbertSpace(VectorCreator creator, ScalarProduct scalarProduct, bool defaultIndex)
  {
    auto V = VectorSpace{std::move(creator),HilbertSpaceNorm{scalarProduct},defaultIndex};
    V.setScalarProduct(std::move(scalarProduct));
    V.setDualSpace(&V);
    V.addDualSpace(V);

    connectSubSpaces(creator);

    return V;
  }

  void connect(VectorSpace& X, VectorSpace& Y)
  {
    X.addDualSpace( Y );
  }

  void connectAsPrimalDualPair(VectorSpace& X, VectorSpace& Y)
  {
    X.addDualSpace( Y );
  }


  void checkSpaceCompatibility(const VectorSpace& V, const VectorSpace& W)
  {
    if( V.index() != W.index() )
      throw IncompatibleSpaceException(V.index(),W.index());
  }

  void checkSpaceCompatibility(const VectorSpace* V, const VectorSpace* W)
  {
    checkSpaceCompatibility(*V,*W);
  }
}
