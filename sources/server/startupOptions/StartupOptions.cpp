#include "stdafx.h"
#include "StartupOptions.h"
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/IntValidator.h>
#include <Poco/Util/RegExpValidator.h>
#include "MustExistPathValidator.h"
#include "EnumValidator.h"
#include "ListValidator.h"

namespace startupOptions
{
   CStartupOptions::CStartupOptions(Poco::Util::AbstractConfiguration & configContainer)
      :m_configContainer(configContainer)
   {

   }

   CStartupOptions::~CStartupOptions()
   {

   }

   void CStartupOptions::defineOptions(Poco::Util::OptionSet& options) const
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
         Poco::Util::Option("databaseSqliteBackupFile", "B", "Choose the database backup file (only for sqlite database)")
         .required(false)
         .repeatable(false)
         .argument("backupFile")
         .binding("server.sqlite.databaseBackupPath", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlHost", "ph", "Specify PotgreSQL database host (only for postgresql database). Name of host to connect to. If this begins with a slash, it specifies Unix-domain communication rather than TCP/IP communication; the value is the name of the directory in which the socket file is stored. The default behavior when host is not specified is to connect to a Unix-domain socket in /tmp (or whatever socket directory was specified when PostgreSQL was built). On machines without Unix-domain sockets, the default is to connect to localhost.")
         .required(false)
         .repeatable(false)
         .argument("pgHost")
         .binding("server.pgsql.host", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlHostAddr", "pha", "Specify PotgreSQL database hostaddr (only for postgresql database). Numeric IP address of host to connect to. This should be in the standard IPv4 address format, e.g., 172.28.40.9. If your machine supports IPv6, you can also use those addresses. TCP/IP communication is always used when a nonempty string is specified for this parameter.Using hostaddr instead of host allows the application to avoid a host name look-up, which might be important in applications with time constraints. However, a host name is required for Kerberos, GSSAPI, or SSPI authentication methods, as well as for verify-full SSL certificate verification. The following rules are used:\n  1/If host is specified without hostaddr, a host name lookup occurs.\n  2/ If hostaddr is specified without host, the value for hostaddr gives the server network address.The connection attempt will fail if the authentication method requires a host name.\n  3/ If both host and hostaddr are specified, the value for hostaddr gives the server network address.The value for host is ignored unless the authentication method requires it, in which case it will be used as the host name.\nNote that authentication is likely to fail if host is not the name of the server at network address hostaddr.Also, note that host rather than hostaddr is used to identify the connection in ~/ .pgpass(see https://www.postgresql.org/docs/9.3/static/libpq-pgpass.html ).\nWithout either a host name or host address, libpq will connect using a local Unix - domain socket; or on machines without Unix - domain sockets, it will attempt to connect to localhost.")
         .required(false)
         .repeatable(false)
         .argument("pgHostAddr")
         .binding("server.pgsql.hostaddr", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlPort", "pp", "Specify PotgreSQL database port (only for postgresql database). Port number to connect to at the server host, or socket file name extension for Unix-domain connections.")
         .required(false)
         .repeatable(false)
         .argument("pgPort")
         .validator(new Poco::Util::IntValidator(1, 65535))
         .binding("server.pgsql.port", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbName", "pn", "Specify PotgreSQL database name (only for postgresql database). The database name. Defaults to be the same as the user name. In certain contexts, the value is checked for extended formats; see https://www.postgresql.org/docs/9.3/static/libpq-connect.html#LIBPQ-CONNSTRING for more details on those.")
         .required(false)
         .repeatable(false)
         .argument("pgName")
         .binding("server.pgsql.dbname", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbLogin", "pl", "Specify PotgreSQL database login (only for postgresql database). PostgreSQL user name to connect as. Defaults to be the same as the operating system name of the user running the application.")
         .required(false)
         .repeatable(false)
         .argument("pgLogin")
         .binding("server.pgsql.login", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbPassword", "pp", "Specify PotgreSQL database password (only for postgresql database). Password to be used if the server demands password authentication.")
         .required(false)
         .repeatable(false)
         .argument("pgPassword")
         .binding("server.pgsql.password", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbConnectTimeout", "pt", "Specify PotgreSQL database connection timeout (only for postgresql database). Maximum wait for connection, in seconds (write as a decimal integer string). Zero or not specified means wait indefinitely. It is not recommended to use a timeout of less than 2 seconds.")
         .required(false)
         .repeatable(false)
         .argument("pgConnectTimeout")
         .validator(new Poco::Util::IntValidator(0, 2147483647))
         .binding("server.pgsql.connect-timeout", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbClientEncoding", "pe", "Specify PotgreSQL database client encoding (only for postgresql database). This sets the client_encoding configuration parameter for this connection. In addition to the values accepted by the corresponding server option, you can use auto to determine the right encoding from the current locale in the client (LC_CTYPE environment variable on Unix systems).")
         .required(false)
         .repeatable(false)
         .argument("pgClientEncoding")
         .binding("server.pgsql.client-encoding", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbOptions", "po", "Specify PotgreSQL database command line options (only for postgresql database). Adds command-line options to send to the server at run-time. For example, setting this to -c geqo=off sets the session's value of the geqo parameter to off. For a detailed discussion of the available options, consult https://www.postgresql.org/docs/9.3/static/runtime-config.html")
         .required(false)
         .repeatable(false)
         .argument("pgOptions")
         .binding("server.pgsql.options", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbKeepAlives", "pk", "Specify PotgreSQL database keep alives (only for postgresql database). Controls whether client-side TCP keepalives are used. The default value is 1, meaning on, but you can change this to 0, meaning off, if keepalives are not wanted. This parameter is ignored for connections made via a Unix-domain socket.")
         .required(false)
         .repeatable(false)
         .argument("pgKeepAlives")
         .validator(new Poco::Util::IntValidator(0, 1))
         .binding("server.pgsql.keep-alives", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbKeepAlivesIdle", "pki", "Specify PotgreSQL database keep alives idle time (only for postgresql database). Controls the number of seconds of inactivity after which TCP should send a keepalive message to the server. A value of zero uses the system default. This parameter is ignored for connections made via a Unix-domain socket, or if keepalives are disabled. It is only supported on systems where the TCP_KEEPIDLE or TCP_KEEPALIVE socket option is available, and on Windows; on other systems, it has no effect.")
         .required(false)
         .repeatable(false)
         .argument("pgKeepAlivesIdle")
         .validator(new Poco::Util::IntValidator(0, 2147483647))
         .binding("server.pgsql.keep-alives-idle", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbKeepAlivesInterval", "pkn", "Specify PotgreSQL database keep alives interval time (only for postgresql database). Controls the number of seconds after which a TCP keepalive message that is not acknowledged by the server should be retransmitted. A value of zero uses the system default. This parameter is ignored for connections made via a Unix-domain socket, or if keepalives are disabled. It is only supported on systems where the TCP_KEEPINTVL socket option is available, and on Windows; on other systems, it has no effect.")
         .required(false)
         .repeatable(false)
         .argument("pgKeepAlivesInterval")
         .validator(new Poco::Util::IntValidator(0, 2147483647))
         .binding("server.pgsql.keep-alives-interval", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbKeepAlivesCount", "pkc", "Specify PotgreSQL database keep alives interval time (only for postgresql database). Controls the number of TCP keepalives that can be lost before the client's connection to the server is considered dead. A value of zero uses the system default. This parameter is ignored for connections made via a Unix-domain socket, or if keepalives are disabled. It is only supported on systems where the TCP_KEEPCNT socket option is available; on other systems, it has no effect.")
         .required(false)
         .repeatable(false)
         .argument("pgKeepAlivesCount")
         .validator(new Poco::Util::IntValidator(0, 2147483647))
         .binding("server.pgsql.keep-alives-count", &m_configContainer));


      std::vector<std::string> sslModes = boost::assign::list_of("")("disable")("allow")("prefer")("require")("verify-ca")("verify-full");

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbSslMode", "ps", "Specify PotgreSQL database ssl mode (only for postgresql database). This option determines whether or with what priority a secure SSL TCP/IP connection will be negotiated with the server. There are six modes:\n  disable : only try a non - SSL connection\n  allow : first try a non - SSL connection; if that fails, try an SSL connection\n  prefer(default) : first try an SSL connection; if that fails, try a non - SSL connection\n  require : only try an SSL connection.If a root CA file is present, verify the certificate in the same way as if verify-ca was specified\n  verify-ca : only try an SSL connection, and verify that the server certificate is issued by a trusted certificate authority(CA)\n  verify-full : only try an SSL connection, verify that the server certificate is issued by a trusted CA and that the server host name matches that in the certificate\n\nSee https://www.postgresql.org/docs/9.3/static/libpq-ssl.html for a detailed description of how these options work.\n\nsslmode is ignored for Unix domain socket communication.If PostgreSQL is compiled without SSL support, using options require, verify - ca, or verify - full will cause an error, while options allow and prefer will be accepted but libpq will not actually attempt an SSL connection.")
         .required(false)
         .repeatable(false)
         .argument("pgSslMode")
         .validator(new CListValidator(sslModes))
         .binding("server.pgsql.ssl-mode", &m_configContainer));


      options.addOption(
         Poco::Util::Option("databasePostgresqlDbRequireSsl", "pr", "Specify PotgreSQL databaserequire ssl (only for postgresql database). If set to 1, an SSL connection to the server is required (this is equivalent to sslmode require). libpq will then refuse to connect if the server does not accept an SSL connection. If set to 0 (default), libpq will negotiate the connection type with the server (equivalent to sslmode prefer). This option is only available if PostgreSQL is compiled with SSL support.")
         .required(false)
         .repeatable(false)
         .argument("pgRequireSsl")
         .validator(new Poco::Util::IntValidator(0, 1))
         .binding("server.pgsql.require-ssl", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbSslCompression", "psslc", "Specify PotgreSQL database use ssl compression (only for postgresql database). If set to 1 (default), data sent over SSL connections will be compressed (this requires OpenSSL version 0.9.8 or later). If set to 0, compression will be disabled (this requires OpenSSL 1.0.0 or later). This parameter is ignored if a connection without SSL is made, or if the version of OpenSSL used does not support it.\n\nCompression uses CPU time, but can improve throughput if the network is the bottleneck.Disabling compression can improve response time and throughput if CPU performance is the limiting factor.")
         .required(false)
         .repeatable(false)
         .argument("pgSslCompression")
         .validator(new Poco::Util::IntValidator(0, 1))
         .binding("server.pgsql.ssl-compression", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbSslcert", "psslcert", "Specify PotgreSQL database ssl certificate name (only for postgresql database). This parameter specifies the file name of the client SSL certificate, replacing the default ~/.postgresql/postgresql.crt. This parameter is ignored if an SSL connection is not made.")
         .required(false)
         .repeatable(false)
         .argument("pgSslcert")
         .binding("server.pgsql.ssl-cert", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbSslkey", "psslk", "Specify PotgreSQL database ssl certificate key (only for postgresql database). This parameter specifies the location for the secret key used for the client certificate. It can either specify a file name that will be used instead of the default ~/.postgresql/postgresql.key, or it can specify a key obtained from an external 'engine' (engines are OpenSSL loadable modules). An external engine specification should consist of a colon-separated engine name and an engine-specific key identifier. This parameter is ignored if an SSL connection is not made.")
         .required(false)
         .repeatable(false)
         .argument("pgSslkey")
         .binding("server.pgsql.ssl-key", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbSslRootCert", "psslrc", "Specify PotgreSQL database ssl root certificate (only for postgresql database). This parameter specifies the name of a file containing SSL certificate authority (CA) certificate(s). If the file exists, the server's certificate will be verified to be signed by one of these authorities. The default is ~/.postgresql/root.crt.")
         .required(false)
         .repeatable(false)
         .argument("pgSslRootCertificate")
         .binding("server.pgsql.ssl-root", &m_configContainer));
      
      options.addOption(
         Poco::Util::Option("databasePostgresqlDbSslRevocationList", "psslrl", "Specify PotgreSQL database ssl certificate revocation list filename (only for postgresql database). This parameter specifies the file name of the SSL certificate revocation list (CRL). Certificates listed in this file, if it exists, will be rejected while attempting to authenticate the server's certificate. The default is ~/.postgresql/root.crl.")
         .required(false)
         .repeatable(false)
         .argument("pgSslRevocationList")
         .binding("server.pgsql.ssl-revocation", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbRequirePeer", "ppeer", "Specify PotgreSQL database require peer (only for postgresql database). This parameter specifies the operating-system user name of the server, for example requirepeer=postgres. When making a Unix-domain socket connection, if this parameter is set, the client checks at the beginning of the connection that the server process is running under the specified user name; if it is not, the connection is aborted with an error. This parameter can be used to provide server authentication similar to that available with SSL certificates on TCP/IP connections. (Note that if the Unix-domain socket is in /tmp or another publicly writable location, any user could start a server listening there. Use this parameter to ensure that you are connected to a server run by a trusted user.) This option is only supported on platforms for which the peer authentication method is implemented; see https://www.postgresql.org/docs/9.3/static/auth-methods.html#AUTH-PEER")
         .required(false)
         .repeatable(false)
         .argument("pgRequirePeer")
         .binding("server.pgsql.require-peer", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbKerberos", "pker", "Specify PotgreSQL database kerberos authentication service (only for postgresql database). Kerberos service name to use when authenticating with Kerberos 5 or GSSAPI. This must match the service name specified in the server configuration for Kerberos authentication to succeed. (See also https://www.postgresql.org/docs/9.3/static/auth-methods.html#KERBEROS-AUTH and https://www.postgresql.org/docs/9.3/static/auth-methods.html#GSSAPI-AUTH)")
         .required(false)
         .repeatable(false)
         .argument("pgKerberos")
         .binding("server.pgsql.kerberos", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbGsslib", "pgss", "Specify PotgreSQL database gss library (only for postgresql database). GSS library to use for GSSAPI authentication. Only used on Windows. Set to gssapi to force libpq to use the GSSAPI library for authentication instead of the default SSPI.")
         .required(false)
         .repeatable(false)
         .argument("pgGsslib")
         .binding("server.pgsql.gss-lib", &m_configContainer));

      options.addOption(
         Poco::Util::Option("databasePostgresqlDbService", "psrv", "Specify PotgreSQL database service (only for postgresql database). Service name to use for additional parameters. It specifies a service name in pg_service.conf that holds additional connection parameters. This allows applications to specify only a service name so connection parameters can be centrally maintained. See https://www.postgresql.org/docs/9.3/static/libpq-pgservice.html")
         .required(false)
         .repeatable(false)
         .argument("pgService")
         .binding("server.pgsql.service", &m_configContainer));

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

   std::string CStartupOptions::getLogLevel() const
   {
      return m_configContainer.getString("server.logLevel", "information");
   }

   unsigned short CStartupOptions::getWebServerPortNumber() const
   {
      return (unsigned short)m_configContainer.getUInt("server.port", 8080);
   }

   unsigned short CStartupOptions::getSSLWebServerPortNumber() const
   {
      return (unsigned short)m_configContainer.getUInt("server.SSLport", 443);
   }
   
   bool CStartupOptions::getIsWebServerUseSSL() const
   {
      return m_configContainer.getBool("server.useSSL", false);
   }

   std::string CStartupOptions::getWebServerIPAddress() const
   {
      return m_configContainer.getString("server.ip", "0.0.0.0");
   }

   std::string CStartupOptions::getWebServerInitialPath() const
   {
      return m_configContainer.getString("server.www", "www");
   }

   EDatabaseEngine CStartupOptions::getDatabaseEngine() const
   {
      return EDatabaseEngine(m_configContainer.getString("server.databaseEngine", EDatabaseEngine::kSqlite.toString()));
   }

   std::string CStartupOptions::getDatabaseSqliteFile() const
   {
      return m_configContainer.getString("server.sqlite.databasePath", "yadoms.db3");
   }

   std::string CStartupOptions::getDatabaseSqliteBackupFile() const
   {
      return m_configContainer.getString("server.sqlite.databaseBackupPath", "yadoms_backup.db3");
   }
   
   std::string CStartupOptions::getDatabasePostgresqlHost() const
   {
      return m_configContainer.getString("server.pgsql.host", "127.0.0.1");
   }

   unsigned int CStartupOptions::getDatabasePostgresqlPort() const
   {
      return m_configContainer.getUInt("server.pgsql.port", 5432);
   }

   std::string CStartupOptions::getDatabasePostgresqlDbName() const
   {
      return boost::to_lower_copy(m_configContainer.getString("server.pgsql.dbname", "yadoms"));
   }

   std::string CStartupOptions::getDatabasePostgresqlLogin() const
   {
      return m_configContainer.getString("server.pgsql.login", "");
   }

   std::string CStartupOptions::getDatabasePostgresqlPassword() const
   {
      return m_configContainer.getString("server.pgsql.password", "");
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlHostAddr() const
   {
      if (m_configContainer.has("server.pgsql.hostaddr"))
         return m_configContainer.getString("server.pgsql.hostaddr", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CStartupOptions::getDatabasePostgresqlConnectTimeout() const
   {
      if (m_configContainer.has("server.pgsql.connect-timeout"))
         return m_configContainer.getInt("server.pgsql.connect-timeout", 60);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlClientEncoding() const
   {
      if (m_configContainer.has("server.pgsql.client-encoding"))
         return m_configContainer.getString("server.pgsql.client-encoding", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlOptions() const
   {
      if (m_configContainer.has("server.pgsql.options"))
         return m_configContainer.getString("server.pgsql.options", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CStartupOptions::getDatabasePostgresqlKeepAlives() const
   {
      if (m_configContainer.has("server.pgsql.keep-alives"))
         return m_configContainer.getInt("server.pgsql.keep-alives", 1);
      return Poco::NULL_GENERIC;
   }


   Poco::Nullable<int> CStartupOptions::getDatabasePostgresqlKeepAlivesIdle() const
   {
      if (m_configContainer.has("server.pgsql.keep-alives-idle"))
         return m_configContainer.getInt("server.pgsql.keep-alives-idle", 0 /* = use system default*/);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CStartupOptions::getDatabasePostgresqlKeepAlivesInterval() const
   {
      if (m_configContainer.has("server.pgsql.keep-alives-interval"))
         return m_configContainer.getInt("server.pgsql.keep-alives-interval", 0 /* = use system default*/);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CStartupOptions::getDatabasePostgresqlKeepAlivesCount() const
   {
      if (m_configContainer.has("server.pgsql.keep-alives-count"))
         return m_configContainer.getInt("server.pgsql.keep-alives-count", 0 /* = use system default*/);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlSslMode() const
   {
      if (m_configContainer.has("server.pgsql.ssl-mode"))
         return m_configContainer.getString("server.pgsql.ssl-mode", "prefer");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CStartupOptions::getDatabasePostgresqlRequireSsl() const
   {
      if (m_configContainer.has("server.pgsql.require-ssl"))
         return m_configContainer.getInt("server.pgsql.require-ssl", 0);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CStartupOptions::getDatabasePostgresqlSslCompression() const
   {
      if (m_configContainer.has("server.pgsql.ssl-compression"))
         return m_configContainer.getInt("server.pgsql.ssl-compression", 1);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlSslCert() const
   {
      if (m_configContainer.has("server.pgsql.ssl-cert"))
         return m_configContainer.getString("server.pgsql.ssl-cert", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlSslKey() const
   {
      if (m_configContainer.has("server.pgsql.ssl-key"))
         return m_configContainer.getString("server.pgsql.ssl-key", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlSslRootCert() const
   {
      if (m_configContainer.has("server.pgsql.ssl-root"))
         return m_configContainer.getString("server.pgsql.ssl-root", "");
      return Poco::NULL_GENERIC;
   }


   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlSslRevocationList() const
   {
      if (m_configContainer.has("server.pgsql.ssl-revocation"))
         return m_configContainer.getString("server.pgsql.ssl-revocation", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlRequirePeer() const
   {
      if (m_configContainer.has("server.pgsql.require-peer"))
         return m_configContainer.getString("server.pgsql.require-peer", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlKerberos() const
   {
      if (m_configContainer.has("server.pgsql.kerberos"))
         return m_configContainer.getString("server.pgsql.kerberos", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlGssLib() const
   {
      if (m_configContainer.has("server.pgsql.gss-lib"))
         return m_configContainer.getString("server.pgsql.gss-lib", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CStartupOptions::getDatabasePostgresqlService() const
   {
      if (m_configContainer.has("server.pgsql.service"))
         return m_configContainer.getString("server.pgsql.service", "");
      return Poco::NULL_GENERIC;
   }

   bool CStartupOptions::getNoPasswordFlag() const
   {
      return m_configContainer.getBool("server.noPassword", false);
   }

   std::string CStartupOptions::getPluginsPath() const
   {
      return m_configContainer.getString("server.pluginsPath", "plugins");
   }

   std::string CStartupOptions::getScriptInterpretersPath() const
   {
      return m_configContainer.getString("server.scriptInterpretersPath", "scriptInterpreters");
   }

   bool CStartupOptions::getIsRunningAsService() const
   {
      return m_configContainer.getBool("application.runAsService", false);
   }

   std::string CStartupOptions::getUpdateSiteUri() const
   {
      return m_configContainer.getString("server.updateSite", "http://www.yadoms.com/downloads/update/");
   }

   std::string CStartupOptions::getBackupPath() const
   {
      return m_configContainer.getString("server.backupPath", "backups");
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
