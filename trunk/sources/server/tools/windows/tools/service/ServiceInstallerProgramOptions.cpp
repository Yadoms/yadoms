#include "stdafx.h"
#include "ServiceInstallerProgramOptions.h"

namespace tools { namespace service {

namespace po = boost::program_options;

   // Options
   void CServiceInstallerProgramOptions::buildOptionsDescription()
   {
      m_optionsDescription.add_options()
         ("install", po::bool_switch(&m_install)->default_value(false), "install software as Windows Service")
         ("remove", po::bool_switch(&m_remove)->default_value(false), "remove software as Windows Service")
         ("serviceStartType", po::value<int>(&m_startType)->default_value(m_defaultValues.getStartType()))
         ("serviceAccount", po::value<std::string>(&m_serviceAccount)->default_value(m_defaultValues.getServiceAccount()))
         ("servicePassword", po::value<std::string>(&m_serviceAccountPassword)->default_value(m_defaultValues.getServiceAccountPassword()))
         ;

   }

   CServiceInstallerProgramOptions::CServiceInstallerProgramOptions(int argc, const char* const argv[], IApplicationServiceInformation & defaultValues)
      :m_optionsDescription("Allowed options"), m_defaultValues(defaultValues)
   {
      buildOptionsDescription();

      try
      {
         boost::program_options::variables_map parsedVariables;
         po::parsed_options parsed = po::command_line_parser(argc, argv).options(m_optionsDescription).allow_unregistered().run();
         po::store(parsed, parsedVariables);
         po::notify(parsedVariables);

         std::vector<std::string> to_pass_further = po::collect_unrecognized(parsed.options, po::include_positional);
         std::vector<std::string>::iterator i;
         m_remainingArguments = "";
         for (i = to_pass_further.begin(); i != to_pass_further.end(); ++i)
         {
            m_remainingArguments += *i + " ";
         }


      }
      catch(po::unknown_option& e)
      {
         //throw CLoaderException(m_optionsDescription, e.what());
      }
      catch(po::validation_error& e)
      {
         //throw CLoaderException(m_optionsDescription, e.what());
      }
      catch(po::invalid_command_line_syntax& e)
      {
         //throw CLoaderException(m_optionsDescription, e.what());
      }
   }

   CServiceInstallerProgramOptions::~CServiceInstallerProgramOptions()
   {
   }

   bool CServiceInstallerProgramOptions::getInstallService()
   {
      return m_install;
   }

   bool CServiceInstallerProgramOptions::getRemoveService()
   {
      return m_remove;
   }


   const std::string & CServiceInstallerProgramOptions::getServiceName()
   {
      return m_defaultValues.getServiceName();
   }

   const std::string & CServiceInstallerProgramOptions::getServiceDisplayName()
   {
      return m_defaultValues.getServiceDisplayName();
   }

   const tools::service::CServiceInstaller::EServiceStartType  CServiceInstallerProgramOptions::getStartType()
   {
      if (m_startType == tools::service::CServiceInstaller::kAutoStart ||
         m_startType == tools::service::CServiceInstaller::kManualStart ||
         m_startType == tools::service::CServiceInstaller::kDisabled)
         return (tools::service::CServiceInstaller::EServiceStartType)m_startType;
      throw std::exception();
   }

   const std::string & CServiceInstallerProgramOptions::getServiceDependencies()
   {
      return m_defaultValues.getServiceDependencies();
   }

   const std::string & CServiceInstallerProgramOptions::getServiceAccount()
   {
      return m_serviceAccount;
   }

   const std::string & CServiceInstallerProgramOptions::getServiceAccountPassword()
   {
      return m_serviceAccountPassword;
   }

   std::string CServiceInstallerProgramOptions::getRemainingArguments()
   {
      return m_remainingArguments;
   }
} // namespace service
} // namespace tools