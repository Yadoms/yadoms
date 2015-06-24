#pragma once
#include <shared/script/IRunner.h>
#include <shared/DataContainer.h>
#include "PythonLibInclude.h"

//--------------------------------------------------------------
/// \brief	Python initializer interface (RAII support)
//--------------------------------------------------------------
class CRunner : public shared::script::IRunner
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   ///\param[in] scriptPath   Script path (without name)
   ///\param[in] interpreterPath       The interpreter path
   ///\param[in] scriptConfiguration   Configuration of the script (optional)
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   CRunner(const std::string& scriptPath, const std::string& interpreterPath, const shared::CDataContainer& scriptConfiguration = shared::CDataContainer());

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRunner();

protected:
   // IRunner Implementation
   virtual void run(shared::script::yScriptApi::IYScriptApi& context);
   virtual void interrupt();
   virtual bool isOk() const;
   virtual std::string error() const;
   // [END] IRunner Implementation

   //--------------------------------------------------------------
   /// \brief	Check if a Python error was raised and if it's the corresponding error
   /// \param[in] pyException Expected exception
   /// \param[in] pyErrorCode Expected error code
   /// \return true is error raised and corresponding to provided data
   //--------------------------------------------------------------
   bool isPythonError(PyObject* pyException, int pyErrorCode) const;

private:
   //--------------------------------------------------------------
   /// \brief	Script path (withou name)
   //--------------------------------------------------------------
   const std::string m_scriptPath;

   //--------------------------------------------------------------
   ///\brief   Configuration of the script
   //--------------------------------------------------------------
   const shared::CDataContainer m_scriptConfiguration;
   
   //--------------------------------------------------------------
   ///\brief   Interpreter path
   //--------------------------------------------------------------
   const std::string m_interpreterPath;

   //--------------------------------------------------------------
   ///\brief   Last error message (empty if no error)
   //--------------------------------------------------------------
   std::string m_lastError;
};



