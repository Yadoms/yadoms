#pragma once

//--------------------------------------------------------------
/// \brief	Python sub-interpreter, needed for multi-thread script execution
//--------------------------------------------------------------
class ISubInterpreter
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ISubInterpreter() {}
};
