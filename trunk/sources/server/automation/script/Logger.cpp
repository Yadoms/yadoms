#include "stdafx.h"
#include "Logger.h"
#include <shared/Log.h>
#include <shared/currentTime/Provider.h>


namespace automation { namespace script
{

CLogger::CLogger(const boost::filesystem::path& scriptPath)
   :m_logFile(logFile(scriptPath).string(), std::ofstream::out | std::ofstream::app)
{
}

CLogger::~CLogger()
{
}

boost::filesystem::path CLogger::logFile(const boost::filesystem::path& scriptPath)
{
   return scriptPath / "yadomsScript.log";
}

std::string CLogger::now()
{
   std::stringstream dateStream;
   boost::posix_time::time_facet* facet(new boost::posix_time::time_facet());
   facet->format("%Y/%m/%d %H:%M:%S");
   dateStream.imbue(std::locale(std::locale::classic(), facet));
   dateStream << shared::currentTime::Provider::now();
   return dateStream.str();
}

void CLogger::log(const std::string& msg)
{
   // Write into file
   m_logFile << now() << " : " << msg << std::endl;

   // Log normally
   YADOMS_LOG(information) << msg;
}

} } // namespace automation::script

