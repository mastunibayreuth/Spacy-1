#ifndef ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH

#include <memory>
#include <string>

#include "operator.hh"
#include "Util/Mixins/eps.hh"
#include "Util/Mixins/iterativeRefinements.hh"
#include "Util/Mixins/verbosity.hh"

#include "terminationCriteria.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  /**
   * \ingroup linalgsolution
   * \brief regularized preconditioned conjugate gradient method
   *
   * This implements a preconditioned CG iteration for an operator \f$ A: X\to x^* \f$, preconditioned by a
   * preconditioner \f$ B^{-1}: X^* \to X \f$. The default termination criterion is based on an estimate of the relative energy error.
   *
   *
   * Different implementations are available:
   *
   * - "cg": Standard preconditioned conjugate gradient method.
   *
   * - "tcg": Truncated preconditioned conjugate gradient method for nonconvex problems. Stops iteration if a direction of negative curvature is
   * encountered.
   *
   * - "rcg": Regularized preconditioned conjugate gradient method for nonconvex problems. Denote the used operator by \f$A\f$ and the preconditioner by \f$P\f$.
   * Then if a direction of negative curvature is encountered \f$A\f$ is implicitly replaced by the regularization $\f$A+\thetaP\f$. Then the CG method is
   * restarted for the regularized problem. The necessary quantities are available during the standard cg implementation, thus the costs for computing the
   * regularization are neglishible.
   *
   * - "trcg": Truncated regularized preconditioned conjugate gradient method for nonconvex problems, mixing the truncated with the regularized conjugate gradient method. If a direction
   * of negative curvature is encounted and the termination criterion indicates sufficient decrease in the used norm the iteration is stopped. Else, denoting
   * the used operator by \f$A\f$ and the preconditioner by \f$P\f, \f$A\f$ is implicitly replaced by the regularization $\f$A+\thetaP\f$. Then the CG method is
   * restarted for the regularized problem. The necessary quantities are available during the standard cg implementation, thus the costs for computing the
   * regularization are neglishible.
   */
  class CGMethod :
      public Mixin::Eps ,
      public Mixin::IterativeRefinements ,
      public Mixin::Verbosity
  {
    enum class Result { Converged, Failed, EncounteredNonConvexity, TruncatedAtNonConvexity };
    enum class Nonconvexity { None , Encountered };
  public:
    /**
     * \brief Set up conjugate gradient solver.
     *
     * \param A linear operator
     * \param P preconditioner
     * \param type conjugate gradient type ("cg", "rcg", "tcg" or "trcg")
     */
    template <class Op1, class Op2,
              class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
              class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
    CGMethod(Op1&& A, Op2&& P, const std::string& type = "cg" ) :
        A_(std::forward<Op1>(A)), P_(std::forward<Op2>(P)),
        terminate(std::make_shared< RelativeEnergyError >()), type_(type)
    {
      attachEps(terminate.get());
    }

    /**
     * @param x initial guess
     * @param b right hand side
     * @param tolerance tolerance of the termination criterion
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b, double tolerance) const;

    /**
     * @param b right hand side
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& b) const;

    /**
     * @param x initial guess
     * @param b right hand side
     */
    FunctionSpaceElement solve(const FunctionSpaceElement& x, const FunctionSpaceElement& b) const;


    /// Set a new termination criterion which must inherit from CGTerminationCriterion.
    template <class Criterion>
    void setTerminationCriterion(const Criterion& newTerminate)
    {
      terminate.reset(std::make_unique<Criterion>(newTerminate));
    }

    /// Access to the termination criterion.
    CGTerminationCriterion& terminationCriterion() noexcept;

    /// Tells us whether non-convex directions occurred.
    bool encounteredNonConvexity() const noexcept;

    /// Energy norm of the computed solution.
    auto getEnergyNormOfSolution() const noexcept;

    void setType(const std::string& otherType);

  private:
    /// CG Implementation.
    FunctionSpaceElement cgLoop (FunctionSpaceElement x, FunctionSpaceElement r) const;

    /**
     * @brief Apply preconditioner.
     */
    FunctionSpaceElement Q(const FunctionSpaceElement& r) const;

    /// Check step length.
    bool vanishingStep(unsigned step) const;

    /**
     * \return true if iteration should terminate. Throws if a direction of negative curvature is encountered in the standard cg implementation.
     */
    bool terminateOnNonconvexity(double qAq, double qPq, FunctionSpaceElement& x, const FunctionSpaceElement& q, unsigned step) const;

    /**
     * @brief Set \f$theta=0\f$.
     */
    void initializeRegularization() const noexcept;

    /**
     * @brief Replace \f$qAq\f$ with \f$ qAq + \theta qPq\f$.
     */
    void regularize(double& qAq, double qPq) const noexcept;

    /**
     * @brief Update regularization parameter \f$\theta\f$ in regularized conjugate gradient methods.
     */
    void updateRegularization(double qAq, double qPq) const;

    /**
     * @brief Replace \f$r\f$ with \f$ r - \alpha\theta\Pq \f$ in regularized conjugate gradient methods.
     */
    void adjustRegularizedResidual(double alpha, const FunctionSpaceElement& Pq, FunctionSpaceElement& r) const;


    Operator A_;
    Operator P_;
    std::shared_ptr< CGTerminationCriterion > terminate = nullptr;
    mutable Result result = Result::Failed; ///< information about reason for termination
    mutable Nonconvexity nonconvexity = Nonconvexity::None;
    mutable double energyNorm2 = 0.; ///< energy norm squared

    std::string type_ = "cg";

    // parameters for regularized conjugate gradient methods
    mutable double theta = 0;
    unsigned maxIncrease = 1000, minIncrease = 2;
  };
}
#endif // ALGORITHM_CONJUGATE_GRADIENTS_IMPLEMENTATION_HH