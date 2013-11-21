#pragma once

#include <boost/filesystem.hpp>

#include "StartupOptionsLoader.h"

namespace po = boost::program_options;

// The optional configuration file name
const std::string CStartupOptionsLoader::OptionalConfigFile("yadoms.cfg");



CStartupOptionsLoaderError::CStartupOptionsLoaderError(const boost::program_options::options_description& optionsDecription,
                                                       const char* message)
   :m_optionsDecription(optionsDecription)
{
   // Don't log to boost here, as this exception is raised if Yadoms was
   // called by command line, so only write to the console.
   // We register the message however.
   std::ostringstream s;

   if (message)
      s << message << std::endl << std::endl;

   s << "Command line usage :" << std::endl;
   s << "   Yadoms [options]"  << std::endl << std::endl;
   s << m_optionsDecription << std::endl;

   m_message = s.str();

   std::cout << m_message << std::endl;
}

CStartupOptionsLoaderError::~CStartupOptionsLoaderError() throw()
{
}

 

CStartupOptionsLoader::CStartupOptionsLoader(int argc, char** argv)
   :m_optionsDescription("Allowed options")
{
   buildOptionsDescription();

   try
   {
      // Load configuration from command line and complete with config file (second data read don't overwrite first)
      po::variables_map vm;
      po::store(po::parse_command_line(argc, argv, m_optionsDescription), vm);
      if (boost::filesystem::exists(OptionalConfigFile))
         po::store(po::parse_config_file<char>(OptionalConfigFile.c_str(), m_optionsDescription, true), vm);
      po::notify(vm);

      if (vm.count("help"))
         throw CStartupOptionsLoaderError(m_optionsDescription);
   }
   catch(po::unknown_option& e)
   {
      throw CStartupOptionsLoaderError(m_optionsDescription, e.what());
   }
   catch(po::validation_error& e)
   {
      throw CStartupOptionsLoaderError(m_optionsDescription, e.what());
   }
}


CStartupOptionsLoader::~CStartupOptionsLoader()
{
}


// Class thrown if there is an inexisting path value given
// Catch as po::invalid_option_value in CStartupOptionsLoader::ctor
// This class add some information in error display
class CInexistingPathValue : public po::invalid_option_value
{
public:
   CInexistingPathValue(const std::string& value)
      :invalid_option_value(value + " : invalid path")
   {}
};

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
   if (boost::filesystem::exists(path))
      v = boost::any(CMustExistPathOption(path));
   else
      throw CInexistingPathValue(path);
}

// Needed for implementation of po::value::default_value()
std::ostream& operator<<(std::ostream& stream, const CMustExistPathOption& pathOption)
{
   stream << pathOption.get();
   stream.flush();
   return stream;
}

void CStartupOptionsLoader::buildOptionsDescription()
{
   m_optionsDescription.add_options()
      ("help", "produce help message")
      ("port", po::value<unsigned int>(&m_portNumber)->default_value(8080),
         "set the web server port number")
      ("logLevel", po::value<boost::log::trivial::severity_level>(&m_logLevel)->default_value(boost::log::trivial::info),
         "set log level, accepted values are : trace, debug, info, warning, error, fatal")
      ("databaseFile", po::value<std::string>(&m_databaseFile)->default_value("/db/db.sql"),
         "use a specific dataBase file")
      ("hardwarePluginsPath", po::value<CMustExistPathOption>(&m_hardwarePluginsPath)->default_value(CMustExistPathOption("/plugins/hardware")),
         "use a specific path to hardware plugins")
      ("devicePluginsPath", po::value<CMustExistPathOption>(&m_devicePluginsPath)->default_value(CMustExistPathOption("/plugins/device")),
         "use a specific path to device plugins")
      ;
}


