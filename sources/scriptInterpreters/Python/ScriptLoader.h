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
   ///\param[in] scriptName   Script name
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   CScriptLoader(const std::string& scriptName);

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
   const boost::filesystem::path m_scriptFile;

   //--------------------------------------------------------------
   /// \brief	Module Python object
   //--------------------------------------------------------------
   CPythonObject m_pyModule;

   //--------------------------------------------------------------
   /// \brief	yMain function Python object
   //--------------------------------------------------------------
   CPythonObject m_pyMainFunction;
};



