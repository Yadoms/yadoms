//
// Yadoms startup options loader by command line and file
//
#pragma once

#include "IStartupOptions.h"
#include "LoaderException.hpp"
#include "LoaderCustomValidators.hpp"

namespace startupOptions
{

   //--------------------------------------------------------------
   /// \class Default application options loader
   /// This loader get each option from :
   /// - the command line first, if not provided
   /// - in config file, if not provided
   /// - the default value
   //--------------------------------------------------------------
   class CLoader : public IStartupOptions
   {
   public:
      //--------------------------------------------------------------
      /// \brief	            Constructor
      /// \param[in]  argc    Main argc parameter (from command line)
      /// \param[in]  argv    Main argv parameter (from command line)
      /// \throw              CLoaderException
      //--------------------------------------------------------------
      CLoader(int argc, const char* const argv[]);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CLoader();

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
      virtual const std::string& getPluginsPath() const
         { return m_PluginsPath.get(); }
      virtual const std::string& getWidgetsPath() const
         { return m_widgetsPath.get(); }
      virtual bool getDebugFlag() const
         { return m_debugFlag; }
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
      CMustExistPathOption m_PluginsPath;
      CMustExistPathOption m_widgetsPath;
      bool m_debugFlag;
   };

} // namespace startupOptions