#pragma once
#include "IScriptLoader.h"

//--------------------------------------------------------------
/// \brief	Python script loader
//--------------------------------------------------------------
class CScriptLoader : public IScriptLoader
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   ///\param[in] scriptPath   Full path of script file
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   CScriptLoader(const std::string& scriptPath);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CScriptLoader();

   // IScriptLoader Implementation
   virtual void load();
   virtual CPythonObject& yMain();
   // [END] IScriptLoader Implementation

private:
   //--------------------------------------------------------------
   /// \brief	Full path of script file
   //--------------------------------------------------------------
   const std::string& m_scriptPath;

   //--------------------------------------------------------------
   /// \brief	Module Python object
   //--------------------------------------------------------------
   CPythonObject m_pyModule;

   //--------------------------------------------------------------
   /// \brief	yMain function Python object
   //--------------------------------------------------------------
   CPythonObject m_pyMainFunction;
};



