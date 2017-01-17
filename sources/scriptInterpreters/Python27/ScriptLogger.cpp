#include "stdafx.h"
#include "ScriptLogger.h"
#include <shared/Log.h>
#include <shared/StringExtension.h>


CScriptLogger::CScriptLogger(boost::shared_ptr<yApi::IYInterpreterApi> api,
                             int scriptInstanceId,
                             const std::string& loggerName)
   : m_api(api),
     m_scriptInstanceId(scriptInstanceId),
     m_loggerName(loggerName),
     m_logger(Poco::Logger::get(m_loggerName))
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
   YADOMS_LOG(information) << shared::CStringExtension::removeEol(line);

   //TODO virer + dépendances
   //m_api->onScriptLog(m_scriptInstanceId,
   //                   false,
   //                   line);
}

void CScriptLogger::error(const std::string& line)
{
   YADOMS_LOG(error) << shared::CStringExtension::removeEol(line);

   //TODO virer + dépendances
   //m_api->onScriptLog(m_scriptInstanceId,
   //                   true,
   //                   line);
}

