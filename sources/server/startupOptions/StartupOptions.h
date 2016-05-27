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
      unsigned int getWebServerPortNumber() const override;
      std::string getWebServerIPAddress() const override;
      std::string getWebServerInitialPath() const override;
      std::string getDatabaseFile() const override;
      std::string getPluginsPath() const override;
      std::string getScriptInterpretersPath() const override;
      bool getNoPasswordFlag() const override;
      bool getIsRunningAsService() const override;
      std::string getUpdateSiteUri() const override;
      int getDatabaseAcquisitionLifetime() const override;
      bool getDeveloperMode() const override;
      // [END] IStartupOptions implementation
      //--------------------------------------------------------------

   private:
      //--------------------------------------------------------------
      /// \brief	   Reference for the configuration container
      //--------------------------------------------------------------
      Poco::Util::AbstractConfiguration & m_configContainer;
   };


} // namespace startupOptions