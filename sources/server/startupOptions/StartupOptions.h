//
// Yadoms startup options loader by command line and file
//
#pragma once

#include "IStartupOptions.h"
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/AbstractConfiguration.h>


namespace startupOptions
{
   //--------------------------------------------------------------
   /// \class Default application options loader
   /// This loader get each option from :
   /// - the command line first, if not provided
   /// - in config file, if not provided
   /// - the default value
   //--------------------------------------------------------------
   class CStartupOptions : public IStartupOptions
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Constructor
      /// \param [in]   configContainer   The configuration container
      //--------------------------------------------------------------
      explicit CStartupOptions(Poco::Util::AbstractConfiguration & configContainer);

      //--------------------------------------------------------------
      /// \brief	   Destructor
      //--------------------------------------------------------------
      virtual ~CStartupOptions();

      //--------------------------------------------------------------
      /// \brief	   Define StartupOptions
      //--------------------------------------------------------------
      void defineOptions(Poco::Util::OptionSet& options) const;

      //--------------------------------------------------------------
      // IStartupOptions implementation (see IStartupOptions declaration for details)
      std::string getLogLevel() const override;
      boost::filesystem::path getLogPath() const override;
      unsigned short getWebServerPortNumber() const override;
      unsigned short getSSLWebServerPortNumber() const override;
      bool getIsWebServerUseSSL() const override;
      std::string getWebServerIPAddress() const override;
      std::string getWebServerInitialPath() const override;
      bool getWebServerAllowExternalAccess() const override;
      EDatabaseEngine getDatabaseEngine() const override;
      std::string getDatabaseSqliteFile() const override;
      std::string getDatabaseSqliteBackupFile() const override;
      std::string getDatabasePostgresqlHost() const override;
      unsigned int getDatabasePostgresqlPort() const override;
      std::string getDatabasePostgresqlDbName() const override;
      std::string getDatabasePostgresqlLogin() const override;
      std::string getDatabasePostgresqlPassword() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlHostAddr() const override;
      Poco::Nullable<int> getDatabasePostgresqlConnectTimeout() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlClientEncoding() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlOptions() const override;
      Poco::Nullable<int> getDatabasePostgresqlKeepAlives() const override;
      Poco::Nullable<int> getDatabasePostgresqlKeepAlivesIdle() const override;
      Poco::Nullable<int> getDatabasePostgresqlKeepAlivesInterval() const override;
      Poco::Nullable<int> getDatabasePostgresqlKeepAlivesCount() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlSslMode() const override;
      Poco::Nullable<int> getDatabasePostgresqlRequireSsl() const override;
      Poco::Nullable<int> getDatabasePostgresqlSslCompression() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlSslCert() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlSslKey() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlSslRootCert() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlSslRevocationList() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlRequirePeer() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlKerberos() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlGssLib() const override;
      Poco::Nullable<std::string> getDatabasePostgresqlService() const override;
      std::string getPluginsPath() const override;
      std::string getScriptInterpretersPath() const override;
      bool getNoPasswordFlag() const override;
      bool getIsRunningAsService() const override;
      Poco::Nullable<std::string> getProxyHost() const override;
      Poco::Nullable<Poco::UInt16> getProxyPort() const override;
      Poco::Nullable<std::string> getProxyUsername() const override;
      Poco::Nullable<std::string> getProxyPassword() const override;
      Poco::Nullable<std::string> getProxyBypass() const override;
      std::string getUpdateSiteUri() const override;
      std::string getBackupPath() const override;
      int getDatabaseAcquisitionLifetime() const override;
      bool getDeveloperMode() const override;
      bool getNoWebServerCacheFlag() const override;
      // [END] IStartupOptions implementation
      //--------------------------------------------------------------

   private:
      //--------------------------------------------------------------
      /// \brief	   Reference for the configuration container
      //--------------------------------------------------------------
      Poco::Util::AbstractConfiguration & m_configContainer;
   };


} // namespace startupOptions