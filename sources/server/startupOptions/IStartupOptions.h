//
// Yadoms startup options interface
//
#pragma once

namespace startupOptions
{

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
      virtual unsigned int getWebServerPortNumber() const = 0;

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
      /// \brief	    Get the path of the database file
      /// \return     Database file path
      /// \note       If not exist, Yadoms will create the file
      //--------------------------------------------------------------
      virtual const std::string getDatabaseFile() const = 0;    
      
      //--------------------------------------------------------------
      /// \brief	    Get the plugin path
      /// \return     The plugin path
      /// \note       If not exist, it returns "plugins"
      //--------------------------------------------------------------
      virtual const std::string getPluginsPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the debug flag
      /// \return     flag indicating if debug mode is activated
      //--------------------------------------------------------------
      virtual bool getDebugFlag() const = 0;   
      
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
   };

} // namespace startupOptions