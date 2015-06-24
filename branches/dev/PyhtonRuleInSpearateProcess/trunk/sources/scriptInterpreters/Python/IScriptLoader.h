#pragma once
#include "PythonObject.h"

//--------------------------------------------------------------
/// \brief	Python script loader interface
//--------------------------------------------------------------
class IScriptLoader
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IScriptLoader() {}

   //--------------------------------------------------------------
   /// \brief	Load the script, to be ready to run it (call its entry point)
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   virtual void load() = 0;

   //--------------------------------------------------------------
   /// \brief	Get the Python module
   /// \return Python module (script should be load)
   //--------------------------------------------------------------
   virtual CPythonObject& module() = 0;

   //--------------------------------------------------------------
   /// \brief	Get the yMain function
   /// \return yMain function pointer as Python object
   //--------------------------------------------------------------
   virtual CPythonObject& yMain() = 0;
};
