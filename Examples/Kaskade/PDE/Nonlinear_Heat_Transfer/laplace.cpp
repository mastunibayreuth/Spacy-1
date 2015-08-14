#include <iostream>

#include <dune/grid/config.h>
#define HAVE_UG
#include <dune/grid/uggrid.hh>

#include "fem/gridmanager.hh"
#include "fem/lagrangespace.hh"
#include "fem/variables.hh"
#include "io/vtk.hh"
#include "utilities/gridGeneration.hh" //  createUnitSquare

#include "Adapter/kaskade.hh"
#include "Algorithm/Newton/newton.hh"
#include "Interface/inducedScalarProduct.hh"

#include "laplace.hh"

using namespace Kaskade;

int main()
{
  std::cout << "Start Laplacian tutorial program" << std::endl;

  constexpr int dim = 2;
  int refinements = 8,
      order       = 1;

  using Grid = Dune::UGGrid<dim>;
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
  using Spaces = boost::fusion::vector<H1Space const*>;
  using VariableDescriptions = boost::fusion::vector< Variable<SpaceIndex<0>,Components<1>,VariableId<0> > >;
  using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>;
  using Functional = HeatFunctional<double,VariableSetDesc>;


  GridManager<Grid> gridManager( createUnitSquare<Grid>(1.,false) );
  gridManager.globalRefine(refinements);

  // construction of finite element space for the scalar solution u.
  H1Space temperatureSpace(gridManager,gridManager.grid().leafGridView(),order);

  Spaces spaces(&temperatureSpace);
  VariableSetDesc variableSetDesc(spaces,{ "u" });

  Functional F;
  
  // compute solution
  auto domain = Algorithm::Kaskade::makeVectorSpace<VariableSetDesc>( temperatureSpace );
  auto range = Algorithm::Kaskade::makeVectorSpace<VariableSetDesc>( temperatureSpace );
  Algorithm::connectPrimalDual(domain,range);

  auto f = Algorithm::Kaskade::makeOperator( F , domain.sharedImpl() , range.sharedImpl() );
  domain.setScalarProduct( inducedScalarProduct(f.linearization(domain.element())) );
  
  auto newton = Algorithm::covariantNewton( f );
//  auto newton = contravariantNewton( f );
//  auto newton = localNewton(f );
  newton.setVerbosity(true);
  newton.setRelativeAccuracy(1e-12);
  
  auto sol = newton.solve();

  //construct Galerkin representation
  VariableSetDesc::VariableSet u(variableSetDesc);
  Algorithm::Kaskade::copy(sol,u);
  
  writeVTKFile(gridManager.grid().leafGridView(), u ,"temperature",IoOptions(),order);

  std::cout << "graphical output finished, data in VTK format is written into file temperature.vtu \n";
  std::cout << "End Laplacian tutorial program" << std::endl;
}
