#ifndef TESTEMVTKOUTPUT_HPP_
#define TESTEMVTKOUTPUT_HPP_


#include <cxxtest/TestSuite.h>
#include "BidomainProblem.hpp"
#include "PlaneStimulusCellFactory.hpp"
#include "PetscSetupAndFinalize.hpp"
#include "CardiacElectroMechanicsProblemVTK.hpp"
#include "LuoRudy1991.hpp"
#include "NonlinearElasticityTools.hpp"
#include "NobleVargheseKohlNoble1998WithSac.hpp"
#include "ElectroMechanicsProblemDefinition.hpp"
#include "AbstractCardiacCellFactory.hpp"


// cell factory which stimulates everything at once
class MyStimulatedTissueCellFactory : public AbstractCardiacCellFactory<2>
{
private:
    boost::shared_ptr<SimpleStimulus> mpStimulus;

public:
    MyStimulatedTissueCellFactory()
        : AbstractCardiacCellFactory<2>(),
          mpStimulus(new SimpleStimulus(-100000.0, 0.5, 100))
    {
    }

    AbstractCardiacCell* CreateCardiacCellForTissueNode(Node<2>* pNode)
    {
        return new CellLuoRudy1991FromCellML(mpSolver, mpStimulus);
    }
};

class TestEMVTKOutput : public CxxTest::TestSuite
{
public:


   void TestSimpleMesh()
   {
        MyStimulatedTissueCellFactory  cell_factory;

        TetrahedralMesh<2,2> electrics_mesh;
        electrics_mesh.ConstructRegularSlabMesh(0.01, 0.05, 0.05);

        QuadraticMesh<2> mechanics_mesh;
        mechanics_mesh.ConstructRegularSlabMesh(0.025, 0.05, 0.05);

        std::vector<unsigned> fixed_nodes;
        std::vector<c_vector<double,2> > fixed_node_locations;

        // fix the node at the origin so that the solution is well-defined (ie unique)
        fixed_nodes.push_back(0);
        fixed_node_locations.push_back(zero_vector<double>(2));

        // for the rest of the nodes, if they lie on X=0, fix x=0 but leave y free.
        for (unsigned i=1 /*not 0*/; i<mechanics_mesh.GetNumNodes(); i++)
        {
            if (fabs(mechanics_mesh.GetNode(i)->rGetLocation()[0])<1e-6)
            {
                c_vector<double,2> new_position;
                new_position(0) = 0.0;
                new_position(1) = SolidMechanicsProblemDefinition<2>::FREE;
                fixed_nodes.push_back(i);
                fixed_node_locations.push_back(new_position);
            }
        }

        ElectroMechanicsProblemDefinition<2> problem_defn(mechanics_mesh);
        problem_defn.SetContractionModel(KERCHOFFS2003,1.0);
        problem_defn.SetUseDefaultCardiacMaterialLaw(COMPRESSIBLE);
        problem_defn.SetFixedNodes(fixed_nodes, fixed_node_locations);
        problem_defn.SetMechanicsSolveTimestep(1.0);

        // the following is just for coverage - applying a zero pressure so has no effect on deformation
        std::vector<BoundaryElement<1,2>*> boundary_elems;
        boundary_elems.push_back(* (mechanics_mesh.GetBoundaryElementIteratorBegin()));
        problem_defn.SetApplyNormalPressureOnDeformedSurface(boundary_elems, 0.0);

        HeartConfig::Instance()->SetSimulationDuration(600.0);

        CardiacElectroMechanicsProblem<2,1>   problem(COMPRESSIBLE,
                                                      MONODOMAIN,
                                                      &electrics_mesh,
                                                      &mechanics_mesh,
                                                      &cell_factory,
                                                      &problem_defn,
                                                      "TestCardiacEmHomogeneousEverythingCompressible");
        

        
        problem.Solve();
   }
};
#endif //