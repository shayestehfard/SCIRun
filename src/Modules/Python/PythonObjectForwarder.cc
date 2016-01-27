/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2016 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
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

#include <Modules/Python/PythonObjectForwarder.h>
#include <Core/Datatypes/String.h>
#include <boost/thread.hpp>

using namespace SCIRun::Modules::Python;
using namespace SCIRun::Core::Datatypes;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Algorithms::Python;

ALGORITHM_PARAMETER_DEF(Python, PollingIntervalMilliseconds);
ALGORITHM_PARAMETER_DEF(Python, NumberOfRetries);
ALGORITHM_PARAMETER_DEF(Python, PythonObject);

const ModuleLookupInfo PythonObjectForwarder::staticInfo_("PythonObjectForwarder", "Python", "SCIRun");

PythonObjectForwarder::PythonObjectForwarder() : Module(staticInfo_) 
{
  INITIALIZE_PORT(PythonMatrix);
  INITIALIZE_PORT(PythonField);
  INITIALIZE_PORT(PythonString);
}

void PythonObjectForwarder::setStateDefaults()
{
  auto state = get_state();
  state->setValue(Parameters::PollingIntervalMilliseconds, 200);
  state->setValue(Parameters::NumberOfRetries, 50);
}

void PythonObjectForwarder::execute()
{
  auto state = get_state();
  int tries = 0;
  const int maxTries = state->getValue(Parameters::NumberOfRetries).toInt();
  const int waitTime = state->getValue(Parameters::PollingIntervalMilliseconds).toInt();
  auto valueOption = state->getTransientValue(Parameters::PythonObject);
  
  while (tries < maxTries && !valueOption)
  {
    std::ostringstream ostr;
    ostr << "PythonObjectForwarder looking up value attempt #" << (tries+1) << "/" << maxTries;
    remark(ostr.str());

    valueOption = state->getTransientValue(Parameters::PythonObject);

    tries++;
    boost::this_thread::sleep(boost::posix_time::milliseconds(waitTime));
  }

  if (valueOption)
  {
    auto valueStr = transient_value_cast<std::string>(valueOption);
    if (!valueStr.empty())
      sendOutput(PythonString, boost::make_shared<String>(valueStr));
    else
      sendOutput(PythonString, boost::make_shared<String>("Empty string or non-string received"));
  }
}