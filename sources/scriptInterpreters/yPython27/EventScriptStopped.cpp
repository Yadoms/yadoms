#include "stdafx.h"
#include "EventScriptStopped.h"


CEventScriptStopped::CEventScriptStopped(int scriptId,
   const std::string& error)
   :m_scriptId(scriptId),
   m_error(error)
{
}

CEventScriptStopped::~CEventScriptStopped()
{
}

int CEventScriptStopped::scriptId() const
{
   return m_scriptId;
}

const std::string& CEventScriptStopped::error() const
{
   return m_error;
}