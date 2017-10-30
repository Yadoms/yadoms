//
// Yadoms startup options interface
//
#pragma once

#include "DatabaseEngine.h"
#include <Poco/Nullable.h>

namespace startupOptions
{
   class IStartupOptions
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IStartupOptions()
      {
      }

      //--------------------------------------------------------------
      /// \brief	    Get the log level
      /// \return     Configured log level
      //--------------------------------------------------------------
      virtual std::string getLogLevel() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the log path
      /// \return     Configured log path
      //--------------------------------------------------------------
      virtual boost::filesystem::path getLogPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the port number
      /// \return     Configured port number
      //--------------------------------------------------------------
      virtual unsigned short getWebServerPortNumber() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the SSL port number
      /// \return     Configured SSL port number
      //--------------------------------------------------------------
      virtual unsigned short getSSLWebServerPortNumber() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Determine if the SSL port is activated
      /// \return     true if SSL is activated 
      //--------------------------------------------------------------
      virtual bool getIsWebServerUseSSL() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the web server ip address
      /// \return     Configured web server ip address
      //--------------------------------------------------------------
      virtual std::string getWebServerIPAddress() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the web server initial path
      /// \return     Configured web server initial path
      //--------------------------------------------------------------
      virtual std::string getWebServerInitialPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Determine if external access of webserver is allowed (only if mobile apps are querying this server)
      /// \return     true if external access are allowed
      //--------------------------------------------------------------
      virtual bool getWebServerAllowExternalAccess() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database engine to use
      /// \return     The database engine (sqlite|postgresql)
      //--------------------------------------------------------------
      virtual EDatabaseEngine getDatabaseEngine() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the path of the database file (sqlite only)
      /// \return     Database file path
      /// \note       If not exist, Yadoms will create the file
      //--------------------------------------------------------------
      virtual std::string getDatabaseSqliteFile() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the path of the backup of database file (sqlite only)
      /// \return     Database file path
      /// \note       If not exist, Yadoms will create the file. if it already exists, backup will be overwritten
      //--------------------------------------------------------------
      virtual std::string getDatabaseSqliteBackupFile() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database host (PostgreSQL database only)
      /// \return     The database host (host name or ip)
      //--------------------------------------------------------------
      virtual std::string getDatabasePostgresqlHost() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database port (PostgreSQL database only)
      /// \return     The database port
      //--------------------------------------------------------------
      virtual unsigned int getDatabasePostgresqlPort() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database name (PostgreSQL database only)
      /// \return     The database name
      //--------------------------------------------------------------
      virtual std::string getDatabasePostgresqlDbName() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database login (PostgreSQL database only)
      /// \return     The database login
      //--------------------------------------------------------------
      virtual std::string getDatabasePostgresqlLogin() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database password (PostgreSQL database only)
      /// \return     The database password
      //--------------------------------------------------------------
      virtual std::string getDatabasePostgresqlPassword() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database host addr (PostgreSQL database only)
      /// \return     The hostaddr
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlHostAddr() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database connection timeout (PostgreSQL database only)
      /// \return     The connection timeout
      //--------------------------------------------------------------
      virtual Poco::Nullable<int> getDatabasePostgresqlConnectTimeout() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database client encoding(PostgreSQL database only)
      /// \return     The client encoding
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlClientEncoding() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database connection command line options (PostgreSQL database only)
      /// \return     The connection command line options
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlOptions() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database keepAlives state (PostgreSQL database only)
      /// \return     The keepAlives state 
      //--------------------------------------------------------------
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlives() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database keepAlives idle (PostgreSQL database only)
      /// \return     The keepAlives idle
      //--------------------------------------------------------------
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlivesIdle() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database keepAlives interval (PostgreSQL database only)
      /// \return     The keepAlives interval
      //--------------------------------------------------------------
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlivesInterval() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database keepAlives count (PostgreSQL database only)
      /// \return     The keepAlives count
      //--------------------------------------------------------------
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlivesCount() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database ssl mode (PostgreSQL database only)
      /// \return     The ssl mode
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslMode() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database ssl required state (PostgreSQL database only)
      /// \return     The ssl required state
      //--------------------------------------------------------------
      virtual Poco::Nullable<int> getDatabasePostgresqlRequireSsl() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database ssl compression state (PostgreSQL database only)
      /// \return     The ssl compression state
      //--------------------------------------------------------------
      virtual Poco::Nullable<int> getDatabasePostgresqlSslCompression() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database ssl certificate (PostgreSQL database only)
      /// \return     The ssl certificate
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslCert() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database ssl key (PostgreSQL database only)
      /// \return     The ssl key
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslKey() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database ssl root certificate (PostgreSQL database only)
      /// \return     The ssl root certificate
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslRootCert() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database ssl revocation list file (PostgreSQL database only)
      /// \return     The ssl revocation list file
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslRevocationList() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database require peer name (PostgreSQL database only)
      /// \return     The require peer name
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlRequirePeer() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database kerberos authentication service (PostgreSQL database only)
      /// \return     The kerberos authentication service
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlKerberos() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database GSSLib path (PostgreSQL database only)
      /// \return     The GSS library path
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlGssLib() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database service(PostgreSQL database only)
      /// \return     The service
      //--------------------------------------------------------------
      virtual Poco::Nullable<std::string> getDatabasePostgresqlService() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the plugin path
      /// \return     The plugin path
      /// \note       If not exist, it returns "plugins"
      //--------------------------------------------------------------
      virtual std::string getPluginsPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the script interpreters path
      /// \return     The script interpreters path
      /// \note       If not exist, it returns "scriptInterpreters"
      //--------------------------------------------------------------
      virtual std::string getScriptInterpretersPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the no password flag
      /// \return     flag indicating if password check is disabled for this instance
      //--------------------------------------------------------------
      virtual bool getNoPasswordFlag() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the no webserver cache flag
      /// \return     flag indicating if webserver cache should be disabled
      //--------------------------------------------------------------
      virtual bool getNoWebServerCacheFlag() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Indicate if yadoms is runnings as a service
      /// \return     true if yadoms is runnings as a service
      //--------------------------------------------------------------
      virtual bool getIsRunningAsService() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the update site URI
      /// \return     The update site URI
      /// \note       If not exist, it returns "http://www.yadoms.com/downloads/update/"
      //--------------------------------------------------------------
      virtual std::string getUpdateSiteUri() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the backup folder
      /// \return     The backup folder
      //--------------------------------------------------------------
      virtual std::string getBackupPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the acquisition lifetime
      /// \return     The acquisition lifetime
      //--------------------------------------------------------------
      virtual int getDatabaseAcquisitionLifetime() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Tell if the developer pode is enabled
      /// \return     true the developer pode is enabled
      //--------------------------------------------------------------
      virtual bool getDeveloperMode() const = 0;
   };
} // namespace startupOptions


