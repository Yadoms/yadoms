//
// Yadoms startup options interface
//
#pragma once

#include "DatabaseEngine.h"

namespace startupOptions {
   class IStartupOptions
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IStartupOptions() {}

      //--------------------------------------------------------------
      /// \brief	    Get the log level
      /// \return     Configured log level
      //--------------------------------------------------------------
      virtual const std::string getLogLevel() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the port number
      /// \return     Configured port number
      //--------------------------------------------------------------
      virtual const unsigned int getWebServerPortNumber() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the web server ip address
      /// \return     Configured web server ip address
      //--------------------------------------------------------------
      virtual const std::string getWebServerIPAddress() const = 0;
      
      //--------------------------------------------------------------
      /// \brief	    Get the web server initial path
      /// \return     Configured web server initial path
      //--------------------------------------------------------------
      virtual const std::string getWebServerInitialPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database engine to use
      /// \return     The database engine (sqlite|postgresql)
      //--------------------------------------------------------------
      virtual const EDatabaseEngine getDatabaseEngine() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the path of the database file (sqlite only)
      /// \return     Database file path
      /// \note       If not exist, Yadoms will create the file
      //--------------------------------------------------------------
      virtual const std::string getDatabaseSqliteFile() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database host (PostgreSQL database only)
      /// \return     The database host (host name or ip)
      //--------------------------------------------------------------
      virtual const std::string getDatabasePostgresqlHost() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database port (PostgreSQL database only)
      /// \return     The database port
      //--------------------------------------------------------------
      virtual const unsigned int getDatabasePostgresqlPort() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database name (PostgreSQL database only)
      /// \return     The database name
      //--------------------------------------------------------------
      virtual const std::string getDatabasePostgresqlDbName() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database login (PostgreSQL database only)
      /// \return     The database login
      //--------------------------------------------------------------
      virtual const std::string getDatabasePostgresqlLogin() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database password (PostgreSQL database only)
      /// \return     The database password
      //--------------------------------------------------------------
      virtual const std::string getDatabasePostgresqlPassword() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the plugin path
      /// \return     The plugin path
      /// \note       If not exist, it returns "plugins"
      //--------------------------------------------------------------
      virtual const std::string getPluginsPath() const = 0;    
      
      //--------------------------------------------------------------
      /// \brief	    Get the script interpreters path
      /// \return     The script interpreters path
      /// \note       If not exist, it returns "scriptInterpreters"
      //--------------------------------------------------------------
      virtual const std::string getScriptInterpretersPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the no password flag
      /// \return     flag indicating if password check is disabled for this instance
      //--------------------------------------------------------------
      virtual bool getNoPasswordFlag() const = 0;     
      
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
      virtual const std::string getUpdateSiteUri() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the acquisition lifetime
      /// \return     The acquisition lifetime
      //--------------------------------------------------------------
      virtual int getDatabaseAcquisitionLifetime() const = 0;
   };

} // namespace startupOptions