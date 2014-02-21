#include "stdafx.h"
#include "Loader.h"
#include "InvalidOptionException.hpp"

namespace startupOptions
{

namespace po = boost::program_options;

// The optional configuration file name
const std::string CLoader::OptionalConfigFile("yadoms.cfg");

// Options
void CLoader::buildOptionsDescription()
{
   m_optionsDescription.add_options()
      ("help", "produce help message")
      ("port,p", po::value<unsigned int>(&m_webServerPortNumber)->default_value(8080),
      "set the web server port number")
      ("webServerIp,i", po::value<CValidIpAddressOption>(&m_webServerIPAddress)->default_value(CValidIpAddressOption("0.0.0.0")),
      "use a specific web server ip address. Use 0.0.0.0 to accepts connections via all interfaces. ")
      ("webServerPath,w", po::value<CMustExistPathOption>(&m_webServerInitialPath)->default_value(CMustExistPathOption("www")),
      "use a specific web server initial folder")
      ("logLevel,l", po::value<boost::log::trivial::severity_level>(&m_logLevel)->default_value(boost::log::trivial::info),
      "set log level, accepted values are : trace, debug, info, warning, error, fatal")
      ("databaseFile,D", po::value<std::string>(&m_databaseFile)->default_value("yadoms.db3"),
      "use a specific dataBase file")
      ("hardwarePluginsPath,h", po::value<CMustExistPathOption>(&m_hardwarePluginsPath)->default_value(CMustExistPathOption("plugins/hardware")),
      "use a specific path to hardware plugins")
      ("widgetsPath", po::value<CMustExistPathOption>(&m_widgetsPath)->default_value(CMustExistPathOption("widgets")),
      "use a specific path to widgets path")
      ("disableXplHubStart,x",
      "don't start the Xpl hub, useful if another Xpl hub is already running on the same machine")
      ("XplNetworkIp,I", po::value<CValidIpAddressOption>(&m_xplNetworkIPAddress)->default_value(CValidIpAddressOption("0.0.0.0")),
      "define on which network Xpl will be listened and send. If not specified, first interface will be taken")
      ("debug", po::bool_switch(&m_debugFlag)->default_value(false), "activate the debug mode (log files are separated by thread)")
      ;
}

CLoader::CLoader(int argc, char** argv)
   :m_optionsDescription("Allowed options")
{
   buildOptionsDescription();

   try
   {
      // Load configuration from command line and complete with config file (second data read don't overwrite first)
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, m_optionsDescription, po::command_line_style::default_style ^ po::command_line_style::allow_guessing), vm);
      if (boost::filesystem::exists(OptionalConfigFile))
         po::store(po::parse_config_file<char>(OptionalConfigFile.c_str(), m_optionsDescription, true), vm);
      po::notify(vm);

      if (vm.count("help"))
         throw CLoaderException(m_optionsDescription);

      m_startXplHub = !vm.count("disableXplHubStart");
   }
   catch(po::unknown_option& e)
   {
      throw CLoaderException(m_optionsDescription, e.what());
   }
   catch(po::validation_error& e)
   {
      throw CLoaderException(m_optionsDescription, e.what());
   }
   catch(po::invalid_command_line_syntax& e)
   {
      throw CLoaderException(m_optionsDescription, e.what());
   }
}

CLoader::~CLoader()
{
}


// Validate provided path
void validate(boost::any& v,
   const std::vector<std::string>& values,
   CMustExistPathOption*, int)
{
   po::validators::check_first_occurrence(v);

   // Extract the first string from 'values'. If there is more than
   // one string, it's an error, and exception will be thrown.
   const std::string& path = po::validators::get_single_string(values);

   // Check if path exist
   if (CMustExistPathOption::validate(path))
      v = boost::any(CMustExistPathOption(path));
   else
      throw CInvalidOptionException(path, "invalid path");
}

// Needed for implementation of po::value::default_value()
std::ostream& operator<<(std::ostream& stream, const CMustExistPathOption& pathOption)
{
   stream << pathOption.get();
   stream.flush();
   return stream;
}

// Validate provided ip address
void validate(boost::any& v,
   const std::vector<std::string>& values,
   CValidIpAddressOption*, int)
{
   po::validators::check_first_occurrence(v);

   // Extract the first string from 'values'. If there is more than
   // one string, it's an error, and exception will be thrown.
   const std::string& ipAddress = po::validators::get_single_string(values);

   // Check if IP address is well formed (if not, CInvalidIpAddressException exception is raised)
   if (CValidIpAddressOption::validate(ipAddress))
      v = boost::any(CValidIpAddressOption(ipAddress));
   else
      throw CInvalidOptionException(ipAddress, "invalid ip address");
}

// Needed for implementation of po::value::default_value()
std::ostream& operator<<(std::ostream& stream, const CValidIpAddressOption& ipAddressOption)
{
   stream << ipAddressOption.get();
   stream.flush();
   return stream;
}

} // namespace startupOptions