#pragma once
#include "IEventScriptStopped.h"
#include <string>

//--------------------------------------------------------------
/// \brief	Python script file
//--------------------------------------------------------------
class CEventScriptStopped : public IEventScriptStopped
{
public:
   explicit CEventScriptStopped(int scriptId,
                                const std::string& error);
   virtual ~CEventScriptStopped();

   // IEventScriptStopped Implementation
   int scriptId() const override;
   const std::string& error() const override;
   // [END] IEventScriptStopped Implementation

private:
   const int m_scriptId;
   const std::string m_error;
};

