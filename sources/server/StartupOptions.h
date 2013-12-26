//
// Yadoms startup options interface
//
#pragma once

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
   virtual boost::log::trivial::severity_level getLogLevel() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the port number
   /// \return     Configured port number
   //--------------------------------------------------------------
   virtual unsigned int getPortNumber() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the web server ip address
   /// \return     Configured web server ip address
   //--------------------------------------------------------------
   virtual const std::string& getWebServerIPAddress() const = 0;
      
   //--------------------------------------------------------------
   /// \brief	    Get the web server initial path
   /// \return     Configured web server initial path
   //--------------------------------------------------------------
   virtual const std::string& getWebServerInitialPath() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the path of the database file
   /// \return     Database file path
   /// \note       If not exist, Yadoms will create the file
   //--------------------------------------------------------------
   virtual const std::string& getDatabaseFile() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the path of the hardware plugins
   /// \return     Hardware plugins directory (absolute)
   /// \note       If explicitly provided, path must exist
   //--------------------------------------------------------------
   virtual const std::string& getHarwarePluginsPath() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the path of the device plugins
   /// \return     Device plugins directory (absolute)
   /// \note       If explicitly provided, path must exist
   //--------------------------------------------------------------
   virtual const std::string& getDevicePluginsPath() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the startXplHub flag
   /// \return     flag indicating if Xpl hub must be started or not
   //--------------------------------------------------------------
   virtual bool getStartXplHubFlag() const = 0;
};


