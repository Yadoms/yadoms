#include "stdafx.h"
#include "ScriptLogger.h"
#include <shared/Log.h>


CScriptLogger::CScriptLogger(boost::shared_ptr<yApi::IYInterpreterApi> api,
                             int scriptInstanceId,
                             const std::string& loggerName)
   : m_api(api),
     m_scriptInstanceId(scriptInstanceId),
     m_loggerName(loggerName)
{
}

CScriptLogger::~CScriptLogger()
{
}

void CScriptLogger::init()
{
   YADOMS_LOG_CONFIGURE(m_loggerName);
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

