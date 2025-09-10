/*

Copyright (c) 2005-2024, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "CardiacElectroMechanicsProblemVTK.hpp"
#include "CardiacElectroMechanicsProblem.hpp"
#include "OutputFileHandler.hpp"
#include "ReplicatableVector.hpp"
#include "HeartConfig.hpp"
#include "LogFile.hpp"
#include "ChastePoint.hpp"
#include "Element.hpp"
#include "BoundaryConditionsContainer.hpp"
#include "AbstractDynamicLinearPdeSolver.hpp"
#include "TimeStepper.hpp"
#include "TrianglesMeshWriter.hpp"
#include "Hdf5ToMeshalyzerConverter.hpp"
#include "Hdf5ToCmguiConverter.hpp"
#include "MeshalyzerMeshWriter.hpp"
#include "PetscTools.hpp"
#include "ImplicitCardiacMechanicsSolver.hpp"
#include "ExplicitCardiacMechanicsSolver.hpp"
#include "CmguiDeformedSolutionsWriter.hpp"
#include "VoltageInterpolaterOntoMechanicsMesh.hpp"
#include "BidomainProblem.hpp"




template<unsigned DIM, unsigned ELEC_PROB_DIM>
CardiacElectroMechanicsProblemVTK<DIM,ELEC_PROB_DIM>::CardiacElectroMechanicsProblemVTK(
            CompressibilityType compressibilityType,
            ElectricsProblemType electricsProblemType,
            TetrahedralMesh<DIM,DIM>* pElectricsMesh,
            QuadraticMesh<DIM>* pMechanicsMesh,
            AbstractCardiacCellFactory<DIM>* pCellFactory,
            ElectroMechanicsProblemDefinition<DIM>* pProblemDefinition,
            std::string outputDirectory)
      : CardiacElectroMechanicsProblem<DIM,ELEC_PROB_DIM>(compressibilityType,electricsProblemType,pElectricsMesh,pMechanicsMesh,pCellFactory,pProblemDefinition,outputDirectory)
{

}

template<unsigned DIM, unsigned ELEC_PROB_DIM>
CardiacElectroMechanicsProblemVTK<DIM,ELEC_PROB_DIM>::~CardiacElectroMechanicsProblemVTK()
{

}



template<unsigned DIM, unsigned ELEC_PROB_DIM>
void CardiacElectroMechanicsProblemVTK<DIM,ELEC_PROB_DIM>::OnEndOfTimeStep(unsigned counter)
{
    std::cout<<counter<<std::endl;
}

// Explicit instantiation

//note: 1d incompressible material doesn't make sense
template class CardiacElectroMechanicsProblemVTK<2,1>;
template class CardiacElectroMechanicsProblemVTK<3,1>;
template class CardiacElectroMechanicsProblemVTK<2,2>;
template class CardiacElectroMechanicsProblemVTK<3,2>;
