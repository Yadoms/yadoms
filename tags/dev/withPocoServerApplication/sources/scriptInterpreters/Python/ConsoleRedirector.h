#pragma once
#include "PythonObject.h"

//--------------------------------------------------------------
/// \brief	Python console redirector
//--------------------------------------------------------------
class CConsoleRedirector
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] yApiTupe Python tuple containing yScriptApi pointer
   //--------------------------------------------------------------
   CConsoleRedirector(CPythonObject& yApiTuple);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CConsoleRedirector();
};



