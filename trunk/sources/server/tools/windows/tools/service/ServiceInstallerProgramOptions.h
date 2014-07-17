//
// Yadoms startup options loader by command line and file
//
#pragma once

#include "IApplicationServiceInformation.h"

namespace tools { namespace service {

   //--------------------------------------------------------------
   /// \class Default application options loader
   /// This loader get each option from :
   /// - the command line first, if not provided
   /// - in config file, if not provided
   /// - the default value
   //--------------------------------------------------------------
   class CServiceInstallerProgramOptions : public IApplicationServiceInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	            Constructor
      /// \param[in]  argc    Main argc parameter (from command line)
      /// \param[in]  argv    Main argv parameter (from command line)
      /// \throw              CLoaderException
      //--------------------------------------------------------------
      CServiceInstallerProgramOptions(int argc, const char* const argv[], IApplicationServiceInformation & defaultValues);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CServiceInstallerProgramOptions();

      //--------------------------------------------------------------
      /// \brief	 Determine if the "install" program option is in commandline
      /// \return	 true if the "install" is present (and not 'remove')
      //--------------------------------------------------------------
      bool getInstallService();

      //--------------------------------------------------------------
      /// \brief	 Determine if the "remove" program option is in commandline
      /// \return	 true if the "remove" is present (and not 'install')
      //--------------------------------------------------------------
      bool getRemoveService();

      // IApplicationServiceInformation implementation
      virtual const std::string & getServiceName();
      virtual const std::string & getServiceDisplayName();
      virtual const tools::service::CServiceInstaller::EServiceStartType  getStartType();
      virtual const std::string & getServiceAccount();
      virtual const std::string & getServiceAccountPassword();
      virtual const std::string & getServiceDependencies();
      // [END] - IApplicationServiceInformation implementation

      //--------------------------------------------------------------
      /// \brief	 Get the remaining arguments (=service arguments)
      /// \return	 the remaining arguments
      //--------------------------------------------------------------
      std::string getRemainingArguments();

   private:
      //--------------------------------------------------------------
      /// \brief	            Build the supported options list
      //--------------------------------------------------------------
      void buildOptionsDescription();

      // Options description
      boost::program_options::options_description m_optionsDescription;
      IApplicationServiceInformation & m_defaultValues;
      std::string m_remainingArguments;
      bool m_install;
      bool m_remove;
      int m_startType;
      std::string m_serviceAccount;
      std::string m_serviceAccountPassword;
   };

} // namespace service
} // namespace tools