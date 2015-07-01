#pragma once
#include "ISubInterpreter.h"

//--------------------------------------------------------------
/// \brief	Python sub-interpreter
//--------------------------------------------------------------
class CSubInterpreter : public ISubInterpreter
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CSubInterpreter();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CSubInterpreter();

   // ISubInterpreter Implementation
   // [END] ISubInterpreter Implementation

   //--------------------------------------------------------------
   /// \brief	The Python thread state
   //--------------------------------------------------------------
   //TODO
   //PyThreadState* m_pythonThreadState;
};



