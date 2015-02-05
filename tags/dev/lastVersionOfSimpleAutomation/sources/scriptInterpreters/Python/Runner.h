#pragma once
#include <shared/script/IRunner.h>
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Python initializer interface (RAII support)
//--------------------------------------------------------------
class CRunner : public shared::script::IRunner
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   ///\param[in] scriptName   Script name
   ///\param[in] scriptConfiguration   Configuration of the script (optional)
   /// \throw CRunnerException if unable to load script
   //--------------------------------------------------------------
   CRunner(const std::string& scriptName, const shared::CDataContainer& scriptConfiguration = shared::CDataContainer());

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRunner();

protected:
   // IRunner Implementation
   virtual void run(shared::script::yScriptApi::IYScriptApi& context);
   virtual void stop();
   virtual bool isOk() const;
   virtual std::string error() const;
   // [END] IRunner Implementation

private:
   //--------------------------------------------------------------
   /// \brief	Script name
   //--------------------------------------------------------------
   const std::string& m_scriptName;

   //--------------------------------------------------------------
   ///\brief   Configuration of the script
   //--------------------------------------------------------------
   const shared::CDataContainer& m_scriptConfiguration;

   //--------------------------------------------------------------
   ///\brief   Last error message (empty if no error)
   //--------------------------------------------------------------
   std::string m_lastError;
};



