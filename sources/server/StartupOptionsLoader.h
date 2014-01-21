//
// Yadoms startup options loader by command line and file
//
#pragma once

#include "StartupOptions.h"
#include "StartupOptionsLoaderException.hpp"
#include "StartupOptionsLoaderCustomValidators.hpp"


//--------------------------------------------------------------
/// \class Default application options loader
/// This loader get each option from :
/// - the command line first, if not provided
/// - in config file, if not provided
/// - the default value
//--------------------------------------------------------------
class CStartupOptionsLoader : public IStartupOptions
{
public:
   //--------------------------------------------------------------
   /// \brief	            Constructor
   /// \param[in]  argc    Main argc parameter (from command line)
   /// \param[in]  argv    Main argv parameter (from command line)
   /// \throw              CStartupOptionsLoaderException
   //--------------------------------------------------------------
   CStartupOptionsLoader(int argc, char** argv);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CStartupOptionsLoader();

   //--------------------------------------------------------------
   // IStartupOptions implementation (see IStartupOptions declaration for details)
   virtual boost::log::trivial::severity_level getLogLevel() const
      { return m_logLevel; }
   virtual unsigned int getWebServerPortNumber() const
      { return m_webServerPortNumber; }
   virtual const std::string& getWebServerIPAddress() const
      { return m_webServerIPAddress.get(); }
   virtual const std::string& getWebServerInitialPath() const
      { return m_webServerInitialPath.get(); }
   virtual const std::string& getDatabaseFile() const
      { return m_databaseFile; }
   virtual const std::string& getHarwarePluginsPath() const
      { return m_hardwarePluginsPath.get(); }
   virtual const std::string& getWidgetsPath() const
      { return m_widgetsPath.get(); }
   virtual bool getStartXplHubFlag() const
      { return m_startXplHub; }
   virtual const std::string& getXplNetworkIpAddress() const
      { return m_xplNetworkIPAddress.get(); }
   // [END] IStartupOptions implementation
   //--------------------------------------------------------------


private:
   //--------------------------------------------------------------
   /// \brief	            Build the supported options list
   //--------------------------------------------------------------
   void buildOptionsDescription();

private:
   static const std::string OptionalConfigFile;

   // Options description
   boost::program_options::options_description m_optionsDescription;

   // Options data
   boost::log::trivial::severity_level m_logLevel;
   unsigned int m_webServerPortNumber;
   CValidIpAddressOption m_webServerIPAddress;
   CMustExistPathOption m_webServerInitialPath;
   std::string m_databaseFile;
   CMustExistPathOption m_hardwarePluginsPath;
   CMustExistPathOption m_widgetsPath;
   bool m_startXplHub;
   CValidIpAddressOption m_xplNetworkIPAddress;
};


