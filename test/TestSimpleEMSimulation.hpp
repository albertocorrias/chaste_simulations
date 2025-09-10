#ifndef TESTEMPERFORMANCE_HPP_
#define TESTEMPERFORMANCE_HPP_


#include <cxxtest/TestSuite.h>
#include "BidomainProblem.hpp"
#include "PlaneStimulusCellFactory.hpp"
#include "PetscSetupAndFinalize.hpp"
#include "CardiacElectroMechanicsProblem.hpp"
#include "CardiacElectroMechProbRegularGeom.hpp"
#include "LuoRudy1991.hpp"
#include "NonlinearElasticityTools.hpp"
#include "NobleVargheseKohlNoble1998WithSac.hpp"
#include "Hdf5DataReader.hpp"
#include "ZeroStimulusCellFactory.hpp"


// cell factory which stimulates everything at once
class EntirelyStimulatedTissueCellFactory : public AbstractCardiacCellFactory<2>
{
private:
    boost::shared_ptr<SimpleStimulus> mpStimulus;

public:
    EntirelyStimulatedTissueCellFactory()
        : AbstractCardiacCellFactory<2>(),
          mpStimulus(new SimpleStimulus(-100000.0, 0.5, 100))
    {
    }

    AbstractCardiacCell* CreateCardiacCellForTissueNode(Node<2>* pNode)
    {
        return new CellLuoRudy1991FromCellML(mpSolver, mpStimulus);
    }
};

class TestEMPerformance : public CxxTest::TestSuite
{
public:

    //        SNESSetType(snes, SNESQN);
    //        SNESQNSetType(snes,SNES_QN_LBFGS);
    //        SNESQNSetScaleType(snes,SNES_QN_SCALE_JACOBIAN);
    //        SNESQNSetRestartType(snes, SNES_QN_RESTART_POWELL);

    void TestImplicitNhs2dOneMechanicsElement()
   {
       EntirelyStimulatedTissueCellFactory cell_factory;

       HeartConfig::Instance()->SetSimulationDuration(500.0);
       HeartConfig::Instance()->SetVisualizeWithVtk(true);

       CardiacElectroMechProbRegularGeom<2> problem(COMPRESSIBLE,
                                                    0.05, /* width (cm) */
                                                    10,    /* mech mesh size*/
                                                    50,    /* elec elem each dir */
                                                    &cell_factory,
                                                    NHS,
                                                    1.0,  /* mechanics solve timestep */
                                                    0.01, /* contraction model ode timestep */
                                                    "TestEMOnSquare");
       c_vector<double,2> pos;
       pos(0) = 0.05;
       pos(1) = 0.0;

       problem.SetWatchedPosition(pos);

       problem.Solve();
   }
};
#endif //