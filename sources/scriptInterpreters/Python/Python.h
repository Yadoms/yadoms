#pragma once
#include <shared/script/IInterpreter.h>
#include "IPythonExecutable.h"

//--------------------------------------------------------------
/// \brief	Python interpreter
//--------------------------------------------------------------
class CPython : public shared::script::IInterpreter
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] pythonInterpreterPath The path containing the current interpreter
   //--------------------------------------------------------------
   explicit CPython(const boost::filesystem::path& pythonInterpreterPath);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPython();

   // IInterpreter implementation
   std::string name() const override;
   std::string type() const override;
   bool isAvailable() const override;
   std::string loadScriptContent(const std::string& scriptPath) const override;
   void saveScriptContent(const std::string& scriptPath,
                          const std::string& content) const override;
   boost::shared_ptr<shared::process::IProcess> createProcess(const std::string& scriptPath,
                                                              boost::shared_ptr<shared::process::ILogger> scriptLogger,
                                                              boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
                                                              boost::shared_ptr<shared::process::IProcessObserver> processObserver) const override;
   // [END] IInterpreter implementation

private:

   //--------------------------------------------------------------
   /// \brief	The path containing the current interpreter
   //--------------------------------------------------------------
   const boost::filesystem::path m_pythonInterpreterPath;

   //--------------------------------------------------------------
   /// \brief	The Python executable accessor
   //--------------------------------------------------------------
   boost::shared_ptr<IPythonExecutable> m_pythonExecutable;
};

