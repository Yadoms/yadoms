#include "stdafx.h"
#include "Logger.h"
#include <shared/Log.h>

#include <Poco/AutoPtr.h>
#include <Poco/PatternFormatter.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FormattingChannel.h>


namespace automation { namespace script
{

CLogger::CLogger(const boost::filesystem::path& scriptPath)
   :m_pocoLogger(Poco::Logger::get(std::string("Scripts.locals.") + scriptPath.stem().string())),
   m_stream(m_pocoLogger)
{
   // Configure the logger

   // The logs are sent to the splitterChannel, that dispatch to the yadomsChannel and
   // to the fileFormatterChannel, which format message and send to fileChannel :
   //
   //                                |==============================> yadomsChannel
   // message ===> splitterChannel ==|
   //                                |====> fileFormatterChannel ===> fileChannel
   //
   // Messages sent to yadomsChannel are not formatted, because this channel already format messages.

   // fileChannel
   Poco::AutoPtr<Poco::SimpleFileChannel> fileChannel(new Poco::SimpleFileChannel);
   fileChannel->setProperty("path", logFile(scriptPath).string());

   // fileFormatterChannel
   Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter);
   patternFormatter->setProperty("pattern", "%Y/%m/%d %H:%M:%S [%p] : %t");
   patternFormatter->setProperty("times", "local"); //use local datetime
   Poco::AutoPtr<Poco::FormattingChannel> fileFormatterChannel(new Poco::FormattingChannel(patternFormatter, fileChannel));

   // yadomsChannel
   Poco::Channel* yadomsChannel = shared::CLog::logger().getChannel();

   // splitterChannel
   Poco::AutoPtr<Poco::SplitterChannel> splitterChannel(new Poco::SplitterChannel);
   splitterChannel->addChannel(fileFormatterChannel);
   splitterChannel->addChannel(yadomsChannel);

   m_pocoLogger.setChannel(splitterChannel);

   // Force log level to not depend on the general logger
   m_pocoLogger.setLevel(Poco::Message::PRIO_TRACE);
}

CLogger::~CLogger()
{
   Poco::Logger::destroy(m_pocoLogger.name());
}

boost::filesystem::path CLogger::logFile(const boost::filesystem::path& scriptPath)
{
   return scriptPath / "yadomsScript.log";
}

std::ostream& CLogger::out()
{
   return m_stream.information();
}

std::ostream& CLogger::error()
{
   return m_stream.error();
}

} } // namespace automation::script

