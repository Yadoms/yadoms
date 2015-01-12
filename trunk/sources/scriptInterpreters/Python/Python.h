#pragma once
#include <shared/script/IInterpreter.h>
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Python interpreter
//--------------------------------------------------------------
class CPython : public shared::script::IInterpreter
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

   // IInterpreter implementation
   virtual bool isAvailable() const;
   virtual bool canInterpret(const std::string& scriptPath) const;
   virtual boost::shared_ptr<shared::script::IRunner> createRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const;
  // [END] IInterpreter implementation
};



