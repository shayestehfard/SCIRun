
/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

////@file BuildFEVolRHS.h
///@brief This module computes a volumetric right-hand-side. This module is needed for TMS simulations in the BrainStimulator package.
///
///@author
/// ported by Moritz Dannhauer (09/24/2014) from SCIRun4
///
///@details
/// Calculates the divergence of a vector field over the volume. It is designed to calculate the volume integral of the vector field
/// (gradient of the potential in electrical simulations). Builds the volume portion of the RHS of FE calculations where the RHS of
/// the function is GRAD dot F.
/// Input: A FE mesh with field vectors distributed on the elements (constant basis). Output: The Grad dot F

#ifndef MODULES_LEGACY_FINITEELEMENTS_BuildFEVOLRHS_H__
#define MODULES_LEGACY_FINITEELEMENTS_BuildFEVOLRHS_H__

#include <Dataflow/Network/Module.h>
#include <Modules/Legacy/FiniteElements/share.h>

namespace SCIRun {
  namespace Modules {
    namespace FiniteElements {

      class SCISHARE BuildFEVolRHS : public Dataflow::Networks::Module,
        public Has1InputPort<FieldPortTag>,
        public Has1OutputPort<MatrixPortTag>
      {
      public:
        BuildFEVolRHS();
        void setStateDefaults() override;
        void execute() override;

        INPUT_PORT(0, Mesh, Field);
        OUTPUT_PORT(0, RHS, Matrix);
        MODULE_TRAITS_AND_INFO(ModuleHasAlgorithm)
      };

    }
  }
}

#endif
