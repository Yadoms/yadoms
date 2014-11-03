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
      "use a specific path for web server")
      ("logLevel,l", po::value<boost::log::trivial::severity_level>(&m_logLevel)->default_value(boost::log::trivial::info),
      "set log level, accepted values are : trace, debug, info, warning, error, fatal")
      ("databaseFile,D", po::value<std::string>(&m_databaseFile)->default_value("yadoms.db3"),
      "use a specific dataBase file")
      ("pluginsPath,P", po::value<CMustExistPathOption>(&m_PluginsPath)->default_value(CMustExistPathOption("plugins")),
      "use a specific path for plugins")
      ("widgetsPath,W", po::value<CMustExistPathOption>(&m_widgetsPath)->default_value(CMustExistPathOption("widgets")),
      "use a specific path to widgets path")
      ("debug", po::bool_switch(&m_debugFlag)->default_value(false), "activate the debug mode (log files are separated by thread)")
      ;
}

CLoader::CLoader(int argc, const char* const argv[])
   :m_optionsDescription("Allowed options"), m_webServerPortNumber(8080), m_debugFlag(false)
{
   buildOptionsDescription();

   try
   {
      // Load configuration from command line and complete with config file (second data read don't overwrite first)
      // allow unregistered options (dont throw, just ignore them)
      po::variables_map vm;
      po::parsed_options parsed = po::command_line_parser(argc, argv).options(m_optionsDescription).allow_unregistered().run();
      po::store(parsed, vm);
      if (boost::filesystem::exists(OptionalConfigFile))
         po::store(po::parse_config_file<char>(OptionalConfigFile.c_str(), m_optionsDescription, true), vm);
      po::notify(vm);

      if (vm.count("help"))
         throw CLoaderException(m_optionsDescription);
   }
   catch(po::ambiguous_option& e)
   {
      throw CLoaderException(m_optionsDescription, e.what());
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