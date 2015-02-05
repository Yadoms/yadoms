#include "stdafx.h"
#include "SubInterpreter.h"

CSubInterpreter::CSubInterpreter()
{
   // Get the GIL
   PyEval_AcquireLock();

   // Create new interpreter
   m_pythonThreadState = Py_NewInterpreter();
}

CSubInterpreter::~CSubInterpreter()
{
   // Finish interpreter
   Py_EndInterpreter(m_pythonThreadState);

   // Release the GIL
   PyEval_ReleaseLock();
}

