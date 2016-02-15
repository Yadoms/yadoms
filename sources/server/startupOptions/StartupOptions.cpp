#include "stdafx.h"
#include "StartupOptions.h"
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/IntValidator.h>
#include <Poco/Util/RegExpValidator.h>
#include "MustExistPathValidator.h"

namespace startupOptions
{
   CStartupOptions::CStartupOptions(Poco::Util::AbstractConfiguration & configContainer)
      :m_configContainer(configContainer)
   {

   }

   CStartupOptions::~CStartupOptions()
   {

   }

   void CStartupOptions::defineOptions(Poco::Util::OptionSet& options)
   {
      options.addOption(
         Poco::Util::Option("port", "p", "set the web server port number")
         .required(false)
         .repeatable(false)
         .argument("port")
         .validator(new Poco::Util::IntValidator(1, 65535))
         .binding("server.port", &m_configContainer));

      options.addOption(
         Poco::Util::Option("webServerIp", "i", "Use a specific web server ip address. Use 0.0.0.0 to accepts connections via all interfaces.")
         .required(false)
         .repeatable(false)
         .argument("ip")
         .validator(new Poco::Util::RegExpValidator("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"))
         .binding("server.ip", &m_configContainer));

      options.addOption(
         Poco::Util::Option("webServerPath", "w", "Use a specific path for web server")
         .required(false)
         .repeatable(false)
         .argument("www")
         .validator(new CMustExistPathValidator())
         .binding("server.www", &m_configContainer));

      options.addOption(
         Poco::Util::Option("logLevel", "l", "set log level, accepted values are : none, fatal, critical, error, warning, notice, information, debug, trace")
         .required(false)
         .repeatable(false)
         .argument("level")
         .validator(new Poco::Util::RegExpValidator("^\\b(none|trace|debug|notice|information|warning|error|fatal|critical)\\b$"))
         .binding("server.logLevel", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databaseFile", "D", "Use a specific dataBase file")
         .required(false)
         .repeatable(false)
         .argument("file")
         .binding("server.databasePath", &m_configContainer));

      options.addOption(
         Poco::Util::Option("debug", "d", "Activate the debug mode (log files are separated by thread)")
         .required(false)
         .repeatable(false)
         .noArgument()
         .binding("server.debug", &m_configContainer));

      options.addOption(
         Poco::Util::Option("noPassword", "d", "Skip password authentication for this instance")
         .required(false)
         .repeatable(false)
         .noArgument()
         .binding("server.noPassword", &m_configContainer));

      options.addOption(
         Poco::Util::Option("acquisitionLifetime", "a", "Specify the acquisition lifetime in days.")
         .required(false)
         .repeatable(false)
         .argument("acquisitionLifetime")
         .validator(new Poco::Util::IntValidator(0, 3650)) //from 0 (unlimited), to 10 years
         .binding("server.acquisitionLifetime", &m_configContainer));
   }

   const std::string CStartupOptions::getLogLevel() const
   {
      return m_configContainer.getString("server.logLevel", "information");
   }

   unsigned int CStartupOptions::getWebServerPortNumber() const
   {
      return m_configContainer.getInt("server.port", 8080);
   }

   const std::string CStartupOptions::getWebServerIPAddress() const
   {
      return m_configContainer.getString("server.ip", "0.0.0.0");
   }

   const std::string CStartupOptions::getWebServerInitialPath() const
   {
      return m_configContainer.getString("server.www", "www");
   }
   const std::string CStartupOptions::getDatabaseFile() const
   {
      return m_configContainer.getString("server.databasePath", "yadoms.db3");
   }
   bool CStartupOptions::getNoPasswordFlag() const
   {
      return m_configContainer.getBool("server.noPassword", false);
   }

   const std::string CStartupOptions::getPluginsPath() const
   {
      return m_configContainer.getString("server.pluginsPath", "plugins");
   }

   const std::string CStartupOptions::getScriptInterpretersPath() const
   {
      return m_configContainer.getString("server.scriptInterpretersPath", "scriptInterpreters");
   }

   bool CStartupOptions::getIsRunningAsService() const
   {
      return m_configContainer.getBool("application.runAsService", false);
   }

   const std::string CStartupOptions::getUpdateSiteUri() const
   {
      return m_configContainer.getString("server.updateSite", "http://www.yadoms.com/downloads/update/");
   }

   int CStartupOptions::getDatabaseAcquisitionLifetime() const
   {
      return m_configContainer.getInt("server.acquisitionLifetime", 30);
   }
} // namespace startupOptions