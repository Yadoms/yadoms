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
    /// \brief Default application options loader
    /// This loader get each option from :
    /// - the command line first, if not provided
    /// - in config file, if not provided
    /// - the default value
    //--------------------------------------------------------------
    class CStartupOptions final : public IStartupOptions
    {
    public:
        //--------------------------------------------------------------
        /// \brief                          Constructor
        /// \param [in]   configContainer   The configuration container
        //--------------------------------------------------------------
        explicit CStartupOptions(Poco::Util::AbstractConfiguration& configContainer);

        ~CStartupOptions() override = default;

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
        std::optional<std::string> getDatabasePostgresqlHostAddr() const override;
        std::optional<int> getDatabasePostgresqlConnectTimeout() const override;
        std::optional<std::string> getDatabasePostgresqlClientEncoding() const override;
        std::optional<std::string> getDatabasePostgresqlOptions() const override;
        std::optional<int> getDatabasePostgresqlKeepAlives() const override;
        std::optional<int> getDatabasePostgresqlKeepAlivesIdle() const override;
        std::optional<int> getDatabasePostgresqlKeepAlivesInterval() const override;
        std::optional<int> getDatabasePostgresqlKeepAlivesCount() const override;
        std::optional<std::string> getDatabasePostgresqlSslMode() const override;
        std::optional<int> getDatabasePostgresqlRequireSsl() const override;
        std::optional<int> getDatabasePostgresqlSslCompression() const override;
        std::optional<std::string> getDatabasePostgresqlSslCert() const override;
        std::optional<std::string> getDatabasePostgresqlSslKey() const override;
        std::optional<std::string> getDatabasePostgresqlSslRootCert() const override;
        std::optional<std::string> getDatabasePostgresqlSslRevocationList() const override;
        std::optional<std::string> getDatabasePostgresqlRequirePeer() const override;
        std::optional<std::string> getDatabasePostgresqlKerberos() const override;
        std::optional<std::string> getDatabasePostgresqlGssLib() const override;
        std::optional<std::string> getDatabasePostgresqlService() const override;
        std::string getPluginsPath() const override;
        std::string getScriptInterpretersPath() const override;
        bool getNoPasswordFlag() const override;
        bool getIsRunningAsService() const override;
        std::optional<std::string> getProxyHost() const override;
        std::optional<std::uint16_t> getProxyPort() const override;
        std::optional<std::string> getProxyUsername() const override;
        std::optional<std::string> getProxyPassword() const override;
        std::optional<std::string> getProxyBypass() const override;
        std::string getUpdateSiteUri() const override;
        std::string getBackupPath() const override;
        int getDatabaseAcquisitionLifetime() const override;
        bool getDeveloperMode() const override;
        bool getNoWebServerCacheFlag() const override;
        std::string getPython2Path() const override;
        std::string getPython3Path() const override;
        // [END] IStartupOptions implementation
        //--------------------------------------------------------------

    private:
        //--------------------------------------------------------------
        /// \brief	   Reference for the configuration container
        //--------------------------------------------------------------
        Poco::Util::AbstractConfiguration& m_configContainer;
    };
} // namespace startupOptions
