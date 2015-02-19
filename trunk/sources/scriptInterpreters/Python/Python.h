#pragma once
#include <shared/script/IInterpreter.h>
#include <shared/DataContainer.h>
#include "IInitializer.h"

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
   virtual std::string name() const;
   virtual bool isAvailable() const;
   virtual std::string loadScriptContent(const std::string& scriptPath) const;
   virtual void saveScriptContent(const std::string& scriptPath, const std::string& content) const;
   virtual boost::shared_ptr<shared::script::IRunner> createRunner(const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration) const;
  // [END] IInterpreter implementation

private:
   //--------------------------------------------------------------
   /// \brief	The Python interpreter initializer
   //--------------------------------------------------------------
   const boost::shared_ptr<IInitializer> m_initializer;
};



