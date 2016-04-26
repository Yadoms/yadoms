#include "stdafx.h"
#include "StartupOptions.h"
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/IntValidator.h>
#include <Poco/Util/RegExpValidator.h>
#include "MustExistPathValidator.h"
#include "EnumValidator.h"

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

      //use separator as variable, because toAllString expect a reference
      std::string separator = ", ";
      std::string allDbEngines = EDatabaseEngine::toAllString(separator);

      options.addOption(
         Poco::Util::Option("databaseEngine", "E", "Choose database engine, accepted values are : " + allDbEngines)
         .required(false)
         .repeatable(false)
         .argument("engine")
         .validator(new CEnumValidator<EDatabaseEngine>())
         .binding("server.databaseEngine", &m_configContainer));    
      
      options.addOption(
         Poco::Util::Option("databaseSqliteFile", "D", "Use a specific database file (only for sqlite database)")
         .required(false)
         .repeatable(false)
         .argument("file")
         .binding("server.sqlite.databasePath", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlHost", "ph", "Specify PotgreSQL database host (only for postgresql database)")
         .required(false)
         .repeatable(false)
         .argument("pgHost")
         .binding("server.pgsql.host", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlPort", "pp", "Specify PotgreSQL database port (only for postgresql database)")
         .required(false)
         .repeatable(false)
         .argument("pgPort")
         .validator(new Poco::Util::IntValidator(1, 65535))
         .binding("server.pgsql.port", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbName", "pn", "Specify PotgreSQL database name (only for postgresql database)")
         .required(false)
         .repeatable(false)
         .argument("pgName")
         .binding("server.pgsql.dbname", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbLogin", "pl", "Specify PotgreSQL database login (only for postgresql database)")
         .required(false)
         .repeatable(false)
         .argument("pgLogin")
         .binding("server.pgsql.login", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbPassword", "pp", "Specify PotgreSQL database password (only for postgresql database)")
         .required(false)
         .repeatable(false)
         .argument("pgPassword")
         .binding("server.pgsql.password", &m_configContainer));

      options.addOption(
         Poco::Util::Option("noPassword", "n", "Skip password authentication for this instance")
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

      options.addOption(
         Poco::Util::Option("developerMode", "d", "Activate the developer mode")
         .required(false)
         .repeatable(false)
         .noArgument()
         .binding("server.developerMode", &m_configContainer));
   }

   const std::string CStartupOptions::getLogLevel() const
   {
      return m_configContainer.getString("server.logLevel", "information");
   }

   const unsigned int CStartupOptions::getWebServerPortNumber() const
   {
      return m_configContainer.getUInt("server.port", 8080);
   }

   const std::string CStartupOptions::getWebServerIPAddress() const
   {
      return m_configContainer.getString("server.ip", "0.0.0.0");
   }

   const std::string CStartupOptions::getWebServerInitialPath() const
   {
      return m_configContainer.getString("server.www", "www");
   }


   const EDatabaseEngine CStartupOptions::getDatabaseEngine() const
   {
      return EDatabaseEngine(m_configContainer.getString("server.databaseEngine", EDatabaseEngine::kSqlite.toString()));
   }

   const std::string CStartupOptions::getDatabaseSqliteFile() const
   {
      return m_configContainer.getString("server.sqlite.databasePath", "yadoms.db3");
   }

   const std::string CStartupOptions::getDatabasePostgresqlHost() const
   {
      return m_configContainer.getString("server.pgsql.host", "127.0.0.1");
   }

   const unsigned int CStartupOptions::getDatabasePostgresqlPort() const
   {
      return m_configContainer.getUInt("server.pgsql.port", 5432);
   }

   const std::string CStartupOptions::getDatabasePostgresqlDbName() const
   {
      return boost::to_lower_copy(m_configContainer.getString("server.pgsql.dbname", "yadoms"));
   }

   const std::string CStartupOptions::getDatabasePostgresqlLogin() const
   {
      return m_configContainer.getString("server.pgsql.login", "");
   }

   const std::string CStartupOptions::getDatabasePostgresqlPassword() const
   {
      return m_configContainer.getString("server.pgsql.password", "");
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

   bool CStartupOptions::getDeveloperMode() const
   {
      return m_configContainer.getBool("server.developerMode", false);
   }

} // namespace startupOptions