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
      CStartupOptions(Poco::Util::AbstractConfiguration & configContainer);

      //--------------------------------------------------------------
      /// \brief	   Destructor
      //--------------------------------------------------------------
      virtual ~CStartupOptions();

      //--------------------------------------------------------------
      /// \brief	   Define StartupOptions
      //--------------------------------------------------------------
      void defineOptions(Poco::Util::OptionSet& options);

      //--------------------------------------------------------------
      // IStartupOptions implementation (see IStartupOptions declaration for details)
      virtual const std::string getLogLevel() const;
      virtual unsigned int getWebServerPortNumber() const;
      virtual const std::string getWebServerIPAddress() const;
      virtual const std::string getWebServerInitialPath() const;
      virtual const std::string getDatabaseFile() const;
      virtual bool getDebugFlag() const;
      virtual bool getNoPasswordFlag() const;
      // [END] IStartupOptions implementation
      //--------------------------------------------------------------

   private:
      //--------------------------------------------------------------
      /// \brief	   Reference for the configuration container
      //--------------------------------------------------------------
      Poco::Util::AbstractConfiguration & m_configContainer;
   };


} // namespace startupOptions