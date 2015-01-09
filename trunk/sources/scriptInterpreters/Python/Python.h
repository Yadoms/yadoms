#pragma once
#include <shared/script/IScriptInterpreter.h>
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Python interpreter
//--------------------------------------------------------------
class CPython : public shared::script::IScriptInterpreter
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPython();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPython();

   // IScriptInterpreter implementation
   virtual bool isAvailable() const;
   virtual bool canInterpret(const std::string& scriptPath) const;
   virtual boost::shared_ptr<shared::script::IScriptRunner> createRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const;
  // [END] IScriptInterpreter implementation
};



