#include "stdafx.h"
#include "ScriptLogger.h"


CScriptLogger::CScriptLogger(boost::shared_ptr<yApi::IYInterpreterApi> api,
                             int scriptInstanceId)
   : m_api(api),
     m_scriptInstanceId(scriptInstanceId)
{
}

CScriptLogger::~CScriptLogger()
{
}

void CScriptLogger::init()
{
}

void CScriptLogger::information(const std::string& line)
{
   m_api->onScriptLog(m_scriptInstanceId,
                      false,
                      line);
}

void CScriptLogger::error(const std::string& line)
{
   m_api->onScriptLog(m_scriptInstanceId,
                      true,
                      line);
}

