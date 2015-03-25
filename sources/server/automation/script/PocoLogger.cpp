#include "stdafx.h"
#include "PocoLogger.h"

#include <Poco/AutoPtr.h>
#include <Poco/PatternFormatter.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/LogStream.h>


namespace automation { namespace script
{

CPocoLogger::CPocoLogger(const std::string& scriptPath)
   :m_pocoLogger(Poco::Logger::get(std::string("Scripts.locals.") + boost::filesystem::path(scriptPath).stem().string()))
{
   // Configure the logger
   Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter);
   patternFormatter->setProperty("pattern", "%Y/%m/%d %H:%M:%S [%p] : %t");

   Poco::AutoPtr<Poco::SimpleFileChannel> fileChannel(new Poco::SimpleFileChannel);
   fileChannel->setProperty("path", scriptPath + "/yadomsScript.log");
   fileChannel->setProperty("secondaryPath", scriptPath + "/yadomsScript1.log");
   fileChannel->setProperty("rotation", "1 M");
   Poco::AutoPtr<Poco::FormattingChannel> formattingChannel(new Poco::FormattingChannel);
   formattingChannel.assign(new Poco::FormattingChannel(patternFormatter, fileChannel));

   m_pocoLogger.setChannel(formattingChannel);

   // Force log level to not depend on the general logger
   m_pocoLogger.setLevel(Poco::Message::PRIO_TRACE);
}

CPocoLogger::~CPocoLogger()
{         
}

void CPocoLogger::logInformation(const std::string& message)
{
   m_pocoLogger.information(message);
}

void CPocoLogger::logError(const std::string& message)
{
   m_pocoLogger.error(message);
}

} } // namespace automation::script

