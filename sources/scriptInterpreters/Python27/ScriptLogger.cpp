#include "stdafx.h"
#include "ScriptLogger.h"
#include <shared/Log.h>
#include <shared/StringExtension.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/AutoPtr.h>


CScriptLogger::CScriptLogger(boost::shared_ptr<yApi::IYInterpreterApi> api,
                             int scriptInstanceId,
                             const std::string& loggerName)
   : m_api(api),
     m_scriptInstanceId(scriptInstanceId),
     m_loggerName(loggerName),
     m_logger(Poco::Logger::get(m_loggerName))
{
   const boost::filesystem::path ruleLogfilepath("logs\\scripts\\6");//TODO ne pas laisser en dur
   const std::string interpreterLoggerName("python27");//TODO ne pas laisser en dur
   const std::string logLevel("debug");//TODO ne pas laisser en dur


   //TODO nettoyer
   Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter);
   Poco::AutoPtr<Poco::FormattingChannel> formattingFileChannel;
   Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel());

   Poco::AutoPtr<Poco::SplitterChannel> splitterChannel(new Poco::SplitterChannel);

   patternFormatter->setProperty("pattern", "%H:%M:%S : %T : [%p] : %t");
   patternFormatter->setProperty("times", "local"); //use local datetime

   if (!boost::filesystem::exists(ruleLogfilepath.parent_path().string()))
      if (!boost::filesystem::create_directories(ruleLogfilepath.parent_path().string()))
         throw std::exception(std::string("Cannot create directory " + ruleLogfilepath.parent_path().string()).c_str());

   fileChannel->setProperty("path", ruleLogfilepath.string());
   fileChannel->setProperty("rotation", "daily");
   fileChannel->setProperty("archive", "timestamp");
   fileChannel->setProperty("compress", "true");
   fileChannel->setProperty("purgeCount", "7");
   formattingFileChannel.assign(new Poco::FormattingChannel(patternFormatter,
      fileChannel));

   splitterChannel->addChannel(formattingFileChannel);
   splitterChannel->addChannel(Poco::Logger::get(interpreterLoggerName).getChannel());

   Poco::Logger::get(loggerName).setChannel(splitterChannel);
   Poco::Logger::get(loggerName).setLevel(logLevel);
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
   m_logger.information(shared::CStringExtension::removeEol(line));//TODO optimiser + nettoyer

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

