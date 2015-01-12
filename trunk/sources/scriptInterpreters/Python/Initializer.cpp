#include "stdafx.h"
#include "Initializer.h"
#include "PythonLibInclude.h"

CInitializer::CInitializer()
{
   Py_Initialize();
}

CInitializer::~CInitializer()
{
   Py_Finalize();
}
